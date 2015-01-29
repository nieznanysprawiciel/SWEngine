#include "..\..\stdafx.h"

#include "meshes_textures_materials.h"
#include "ModelsManager.h"

ModelsManager* Model3DFromFile::models_manager = nullptr;




//-------------------------------------------------------------------------------//
//							wersja DirectX11
//-------------------------------------------------------------------------------//




//----------------------------------------------------------------------------------------------//
//									referenced_object											//
//----------------------------------------------------------------------------------------------//

//==============================================================================================//


referenced_object::referenced_object(int id)
{
	file_references = 0;
	object_references = 0;
	unique_id = id;
}


referenced_object::~referenced_object()
{}

bool referenced_object::can_delete(unsigned int& file_ref, unsigned int& other_ref)
{
	file_ref = file_references;
	other_ref = object_references;

	if (file_references == 0 && object_references == 0)
		return TRUE;
	return FALSE;
}

//----------------------------------------------------------------------------------------------//
//									Model3DFromFile												//
//----------------------------------------------------------------------------------------------//

//==============================================================================================//


//----------------------------------------------------------------------------------------------//
//								contructor, destructor											//
//----------------------------------------------------------------------------------------------//

Model3DFromFile::Model3DFromFile( const std::wstring& file_name )
: referenced_object( WRONG_ID )
{
	file_path = file_name;
	vertex_buffer = nullptr;
	tmp_data = nullptr;
}


Model3DFromFile::~Model3DFromFile( )
{
	if ( tmp_data )
	{
		delete[] tmp_data->table;
		delete tmp_data;
		tmp_data = nullptr;
	}


	//najpierw kasujemy odwo³ania do obiektów
	for ( unsigned int k = 0; k < model_parts.size( ); ++k )
	{//ka¿dy element mo¿e byæ nullptrem
		register ModelPart* part = &model_parts[k];

		//ka¿dy element mo¿e byæ nullptrem
		if ( part->material )
			part->material->delete_file_reference( );
		if ( part->index_buffer )
			part->index_buffer->delete_file_reference( );
		if ( part->mesh )
		{
			//kasujemy obiekty MeshParts (to s¹ jedyne, którymi zarz¹dza ta klasa)
			part->mesh->delete_file_reference();
			delete part->mesh;
		}
		if ( part->pixel_shader )
			part->pixel_shader->delete_file_reference( );
		for ( int i = 0; i < ENGINE_MAX_TEXTURES; ++i )
			if ( part->texture )
				part->texture[i]->delete_file_reference( );
		if ( part->vertex_shader )
			part->vertex_shader->delete_file_reference( );
	}


	model_parts.clear( );
}




//----------------------------------------------------------------------------------------------//
//									Funkcje do tworzenia obiektu								//
//----------------------------------------------------------------------------------------------//


/*Funkcje BeginEdit i EndEdit maj¹ byæ wywo³ane przez klasê ModelsManager przed podaniem do loadera
i po zakoñczeniu wpisywania danych.

BeginEdit przygotowuje strukturê pod wpisywanie danych.*/
void Model3DFromFile::BeginEdit()
{
	tmp_data = new EditTMP;
	//Inicjujemy tablicê o pocz¹tkowej d³ugoœci domyœ³nej
	tmp_data->table = new TMP_data*[tmp_data->table_length];
}


/*Funkcje BeginEdit i EndEdit maj¹ byæ wywo³ane przez klasê ModelsManager przed podaniem do loadera
i po zakoñczeniu wpisywania danych.

EndEdit zatwierdza wpisane dane, tworzy w³aœciwa strukturê i wpisuje nie istniej¹ce jeszcze obiekty
do ModelsManagera.*/
void Model3DFromFile::EndEdit()
{
	//ró¿ne rzeczy

	//zwalniamy pamiêæ
	delete[] tmp_data->table;
	delete tmp_data;
	tmp_data = nullptr;
}


/*Funkcje BeginPart i EndPart s¹ wywo³ywane przez loader i maj¹ otaczaæ wszystkie instrukcje dodaj¹ce
jedn¹ czêœæ mesha do struktury.*/
void Model3DFromFile::BeginPart()
{
	//sprawdzamy czy tablica nam siê nie skoñczy³a
	if ( tmp_data->current_pointer <= tmp_data->table_length )
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
	tmp_data->table[tmp_data->current_pointer]->new_part.mesh->add_file_reference();
}

/*Funkcje BeginPart i EndPart s¹ wywo³ywane przez loader i maj¹ otaczaæ wszystkie instrukcje dodaj¹ce
jedn¹ czêœæ mesha do struktury.*/
void Model3DFromFile::EndPart()
{
	//inkrementujemy aktualny index wewn¹trz tablicy
	tmp_data->current_pointer++;
}



/*Dodaje teksturê na aktualnej pozycji w tablicy.
Tekstura jest umieszczana zgodnie z indeksem reprezentowanym przez type.*/
unsigned int Model3DFromFile::add_texture( const std::wstring& file_name, TEXTURES_TYPES type )
{
	// Ktoœ móg³ podaæ indeks w tablicy zamiast jednego z enumów, wiêc musimy pilnowaæ czy nie przekroczy³ zakresu
	if ( type > ENGINE_MAX_TEXTURES )
		return WRONG_ID;

	TextureObject* texture = models_manager->texture.get( file_name );
	if ( !texture )
	{
		// Nie by³o tekstury, trzeba j¹ stworzyæ i dodaæ
		texture = TextureObject::create_from_file( file_name );
		if ( !texture )		// Tekstura mog³a mieæ z³y format, a nie chcemy dodawaæ nullptra do ModelsManagera
			return WRONG_ID;

		models_manager->texture.unsafe_add( file_name, texture );	// Dodaliœmy teksturê
	}

	// Teraz musimy dodaæ teksturê na odpowiednie miejsce w tablicy
	tmp_data->table[tmp_data->current_pointer]->new_part.texture[type] = texture;

	// Zwracamy id tekstury, ale w zasadzie tylko po to, ¿eby ktoœ móg³ sprawdziæ czy wszystko posz³o dobrze
	return texture->get_id();
}

unsigned int Model3DFromFile::add_material( const MaterialObject* material, const std::wstring& material_name )
{
	std::wstring name = file_path;
	name += L"::";
	name += material_name;

	MaterialObject* new_material = models_manager->material.get( name );
	if ( !new_material )
	{
		// Nie by³o materia³u, trzeba j¹ stworzyæ i dodaæ
		new_material = new MaterialObject( material );

		models_manager->material.unsafe_add( name, new_material );	// Dodaliœmy teksturê
	}

	// Teraz musimy dodaæ materia³ na odpowiednie miejsce w tablicy
	tmp_data->table[tmp_data->current_pointer]->new_part.material = new_material;

	// Zwracamy id materia³y, ale w zasadzie tylko po to, ¿eby ktoœ móg³ sprawdziæ czy wszystko posz³o dobrze
	// Akurat w przypadku materia³ów nic nie mo¿e pójœæ Ÿle
	return new_material->get_id( );
}

unsigned int Model3DFromFile::add_vertex_shader( const std::wstring& file_name )
{
	VertexShaderObject* vertex_shader = models_manager->vertex_shader.get( file_name );
	if ( !vertex_shader )
	{
		// Nie by³o shadera, trzeba go stworzyæ i dodaæ
		vertex_shader = VertexShaderObject::create_from_file( file_name, "VS" );
		if ( !vertex_shader )		// shader móg³ mieæ z³y format, a nie chcemy dodawaæ nullptra do ModelsManagera
			return WRONG_ID;

		models_manager->vertex_shader.unsafe_add( file_name, vertex_shader );	// Dodaliœmy teksturê
	}

	// Teraz musimy dodaæ teksturê na odpowiednie miejsce w tablicy
	tmp_data->table[tmp_data->current_pointer]->new_part.vertex_shader = vertex_shader;

	// Zwracamy id tekstury, ale w zasadzie tylko po to, ¿eby ktoœ móg³ sprawdziæ czy wszystko posz³o dobrze
	return vertex_shader->get_id( );
}

unsigned int Model3DFromFile::add_pixel_shader( const std::wstring& file_name )
{
	PixelShaderObject* pixel_shader = models_manager->pixel_shader.get( file_name );
	if ( !pixel_shader )
	{
		// Nie by³o tekstury, trzeba j¹ stworzyæ i dodaæ
		pixel_shader = PixelShaderObject::create_from_file( file_name, "PS" );
		if ( !pixel_shader )		// Tekstura mog³a mieæ z³y format, a nie chcemy dodawaæ nullptra do ModelsManagera
			return WRONG_ID;

		models_manager->pixel_shader.unsafe_add( file_name, pixel_shader );	// Dodaliœmy teksturê
	}

	// Teraz musimy dodaæ teksturê na odpowiednie miejsce w tablicy
	tmp_data->table[tmp_data->current_pointer]->new_part.pixel_shader = pixel_shader;

	// Zwracamy id tekstury, ale w zasadzie tylko po to, ¿eby ktoœ móg³ sprawdziæ czy wszystko posz³o dobrze
	return pixel_shader->get_id( );
}


/*Funkcja dodaje do tymczasowej tablicy bufor wierzcho³ków.
Podane dane s¹ przepisywane, a za zwolnienie pamiêci odpowiada ten kto wywo³a³ funkcjê.

Bufory wierzcho³ków s¹ ³¹czone w jedna ca³oœæ w funkcji EndEdit.*/
unsigned int Model3DFromFile::add_vertex_buffer( const VertexNormalTexCord1* buffer, unsigned int vert_count )
{
	if ( vert_count == 0 )
		return WRONG_ID;		// Nie damy siê zrobiæ w balona

	auto data = tmp_data->table[tmp_data->current_pointer];

	if ( data->vertices_tab )
		return WRONG_ID;		// Je¿eli jakaœ tablica ju¿ istania³a, to ktoœ pope³ni³ b³¹d

	data->vertices_count = vert_count;							// Zapisujemy liczbê wierzcho³ków
	data->vertices_tab = new VertexNormalTexCord1[vert_count];	// Tworzymy now¹ tablice wierzcho³ków
	memcpy( data->vertices_tab, buffer, vert_count );			// Przepisujemy tablicê

	return WRONG_ID;
}


/*Ka¿da czêœæ mesha ma przypisan¹ do siebie transformacjê, któr¹ dodaje w³asnie ta funkcja.
Je¿eli dla jakiegoœ bufora wierzcho³ków nie zostanie przydzielona transformacja, to 
domyslnie znajduje siê tu macierz identycznoœciowa.*/
void Model3DFromFile::add_transformation( const DirectX::XMFLOAT4X4& transform )
{
	tmp_data->table[tmp_data->current_pointer]->new_part.mesh->transform_matrix = transform;
}


/*Funkcja dodaje do tymczasowej tablicy dane potrzebne do stworzenia bufora wierzcho³ków.
Indeksy s¹ przepsiywane z podanej tablicy i trzeba j¹ zwolniæ samemu.

Je¿eli tablica indeksów zosta³a wczeœniej stworzona, zostanie zwrócona wartoœæ
WRONG_ID. W przeciwnym razie zostanie zwrócona wartoœæ 1.*/
unsigned int Model3DFromFile::add_index_buffer( const unsigned int* buffer, unsigned int ind_count, int vertex_buffer_offset )
{
	if ( ind_count == 0 )
		return WRONG_ID;		// Nie damy siê zrobiæ w balona

	// Bufor indeksów przepisujemy do tablicy tymczasowej. Zostanie on potem scalony z reszt¹ buforów
	// indeksów w funkcji EndEdit.
	unsigned int cur_ptr = tmp_data->current_pointer;

	if ( tmp_data->table[cur_ptr]->indicies_tab )
		return WRONG_ID;		// Je¿eli jakaœ tablica ju¿ istania³a, to ktoœ pope³ni³ b³¹d

	tmp_data->table[cur_ptr]->indicies_count = ind_count;			// Przepisujemy liczbê indeksów
	tmp_data->table[cur_ptr]->indicies_tab = new unsigned int[ind_count];	// Alokujemy now¹ tablicê indeksów
	memcpy( tmp_data->table[cur_ptr]->indicies_tab, buffer, ind_count );	// Przepisujemy tablicê

	switch ( vertex_buffer_offset )
	{
		case VERTEX_BUFFER_OFFSET::BEGIN:
			tmp_data->table[cur_ptr]->indicies_offset = 0;
			break;
		case VERTEX_BUFFER_OFFSET::LAST:
			tmp_data->table[cur_ptr]->indicies_offset = get_buffer_offset_to_last();
			break;
	}

	return 1;
}

//----------------------------------------------------------------------------------------------//
//								funkcje pomocnicze												//
//----------------------------------------------------------------------------------------------//

/*Zwraca offset o jaki trzeba przesun¹c bufor indeksów wzglêdem pocz¹tka
bufora wierzcho³ków, je¿eli podano opcjê VERTEX_BUFFER_OFFSET::LAST.
Oznacza to zliczenie wszystkich wierzcho³ków we wszystkich poprzedzaj¹cych
buforach wierzcho³ków.*/
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


//----------------------------------------------------------------------------------------------//
//									Funkcje do korzystania z obiektów							//
//----------------------------------------------------------------------------------------------//


const ModelPart* Model3DFromFile::get_part( unsigned int index )
{
	if ( index < model_parts.size( ) )
		return &model_parts[index];
	return nullptr;
}


unsigned int Model3DFromFile::get_parts_count()
{
	return model_parts.size();
}



#ifndef __UNUSED

//-------------------------------------------------------------------------------//
//							wersja DirectX 9
//-------------------------------------------------------------------------------//


//----------------------------------------------------------------------------------------------//
//								contructor, destructor											//
//----------------------------------------------------------------------------------------------//

Model3DFromFile::Model3DFromFile( int id )
: referenced_object( id )
{
}


Model3DFromFile::~Model3DFromFile( )
{
	//najpierw kasujemy odwo³ania do obiektów
	for ( unsigned int i = 0; i < mesh_parts.size( ); ++i )
		mesh_parts[i]->mesh_object->delete_file_reference( );
	for ( unsigned int i = 0; i < textures.size( ); ++i )
		textures[i]->delete_file_reference( );;
	for ( unsigned int i = 0; i < materials.size( ); ++i )
		materials[i]->delete_file_reference( );

	//kasujemy obiekty MeshParts (to s¹ jedyne, którymi zarz¹dza ta klasa)
	for ( unsigned int i = 0; i < mesh_parts.size( ); ++i )
		delete mesh_parts[i];
}



//----------------------------------------------------------------------------------------------//
//									building models												//
//----------------------------------------------------------------------------------------------//


unsigned int Model3DFromFile::add_material( const D3DMATERIAL9 &material )
{
	MaterialObject* result;
	unsigned int id;
	result = models_manager->add_material( material, id );

	if ( result == nullptr )
		return WRONG_MATERIAL_ID;

	materials.push_back( result );
	result->add_file_reference( );		//pamietajmy o dodawaniu odwo³añ

	return id;
}

/*Je¿eli jakis materia³ zosta³ dodany bezpoœrednio do ModelsManagera, to mo¿na siê do niego odwo³ywaæ po id.
Zwraca nullptr je¿eli nie istnieje taki materia³ lub wskaŸnik na ten materia³.

Materia³ nie zostaje dodany do listy, je¿eli nie istnia³!!!!!!!!!!*/
MaterialObject* Model3DFromFile::add_material( unsigned int id )
{
	MaterialObject* result = models_manager->get_material( id );
	if ( result != nullptr )
	{
		materials.push_back( result );
		result->add_file_reference( );		//pamietajmy o dodawaniu odwo³añ
	}

	return result;
}

/*Niektóre kawa³ki mesha mog¹ nie mieæ materia³u.*/
void Model3DFromFile::add_null_material( )
{
	materials.push_back( nullptr );
}


unsigned int Model3DFromFile::add_texture( const std::wstring& path )
{
	TextureObject* result;
	unsigned int id;
	result = models_manager->add_texture( path, id );

	if ( result == nullptr )
		return WRONG_TEXTURE_ID;

	textures.push_back( result );
	result->add_file_reference( );		//pamietajmy o dodawaniu odwo³añ

	return id;
}


/*Je¿eli jakaœ tekstura zosta³a dodana bezpoœrednio do ModelsManagera, to mo¿na siê do niej odwo³ywaæ po id.
Zwraca nullptr je¿eli nie istnieje taka tekstura lub wskaŸnik na teksture, je¿eli istnieje.

Tekstura nie zostaje dodana do listy je¿eli nie istnia³a!!!!!!*/
TextureObject* Model3DFromFile::add_texture( unsigned int id )
{
	TextureObject* result = models_manager->get_texture( id );
	if ( result != nullptr )
	{
		textures.push_back( result );
		result->add_file_reference( );		//pamietajmy o dodawaniu odwo³añ
	}

	return result;
}

/*Niektóre kawa³ki mesha mog¹ nie mieæ tekstury.*/
void Model3DFromFile::add_null_texture( )
{
	textures.push_back( nullptr );
}


MeshPart* Model3DFromFile::add_mesh( unsigned int id, DirectX::XMFLOAT4X4& matrix )
{
	MeshObject* result = models_manager->get_mesh( id );
	if ( result == nullptr )
		return nullptr;

	MeshPart* new_mesh_part = new MeshPart;
	new_mesh_part->transform_matrix = matrix;
	new_mesh_part->mesh_object = result;

	mesh_parts.push_back( new_mesh_part );
	result->add_file_reference( );		//pamietajmy o dodawaniu odwo³añ

	return new_mesh_part;
}


/*Dodajemy wierzcho³ki do listy modelu z danego pliku, przypisuj¹c mu podan¹ w ostatnim parametrze macierz przekszta³cenia.
Mesh jest na pocz¹tku dodawany do ModelsManagera, je¿eli ju¿ istnia³, nie zostaje zduplikowany.*/
unsigned int Model3DFromFile::add_mesh( Vertex_Normal_TexCords1* vertices, unsigned int vert_num, const DirectX::XMFLOAT4X4& matrix )
{
	MeshObject* result;
	unsigned int id;
	result = models_manager->add_mesh( vertices, vert_num, id );
	if ( result == nullptr )	return WRONG_MESH_ID;		//coœ posz³o nie tak

	MeshPart* mesh_part = new MeshPart( );
	mesh_part->mesh_object = result;
	mesh_part->transform_matrix = matrix;

	mesh_parts.push_back( mesh_part );

	result->add_file_reference( );		//pamiêtajmy o dodawaniu odwo³añ

	return id;
}

void Model3DFromFile::add_reference( )
{
	++references;
}

void Model3DFromFile::delete_reference( )
{
	--references;
}

/*Sprawdza czy s¹ jakieœ odwo³ania do podanego pliku. Je¿eli nie ma to znaczy, ¿e obiekt mo¿e
zostac bezpiecznie usuniêty.*/
bool Model3DFromFile::can_delete( unsigned int& ref )
{
	if ( references > 0 )
		return FALSE;
	return TRUE;
}

unsigned int Model3DFromFile::get_parts_count( )
{
	unsigned int size1 = materials.size( );
	unsigned int size2 = textures.size( );
	unsigned int size3 = mesh_parts.size( );

	unsigned int min = size1 < size2 ? size1 : size2;
	return min < size3 ? min : size3;
}

MaterialObject* Model3DFromFile::get_material( unsigned int index )
{
	if ( index < materials.size( ) )
		return materials[index];
	return nullptr;
}

TextureObject* Model3DFromFile::get_texture( unsigned int index )
{
	if ( index < textures.size( ) )
		return textures[index];
	return nullptr;
}

MeshPart* Model3DFromFile::get_mesh_part( unsigned int index )
{
	if ( index < mesh_parts.size( ) )
		return mesh_parts[index];
	return nullptr;
}

#endif