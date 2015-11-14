#include "EngineCore/stdafx.h"
#include "ModelsManager.h"
#include "EngineCore/MainEngine/Engine.h"
#include "Loaders/ILoader.h"
#include "Loaders/FBX_files_loader/FBX_loader.h"
#include "Common/ObjectDeleter.h"
#include "GraphicAPI/ResourcesFactory.h"
#include "Common/macros_switches.h"

#include "Common/memory_leaks.h"

using namespace DirectX;

//-------------------------------------------------------------------------------//
//							wersja DirectX11
//-------------------------------------------------------------------------------//

ModelsManager::ModelsManager( Engine* engine )
: m_engine( engine )
{
	Model3DFromFile::models_manager = this;

	//Loader plik�w FBX jest domy�lnym narz�dziem do wczytywania
	FBX_loader* fbx_loader = new FBX_loader( this );
	m_loader.push_back( fbx_loader );
}



ModelsManager::~ModelsManager( )
{
	// Kasujemy obiekty do wczytywania danych
	for ( unsigned int i = 0; i < m_loader.size(); ++i )
		delete m_loader[i];
}



#ifdef __TEST
void ModelsManager::test( )
{
	
	//stworzymy sobie prosty obiekt do wy�wietlania
	VertexNormalTexCord1 g_Vertices[] =
	{
	// front
	{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 3.0f) },
	{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(3.0f, 3.0f) },

	{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(3.0f, 0.0f) },
	{ XMFLOAT3( 1.0f, -1.0f, -1.0f ), XMFLOAT3( 0.0f, 0.0f, -1.0f ), XMFLOAT2( 3.0f, 3.0f ) },

	// right
	{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },

	{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3( 1.0f, -1.0f, 1.0f ), XMFLOAT3( 1.0f, 0.0f, 0.0f ), XMFLOAT2( 1.0f, 1.0f ) },

	// back
	{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },

	{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT3( 0.0f, 0.0f, 1.0f ), XMFLOAT2( 1.0f, 1.0f ) },

	// left/ left
	{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },

	{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3( -1.0f, -1.0f, -1.0f ), XMFLOAT3( -1.0f, 0.0f, 0.0f ), XMFLOAT2( 1.0f, 1.0f ) },

	// top/ top
	{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },

	{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3( 1.0f, 1.0f, -1.0f ), XMFLOAT3( 0.0f, 1.0f, 0.0f ), XMFLOAT2( 1.0f, 1.0f ) },

	// bootom/ bootom
	{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },

	{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3( 1.0f, -1.0f, 1.0f ), XMFLOAT3( 0.0f, -1.0f, 0.0f ), XMFLOAT2( 1.0f, 1.0f ) },
	};

	
	MaterialObject material;

//#define mat (*material)
#define mat material

	// Set the RGBA for diffuse reflection.
	mat.Diffuse.x = 0.5f;
	mat.Diffuse.y = 0.5f;
	mat.Diffuse.z = 0.67f;
	mat.Diffuse.w = 1.0f;

	// Set the RGBA for ambient reflection.
	mat.Ambient.x = 0.3f;
	mat.Ambient.y = 0.5f;
	mat.Ambient.z = 0.3f;

	// Set the color and sharpness of specular highlights.
	mat.Specular.x = 1.0f;
	mat.Specular.y = 1.0f;
	mat.Specular.z = 1.0f;
	mat.Power = 50.0f;

	// Set the RGBA for emissive color.
	mat.Emissive.x = 0.0f;
	mat.Emissive.y = 0.0f;
	mat.Emissive.z = 0.0f;


	Model3DFromFile* new_model = new Model3DFromFile(L"skrzynia");
	new_model->BeginEdit();
	new_model->BeginPart();
	
	new_model->add_vertex_buffer( g_Vertices, 36 );
	new_model->add_material( &mat, L"::skrzynia_material" );
	//L"..\tylko_do_testow\tex.bmp"
	new_model->add_texture( L"..\tylko_do_testow\tex.bmp" );

	new_model->EndPart();
	new_model->EndEdit();
	/*
	new_model->vertex_buffer = BufferObject::create_from_memory( g_Vertices,
																 sizeof(VertexNormalTexCord1),
																 36,
																 D3D11_BIND_VERTEX_BUFFER );
	vertex_buffer.unsafe_add( L"skrzynia", new_model->vertex_buffer );


	ModelPart part;
	part.mesh = new MeshPartObject;
	part.mesh->vertices_count = 36;
	part.material = material;
	part.m_pixelShader = m_pixelShader.get( L"default_pixel_shader" );
	part.m_vertexShader = m_vertexShader.get( L"default_vertex_shader" );

	new_model->model_parts.push_back(part);*/



	m_fileModel.unsafe_add( L"skrzynia", new_model );


	LoadModelFromFile( L"tylko_do_testow/ARC.FBX" );
	LoadModelFromFile( L"tylko_do_testow/moon/moon.FBX" );
	LoadModelFromFile( L"tylko_do_testow/Nebulon/Nebulon.FBX" );
	LoadModelFromFile( L"tylko_do_testow/VadersTIE.FBX" );
	LoadModelFromFile( L"tylko_do_testow/TIE_Fighter/TIE_Fighter.FBX" );
}
#endif






/**@brief Dopasowuje najlepszy z domy�lnych shader�w, kt�ry pasuje do podanej tablicy
tekstur. Tablica ma tyle element�w ile zmienna @ref ENGINE_MAX_TEXTURES.

Ka�da pozycja w tablicy ma przypisane domy�lne znaczenie zgodnie z enumeracj� 
@ref TextureUse. Najlepszy shader jest wybierany na podstawie obecno�ci
lub nieobecno�ci tekstury w tablicy.

@todo Oddelegowa� jaki� inny obiekt do ob��ugi warto�ci domy�lnych albo przemy�le� lepiej jak to powinno w�a�ciwie wygl�da�.

@param[in] textures Tablica tekstur z obiektu ModelPart.
@return Zwraca obiekt vertex shadera.*/
VertexShaderObject* ModelsManager::FindBestVertexShader( TextureObject** textures )
{
	// Na razie nie mamy innych domy�lnych shader�w
	return m_vertexShader.get( DEFAULT_VERTEX_SHADER_STRING );
}


/**@brief Dopasowuje najlepszy z domy�lnych shader�w, kt�ry pasuje do podanej tablicy
tekstur. Tablica ma tyle element�w ile zmienna @ref ENGINE_MAX_TEXTURES.

Ka�da pozycja w tablicy ma przypisane domy�lne znaczenie zgodnie z enumeracj�
@ref TextureUse. Najlepszy shader jest wybierany na podstawie obecno�ci
lub nieobecno�ci tekstury w tablicy.

@todo Oddelegowa� jaki� inny obiekt do ob��ugi warto�ci domy�lnych albo przemy�le� lepiej jak to powinno w�a�ciwie wygl�da�.

@param[in] textures Tablica tekstur z obiektu ModelPart.
@return Zwraca obiekt pixel shadera.*/
PixelShaderObject* ModelsManager::FindBestPixelShader( TextureObject** textures )
{
	PixelShaderObject* return_shader = nullptr;

	// Na razie nie ma innych tekstur ni� diffuse, wi�c algorytm nie jest skomplikowany
	if ( textures[TextureUse::TEX_DIFFUSE] )
		return_shader = m_pixelShader.get( DEFAULT_TEX_DIFFUSE_PIXEL_SHADER_PATH );
	
	
	if ( !return_shader )	// Je�eli nadal jest nullptrem to dajemy mu domy�lny shader
		return_shader = m_pixelShader.get( DEFAULT_PIXEL_SHADER_STRING );

	return return_shader;
}


/** @brief Znajduje Loader pasuj�cy do pliku podanego w parametrze.
@param[in] path �cie�ka do pliku, dla kt�rej szukamy loadera.
@return Wska�nik na odpowiedni loader lub nullptr, je�eli nie znaleziono pasuj�cego.*/
ILoader* ModelsManager::FindLoader( const std::wstring& path )
{
	for ( unsigned int i = 0; i < m_loader.size( ); ++i )
	if ( m_loader[i]->can_load( path ) )
		return m_loader[i];
	return nullptr;
}


//-------------------------------------------------------------------------------//
//							funkcje do zarzadzania assetami
//-------------------------------------------------------------------------------//



/**@brief Wczytuje model z podanego pliku.
@param[in] file Plik do wczytania
@return Jedna z warto�ci @ref ModelsManagerResult. Funkcja mo�e zwr�ci� @ref ModelsManagerResult::MODELS_MANAGER_OK,
@ref ModelsManagerResult::MODELS_MANAGER_LOADER_NOT_FOUND lub @ref ModelsManagerResult::MODELS_MANAGER_CANNOT_LOAD.*/
ModelsManagerResult ModelsManager::LoadModelFromFile( const std::wstring& file )
{
	// Sprawdzamy czy plik nie zosta� ju� wczytany
	Model3DFromFile* new_model = m_fileModel.get( file );
	if ( new_model != nullptr )
		return ModelsManagerResult::MODELS_MANAGER_OK;	// Udajemy, �e wszystko posz�o dobrze

	// Sprawdzamy, kt�ry loader potrafi otworzy� plik
	ILoader* loader = FindLoader( file );
	if ( loader == nullptr )
		return ModelsManagerResult::MODELS_MANAGER_LOADER_NOT_FOUND;		// �aden nie potrafi

	// Tworzymy obiekt Model3DFromFile, do kt�rego loader wpisze zawarto�� pliku
	new_model = new Model3DFromFile( file );

	// Wszystkie operacje wpisywania musz� by� zamkni�te tymi wywo�aniami
	new_model->BeginEdit();
	LoaderResult result = loader->load_mesh( new_model, file );
	new_model->EndEdit();

	if ( result != LoaderResult::MESH_LOADING_OK )
	{	// load_mesh powinno zwr�ci� 0
		// Destruktor jest prywatny, wi�c nie mo�emy kasowa� obiektu bezpo�rednio.
		ObjectDeleter<Model3DFromFile>::delete_object( new_model, ObjectDeleterKey<Model3DFromFile>() );
		return ModelsManagerResult::MODELS_MANAGER_CANNOT_LOAD;
	}

	// Dodajemy model do tablic
	m_fileModel.unsafe_add( file, new_model );

	return ModelsManagerResult::MODELS_MANAGER_OK;
}


/**@brief Tworzy nowy render target.

Funkcja dodaje stworzony obiekt do tablicy m_renderTarget. Je�eli tekstury
bufora color�w, g��boko�ci i stencilu nie s� nullptrami, to i one s� dodawane do tablicy m_texture.

Tekstury te maj� nazwy jak render target + dodany jest cz�on
- ::color
- ::depth
- ::stencil

@todo Przy dodawaniu tekstur nie jest sprawdzane czy one ju� istniej�. Trzeba albo to sprawdza�, albo zapewni�
np. jak�� polityk� nazewnictwa, �e w ten spos�b nie nadpisujemy istniej�cej tekstury.

@param[in] name Nazwa identyfikuj�ca render target.
@param[in] renderTargetDescriptor Deskryptor opisuj�cy parametry render targetu.
@return Zwraca stworzony obiekt lub nullptr w przypadku niepowodzenia. Je�eli render target ju� istnia�, to zwracany jest istniej�cy obiekt.
*/
RenderTargetObject* ModelsManager::CreateRenderTarget( const std::wstring& name, const RenderTargetDescriptor& renderTargetDescriptor )
{
	RenderTargetObject* newRenderTarget = m_renderTarget.get( name );
	if( !newRenderTarget )
	{
		newRenderTarget = ResourcesFactory::CreateRenderTarget( name, renderTargetDescriptor );
		m_renderTarget.unsafe_add( name, newRenderTarget );
		
		auto colorBuff = newRenderTarget->GetColorBuffer();
		if( colorBuff )
			m_texture.unsafe_add( colorBuff->GetFileName(), colorBuff );

		auto depthBuffer = newRenderTarget->GetDepthBuffer();
		if( depthBuffer )
			m_texture.unsafe_add( depthBuffer->GetFileName(), depthBuffer );

		auto stencilBuffer = newRenderTarget->GetStencilBuffer();
		if( stencilBuffer )
			m_texture.unsafe_add( stencilBuffer->GetFileName(), stencilBuffer );
	}

	return newRenderTarget;
}



/**@brief Dodaje materia� do ModelsManagera, je�eli jeszcze nie istnia�.
@note Funkcja nie dodaje odwo�ania do obiektu, bo nie zak�ada, �e kto� go od razu u�yje.
W ka�dym miejscu, gdzie zostanie przypisany zwr�cony obiekt, nale�y pami�ta� o dodaniu odwo�ania oraz
skasowaniu go, gdy obiekt przestanie by� u�ywany.

@note Je�eli materia� ju� istnia� (jego nazwa), to ten podany w parametrze nie zostanie dodany.
Oznacza to, �e za jego zwolnienie odpowiada ten, kto go stworzy�. Trzeba zawsze sprawdzi� czy
zwr�cona warto�� jest tym samym co podali�my.

@todo Nie mo�e tak zosta�, �e kto� dodaje materia� i musi sprawdzi� czy nie dosta� innego. Nie mo�na
te� zmusza� kogo� do zwalniania pami�ci po materiale.

@param[in] material Materia�, kt�ry ma zosta� dodany
@param[in] materialName Nazwa materia�u. Do materia�u b�dzie mo�na si� odwo�a� podaj�c ci�g znak�w
[nazwa_pliku]::[nazwa_materia�u]. Oznacza to, �e mog� istnie� dwa takie same materia�y, poniewa� nie jest sprawdzana
zawarto��, a jedynie nazwy.
@return Zwraca wska�nik na dodany materia�.*/
MaterialObject* ModelsManager::AddMaterial( MaterialObject* addMaterial, const std::wstring& materialName )
{
	MaterialObject* newMaterial = m_material.get( materialName );
	if ( !newMaterial )
		m_material.unsafe_add( materialName, addMaterial );	// Dodali�my materia�

	return newMaterial;
}

/**@brief Dodaje renderTarget do ModelsManagera, je�eli jeszcze nie istnia�.
@note Funkcja nie dodaje odwo�ania do obiektu, bo nie zak�ada, �e kto� go od razu u�yje.
W ka�dym miejscu, gdzie zostanie przypisany zwr�cony obiekt, nale�y pami�ta� o dodaniu odwo�ania oraz
skasowaniu go, gdy obiekt przestanie by� u�ywany.

@note Je�eli renderTarget ju� istnia� (jego nazwa), to ten podany w parametrze nie zostanie dodany.
Oznacza to, �e za jego zwolnienie odpowiada ten, kto go stworzy�. Trzeba zawsze sprawdzi� czy
zwr�cona warto�� jest tym samym co podali�my.

@todo Nie mo�e tak zosta�, �e kto� dodaje renderTarget i musi sprawdzi� czy nie dosta� innego. Nie mo�na
te� zmusza� kogo� do zwalniania pami�ci po renderTargecie. Wog�le dodawanie renderTarget�w musi si� odbywa� jako� inaczej.
Najlepiej, �eby by�y one tworzone przez ModelsManager, ale wtedy trzeba wymy�le� spos�b dodawania renderTargetu zwi�zanego z buforem okna.

@param[in] renderTarget renderTarget, kt�ry ma zosta� dodany.
@param[in] name Nazwa renderTargetu. Do materia�u b�dzie mo�na si� odwo�a� podaj�c ci�g znak�w
@return Zwraca wska�nik na dodany renderTarget.*/
RenderTargetObject* ModelsManager::AddRenderTarget( RenderTargetObject* renderTarget, const std::wstring& name )
{
	RenderTargetObject* newRenderTarget = m_renderTarget.get( name );
	if ( !newRenderTarget )
		m_renderTarget.unsafe_add( name, renderTarget );	// Dodali�my materia�

	return newRenderTarget;
}

/**@brief Dodaje vertex shader do ModelsManagera. Je�eli obiekt ju� istnia�, to nie jest tworzony nowy.
@note Funkcja nie dodaje odwo�ania do obiektu, bo nie zak�ada, �e kto� go od razu u�yje.
W ka�dym miejscu, gdzie zostanie przypisany zwr�cony obiekt, nale�y pami�ta� o dodaniu odwo�ania oraz
skasowaniu go, gdy obiekt przestanie by� u�ywany.

@param[in] fileName Nazwa pliku, w kt�rym znajduje si� vertex shader.
@param[in] shaderEntry Nazwa funkcji od kt�rej ma si� zacz�� wykonywanie shadera.
@return Zwraca obiekt dodanego shadera. Zwraca nullptr, je�eli shadera nie uda�o si� skompilowa�.*/
VertexShaderObject* ModelsManager::AddVertexShader( const std::wstring& fileName, const std::string& shaderEntry )
{
	VertexShaderObject* shader = m_vertexShader.get( fileName );
	if ( !shader )
	{
		// Nie by�o shadera, trzeba go stworzy� i doda�
		shader = ResourcesFactory::CreateVertexShaderFromFile( fileName, shaderEntry );
		if ( !shader )		// shader m�g� mie� z�y format, a nie chcemy dodawa� nullptra do ModelsManagera
			return nullptr;

		m_vertexShader.unsafe_add( fileName, shader );	// Dodali�my tekstur�
	}

	return shader;
}

/**@brief Dodaje vertex shader do ModelsManagera. Je�eli obiekt ju� istnia�, to nie jest tworzony nowy.
Tworzy te� layout wierzcho�ka zwi�zany z tym shaderem i zwraca go w zmiennej layout.

Je�eli vertex shader wcze�niej istnia�, to stworzenie layoutu wymaga ponownego skompilowania shadera. Shader taki jest potem
kasowany i nie zostaje zdublowany w ModelsManagerze, ale niepotrzebna praca zostaje w�o�ona. Jest wi�c zadaniem programisty, �eby
do takich rzeczy dochodzi�o jak najrzadziej.

@note Funkcja nie dodaje odwo�ania do obiektu, bo nie zak�ada, �e kto� go od razu u�yje.
W ka�dym miejscu, gdzie zostanie przypisany zwr�cony obiekt, nale�y pami�ta� o dodaniu odwo�ania oraz
skasowaniu go, gdy obiekt przestanie by� u�ywany.

@param[in] fileName Nazwa pliku, w kt�rym znajduje si� vertex shader.
@param[in] shaderEntry Nazwa funkcji od kt�rej ma si� zacz�� wykonywanie shadera.
@param[out] layout W zmiennej umieszczany jest wska�nik na layout wierzcho�ka. Nawet je�eli shader si� nie skompilowa�, to pole mo�e mie� warto�� inn� ni� nullptr.
Dzieje si� tak wtedy, gdy layout istnia� ju� wcze�niej.
@attention Je�eli vertex shader wcze�niej istnia�, to stworzenie layoutu wymaga ponownego skompilowania shadera. Shader taki jest potem 
kasowany i nie zostaje zdublowany w ModelsManagerze, ale niepotrzebna praca zostaje w�o�ona. Jest wi�c zadaniem programisty, �eby
do takich rzeczy dochodzi�o jak najrzadziej.
@param[in] layoutDesc Deskryptor opisujacy tworzony layout.
@return Zwraca obiekt dodanego shadera. Zwraca nullptr, je�eli shadera nie uda�o si� skompilowa�.*/
VertexShaderObject* ModelsManager::AddVertexShader( const std::wstring& fileName,
													const std::string& shaderEntry,
													ShaderInputLayoutObject** layout,
													InputLayoutDescriptor* layoutDesc )
{
	/// @todo Ten kod to jaki� totalny shit. Jak komu� si� b�dzie nudzi�o kiedy� (ha ha), to mo�e niech poprawi.
	*layout = nullptr;
	VertexShaderObject* shader = m_vertexShader.get( fileName );
	VertexShaderObject* newShader = nullptr;
	ShaderInputLayoutObject* inputLayout = m_vertexLayout.get( layoutDesc->GetName() );


	// Tworzymy potrzebne obiekty
	if( !inputLayout )
	{
		// Tworzymy shader niezale�nie czy istnieje. Inaczej nie mogliby�my stworzy� layoutu.
		// Shader zostanie potem usuni�ty.
		newShader = ResourcesFactory::CreateVertexShaderFromFile( fileName, shaderEntry, layout, layoutDesc );
		if ( !newShader )		// shader m�g� mie� z�y format, a nie chcemy dodawa� nullptra do ModelsManagera
			return nullptr;		// layout te� jest nullptrem, nie trzeba si� martwi�.
	}
	else if( !shader )
	{
		// Layout istnieje, ale shader nie.
		newShader = ResourcesFactory::CreateVertexShaderFromFile( fileName, shaderEntry );
		*layout = inputLayout;	// Je�eli layout istnia�, to przepisujemy go na wyj�cie. Je�eli nie to i tak b�dzie nullptr.
		if ( !newShader )		// shader m�g� mie� z�y format, a nie chcemy dodawa� nullptra do ModelsManagera
			return nullptr;
	}
	else
	{// Wszystkie obiekty istania�y ju� wcze�niej.
		*layout = inputLayout;
		return shader;
	}

	// Nowo powsta�e obiekty musz� zosta� dodane do kontener�w.
	if ( !shader )
	{
		// Nie by�o shadera, trzeba go doda�
		m_vertexShader.unsafe_add( fileName, newShader );	// Dodali�my shader
		shader = newShader;
	}
	else
	{	// Shader ju� by�, wi�c kasujemy nowy
		// Destruktor jest prywatny, wi�c nie mo�emy kasowa� obiektu bezpo�rednio.
		ObjectDeleter<VertexShaderObject>::delete_object( shader, ObjectDeleterKey<VertexShaderObject>() );
	}

	if( !inputLayout )	// Layoutu nie by�o wcze�niej wi�c dodajemy.
		m_vertexLayout.unsafe_add( layoutDesc->GetName(), *layout );

	return shader;
}


/**@brief Dodaje pixel shader do ModelsManagera. Je�eli obiekt ju� istnia�, to nie jest tworzony nowy.
@note Funkcja nie dodaje odwo�ania do obiektu, bo nie zak�ada, �e kto� go od razu u�yje.
W ka�dym miejscu, gdzie zostanie przypisany zwr�cony obiekt, nale�y pami�ta� o dodaniu odwo�ania oraz
skasowaniu go, gdy obiekt przestanie by� u�ywany.

@param[in] fileName Nazwa pliku, w kt�rym znajduje si� pixel shader.
@param[in] shaderEntry Nazwa funkcji od kt�rej ma si� zacz�� wykonywanie shadera.
@return Zwraca obiekt dodanego shadera. Zwraca nullptr, je�eli shadera nie uda�o si� skompilowa�.*/
PixelShaderObject* ModelsManager::AddPixelShader( const std::wstring& fileName, const std::string& shaderEntry )
{
	PixelShaderObject* shader = m_pixelShader.get( fileName );
	if ( !shader )
	{
		// Nie by�o shadera, trzeba go stworzy� i doda�
		shader = ResourcesFactory::CreatePixelShaderFromFile( fileName, shaderEntry );
		if ( !shader )		// shader m�g� mie� z�y format, a nie chcemy dodawa� nullptra do ModelsManagera
			return nullptr;

		m_pixelShader.unsafe_add( fileName, shader );	// Dodali�my tekstur�
	}

	return shader;
}

/**@brief Dodaje tekstur� do ModelManagera, je�eli jeszcze nie istnia�a.
@note Funkcja nie dodaje odwo�ania do obiektu, bo nie zak�ada, �e kto� go od razu u�yje.
W ka�dym miejscu, gdzie zostanie przypisany zwr�cony obiekt, nale�y pami�ta� o dodaniu odwo�ania oraz
skasowaniu go, gdy obiekt przestanie by� u�ywany.

@param[in] fileName �cie�ka do tekstury

@return Zwraca wska�nik na dodan� tekstur� lub nullptr, je�eli nie da�o si� wczyta�.*/
TextureObject* ModelsManager::AddTexture( const std::wstring& fileName )
{

	TextureObject* tex = m_texture.get( fileName );
	if ( !tex )
	{
		// Nie by�o tekstury, trzeba j� stworzy� i doda�
		tex = ResourcesFactory::CreateTextureFromFile( fileName );
		if ( !tex )		// Tekstura mog�a mie� z�y format, a nie chcemy dodawa� nullptra do ModelsManagera
			return nullptr;

		m_texture.unsafe_add( fileName, tex );	// Dodali�my tekstur�
	}

	return tex;
}

/**@brief Dodaje do ModelsManagera bufor wierzcho�k�w.
Je�eli pod tak� nazw� istnieje jaki� bufor, to zostanie zwr�cony wska�nik na niego.
@note Funkcja nie dodaje odwo�ania do obiektu, bo nie zak�ada, �e kto� go od razu u�yje.
W ka�dym miejscu, gdzie zostanie przypisany zwr�cony obiekt, nale�y pami�ta� o dodaniu odwo�ania oraz
skasowaniu go, gdy obiekt przestanie by� u�ywany.

@param[in] name Nazwa bufora, po kt�rej mo�na si� b�dzie odwo�a�.
@param[in] buffer Wska�nik na bufor z danym, kt�re maj� by� przeniesione do bufora DirectXowego.
@param[in] elementSize Rozmiar pojedynczego elementu w buforze.
@param[in] vertCount Liczba wierzcho�k�w/indeks�w w buforze.
@return Dodany bufor wierzcho�k�w. Zwraca nullptr, je�eli nie uda�o si� stworzy� bufora.*/
BufferObject* ModelsManager::AddVertexBuffer( const std::wstring& name,
												const void* buffer,
												unsigned int elementSize,
												unsigned int vertCount )
{
	BufferObject* vertexBuff = m_vertexBuffer.get( name );
	if ( vertexBuff )	// Je�eli znale�li�my bufor, to go zwracamy
		return vertexBuff;

	// Tworzymy obiekt bufora indeks�w i go zapisujemy
	vertexBuff = ResourcesFactory::CreateBufferFromMemory( buffer, elementSize, vertCount, ResourceBinding::BIND_RESOURCE_VERTEX_BUFFER, ResourceUsage::RESOURCE_USAGE_DEFAULT );
	if ( !vertexBuff )		// Bufor m�g� si� nie stworzy�, a nie chcemy dodawa� nullptra do ModelsManagera
		return nullptr;

	m_vertexBuffer.unsafe_add( name, vertexBuff );	// Dodali�my bufor

	return vertexBuff;
}

/**@brief Dodaje do ModelsManagera bufor indeks�w.
Je�eli pod tak� nazw� istnieje jaki� bufor, to zostanie zwr�cony wska�nik na niego.
@note Funkcja nie dodaje odwo�ania do obiektu, bo nie zak�ada, �e kto� go od razu u�yje.
W ka�dym miejscu, gdzie zostanie przypisany zwr�cony obiekt, nale�y pami�ta� o dodaniu odwo�ania oraz
skasowaniu go, gdy obiekt przestanie by� u�ywany.

@param[in] name Nazwa bufora, po kt�rej mo�na si� b�dzie odwo�a�.
@param[in] buffer Wska�nik na bufor z danym, kt�re maj� by� przeniesione do bufora DirectXowego.
@param[in] elementSize Rozmiar pojedynczego elementu w buforze.
@param[in] vertCount Liczba wierzcho�k�w/indeks�w w buforze.
@return Dodany bufor indeks�w. Zwraca nullptr, je�eli nie uda�o si� stworzy� bufora.*/
BufferObject* ModelsManager::AddIndexBuffer( const std::wstring& name,
											   const void* buffer,
											   unsigned int elementSize,
											   unsigned int vertCount )
{
	BufferObject* indexBuff = m_indexBuffer.get( name );
	if ( indexBuff )	// Je�eli znale�li�my bufor, to go zwracamy
		return indexBuff;

	// Tworzymy obiekt bufora indeks�w i go zapisujemy
	indexBuff = ResourcesFactory::CreateBufferFromMemory( buffer, elementSize, vertCount, ResourceBinding::BIND_RESOURCE_INDEX_BUFFER, ResourceUsage::RESOURCE_USAGE_DEFAULT );
	if ( !indexBuff )		// Bufor m�g� si� nie stworzy�, a nie chcemy dodawa� nullptra do ModelsManagera
		return nullptr;

	m_indexBuffer.unsafe_add( name, indexBuff );	// Dodali�my bufor

	return indexBuff;
}

/**@brief Dodaje do ModelsManagera bufor sta�ch dla shadera.
Je�eli pod tak� nazw� istnieje jaki� bufor, to zostanie zwr�cony wska�nik na niego.
@note Funkcja nie dodaje odwo�ania do obiektu, bo nie zak�ada, �e kto� go od razu u�yje.
W ka�dym miejscu, gdzie zostanie przypisany zwr�cony obiekt, nale�y pami�ta� o dodaniu odwo�ania oraz
skasowaniu go, gdy obiekt przestanie by� u�ywany.

@param[in] name Nazwa bufora, po kt�rej mo�na si� b�dzie odwo�a�.
@param[in] buffer Wska�nik na bufor z danym, kt�re maj� by� przeniesione do bufora DirectXowego.
@param[in] size Rozmiar bufora.
@return Dodany bufor indeks�w. Zwraca nullptr, je�eli nie uda�o si� stworzy� bufora.*/
BufferObject* ModelsManager::AddConstantsBuffer( const std::wstring& name, const void* buffer, unsigned int size )
{
	BufferObject* constBuff = m_constantBuffer.get( name );
	if ( constBuff )	// Je�eli znale�li�my bufor, to go zwracamy
		return constBuff;

	constBuff = ResourcesFactory::CreateBufferFromMemory( buffer, size, 1, ResourceBinding::BIND_RESOURCE_CONSTANT_BUFFER, ResourceUsage::RESOURCE_USAGE_DEFAULT );
	if ( !constBuff )		// Bufor m�g� si� nie stworzy�, a nie chcemy dodawa� nullptra do ModelsManagera
		return nullptr;

	m_constantBuffer.unsafe_add( name, constBuff );	// Dodali�my bufor
	return constBuff;
}

