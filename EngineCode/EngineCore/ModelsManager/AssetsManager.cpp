#include "EngineCore/stdafx.h"
#include "AssetsManager.h"

#include "Loaders/ILoader.h"
#include "Loaders/FBX_files_loader/FBX_loader.h"
#include "Loaders/Texture/TextureLoader.h"
#include "Loaders/Material/SWMat/SWMaterialLoader.h"

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
VertexShader*	AssetsManager::FindBestVertexShader	( TextureObject** textures )
{
	// Na razie nie mamy innych domy�lnych shader�w
	return m_vertexShader.get( DefaultAssets::DEFAULT_VERTEX_SHADER_STRING );
}


/**@brief Dopasowuje najlepszy z domy�lnych shader�w, kt�ry pasuje do podanej tablicy
tekstur. Tablica ma tyle element�w ile zmienna @ref ENGINE_MAX_TEXTURES.

Ka�da pozycja w tablicy ma przypisane domy�lne znaczenie zgodnie z enumeracj�
@ref TextureUse. Najlepszy shader jest wybierany na podstawie obecno�ci
lub nieobecno�ci tekstury w tablicy.

@todo Oddelegowa� jaki� inny obiekt do ob��ugi warto�ci domy�lnych albo przemy�le� lepiej jak to powinno w�a�ciwie wygl�da�.

@param[in] textures Tablica tekstur z obiektu ModelPart.
@return Zwraca obiekt pixel shadera.*/
PixelShader*	AssetsManager::FindBestPixelShader	( TextureObject** textures )
{
	PixelShader* return_shader = nullptr;

	// Na razie nie ma innych tekstur ni� diffuse, wi�c algorytm nie jest skomplikowany
	if( textures[ TextureUse::TEX_DIFFUSE ] )
		return_shader = m_pixelShader.get( DefaultAssets::DEFAULT_TEX_DIFFUSE_PIXEL_SHADER_PATH );


	if( !return_shader )	// Je�eli nadal jest nullptrem to dajemy mu domy�lny shader
		return_shader = m_pixelShader.get( DefaultAssets::DEFAULT_PIXEL_SHADER_STRING );

	return return_shader;
}

/**@brief */
GeometryShader*		AssetsManager::FindBestGeometryhader	( TextureObject** textures )
{
	return nullptr;
}

/**@brief */
EvaluationShader*	AssetsManager::FindBestEvaluationShader	( TextureObject** textures )
{
	return nullptr;
}

/**@brief */
ControlShader*		AssetsManager::FindBestControlShader	( TextureObject** textures )
{
	return nullptr;
}


/**@brief Fills struct with best shaders matching to textures data.*/
void			AssetsManager::FillBestShaders		( MaterialInitData& initData )
{
	TextureObject* textures[ MAX_MATERIAL_TEXTURES ];
	for( int i = 0; i < MAX_MATERIAL_TEXTURES; i++ )
		textures[ i ] = initData.Textures[ i ].Ptr();

	initData.VertexShader = FindBestVertexShader( textures );
	initData.PixelShader = FindBestPixelShader( textures );
	initData.GeometryShader = FindBestGeometryhader( textures );
	initData.TesselationEvaluationShader = FindBestEvaluationShader( textures );
	initData.TesselationControlShader = FindBestControlShader( textures );
}


/** @brief Znajduje Loader pasuj�cy do pliku podanego w parametrze.
@param[in] path �cie�ka do pliku, dla kt�rej szukamy loadera.
@return Wska�nik na odpowiedni loader lub nullptr, je�eli nie znaleziono pasuj�cego.*/
ILoader*		AssetsManager::FindLoader			( const std::wstring& path )
{
	for ( unsigned int i = 0; i <  m_loader.size( ); ++i )
	if ( m_loader[i]->can_load( path ) )
		return m_loader[i];
	return nullptr;
}


//-------------------------------------------------------------------------------//
//							Loading functions
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

/**@brief Loads mesh from file.
@return Returns nullptr when loader could not be found or mesh loading failed.*/
ResourcePtr< MeshAsset >		AssetsManager::LoadMesh		( const filesystem::Path& file )
{
	std::wstring filePath = file.WString();

	// Check exisitng meshes.
	MeshAsset* newMesh = m_meshes.get( filePath );
	if ( newMesh != nullptr )
		return newMesh;

	ILoader* loader = FindLoader( filePath );
	if ( loader == nullptr )
		return nullptr;

	Nullable< MeshInitData > meshData = loader->LoadMesh( file );
	if( !meshData.IsValid )
	{
		///@todo Use logging mechanism to log error.
		return nullptr;
	}

	return CreateMesh( filePath, std::move( meshData.Value ) );
}

/**@brief Loads material from file.*/
ResourcePtr< MaterialAsset >	AssetsManager::LoadMaterial	( const filesystem::Path& file )
{
	std::wstring filePath = Convert::FromString< std::wstring >( file.String(), L"" );

	// Check exisitng materials.
	MaterialAsset* newMaterial = m_material.get( filePath );
	if ( newMaterial != nullptr )
		return newMaterial;

	SWMaterialLoader loader( this );

	if( !loader.CanLoad( file ) )
	{
		/// @todo Log message.
		return nullptr;
	}

	Nullable< MaterialInitData > materialData = loader.LoadMaterial( file );
	if( !materialData.IsValid )
	{
		///@todo Use logging mechanism to log error.
		return nullptr;
	}

	return CreateMaterial( filePath, std::move( materialData.Value ) );
}

//====================================================================================//
//			Assets creation	
//====================================================================================//

/**@brief Creates mesh from provided data.*/
ResourcePtr< MeshAsset >		AssetsManager::CreateMesh	( const std::wstring& name, MeshInitData&& initData )
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
		
		if( initData.ExtendedIndex )
		{
			indexInit.DataType = TypeID::get< Index32 >();
			indexInit.ElementSize = sizeof( Index32 );
		}
		else
		{
			indexInit.ElementSize = sizeof( Index16 );
			indexInit.DataType = TypeID::get< Index16 >();
		}

		indexInit.Topology = initData.Topology;
		indexInit.NumElements = initData.NumIndicies;
		indexInit.Usage = ResourceUsage::RESOURCE_USAGE_STATIC;

		indexBuffer = CreateIndexBuffer( name, indexInit );
		if( !indexBuffer )
			return nullptr;
	}

	// Validate segments data
	for( auto& segment : initData.MeshSegments )
	{
		uint16 flags = 0;
		if( initData.ExtendedIndex )
			flags = flags | MeshPartFlags::Use4BytesIndex;

		if( segment.Material->GetDescriptor().AdditionalBuffers.size() != 0 )
			flags = flags | MeshPartFlags::UseAdditionalBuffer;

		segment.Flags = flags;
	}

	MeshCreateData meshData;
	meshData.MeshSegments = std::move( initData.MeshSegments );
	meshData.VertexLayout = std::move( initData.VertexLayout );
	meshData.VertexBuffer = std::move( vertexBuffer );
	meshData.IndexBuffer = std::move( indexBuffer );

	return CreateMesh( name, std::move( meshData ) );
}

/**@brief Creates MeshAsset.

@return Returns nullptr if asset under this name existed in AssetManager before.
@note This is difference between CreateMesh and LoadMesh. LoadMesh returns existing
material not nullptr in this case.*/
ResourcePtr< MeshAsset >		AssetsManager::CreateMesh		( const std::wstring& name, MeshCreateData&& initData )
{
	// Check exisitng materials.
	MeshAsset* newMesh = m_meshes.get( name );
	if ( newMesh != nullptr )
		return nullptr;

	newMesh = new MeshAsset( name, std::move( initData ) );
	m_meshes.UnsafeAdd( name, newMesh );

	return newMesh;
}

/**@brief Creates MaterialAsset.

@return Returns nullptr if asset under this name existed in AssetManager before.
@note This is difference between CreateMaterial and LoadMaterial. LoadMaterial returns existing
material not nullptr in this case.*/
ResourcePtr< MaterialAsset >	AssetsManager::CreateMaterial	( const std::wstring& name, MaterialInitData&& initData )
{
	if( !initData.ShadingData )
		return nullptr;

	ConstantBufferInitData bufferData;
	bufferData.DataType = initData.ShadingData->GetShadingModelType();
	bufferData.ElementSize = (uint32)initData.ShadingData->GetShadingModelSize();
	bufferData.Data = initData.ShadingData->GetShadingModelData();

	// Constant buffers must be 16 bytes aligned.
	if( bufferData.ElementSize % 16 != 0 )
	{
		///@todo Logging
		return nullptr;
	}

	auto materialBuffer = CreateConstantsBuffer( name, bufferData );
	if( !materialBuffer )
		return nullptr;

	MaterialCreateData data;
	data.Data = std::move( initData );
	data.MaterialBuffer = materialBuffer;

	return CreateMaterial( name, std::move( data ) );
}

/**@brief Creates MaterialAsset.

@return Returns nullptr if asset under this name existed in AssetManager before.
@note This is difference between CreateMaterial and LoadMaterial. LoadMaterial returns existing
material not nullptr in this case.*/
ResourcePtr< MaterialAsset >	AssetsManager::CreateMaterial				( const std::wstring& name, MaterialCreateData&& initData )
{
	// Check exisitng materials.
	MaterialAsset* newMaterial = m_material.get( name );
	if ( newMaterial != nullptr )
		return nullptr;

	newMaterial = new MaterialAsset( name, std::move( initData ) );
	m_material.UnsafeAdd( name, newMaterial );

	return newMaterial;
}

/**@brief Dodaje materia� do AssetManagera, je�eli jeszcze nie istnia�.
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
MaterialObject* AssetsManager::AddMaterialObject( MaterialObject* addMaterial, const std::wstring& materialName )
{
	MaterialObject* newMaterial = m_materialObject.get( materialName );
	if ( !newMaterial )
		m_materialObject.UnsafeAdd( materialName, addMaterial );	// Dodali�my materia�

	return newMaterial;
}


//====================================================================================//
//			Listowanie asset�w
//====================================================================================//


/**@brief Listowanie materia��w.*/
std::vector< ResourcePtr< MaterialAsset > >		AssetsManager::ListMaterials()
{
	return m_material.List();
}

/**@brief Listowanie meshy.*/
std::vector< ResourcePtr< MeshAsset > >			AssetsManager::ListMeshes()
{
	return m_meshes.List();
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

