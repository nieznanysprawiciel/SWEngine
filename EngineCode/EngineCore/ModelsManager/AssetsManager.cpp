/**
@file AssetsManager.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "EngineCore/stdafx.h"
#include "AssetsManager.h"

#include "Loaders/ILoader.h"
#include "Loaders/FBX_files_loader/FBX_loader.h"
#include "Loaders/Texture/TextureLoader.h"
#include "Loaders/Material/SWMat/SWMaterialLoader.h"

#include "swCommonLib/Common/ObjectDeleter.h"
#include "EngineCore/MainEngine/MacrosSwitches.h"
#include "swGraphicAPI/Resources/ResourcesFactory.h"

#include "swCommonLib/Common/Converters.h"

#include "swCommonLib/Common/MemoryLeaks.h"

using namespace DirectX;

//-------------------------------------------------------------------------------//
//							wersja DirectX11
//-------------------------------------------------------------------------------//

AssetsManager::AssetsManager( Engine* engine )
	: m_engine( engine )
{
	//Loader plików FBX jest domyœlnym narzêdziem do wczytywania
	FBX_loader* fbx_loader = new FBX_loader( this );
	m_loader.push_back( fbx_loader );
}



AssetsManager::~AssetsManager( )
{
	// Kasujemy obiekty do wczytywania danych
	for ( unsigned int i = 0; i <  m_loader.size(); ++i )
		delete m_loader[i];
}




/**@brief Dopasowuje najlepszy z domyœlnych shaderów, który pasuje do podanej tablicy
tekstur. Tablica ma tyle elementów ile zmienna @ref ENGINE_MAX_TEXTURES.

Ka¿da pozycja w tablicy ma przypisane domyœlne znaczenie zgodnie z enumeracj¹ 
@ref TextureUse. Najlepszy shader jest wybierany na podstawie obecnoœci
lub nieobecnoœci tekstury w tablicy.

@todo Oddelegowaæ jakiœ inny obiekt do obœ³ugi wartoœci domyœlnych albo przemyœleæ lepiej jak to powinno w³aœciwie wygl¹daæ.

@param[in] textures Tablica tekstur z obiektu ModelPart.
@return Zwraca obiekt vertex shadera.*/
VertexShader*	AssetsManager::FindBestVertexShader	( TextureObject** textures )
{
	// Na razie nie mamy innych domyœlnych shaderów
	return m_vertexShader.get( DefaultAssets::DEFAULT_VERTEX_SHADER_STRING );
}


/**@brief Dopasowuje najlepszy z domyœlnych shaderów, który pasuje do podanej tablicy
tekstur. Tablica ma tyle elementów ile zmienna @ref ENGINE_MAX_TEXTURES.

Ka¿da pozycja w tablicy ma przypisane domyœlne znaczenie zgodnie z enumeracj¹
@ref TextureUse. Najlepszy shader jest wybierany na podstawie obecnoœci
lub nieobecnoœci tekstury w tablicy.

@todo Oddelegowaæ jakiœ inny obiekt do obœ³ugi wartoœci domyœlnych albo przemyœleæ lepiej jak to powinno w³aœciwie wygl¹daæ.

@param[in] textures Tablica tekstur z obiektu ModelPart.
@return Zwraca obiekt pixel shadera.*/
PixelShader*	AssetsManager::FindBestPixelShader	( TextureObject** textures )
{
	PixelShader* return_shader = nullptr;

	// Na razie nie ma innych tekstur ni¿ diffuse, wiêc algorytm nie jest skomplikowany
	if( textures[ TextureUse::TEX_DIFFUSE ] )
		return_shader = m_pixelShader.get( DefaultAssets::DEFAULT_TEX_DIFFUSE_PIXEL_SHADER_PATH );


	if( !return_shader )	// Je¿eli nadal jest nullptrem to dajemy mu domyœlny shader
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


/** @brief Znajduje Loader pasuj¹cy do pliku podanego w parametrze.
@param[in] path Œcie¿ka do pliku, dla której szukamy loadera.
@return WskaŸnik na odpowiedni loader lub nullptr, je¿eli nie znaleziono pasuj¹cego.*/
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

		if( segment.Material && segment.Material->GetDescriptor().AdditionalBuffers.size() != 0 )
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
	ResourcePtr< BufferObject > materialBuffer;

	if( initData.ShadingData )
	{

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

		materialBuffer = CreateConstantsBuffer( name, bufferData );
		if( !materialBuffer )
			return nullptr;
	}

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

//====================================================================================//
//			Listowanie assetów
//====================================================================================//


/**@brief Listowanie materia³ów.*/
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

