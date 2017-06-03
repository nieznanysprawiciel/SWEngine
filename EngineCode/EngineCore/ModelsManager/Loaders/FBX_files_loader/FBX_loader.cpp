#include "EngineCore/stdafx.h"
#include "FBX_loader.h"
#include "FbxLoader.inl"

#include "Tools/Geometric/Processors/Converter.h"


#include "Common/Converters.h"
#include "Common/MemoryLeaks.h"


#include <limits>


using namespace DirectX;



// ================================ //
// Helpers declarations

DirectX::XMFLOAT3	Get			( const fbxsdk::FbxVector4& vector );
bool				operator==	( const fbxsdk::FbxVector4& vec1, const DirectX::XMFLOAT3& vec2 );


/**@brief Suported file extensions which can be load by loader.*/
const char*	SupportedExtensions[] =
{
	".FBX",
	".OBJ",
	".3DS",
	".DXF"
};


///@brief Kontruktr inicjuje obiekty FBX SDK, konieczne do wczytania modelu.
FBX_loader::FBX_loader(AssetsManager* models_manager)
	: ILoader(models_manager)
{
	fbx_manager = FbxManager::Create();
	fbx_IOsettings = FbxIOSettings::Create(fbx_manager, IOSROOT);
	fbx_manager->SetIOSettings(fbx_IOsettings);

	cur_model = nullptr;
}


FBX_loader::~FBX_loader()
{
	fbx_manager->Destroy();
	//nie zwalniamy fbx_IOsettings, bo zajmuje siê tym fbx_manager
}



/**@brief Dla podanego polygona odczytujemy jego indeks materia³u. Je¿eli nie ma ¿adnych materia³ów przypisanych
to dostajemy 0. Kiedy potem bêdziemy wybieraæ, do której tablicy mesha zapisaæ wierzcho³ek, wszystko
bêdzie dzia³a³o poprawnie nawet, jak materi¹³u nie bêdzie.*/
int FBX_loader::ReadMaterialIndex(FbxMesh* mesh, unsigned int polygon_counter)
{
	if (mesh->GetElementMaterialCount() < 1)
		return 0;		//nie by³o materia³ów to mamy tylko jedn¹ tablicê do indeksowania

	FbxGeometryElementMaterial* material = mesh->GetElementMaterial();
	int index;

	switch (material->GetMappingMode())
	{
	case FbxGeometryElement::eAllSame:
		index = material->GetIndexArray()[0];		//mamy tylko jeden materia³ dla ca³ego mesha
		break;

	case FbxGeometryElement::eByPolygon:
		index = material->GetIndexArray()[polygon_counter];
		break;
	}
	return index;
}

/**@brief Wczytuje UVs dla podanego wierzcho³ka.*/
DirectX::XMFLOAT2		FBX_loader::ReadUVs(FbxMesh* mesh, int control_point, unsigned int vertex_counter )
{
	if (mesh->GetUVLayerCount() < 1)
		return DirectX::XMFLOAT2( 0.0, 0.0 );

	DirectX::XMFLOAT2 UV_cords;
	
	FbxGeometryElementUV* UVs = mesh->GetElementUV();
	int index;

	switch (UVs->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		switch (UVs->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			UV_cords.x = static_cast<float>(UVs->GetDirectArray().GetAt(control_point).mData[0]);
			UV_cords.y = 1 - static_cast<float>(UVs->GetDirectArray().GetAt(control_point).mData[1]);
			break;

		case FbxGeometryElement::eIndexToDirect:
			index = UVs->GetIndexArray().GetAt(control_point);
			UV_cords.x = static_cast<float>(UVs->GetDirectArray().GetAt(index).mData[0]);
			UV_cords.y = 1 - static_cast<float>(UVs->GetDirectArray().GetAt(index).mData[1]);
			break;
		}
		break;

	case FbxGeometryElement::eByPolygonVertex:
		switch (UVs->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			UV_cords.x = static_cast<float>(UVs->GetDirectArray().GetAt(vertex_counter).mData[0]);
			UV_cords.y = 1 - static_cast<float>(UVs->GetDirectArray().GetAt(vertex_counter).mData[1]);
			break;

		case FbxGeometryElement::eIndexToDirect:
			index = UVs->GetIndexArray().GetAt(vertex_counter);
			UV_cords.x = static_cast<float>(UVs->GetDirectArray().GetAt(index).mData[0]);
			UV_cords.y = 1 - static_cast<float>(UVs->GetDirectArray().GetAt(index).mData[1]);
			break;
		}
		break;
	}

	return UV_cords;
}



//====================================================================================//
//			New loading	
//====================================================================================//

/**@brief Loads file.*/
Nullable< MeshInitData >	FBX_loader::LoadMesh	( const filesystem::Path& fileName )
{
	FbxImporter* FbxImporter = FbxImporter::Create( fbx_manager, "" );
	m_filePath = fileName;

	if( !FbxImporter->Initialize( m_filePath.String().c_str(), -1, fbx_manager->GetIOSettings() ) )
	{
		auto& status = FbxImporter->GetStatus();
		return status.GetErrorString();
	}

	FbxScene* scene = FbxScene::Create( fbx_manager, "Scene" );
	FbxImporter->Import( scene );
	FbxImporter->Destroy();

	// Triangulate scene before loading.
	FbxGeometryConverter triangulator( fbx_manager );
	triangulator.Triangulate( scene, true );

	//FbxNode* root = scene->GetRootNode();
	//if( root == nullptr )
	//	return "Root node is nullptr";


	Nullable< FbxMeshCollection > meshData( NullableInit::Valid );
	for( int i = 0; i < scene->GetNodeCount(); i++ )
		meshData = ProcessNode( scene->GetNode( i ), meshData );

	ReturnIfInvalid( meshData );

	FbxAssetsCollection assets;
	FbxMaterialsCollection& materials = assets.Material;
	FbxTexturesCollection& textures = assets.Textures;
	materials.reserve( scene->GetMaterialCount() );
	textures.reserve( scene->GetTextureCount() );

	for( int i = 0; i < scene->GetMaterialCount(); ++i )
		materials.push_back( scene->GetMaterial( i ) );

	for( int i = 0; i < scene->GetTextureCount(); ++i )
		textures.push_back( scene->GetTexture( i ) );


	Nullable< TemporaryMeshInit > tempMeshInit( NullableInit::Valid );

	for( auto& mesh : meshData.Value.Segments )
	{
		tempMeshInit = ProcessMesh( mesh, assets, tempMeshInit );
	}

	ReturnIfInvalid( tempMeshInit );


	Size numVerticies = tempMeshInit.Value.Verticies.size();
	Size numIndicies = 0;
	for( Size i = 0; i < tempMeshInit.Value.Segments.size(); ++i )
	{
		numIndicies += tempMeshInit.Value.Indicies[ i ].size();
	}

	bool extendedIndex = numVerticies > std::numeric_limits< Index16 >::max();
	int indexSize = extendedIndex ? sizeof( Index32 ) : sizeof( Index16 );

	if( numVerticies > std::numeric_limits< Index32 >::max() )
		return "Verticies count is out of 32 bit range.";

	MemoryChunk indexChunk( (uint32)numIndicies * indexSize );
	MemoryChunk vertexChunk( (uint32)numVerticies * sizeof( VertexNormalTexCoord ) );

	CopyVertexBuffer( tempMeshInit.Value.Verticies, vertexChunk );
	
	if( indexSize == sizeof( Index16 ) )
		CopyIndexBuffer< Index16 >( tempMeshInit.Value.Indicies, indexChunk );
	else if( indexSize == sizeof( Index32 ) )
		CopyIndexBuffer< Index32 >( tempMeshInit.Value.Indicies, indexChunk );
	else
		assert( false );

	MeshInitData initData;
	initData.Topology = PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	initData.VertexSize = sizeof( VertexNormalTexCoord );
	initData.MeshSegments = std::move( tempMeshInit.Value.Segments );
	initData.ExtendedIndex = extendedIndex;
	initData.NumVerticies = (uint32)numVerticies;
	initData.NumIndicies = (uint32)numIndicies;
	initData.IndexBuffer = std::move( indexChunk );
	initData.VertexBuffer = std::move( vertexChunk );
	initData.VertexLayout = models_manager->GetLayout( DefaultAssets::LAYOUT_POSITION_NORMAL_COORD->GetName() );

	return std::move( initData );
}

/**@brief Returns true if loader can load specific file.

This function uses file extension to determine if it can load file.*/
bool						FBX_loader::CanLoad		( const filesystem::Path& fileName )
{
	std::string extension = fileName.GetExtension();
	std::transform( extension.begin(), extension.end(), extension.begin(), toupper );

	for( auto ext : SupportedExtensions )
	{
		if( extension == ext )
			return true;
	}
	return false;
}


/**@brief Process single node.*/
Nullable< FbxMeshCollection >		FBX_loader::ProcessNode		( FbxNode* node, Nullable< FbxMeshCollection >& meshes )
{
	ReturnIfInvalid( meshes );

	if( node == nullptr )
		return std::move( meshes );

	// Compute transform matrix for this node.
	FbxAMatrix transformMatrix = node->EvaluateGlobalTransform();
	DirectX::XMFLOAT4X4 transformation;
	for( int i = 0; i < 4; ++i )
	{
		for( int j = 0; j < 4; ++j )
			transformation( i, j ) = static_cast<float>( transformMatrix.Get( i, j ) );
	}

	for( int i = 0; i < node->GetNodeAttributeCount(); i++ )
	{
		if( ( node->GetNodeAttributeByIndex( i ) )->GetAttributeType() == FbxNodeAttribute::eMesh )
		{
			// Look only for meshes. Ignore rest
			FbxNodeMesh mesh;
			mesh.Transformation = transformation;
			mesh.Node = node;
			mesh.Mesh = (FbxMesh*)node->GetNodeAttributeByIndex( i );

			meshes.Value.Segments.push_back( mesh );
		}
	}

	//for( int i = 0; i < node->GetChildCount(); i++ )
	//	meshes = ProcessNode( node->GetChild( i ), meshes );

	return std::move( meshes );
}


/**@brief Process single mesh.*/
Nullable< TemporaryMeshInit >		FBX_loader::ProcessMesh		( FbxNodeMesh& nodeData, FbxAssetsCollection& assets, Nullable< TemporaryMeshInit >& mesh )
{
	ReturnIfInvalid( mesh );

	FbxNode* fbxNode = nodeData.Node;
	FbxMesh* fbxMesh = nodeData.Mesh;
	uint32 ctrlPointsOffset = (uint32)mesh.Value.Verticies.size();

	int materialsCount = fbxNode->GetMaterialCount();
	unsigned int polygonsCount = fbxMesh->GetPolygonCount();
	
	unsigned int vertexCounter = 0;			// For FbxGeometryElement::eByPolygonVertex mapping type.
	unsigned int polygonCounter = 0;		// For FbxGeometryElement::eByPolygon maping type.


	std::vector< std::vector< Index32 > >		indicies;		indicies.resize( materialsCount );
	std::vector< VertexNormalTexCoord >			verticies;		verticies.reserve( 3 * polygonsCount );


	auto controlPoints = fbxMesh->GetControlPoints();


	// Create vertex buffer.
	while( polygonCounter < polygonsCount )
	{
		for( int vertexIdx = 0; vertexIdx < 3; ++vertexIdx )
		{
			Index32 controlPointIdx = fbxMesh->GetPolygonVertex( polygonCounter, vertexIdx );
			VertexNormalTexCoord curVertex;

			// Find vertex normal and uvs coord.
			FbxVector4 fbxNormal;
			fbxMesh->GetPolygonVertexNormal( polygonCounter, vertexIdx, fbxNormal );

			curVertex.Position = Get( controlPoints[ controlPointIdx ] );
			curVertex.Normal = Get( fbxNormal );
			curVertex.TexCoord = ReadUVs( fbxMesh, controlPointIdx, vertexCounter );

			//mesh.Value.Verticies.push_back( curVertex );
			verticies.push_back( curVertex );

			++vertexCounter;		//zliczamy wierzcho³ki
		}

		polygonCounter++;
	}

	auto newIndicies = Geometric::Converter::MakeIndexed< VertexNormalTexCoord, Index32 >( verticies, mesh.Value.Verticies );
	//std::vector< Index32 > newIndicies;
	//newIndicies.resize( 3 * polygonsCount );
	//std::iota( newIndicies.begin(), newIndicies.end(), ctrlPointsOffset );


	if( materialsCount > 1 )
	{
		for( int idx = 0; idx < newIndicies.size(); idx += 3 )
		{
			int materialIdx = ReadMaterialIndex( fbxMesh, idx / 3 );
			indicies[ materialIdx ].push_back( newIndicies[ idx ] );
			indicies[ materialIdx ].push_back( newIndicies[ idx + 1 ] );
			indicies[ materialIdx ].push_back( newIndicies[ idx + 2 ] );
		}
	}
	else
		indicies[ 0 ] = std::move( newIndicies );
	

	// Verticies in FBX file have transformations which we must apply.
	// We don't have to preserve transformation matrix for each segment with this approach.
	TransformVerticies( mesh.Value.Verticies, ctrlPointsOffset, nodeData.Transformation );

	Size indiciesOffset = 0;
	for( Size i = 0; i < mesh.Value.Indicies.size(); ++i )
		indiciesOffset += mesh.Value.Indicies[ i ].size();


	// Rewrite index buffers. Create material. Add segment data.
	for( int i = 0; i < indicies.size(); ++i )
	{
		MeshPart part;

		FbxSurfaceMaterial* material = static_cast<FbxSurfaceMaterial*>( fbxNode->GetMaterial( i ) );
		part.Material = ProcessMaterial( material, assets );

		part.BufferOffset = (uint32)indiciesOffset;
		part.BaseVertex = 0;
		part.NumVertices = (uint32)indicies[ i ].size();
		part.Topology = PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		indiciesOffset += indicies[ i ].size();

		mesh.Value.Indicies.push_back( std::move( indicies[ i ] ) );
		mesh.Value.Segments.push_back( part );
	}


	return std::move( mesh );
}

// ================================ //
//
ResourcePtr< MaterialAsset >		FBX_loader::ProcessMaterial	( FbxSurfaceMaterial* FBXmaterial, FbxAssetsCollection& assets )
{
	for( auto& mat : assets.Material )
	{
		if( mat.Surface == FBXmaterial )
		{
			if( mat.EngineMaterial )	
				return mat.EngineMaterial;
			else
			{
				mat.EngineMaterial = CreateMaterial( FBXmaterial, assets );
				return mat.EngineMaterial;
			}
		}
	}

	return ResourcePtr<MaterialAsset>();
}

// ================================ //
//
ResourcePtr< MaterialAsset >		FBX_loader::CreateMaterial	( FbxSurfaceMaterial* FBXmaterial, FbxAssetsCollection& assets )
{
	FbxShadingModel model;

	// Get the implementation to see if it's a hardware shader.
	const FbxImplementation* lImplementation = GetImplementation( FBXmaterial, FBXSDK_IMPLEMENTATION_HLSL );

	if( lImplementation )
	{
		assert( !"Hardware shader currently not supported" );
		model = FbxShadingModel::HardwareShader;
	}
	else if( FBXmaterial->GetClassId().Is( FbxSurfacePhong::ClassId ) )
		model = FbxShadingModel::Phong;
	else if( FBXmaterial->GetClassId().Is( FbxSurfaceLambert::ClassId ) )
		model = FbxShadingModel::Lambert;
	else
		assert( !"Wrong shading model" );


	MaterialInitData initData;
	PhongMaterial engineMaterial;
	FbxSurfaceLambert* surfMaterial = static_cast<FbxSurfaceLambert*>( FBXmaterial );

	engineMaterial.SetNullMaterial();

	if( model == FbxShadingModel::Lambert )
		engineMaterial = CopyMaterial( *surfMaterial );
	else
		engineMaterial = CopyMaterial( *static_cast<FbxSurfacePhong*>( surfMaterial ) );


	initData.ShadingData = UPtr< ShadingModelData< PhongMaterial > >( new ShadingModelData< PhongMaterial >( engineMaterial ) );


	ResourcePtr< TextureObject > diffuseTexture = nullptr;
	ResourcePtr< TextureObject > specularTexture = nullptr;
	ResourcePtr< TextureObject > bumpMapTexture = nullptr;
	ResourcePtr< TextureObject > displacementTexture = nullptr;
	//ResourcePtr< TextureObject > normalMapTexture = nullptr;
	ResourcePtr< TextureObject > emmisiveTexture = nullptr;


	if( surfMaterial->Diffuse.GetSrcObjectCount() > 0 )
	{
		FbxFileTexture* texture = static_cast<FbxFileTexture*>( surfMaterial->Diffuse.GetSrcObject() );
		if( texture != nullptr )
			diffuseTexture = ProcessTexture( texture, assets );
	}

	if( model == FbxShadingModel::Phong )
	{
		auto fbxPhongMat = static_cast< FbxSurfacePhong* >( surfMaterial );

		if( fbxPhongMat->Specular.GetSrcObjectCount() > 0 )
		{
			FbxFileTexture* texture = static_cast<FbxFileTexture*>( fbxPhongMat->Specular.GetSrcObject() );
			if( texture != nullptr )
				specularTexture = ProcessTexture( texture, assets );
		}

		if( fbxPhongMat->DisplacementColor.GetSrcObjectCount() > 0 )
		{
			FbxFileTexture* texture = static_cast<FbxFileTexture*>( fbxPhongMat->DisplacementColor.GetSrcObject() );
			if( texture != nullptr )
				displacementTexture = ProcessTexture( texture, assets );
		}

		if( fbxPhongMat->Emissive.GetSrcObjectCount() > 0 )
		{
			FbxFileTexture* texture = static_cast<FbxFileTexture*>( fbxPhongMat->Emissive.GetSrcObject() );
			if( texture != nullptr )
				emmisiveTexture = ProcessTexture( texture, assets );
		}

		if( fbxPhongMat->Bump.GetSrcObjectCount() > 0 )
		{
			FbxFileTexture* texture = static_cast<FbxFileTexture*>( fbxPhongMat->Bump.GetSrcObject() );
			if( texture != nullptr )
				bumpMapTexture = ProcessTexture( texture, assets );
		}

		//if( fbxPhongMat->NormalMap.GetSrcObjectCount() > 0 )
		//{
		//	FbxFileTexture* texture = static_cast<FbxFileTexture*>( fbxPhongMat->NormalMap.GetSrcObject() );
		//	if( texture != nullptr )
		//		normalMapTexture = ProcessTexture( texture, assets );
		//}
	}

	initData.Textures[ TextureUse::TEX_DIFFUSE ] = diffuseTexture;
	initData.Textures[ TextureUse::TEX_SPECULAR ] = specularTexture;
	initData.Textures[ TextureUse::TEX_EMISSIVE ] = emmisiveTexture;
	initData.Textures[ TextureUse::TEX_BUMP_MAP ] = bumpMapTexture;
	initData.Textures[ TextureUse::TEX_DISPLACEMENT_MAP ] = displacementTexture;

	models_manager->FillBestShaders( initData );

	auto path = Convert::FromString< std::wstring >( m_filePath.String() + "::" + FBXmaterial->GetName(), std::wstring( L"" ) );

	return models_manager->CreateMaterial( path, std::move( initData ) );
}

// ================================ //
//
ResourcePtr< TextureObject >		FBX_loader::ProcessTexture			( FbxFileTexture* FBXTexture, FbxAssetsCollection& assets )
{
	for( auto& tex : assets.Textures )
	{
		if( tex.FbxTex == FBXTexture )
		{
			if( tex.EngineTex )	
				return tex.EngineTex;
			else
			{
				filesystem::Path texPath = FBXTexture->GetRelativeFileName();
				texPath = m_filePath.GetParent() / texPath;
				auto newTexture = models_manager->LoadTexture( texPath.WString() );

				tex.EngineTex = newTexture;
				return newTexture;
			}
		}
	}

	return ResourcePtr< TextureObject >();
}



// ================================ //
//
void								FBX_loader::TransformVerticies		( std::vector< VertexNormalTexCoord >& verticies, uint32 offset, const DirectX::XMFLOAT4X4& matrix )
{
	XMMATRIX transform = XMLoadFloat4x4( &matrix );
	XMMATRIX normalTransform = XMMatrixTranspose( XMMatrixInverse( nullptr, transform ) );

	for( uint32 i = offset; i < verticies.size(); ++i )
	{
		auto& vertex = verticies[ i ];

		XMVECTOR position = XMLoadFloat3( &vertex.Position );
		XMVECTOR normal = XMLoadFloat3( &vertex.Normal );

		position = XMVector3Transform( position, transform );
		normal = XMVector3TransformNormal( normal, normalTransform );

		XMStoreFloat3( &vertex.Position, position );
		XMStoreFloat3( &vertex.Normal, normal );
	}
}

// ================================ //
// Adds vertex to array if neither of verticies was equal.
Index32						FBX_loader::FindUniqueVertex			( VertexNormalTexCoord& vertex, std::vector< VertexNormalTexCoord >& verticies, Index32 startIndex )
{
	for( Index32 i = startIndex; i < verticies.size(); ++i )
	{
		VertexNormalTexCoord& existingVertex = verticies[ i ];
		if( existingVertex.Position == vertex.Position &&
			existingVertex.Normal == vertex.Normal &&
			existingVertex.TexCoord == vertex.TexCoord )
		{
			return i;
		}
	}
	
	// Vertex not found.
	verticies.push_back( vertex );
	return (Index32)( verticies.size() - 1 );
}

// ================================ //
//
PhongMaterial						FBX_loader::CopyMaterial			( const FbxSurfaceLambert& FBXmaterial )
{
	PhongMaterial engineMaterial;

	FbxDouble3 diffuse = static_cast<FbxDouble3>( FBXmaterial.Diffuse.Get() );
	FbxDouble3 ambient = static_cast<FbxDouble3>( FBXmaterial.Ambient.Get() );
	FbxDouble3 emissive = static_cast<FbxDouble3>( FBXmaterial.Emissive.Get() );
	FbxDouble transparent = static_cast<FbxDouble>( FBXmaterial.TransparencyFactor.Get() );

	FbxDouble diffuseFactor = static_cast<FbxDouble>( FBXmaterial.DiffuseFactor.Get() );
	FbxDouble ambientFactor = static_cast<FbxDouble>( FBXmaterial.AmbientFactor.Get() );
	FbxDouble emissiveFactor = static_cast<FbxDouble>( FBXmaterial.EmissiveFactor.Get() );

	engineMaterial.Diffuse.w = static_cast<float>( transparent );

	engineMaterial.Diffuse.x = static_cast<float>( diffuse[ 0 ] * diffuseFactor );
	engineMaterial.Diffuse.y = static_cast<float>( diffuse[ 1 ] * diffuseFactor );
	engineMaterial.Diffuse.z = static_cast<float>( diffuse[ 2 ] * diffuseFactor );

	engineMaterial.Ambient.x = static_cast<float>( ambient[ 0 ] * ambientFactor );
	engineMaterial.Ambient.y = static_cast<float>( ambient[ 1 ] * ambientFactor );
	engineMaterial.Ambient.z = static_cast<float>( ambient[ 2 ] * ambientFactor );

	engineMaterial.Emissive.x = static_cast<float>( emissive[ 0 ] * emissiveFactor );
	engineMaterial.Emissive.y = static_cast<float>( emissive[ 1 ] * emissiveFactor );
	engineMaterial.Emissive.z = static_cast<float>( emissive[ 2 ] * emissiveFactor );

	engineMaterial.Specular.x = 0.0f;
	engineMaterial.Specular.y = 0.0f;
	engineMaterial.Specular.z = 0.0f;

	engineMaterial.Power = 1.0f;

	return engineMaterial;
}

// ================================ //
//
PhongMaterial						FBX_loader::CopyMaterial			( const FbxSurfacePhong& FBXmaterial )
{
	PhongMaterial engineMaterial = CopyMaterial( static_cast<const FbxSurfaceLambert&>( FBXmaterial ) );

	if( !FBXmaterial.Specular.IsValid() )
		assert( false );

	FbxDouble3 specular = static_cast<FbxDouble3>( FBXmaterial.Specular.Get() );
	FbxDouble specularFactor = static_cast<FbxDouble>( FBXmaterial.SpecularFactor.Get() );
	FbxDouble power = static_cast<FbxDouble>( FBXmaterial.Shininess.Get() );

	engineMaterial.Specular.x = static_cast<float>( specular[ 0 ] * specularFactor );
	engineMaterial.Specular.y = static_cast<float>( specular[ 1 ] * specularFactor );
	engineMaterial.Specular.z = static_cast<float>( specular[ 2 ] * specularFactor );

	engineMaterial.Power = static_cast<float>( power );

	return engineMaterial;
}


//====================================================================================//
//			Helpers	
//====================================================================================//


// ================================ //
//
DirectX::XMFLOAT3	Get( const fbxsdk::FbxVector4& vector )
{
	DirectX::XMFLOAT3 result;
	result.x = (float)vector.mData[ 0 ];
	result.y = (float)vector.mData[ 1 ];
	result.z = (float)vector.mData[ 2 ];
	return result;
}


// ================================ //
//
bool				operator==	( const fbxsdk::FbxVector4& vec1, const DirectX::XMFLOAT3& vec2 )
{
	return Get( vec1 ) == vec2;
}


//====================================================================================//
//			@deprecated Compatibility	
//====================================================================================//

/**@deprecated*/
bool FBX_loader::can_load( const std::wstring & name )
{
	return CanLoad( filesystem::Path( name ) );;
}

/**@deprecated*/
LoaderResult FBX_loader::load_mesh( Model3DFromFile * new_file_mesh, const std::wstring & name )
{
	return LoaderResult();
}
