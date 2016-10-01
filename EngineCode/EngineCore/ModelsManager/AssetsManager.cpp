#include "EngineCore/stdafx.h"
#include "AssetsManager.h"

#include "Loaders/ILoader.h"
#include "Loaders/FBX_files_loader/FBX_loader.h"
#include "Loaders/Texture/TextureLoader.h"

#include "Common/ObjectDeleter.h"
#include "Common/MacrosSwitches.h"
#include "GraphicAPI/ResourcesFactory.h"

#include "Common/Converters.h"

#include "Common/MemoryLeaks.h"

using namespace DirectX;

//-------------------------------------------------------------------------------//
//							wersja DirectX11
//-------------------------------------------------------------------------------//

AssetsManager::AssetsManager( Engine* engine )
: m_engine( engine )
{
	Model3DFromFile::models_manager = this;

	//Loader plik�w FBX jest domy�lnym narz�dziem do wczytywania
	FBX_loader* fbx_loader = new FBX_loader( this );
	m_loader.push_back( fbx_loader );
}



AssetsManager::~AssetsManager( )
{
	// Kasujemy obiekty do wczytywania danych
	for ( unsigned int i = 0; i <  m_loader.size(); ++i )
		delete m_loader[i];
}




/**@brief Dopasowuje najlepszy z domy�lnych shader�w, kt�ry pasuje do podanej tablicy
tekstur. Tablica ma tyle element�w ile zmienna @ref ENGINE_MAX_TEXTURES.

Ka�da pozycja w tablicy ma przypisane domy�lne znaczenie zgodnie z enumeracj� 
@ref TextureUse. Najlepszy shader jest wybierany na podstawie obecno�ci
lub nieobecno�ci tekstury w tablicy.

@todo Oddelegowa� jaki� inny obiekt do ob��ugi warto�ci domy�lnych albo przemy�le� lepiej jak to powinno w�a�ciwie wygl�da�.

@param[in] textures Tablica tekstur z obiektu ModelPart.
@return Zwraca obiekt vertex shadera.*/
VertexShader* AssetsManager::FindBestVertexShader( TextureObject** textures )
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
PixelShader* AssetsManager::FindBestPixelShader( TextureObject** textures )
{
	PixelShader* return_shader = nullptr;

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
ILoader* AssetsManager::FindLoader( const std::wstring& path )
{
	for ( unsigned int i = 0; i <  m_loader.size( ); ++i )
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
ModelsManagerResult AssetsManager::LoadModelFromFile( const std::wstring& file )
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
		ObjectDeleter< Model3DFromFile>::delete_object( new_model, ObjectDeleterKey< Model3DFromFile>() );
		return ModelsManagerResult::MODELS_MANAGER_CANNOT_LOAD;
	}

	// Dodajemy model do tablic
	m_fileModel.UnsafeAdd( file, new_model );

	return ModelsManagerResult::MODELS_MANAGER_OK;
}


/**@brief */
ResourcePtr<MeshAsset> AssetsManager::CreateMesh( const std::wstring& name, MeshInitData&& initData )
{
	VertexBufferInitData vertexInit;
	vertexInit.Data = initData.VertexBuffer.GetMemory< uint8 >();
	vertexInit.Usage = ResourceUsage::RESOURCE_USAGE_STATIC;
	vertexInit.ElementSize = initData.VertexSize;
	vertexInit.NumElements = initData.NumVerticies;
	vertexInit.VertexLayout = initData.VertexLayout;
	vertexInit.Topology = initData.Topology;

	auto vertexBuffer = CreateVertexBuffer( name, vertexInit );
	if( !vertexBuffer )
		return nullptr;

	ResourcePtr< BufferObject > indexBuffer;
	if( !initData.IndexBuffer.IsNull() )
	{
		IndexBufferInitData indexInit;
		indexInit.Data = initData.IndexBuffer.GetMemory< uint8 >();
		indexInit.ElementSize = 2;
		if( initData.ExtendedIndex )
			indexInit.ElementSize = 4;
		indexInit.Topology = initData.Topology;
		indexInit.NumElements = initData.NumIndicies;
		indexInit.Usage = ResourceUsage::RESOURCE_USAGE_STATIC;

		auto indexBuffer = CreateIndexBuffer( name, indexInit );
		if( !indexBuffer )
			return nullptr;
	}

	MeshCreateData meshData;
	meshData.MeshSegments = std::move( initData.MeshSegments );
	meshData.VertexLayout = std::move( initData.VertexLayout );
	meshData.VertexBuffer = std::move( vertexBuffer );
	meshData.IndexBuffer = std::move( indexBuffer );

	return CreateMesh( name, std::move( meshData ) );
}

/**@brief */
ResourcePtr<MeshAsset> AssetsManager::CreateMesh( const std::wstring& name, MeshCreateData&& initData )
{
	assert( false );
	return ResourcePtr<MeshAsset>();
}

/**@brief */
ResourcePtr< MaterialAsset >	AssetsManager::CreateMaterial( const std::wstring& name, MaterialInitData&& initData )
{
	assert( false );
	return ResourcePtr< MaterialAsset >();
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
MaterialObject* AssetsManager::AddMaterial( MaterialObject* addMaterial, const std::wstring& materialName )
{
	MaterialObject* newMaterial = m_material.get( materialName );
	if ( !newMaterial )
		m_material.UnsafeAdd( materialName, addMaterial );	// Dodali�my materia�

	return newMaterial;
}


//====================================================================================//
//			Listowanie asset�w
//====================================================================================//


/**@brief Listowanie materia��w.*/
std::vector< ResourcePtr< MaterialObject > >	AssetsManager::ListMaterials()
{
	return m_material.List();
}

/**@brief Listowanie meshy.*/
std::vector< ResourcePtr< Model3DFromFile > >	AssetsManager::ListMeshes()
{
	return m_fileModel.List();
}

/**@brief Implementation of texture loading.

This is hack function. Resource manager have no texture loading function beacause
it needs separate library for this. Derived classes will implement it, but in future
this must change. ResourceManager must be fully operational class. Otherwise GUI 
won't load textures.*/
MemoryChunk										AssetsManager::LoadTextureImpl	( const filesystem::Path& filePath, TextureInfo& texInfo )
{
	return TextureLoader::LoadTexture( texInfo.FilePath, texInfo );
}

