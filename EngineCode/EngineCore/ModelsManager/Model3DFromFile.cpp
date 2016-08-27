#include "EngineCore/stdafx.h"
#include "ModelsManager.h"
#include "EngineCore/EngineHelpers/Converters.h"

#include "Common/Serialization/SW/Serialization.h"

#include "Common/MemoryLeaks.h"


ModelsManager* Model3DFromFile::models_manager = nullptr;


//----------------------------------------------------------------------------------------------//
//									Model3DFromFile												//
//----------------------------------------------------------------------------------------------//

//==============================================================================================//


RTTR_REGISTRATION
{
	rttr::registration::class_< std::vector< ModelPart > >( "Vector< ModelPart >" );

	rttr::registration::class_< Model3DFromFile >( "Model3DFromFile" )
	.property( "FileName", &Model3DFromFile::m_filePath )
	.property( "VertexBuffer", &Model3DFromFile::vertex_buffer )
	.property( "IndexBuffer", &Model3DFromFile::index_buffer )
	.property( "Segments", &Model3DFromFile::model_parts );
}

//----------------------------------------------------------------------------------------------//
//								contructor, destructor											//
//----------------------------------------------------------------------------------------------//

/**@brief Inicjuje obiekt œcie¿k¹ do pliku, który zostanie do niego wczytany ( nie wczytuje pliku).*/
Model3DFromFile::Model3DFromFile( const std::wstring& file_name )
: ResourceObject( WRONG_ID )
{
	m_filePath = file_name;
	vertex_buffer = nullptr;
	index_buffer = nullptr;
	tmp_data = nullptr;
}


Model3DFromFile::~Model3DFromFile( )
{
	if ( tmp_data )
	{
		for ( int i = 0; i < tmp_data->current_pointer; ++i )
		{
			delete[] tmp_data->table[i]->indicies_tab;
			delete[] tmp_data->table[i]->vertices_tab;
			delete tmp_data->table[i]->new_part.mesh;
		}
		
		for ( int i = 0; i < tmp_data->current_pointer; ++i )
			delete tmp_data->table[i];
		delete[] tmp_data->table;
		delete tmp_data;
		tmp_data = nullptr;
	}

	// Kasujemy odwo³ania plikowe do buforów
	if ( vertex_buffer )
		vertex_buffer->DeleteAssetReference();
	if ( index_buffer )
		index_buffer->DeleteAssetReference();

	// Kasujemy odwo³ania plikowe do obiektów w talicach
	for ( unsigned int k = 0; k < model_parts.size( ); ++k )
	{//ka¿dy element mo¿e byæ nullptrem
		register ModelPart* part = &model_parts[k];

		//ka¿dy element mo¿e byæ nullptrem
		if ( part->material )
			part->material->DeleteAssetReference( );

		if ( part->mesh )
		{
			//kasujemy obiekty MeshParts (to s¹ jedyne, którymi zarz¹dza ta klasa)
			part->mesh->DeleteAssetReference();
			delete part->mesh;
		}
		if ( part->pixel_shader )
			part->pixel_shader->DeleteAssetReference( );
		for ( int i = 0; i < ENGINE_MAX_TEXTURES; ++i )
			if ( part->texture[i] )
				part->texture[i]->DeleteAssetReference( );
		if ( part->vertex_shader )
			part->vertex_shader->DeleteAssetReference( );
	}


	model_parts.clear( );
}




//----------------------------------------------------------------------------------------------//
//									Funkcje do tworzenia obiektu								//
//----------------------------------------------------------------------------------------------//


/**@brief BeginEdit przygotowuje strukturê pod wpisywanie danych.

Funkcje BeginEdit i EndEdit maj¹ byæ wywo³ane przez klasê ModelsManager przed podaniem do loadera
i po zakoñczeniu wpisywania danych.
*/
void Model3DFromFile::BeginEdit()
{
	tmp_data = new EditTMP;
	//Inicjujemy tablicê o pocz¹tkowej d³ugoœci domyœlnej
	tmp_data->table = new TMP_data*[tmp_data->table_length];
}


/**@brief EndEdit zatwierdza wpisane dane, tworzy w³aœciwa strukturê i wpisuje nie istniej¹ce jeszcze obiekty
do ModelsManagera.

Funkcje BeginEdit i EndEdit maj¹ byæ wywo³ane przez klasê ModelsManager przed podaniem do loadera
i po zakoñczeniu wpisywania danych.
*/
void Model3DFromFile::EndEdit()
{
	// Najpierw scalamy bufory wierzcho³ków.
	EndEdit_vertex_buffer_processing();
	// Scalamy bufory indeksów.
	EndEdit_index_buffer_processing();

	// Dodajemy do strutkury ModelPart dane o buforach wierzcho³ków oraz przypisujemy wbudowane
	// shadery, je¿eli nie podano ¿adnych
	EndEdit_prepare_ModelPart();

	// Zapisujemy gotowe struktury
	for ( int i = 0; i < tmp_data->current_pointer; ++i )
		model_parts.push_back( tmp_data->table[i]->new_part );

	//zwalniamy pamiêæ
	for ( int i = 0; i < tmp_data->current_pointer; ++i )
		delete tmp_data->table[i];
	delete[] tmp_data->table;
	delete tmp_data;
	tmp_data = nullptr;
}


/**@brief Funkcje BeginPart i EndPart s¹ wywo³ywane przez loader i maj¹ otaczaæ wszystkie instrukcje dodaj¹ce
jedn¹ czêœæ mesha do struktury.*/
void Model3DFromFile::BeginPart()
{
	//sprawdzamy czy tablica nam siê nie skoñczy³a
	if ( tmp_data->current_pointer >= tmp_data->table_length )
	{//alokujemy troszke wiêcej pamiêci i przepisujemy
		unsigned int new_length = tmp_data->table_length << 1;

		TMP_data** new_table = new TMP_data*[new_length];
		for ( int i = 0; i < tmp_data->table_length; ++i )
			new_table[i] = tmp_data->table[i];

		tmp_data->table_length = new_length;
		delete[] tmp_data->table;
		tmp_data->table = new_table;
	}

	//dodajemy do tabeli nowy element
	tmp_data->table[tmp_data->current_pointer] = new TMP_data;
	tmp_data->table[tmp_data->current_pointer]->new_part.mesh = new MeshPartObject;

	//nie jest to konieczne, ale dla konwencji dodajemy
	tmp_data->table[tmp_data->current_pointer]->new_part.mesh->AddAssetReference();
}

/**@brief Funkcje BeginPart i EndPart s¹ wywo³ywane przez loader i maj¹ otaczaæ wszystkie instrukcje dodaj¹ce
jedn¹ czêœæ mesha do struktury.*/
void Model3DFromFile::EndPart()
{
	//inkrementujemy aktualny index wewn¹trz tablicy
	tmp_data->current_pointer++;
}



/**@brief Dodaje teksturê na aktualnej pozycji w tablicy.

Tekstura jest umieszczana w tablicy ModelPart.texture zgodnie z indeksem reprezentowanym przez type.
Obiekt jest dodawany do ModelsManagera je¿eli jeszcze nie istnia³. Je¿eli istnia³
nie jest duplikowany, ale zamiast niego wstawia siê wskaŸnik na istniej¹cy obiekt.
@param[in] file_name Œcie¿ka do tekstury
@param[in] type Type tekstury. Zobacz definicjê TextureUse.
@return Identyfiaktor tekstury lub WRONG_ID, je¿eli coœ posz³o nie tak.*/
unsigned int Model3DFromFile::add_texture( const std::wstring& file_name, TextureUse type )
{
	// Ktoœ móg³ podaæ indeks w tablicy zamiast jednego z enumów, wiêc musimy pilnowaæ czy nie przekroczy³ zakresu
	if ( type > ENGINE_MAX_TEXTURES )
		return WRONG_ID;

	TextureObject* texture = models_manager->AddTexture( file_name );
	if( !texture )
		return WRONG_ID;

	// Teraz musimy dodaæ teksturê na odpowiednie miejsce w tablicy
	tmp_data->table[tmp_data->current_pointer]->new_part.texture[type] = texture;
	// Dodajemy odwo³anie plikowe
	texture->AddAssetReference();

	// Zwracamy id tekstury, ale w zasadzie tylko po to, ¿eby ktoœ móg³ sprawdziæ czy wszystko posz³o dobrze
	return texture->GetID();
}

/**@brief Dodaje materia³ na aktualnej pozycji w tablicy.

Obiekt jest dodawany do ModelsManagera je¿eli jeszcze nie istnia³. Je¿eli istnia³
nie jest duplikowany, ale zamiast niego wstawia siê wskaŸnik na istniej¹cy obiekt.

@param[in] material Materia³, który ma zostaæ dodany
@param[in] material_name Nazwa materia³u. Do materia³u bêdzie mo¿na siê odwo³aæ podaj¹c ci¹g znaków
[nazwa_pliku]::[nazwa_materia³u]. Oznacza to, ¿e mog¹ istnieæ dwa takie same materia³y, poniewa¿ nie jest sprawdzana
zawartoœæ, a jedynie nazwy.
@return Indentyfikator obiektu.*/
unsigned int Model3DFromFile::add_material( const MaterialObject* material, const std::wstring& material_name )
{
	std::wstring name = m_filePath;
	name += L"::";
	name += material_name;

	MaterialObject* new_material = models_manager->m_material.get( name );
	if ( !new_material )
	{
		// Nie by³o materia³u, trzeba j¹ stworzyæ i dodaæ
		new_material = new MaterialObject( material );

		models_manager->m_material.UnsafeAdd( name, new_material );	// Dodaliœmy teksturê
	}

	// Teraz musimy dodaæ materia³ na odpowiednie miejsce w tablicy
	tmp_data->table[tmp_data->current_pointer]->new_part.material = new_material;
	// Dodajemy odwo³anie plikowe
	new_material->AddAssetReference( );

	// Zwracamy id materia³y, ale w zasadzie tylko po to, ¿eby ktoœ móg³ sprawdziæ czy wszystko posz³o dobrze
	// Akurat w przypadku materia³ów nic nie mo¿e pójœæ Ÿle
	return new_material->GetID( );
}

/**@brief Dodaje vertex shader na aktualnej pozycji ModelPart.

Obiekt jest dodawany do ModelsManagera je¿eli jeszcze nie istnia³. Je¿eli istnia³
nie jest duplikowany, ale zamiast niego wstawia siê wskaŸnik na istniej¹cy obiekt.

@param[in] file_name Nazwa pliku, w którym znajduje siê vertex shader. Nazwa funkcji na razie jest niezmienna
i okreœla j¹ makro DEFAULT_VERTEX_SHADER_ENTRY.
@return Indentyfikator obiektu.*/
unsigned int Model3DFromFile::add_vertex_shader( const std::wstring& file_name )
{
	VertexShader* vertex_shader = models_manager->AddVertexShader( file_name, DEFAULT_VERTEX_SHADER_ENTRY );
	if ( !vertex_shader )
		return WRONG_ID;

	// Teraz musimy dodaæ shader na odpowiednie miejsce w tablicy
	tmp_data->table[tmp_data->current_pointer]->new_part.vertex_shader = vertex_shader;
	// Dodajemy odwo³anie plikowe
	vertex_shader->AddAssetReference( );

	// Zwracamy id shadera, ale w zasadzie tylko po to, ¿eby ktoœ móg³ sprawdziæ czy wszystko posz³o dobrze
	return vertex_shader->GetID( );
}

/**@brief Dodaje pixel shader na aktualnej pozycji ModelPart.

Obiekt jest dodawany do ModelsManagera je¿eli jeszcze nie istnia³. Je¿eli istnia³
nie jest duplikowany, ale zamiast niego wstawia siê wskaŸnik na istniej¹cy obiekt.

@param[in] file_name Nazwa pliku, w którym znajduje siê pixel shader. Nazwa funkcji na razie jest niezmienna
i okreœla j¹ makro DEFAULT_PIXEL_SHADER_ENTRY.
@return Indentyfikator obiektu.*/
unsigned int Model3DFromFile::add_pixel_shader( const std::wstring& file_name )
{
	PixelShader* pixel_shader = models_manager->AddPixelShader( file_name, DEFAULT_PIXEL_SHADER_ENTRY );
	if ( !pixel_shader )
		return WRONG_ID;

	// Teraz musimy dodaæ shader na odpowiednie miejsce w tablicy
	tmp_data->table[tmp_data->current_pointer]->new_part.pixel_shader = pixel_shader;
	// Dodajemy odwo³anie plikowe
	pixel_shader->AddAssetReference( );

	// Zwracamy id shadera, ale w zasadzie tylko po to, ¿eby ktoœ móg³ sprawdziæ czy wszystko posz³o dobrze
	return pixel_shader->GetID( );
}


/**@brief Funkcja dodaje do tymczasowej tablicy bufor wierzcho³ków.

Funkcja dodaje do tymczasowej tablicy bufor wierzcho³ków.
Podane dane s¹ przepisywane, a za zwolnienie pamiêci odpowiada ten kto wywo³a³ funkcjê.
Nie wolno dodaæ kolejnego bufora, je¿eli w danym ModelPart jakiœ juz istania³.
W takiej sytuacji funkcja natychmiast powraca.

Bufory wierzcho³ków s¹ ³¹czone w jedna ca³oœæ w funkcji EndEdit.

@param[in] buffer WskaŸnik na tablicê wierzcho³ków. Zwalnia wywo³uj¹cy.
@param[in] vert_count Liczba wierzcho³ków w buforze.
@return Funkcja  zwraca WRONG_ID w przypadku niepowodzenia lub 1 w przypadku powodzenia.*/
unsigned int Model3DFromFile::add_vertex_buffer( const VertexNormalTexCord1* buffer, unsigned int vert_count )
{
	if ( vert_count == 0 )
		return WRONG_ID;		// Nie damy siê zrobiæ w balona

	auto data = tmp_data->table[tmp_data->current_pointer];

	if ( data->vertices_tab )
		return WRONG_ID;		// Je¿eli jakaœ tablica ju¿ istania³a, to ktoœ pope³ni³ b³¹d

	data->vertices_count = vert_count;							// Zapisujemy liczbê wierzcho³ków
	data->vertices_tab = new VertexNormalTexCord1[vert_count];	// Tworzymy now¹ tablice wierzcho³ków
	// Tablica zwalniana w funkcji EndEdit_vertex_buffer_processing()
	memcpy( data->vertices_tab, buffer, sizeof(VertexNormalTexCord1)*vert_count );	// Przepisujemy tablicê

	return 1;
}


/**@brief Nadaje aktualnej czêœci mesha macierz transformacji.

Ka¿da czêœæ mesha ma przypisan¹ do siebie transformacjê wzglêdem œrodka uk³adu wspó³rzednych modelu.
Je¿eli dla jakiegoœ bufora wierzcho³ków nie zostanie przydzielona transformacja, to 
domyœlnie znajduje siê tu macierz identycznoœciowa.

@param[in] Macierz transformacji*/
void Model3DFromFile::add_transformation( const DirectX::XMFLOAT4X4& transform )
{
	tmp_data->table[tmp_data->current_pointer]->new_part.mesh->transform_matrix = transform;
}


/**@brief Funkcja dodaje do tymczasowej tablicy dane potrzebne do stworzenia bufora wierzcho³ków.
Indeksy s¹ przepisywane z podanej tablicy i trzeba j¹ zwolniæ samemu.
*/
unsigned int Model3DFromFile::add_index_buffer( const VERT_INDEX* buffer, unsigned int ind_count, int vertex_buffer_offset )
{
	if ( ind_count == 0 )
		return WRONG_ID;		// Nie damy siê zrobiæ w balona

	// Bufor indeksów przepisujemy do tablicy tymczasowej. Zostanie on potem scalony z reszt¹ buforów
	// indeksów w funkcji EndEdit.
	unsigned int cur_ptr = tmp_data->current_pointer;

	if ( tmp_data->table[cur_ptr]->indicies_tab )
		return WRONG_ID;		// Je¿eli jakaœ tablica ju¿ istania³a, to ktoœ pope³ni³ b³¹d

	tmp_data->table[cur_ptr]->indicies_count = ind_count;			// Przepisujemy liczbê indeksów
	tmp_data->table[cur_ptr]->indicies_tab = new VERT_INDEX[ind_count];	// Alokujemy now¹ tablicê indeksów
	// Tablica zwalniana w funkcji EndEdit_index_buffer_processing()
	memcpy( tmp_data->table[cur_ptr]->indicies_tab, buffer, sizeof(VERT_INDEX)*ind_count );	// Przepisujemy tablicê

	switch ( vertex_buffer_offset )
	{
		case VERTEX_BUFFER_OFFSET::BEGIN:
			tmp_data->table[cur_ptr]->indicies_offset = 0;
			break;
		case VERTEX_BUFFER_OFFSET::LAST:
			tmp_data->table[cur_ptr]->indicies_offset = get_buffer_offset_to_last();
			break;
	}

	return WRONG_ID;
}

//----------------------------------------------------------------------------------------------//
//								funkcje pomocnicze												//
//----------------------------------------------------------------------------------------------//

/**@brief Zwraca offset o jaki trzeba przesun¹æ bufor indeksów wzglêdem pocz¹tka
bufora wierzcho³ków, je¿eli podano opcjê VERTEX_BUFFER_OFFSET::LAST.
Oznacza to zliczenie wszystkich wierzcho³ków we wszystkich poprzedzaj¹cych
buforach wierzcho³ków.

@note Funkcja do wywo³ywania wewêtrznego przez Model3DFromFile.
*/
unsigned int Model3DFromFile::get_buffer_offset_to_last( )
{
	unsigned int offset = 0;
	unsigned int max_ptr = 0;
	// Znajdujemy ostatni bufor wierzcho³ków istniej¹cy w tablicy
	for ( unsigned int i = tmp_data->current_pointer; i >= 0; --i )
		if ( tmp_data->table[i]->vertices_tab != nullptr )
			max_ptr = i;

	// Sumujemy liczbê wierzcho³ków we wszystkich wczeœniejszych buforach (ostatniego nie wliczaj¹c)
	for ( unsigned int i = 0; i < max_ptr; ++i )
		offset += tmp_data->table[tmp_data->current_pointer]->vertices_count;

	return offset;
}


/**@brief Tworzymy z rozproszonych buforów wierzcho³ków jeden wiêkszy.
Nastêpnie tworzymy obiekt bufora i dodajemy go do ModelsManagera
oraz przypisujemy zmiennej vertex_buffer.

@note Funkcja do wywo³ywania wewêtrznego przez Model3DFromFile.*/
void Model3DFromFile::EndEdit_vertex_buffer_processing( )
{
	// Podliczamy ile miejsca potrzeba dla bufora wierzcho³ków
	unsigned int vertex_buffer_length = 0;

	for ( int i = 0; i < tmp_data->current_pointer; ++i )	// Bufor wierzcho³ków
		vertex_buffer_length += tmp_data->table[i]->vertices_count;

	if ( vertex_buffer_length == 0 )
		return;			// Nie ma bufora wierzcho³ków. Sytuacja jest chora, ale jak ktoœ poda³ taki model, to jego sprawa.

	// Przepisujemy bufor wierzcho³ków
	VertexNormalTexCord1* verticies = new VertexNormalTexCord1[vertex_buffer_length];
	PtrOffset offset = 0;
	for ( int i = 0; i < tmp_data->current_pointer; ++i )
	{
		// verticies + offset - tu jest stosowana arytmetyka dla wskaŸników, a nie zwyk³e dodawanie
		VertexNormalTexCord1* dest_ptr = verticies + offset;
		memcpy( dest_ptr, tmp_data->table[i]->vertices_tab, sizeof(VertexNormalTexCord1)*tmp_data->table[i]->vertices_count );
		
		// Je¿eli nie ma bufora indeksów to zapisujemy offset wzglêdem pocz¹tku bufora wierzcho³ków
		if ( !tmp_data->table[i]->indicies_tab )
			tmp_data->table[i]->new_part.mesh->buffer_offset = offset;

		offset += tmp_data->table[i]->vertices_count;

		delete[] tmp_data->table[i]->vertices_tab;		// Tablica alokowana w add_vertex_buffer()
	}
	// Tworzymy obiekt bufora wierzcho³ków i go zapisujemy
	vertex_buffer = models_manager->AddVertexBuffer( m_filePath, verticies, sizeof( VertexNormalTexCord1 ), vertex_buffer_length );
	vertex_buffer->AddAssetReference( );		// Zaznaczamy, ¿e siê do niego odwo³ujemy
	
	
	delete[] verticies;							// Bufor by³ tylko tymczasowy
	// Nie sprawdzi³em czy pod tak¹ œcie¿k¹ ju¿ czegoœ nie by³o. Nie mia³o prawa byæ, bo 
	// bufory wierzcho³ków nie s¹ wspó³dzielone miedzy plikami. Jednak je¿eli pojawi³by siê
	// jakiœ bli¿ej nieokreœlony b³¹d (którego trochê nie umiem sobie wyobraziæ), to takie coœ mo¿e
	// doprowadziæ do wycieków pamiêci. Z tego wzglêdu je¿eli kiedykolwiek bufory bêd¹ wspó³dzielone
	// z innymi plikami, to trzeba to zmodyfikowaæ.
}

/**@brief Scalamy tymczasowe bufory indeksów w jeden du¿y

@note Funkcja do wywo³ywania wewêtrznego przez Model3DFromFile.*/
void Model3DFromFile::EndEdit_index_buffer_processing( )
{
	// Podliczamy ile miejsca potrzeba dla bufora indeksów, a ile dla bufora wierzcho³ków
	unsigned int index_buffer_length = 0;

	for ( int i = 0; i < tmp_data->current_pointer; ++i )	// Bufor indeksów
		index_buffer_length += tmp_data->table[i]->indicies_count;

	if ( index_buffer_length == 0 )
		return;		// NajwyraŸniej nie ma bufora indeksów

	// Przepisujemy bufor indeksów
	VERT_INDEX* indicies = new VERT_INDEX[index_buffer_length];
	PtrOffset offset = 0;
	for ( int i = 0; i < tmp_data->current_pointer; ++i )
	{
		// verticies + offset - tu jest stosowana arytmetyka dla wskaŸników, a nie zwyk³e dodawanie
		memcpy( indicies + offset, tmp_data->table[i]->indicies_tab, sizeof(VERT_INDEX)*tmp_data->table[i]->indicies_count );

		// Je¿eli jest bufor indeksów, to zapisujemy offset wzglêdem pocz¹tku tego bufora
		if ( tmp_data->table[i]->indicies_tab )
			tmp_data->table[i]->new_part.mesh->buffer_offset = offset;

		offset += tmp_data->table[i]->indicies_count;

		delete[] tmp_data->table[i]->indicies_tab;		// Tablica alokowana w add_index_buffer()
		// UWAGA!! Nie przypisujemy nullptra, bo chcemy wiedzieæ, ¿e jest bufor indeksów !!!!
	}
	// Tworzymy obiekt bufora indeksów i go zapisujemy
	index_buffer = models_manager->AddIndexBuffer( m_filePath, indicies, sizeof( VERT_INDEX ), index_buffer_length );
	index_buffer->AddAssetReference( );		// Zaznaczamy, ¿e siê do niego odwo³ujemy

	delete[] indicies;							// Bufor by³ tylko tymczasowy
	// Nie sprawdzi³em czy pod tak¹ œcie¿k¹ ju¿ czegoœ nie by³o. Komentarz ten sam co w funkcji EndEdit_vertex_buffer_processing
}


/**@brief Dodajemy do strutkury ModelPart dane o buforach wierzcho³ków oraz przypisujemy wbudowane
shadery, je¿eli nie podano ¿adnych.
Dodajemy te¿ domyœlny materia³, je¿eli nie ma.

@note Funkcja do wywo³ywania wewêtrznego przez Model3DFromFile.*/
void Model3DFromFile::EndEdit_prepare_ModelPart( )
{
	for ( int i = 0; i < tmp_data->current_pointer; ++i )
	{
		ModelPart& part = tmp_data->table[i]->new_part;

		// Je¿eli istnieje bufor ideksów, to dzia³amy w trybie indeksowanym
		if ( tmp_data->table[i]->indicies_tab )
		{
			part.mesh->use_index_buf = true;		// U¿ywamy bufora wierzcho³ków
			part.mesh->vertices_count = tmp_data->table[i]->indicies_count;	// Liczba wierzcho³ków z bufora do wyœwietlenia
			part.mesh->base_vertex = tmp_data->table[i]->indicies_offset;	// Offset wzglêdem pocz¹tku bufora wierzcho³ków
			// Do ka¿dego indeksu z tablicy zostanie dodana ta wartoœæ i dopiero potem odczytany wierzcho³ek

			// buffer_offset jest ju¿ za³atwiony w funkcjach EndEdit_index_buffer_processing i EndEdit_vertex_buffer_processing
		}
		else // W przeciwnym razie jest tryb bezpoœredni
		{
			part.mesh->use_index_buf = false;		// Co prawda tak jest domyœlnie, ale nie zaszkodzi jawnie to napisaæ
			part.mesh->vertices_count = tmp_data->table[i]->vertices_count;

			// buffer_offset jest ju¿ za³atwiony w funkcjach EndEdit_index_buffer_processing i EndEdit_vertex_buffer_processing
		}

		// Teraz trzeba dopisaæ shadery
		if ( !part.pixel_shader )
			part.pixel_shader = models_manager->FindBestPixelShader( part.texture );
		if ( !part.vertex_shader )
			part.vertex_shader = models_manager->FindBestVertexShader( part.texture );
		// I materia³
		if ( part.material == nullptr )
			part.material = models_manager->m_material.get( DEFAULT_MATERIAL_STRING );
	}

}


//----------------------------------------------------------------------------------------------//
//									Funkcje do korzystania z obiektów							//
//----------------------------------------------------------------------------------------------//

/**@brief zwraca wskaŸnik na ModelPart spod podanego indeksu.
@param[in] Indeks.
@return WskaŸnik na ModelPart lub nullptr, je¿eli indeks by³ nieprawid³owy.*/
const ModelPart* Model3DFromFile::get_part( unsigned int index )
{
	if ( index < model_parts.size( ) )
		return &model_parts[index];
	return nullptr;
}

/**@brief Zwraca liczbê obiektów ModelPart w tablicy.*/
Size Model3DFromFile::get_parts_count()
{
	return model_parts.size();
}

/**@brief */
std::string Model3DFromFile::GetResourceName() const
{
	return Converters::ToString( m_filePath );
}


//====================================================================================//
//			Serializacja	
//====================================================================================//

/**@brief Domyœlna serializacja.

@todo W zasadzie domyœlny tryb serializacji powinna implementowaæ klasa EngineObject.*/
void Model3DFromFile::Serialize( ISerializer* ser ) const
{
	Serialization::DefaultSerialize( ser, this );
}

void Model3DFromFile::Deserialize( IDeserializer* deser )
{ }


