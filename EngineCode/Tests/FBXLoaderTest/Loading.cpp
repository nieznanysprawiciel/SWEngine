#include "catch.hpp"

#include "EngineCore/MainEngine/Engine.h"
#include "EngineCore/ModelsManager/AssetsManager.h"
#include "EngineCore/GamePlay/IGamePlay.h"

#include "EngineCore/ModelsManager/Assets/Materials/PhongMaterialData.h"


#define WINDOW_WIDTH		1280
#define WINDOW_HEIGHT		800



class EntryPointGamePlay :	public IGamePlay
{
private:

protected:

public:
	EntryPointGamePlay() {};
	~EntryPointGamePlay() {};

	// Funkcje czysto wirtualne odziedziczone po IGamePlay
	virtual void		ProceedGameLogic( float time_interval ) override {};
	virtual int			LoadLevel() override { return 0; };
	virtual int			UnloadLevel() override { return 0; };
};


void		TestIndexBuffer		( const ResourcePtr< BufferObject >& buffer, uint32 expNumElements, uint32 expElemSize, TypeID type );
void		TestVertexBuffer	( const ResourcePtr< BufferObject >& buffer, uint32 expNumElements, uint32 expElemSize, TypeID type, PrimitiveTopology topology );
void		TestBufferObject	( const ResourcePtr< BufferObject >& buffer, uint32 expNumElements, uint32 expElemSize, TypeID type );


TEST_CASE( "Loading assets", "[FBXLoader]" )
{
	Engine* engine = new Engine();
	REQUIRE( engine->InitEngine( WINDOW_WIDTH, WINDOW_HEIGHT, false, 0 ) == 1 );

	EntryPointGamePlay* entryPoint = new EntryPointGamePlay();
	engine->SetEntryPoint( entryPoint );

	AssetsManager& assetsManager = *engine->GetAssetsManager();

	const filesystem::Path CLONE_FIGHTER = "tylko_do_testow/ARC.FBX";
	const filesystem::Path SIMPLE_BOX = "tylko_do_testow/SimpleBox/SimpleBox.FBX";
	const filesystem::Path MOON = "tylko_do_testow/moon/moon.FBX";
	const filesystem::Path NEBULON = "tylko_do_testow/Nebulon/Nebulon.FBX";
	const filesystem::Path VADER_TIE = "tylko_do_testow/VadersTIE.FBX";
	const filesystem::Path TIE_FIGHTER = "tylko_do_testow/TIE_Fighter/TIE_Fighter.FBX";
	const filesystem::Path IMPERIAL_STAR_DESTROYER = "tylko_do_testow/ImperialStarDestroyer.FBX";
	const filesystem::Path CHURCH = "tylko_do_testow/Church/AbandonedChurch.FBX";
	const filesystem::Path GATE = "meshes/Wall/Castle wall/Castle wall.FBX";


//====================================================================================//
//			Clone Fighter	
//====================================================================================//

	SECTION( "Loading Clone Fighter" )
	{
		auto cloneFighterMesh = assetsManager.LoadMesh( CLONE_FIGHTER );
		REQUIRE( cloneFighterMesh != nullptr );

		auto indexBuffer = cloneFighterMesh->GetIndexBuffer();
		auto vertexBuffer = cloneFighterMesh->GetVertexBuffer();
		
		REQUIRE( indexBuffer != nullptr );
		REQUIRE( vertexBuffer != nullptr );

		CHECK( cloneFighterMesh->GetResourceName() == CLONE_FIGHTER.String() );
		
		TestIndexBuffer( indexBuffer, 155166,  sizeof( Index16 ), TypeID::get< Index16 >() );
		TestVertexBuffer( vertexBuffer, vertexBuffer->GetDescriptor().NumElements,  sizeof( VertexNormalTexCoord ), TypeID::get< VertexNormalTexCoord >(), PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLELIST );
		// vertexBuffer->GetDescriptor().NumElements - I don't know how many elements should be yet. [Suppres warning]


		auto& segments = cloneFighterMesh->GetSegments();
		REQUIRE( segments.size() == 2 );

		
// ================================ //
// Segment 0

		auto& segment0 = segments[ 0 ];
		CHECK( segment0.BaseVertex == 0 );
		CHECK( segment0.BufferOffset == 0 );
		CHECK( segment0.NumVertices == 2067 );
		CHECK( segment0.Topology == PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLELIST );

		auto& material0 = segment0.Material;
		
		CHECK( material0 != nullptr );
		if( material0 )
		{
			auto& pixelShader = material0->GetPixelShader();
			CHECK( pixelShader->GetShaderFile() == L"shaders/default_shaders.fx" );
			CHECK( pixelShader->GetShaderEntry() == "pixel_shader" );

			auto& vertexShader = material0->GetVertexShader();
			CHECK( vertexShader->GetShaderFile() == L"shaders/default_shaders.fx" );
			CHECK( vertexShader->GetShaderEntry() == "vertex_shader" );

			CHECK( material0->GetGeometryShader() == nullptr );
			CHECK( material0->GetTessControlShader() == nullptr );
			CHECK( material0->GetTessEvaluationShader() == nullptr );

			CHECK( material0->GetTexture( 0 ) == nullptr );
			CHECK( material0->GetTexture( 1 ) == nullptr );
			CHECK( material0->GetTexture( 2 ) == nullptr );
			CHECK( material0->GetTexture( 3 ) == nullptr );
			CHECK( material0->GetTexture( 4 ) == nullptr );

			TestBufferObject( material0->GetMaterialBuffer(), 1, sizeof( PhongMaterial ), TypeID::get< PhongMaterial >() );
			
			auto& desc = material0->GetDescriptor();
			CHECK( desc.AdditionalBuffers.size() == 0 );
			CHECK( desc.ShadingData.get() != nullptr );
		}


// ================================ //
// Segment 1

		auto& segment1 = segments[ 1 ];
		CHECK( segment1.BaseVertex == 0 );
		CHECK( segment1.BufferOffset == 2067 );
		CHECK( segment1.NumVertices == 153099 );
		CHECK( segment1.Topology == PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLELIST );

		auto& material1 = segment1.Material;

		CHECK( material1 != nullptr );
		if( material1 )
		{
			auto& pixelShader = material1->GetPixelShader();
			CHECK( pixelShader->GetShaderFile() == L"shaders\\tex_diffuse_shader.fx" );
			CHECK( pixelShader->GetShaderEntry() == "pixel_shader" );

			auto& vertexShader = material1->GetVertexShader();
			CHECK( vertexShader->GetShaderFile() == L"shaders/default_shaders.fx" );
			CHECK( vertexShader->GetShaderEntry() == "vertex_shader" );

			CHECK( material1->GetGeometryShader() == nullptr );
			CHECK( material1->GetTessControlShader() == nullptr );
			CHECK( material1->GetTessEvaluationShader() == nullptr );

			CHECK( material1->GetTexture( 0 ) != nullptr );
			if( material1->GetTexture( 0 ) )
			{
				auto tex = material1->GetTexture( 0 );
				CHECK( tex->GetFilePath().String() == filesystem::Path( "tylko_do_testow/clone_fighter/ARC170_TXT_VERSION_4_D.jpg" ).String() );
			}

			CHECK( material1->GetTexture( 1 ) == nullptr );
			CHECK( material1->GetTexture( 2 ) == nullptr );
			CHECK( material1->GetTexture( 3 ) == nullptr );
			CHECK( material1->GetTexture( 4 ) == nullptr );

			TestBufferObject( material1->GetMaterialBuffer(), 1, sizeof( PhongMaterial ), TypeID::get< PhongMaterial >() );
			
			auto& desc = material1->GetDescriptor();
			CHECK( desc.AdditionalBuffers.size() == 0 );
			CHECK( desc.ShadingData.get() != nullptr );
		}

	}

//====================================================================================//
//			SimpleBox	
//====================================================================================//


	SECTION( "Loading Simple Box" )
	{
		auto simpleBoxMesh = assetsManager.LoadMesh( SIMPLE_BOX );
		REQUIRE( simpleBoxMesh != nullptr );

		auto indexBuffer = simpleBoxMesh->GetIndexBuffer();
		auto vertexBuffer = simpleBoxMesh->GetVertexBuffer();
		
		REQUIRE( indexBuffer != nullptr );
		REQUIRE( vertexBuffer != nullptr );

		CHECK( simpleBoxMesh->GetResourceName() == SIMPLE_BOX.String() );
		
		TestIndexBuffer( indexBuffer, 36,  sizeof( Index16 ), TypeID::get< Index16 >() );
		TestVertexBuffer( vertexBuffer, 24,  sizeof( VertexNormalTexCoord ), TypeID::get< VertexNormalTexCoord >(), PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLELIST );

		auto& segments = simpleBoxMesh->GetSegments();
		REQUIRE( segments.size() == 1 );

// ================================ //
// Segment 0 

		auto& segment0 = segments[ 0 ];
		CHECK( segment0.BaseVertex == 0 );
		CHECK( segment0.BufferOffset == 0 );
		CHECK( segment0.NumVertices == 36 );
		CHECK( segment0.Topology == PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLELIST );

		auto& material0 = segment0.Material;
		
		CHECK( material0 != nullptr );
		if( material0 )
		{
			auto& pixelShader = material0->GetPixelShader();
			CHECK( pixelShader->GetShaderFile() == L"shaders\\tex_diffuse_shader.fx" );
			CHECK( pixelShader->GetShaderEntry() == "pixel_shader" );

			auto& vertexShader = material0->GetVertexShader();
			CHECK( vertexShader->GetShaderFile() == L"shaders/default_shaders.fx" );
			CHECK( vertexShader->GetShaderEntry() == "vertex_shader" );

			CHECK( material0->GetGeometryShader() == nullptr );
			CHECK( material0->GetTessControlShader() == nullptr );
			CHECK( material0->GetTessEvaluationShader() == nullptr );

			CHECK( material0->GetTexture( 0 ) != nullptr );
			if( material0->GetTexture( 0 ) )
			{
				auto tex = material0->GetTexture( 0 );
				CHECK( tex->GetFilePath().String() == filesystem::Path( "tylko_do_testow/SimpleBox/Diffuse.jpg" ).String() );
			}

			CHECK( material0->GetTexture( 0 ) != nullptr );
			if( material0->GetTexture( 1 ) )
			{
				auto tex = material0->GetTexture( 1 );
				CHECK( tex->GetFilePath().String() == filesystem::Path( "tylko_do_testow/SimpleBox/Specular.jpg" ).String() );
			}

			CHECK( material0->GetTexture( 2 ) == nullptr );
			CHECK( material0->GetTexture( 3 ) == nullptr );
			CHECK( material0->GetTexture( 4 ) == nullptr );

			TestBufferObject( material0->GetMaterialBuffer(), 1, sizeof( PhongMaterial ), TypeID::get< PhongMaterial >() );
			
			auto& desc = material0->GetDescriptor();
			CHECK( desc.AdditionalBuffers.size() == 0 );
			CHECK( desc.ShadingData.get() != nullptr );
		}

	}


//====================================================================================//
//			Moon	
//====================================================================================//

	SECTION( "Loading Moon")
	{
		auto moonMesh = assetsManager.LoadMesh( MOON );
		REQUIRE( moonMesh != nullptr );

		auto indexBuffer = moonMesh->GetIndexBuffer();
		auto vertexBuffer = moonMesh->GetVertexBuffer();
		
		REQUIRE( indexBuffer != nullptr );
		REQUIRE( vertexBuffer != nullptr );

		CHECK( moonMesh->GetResourceName() == MOON.String() );
		
		TestIndexBuffer( indexBuffer, indexBuffer->GetDescriptor().NumElements,  sizeof( Index16 ), TypeID::get< Index16 >() );
		TestVertexBuffer( vertexBuffer, vertexBuffer->GetDescriptor().NumElements,  sizeof( VertexNormalTexCoord ), TypeID::get< VertexNormalTexCoord >(), PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLELIST );
		// vertexBuffer->GetDescriptor().NumElements - I don't know how many elements should be yet. [Suppres warning]

	}


//====================================================================================//
//			Nebulon	
//====================================================================================//

	
	SECTION( "Loading Nebulon")
	{
		auto nebulonMesh = assetsManager.LoadMesh( NEBULON );
		REQUIRE( nebulonMesh != nullptr );

		auto indexBuffer = nebulonMesh->GetIndexBuffer();
		auto vertexBuffer = nebulonMesh->GetVertexBuffer();
		
		REQUIRE( indexBuffer != nullptr );
		REQUIRE( vertexBuffer != nullptr );

		CHECK( nebulonMesh->GetResourceName() == NEBULON.String() );
		
		TestIndexBuffer( indexBuffer, indexBuffer->GetDescriptor().NumElements,  sizeof( Index16 ), TypeID::get< Index16 >() );
		TestVertexBuffer( vertexBuffer, vertexBuffer->GetDescriptor().NumElements,  sizeof( VertexNormalTexCoord ), TypeID::get< VertexNormalTexCoord >(), PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLELIST );
		// vertexBuffer->GetDescriptor().NumElements - I don't know how many elements should be yet. [Suppres warning]

	}

//====================================================================================//
//			Vader TIE	
//====================================================================================//

	SECTION( "Loading vader TIE")
	{
		auto vaderTIEMesh = assetsManager.LoadMesh( VADER_TIE );
		REQUIRE( vaderTIEMesh != nullptr );

		auto indexBuffer = vaderTIEMesh->GetIndexBuffer();
		auto vertexBuffer = vaderTIEMesh->GetVertexBuffer();
		
		REQUIRE( indexBuffer != nullptr );
		REQUIRE( vertexBuffer != nullptr );

		CHECK( vaderTIEMesh->GetResourceName() == VADER_TIE.String() );
		
		TestIndexBuffer( indexBuffer, indexBuffer->GetDescriptor().NumElements,  sizeof( Index32 ), TypeID::get< Index32 >() );
		TestVertexBuffer( vertexBuffer, vertexBuffer->GetDescriptor().NumElements,  sizeof( VertexNormalTexCoord ), TypeID::get< VertexNormalTexCoord >(), PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLELIST );
		// vertexBuffer->GetDescriptor().NumElements - I don't know how many elements should be yet. [Suppres warning]

		auto& segments = vaderTIEMesh->GetSegments();
		REQUIRE( segments.size() == 14 );
	}

//====================================================================================//
//			TIE Fighter	
//====================================================================================//

	SECTION( "Loading TIE Fighter")
	{
		auto tieMesh = assetsManager.LoadMesh( TIE_FIGHTER );
		REQUIRE( tieMesh != nullptr );

		auto indexBuffer = tieMesh->GetIndexBuffer();
		auto vertexBuffer = tieMesh->GetVertexBuffer();
		
		REQUIRE( indexBuffer != nullptr );
		REQUIRE( vertexBuffer != nullptr );

		CHECK( tieMesh->GetResourceName() == TIE_FIGHTER.String() );
		
		TestIndexBuffer( indexBuffer, indexBuffer->GetDescriptor().NumElements,  sizeof( Index32 ), TypeID::get< Index32 >() );
		TestVertexBuffer( vertexBuffer, vertexBuffer->GetDescriptor().NumElements,  sizeof( VertexNormalTexCoord ), TypeID::get< VertexNormalTexCoord >(), PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLELIST );
		// vertexBuffer->GetDescriptor().NumElements - I don't know how many elements should be yet. [Suppres warning]
	}

//====================================================================================//
//			Gate	
//====================================================================================//

	SECTION( "Loading Gate")
	{
		auto gateMesh = assetsManager.LoadMesh( GATE );
		REQUIRE( gateMesh != nullptr );

		auto indexBuffer = gateMesh->GetIndexBuffer();
		auto vertexBuffer = gateMesh->GetVertexBuffer();
		
		REQUIRE( indexBuffer != nullptr );
		REQUIRE( vertexBuffer != nullptr );

		CHECK( gateMesh->GetResourceName() == GATE.String() );
		
		TestIndexBuffer( indexBuffer, indexBuffer->GetDescriptor().NumElements,  sizeof( Index16 ), TypeID::get< Index16 >() );
		TestVertexBuffer( vertexBuffer, vertexBuffer->GetDescriptor().NumElements,  sizeof( VertexNormalTexCoord ), TypeID::get< VertexNormalTexCoord >(), PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLELIST );
		// vertexBuffer->GetDescriptor().NumElements - I don't know how many elements should be yet. [Suppres warning]
	}




	delete engine;
}

// ================================ //
//
void TestIndexBuffer( const ResourcePtr<BufferObject>& buffer, uint32 expNumElements, uint32 expElemSize, TypeID type )
{
	CHECK( buffer->GetDescriptor().BufferType == BufferType::IndexBuffer );

	TestBufferObject( buffer, expNumElements, expElemSize, type );
}

// ================================ //
//
void TestVertexBuffer( const ResourcePtr<BufferObject>& buffer, uint32 expNumElements, uint32 expElemSize, TypeID type, PrimitiveTopology topology )
{
	CHECK( buffer->GetDescriptor().BufferType == BufferType::VertexBuffer );
	CHECK( buffer->GetDescriptor().Topology == topology );

	TestBufferObject( buffer, expNumElements, expElemSize, type );
}

// ================================ //
//
void TestBufferObject( const ResourcePtr<BufferObject>& buffer, uint32 expNumElements, uint32 expElemSize, TypeID type )
{
	if( buffer->GetDescriptor().DataType != type )
		WARN( "Buffer content type are different. (This is only warning. Providing content type is optional)" );
	CHECK( buffer->GetDescriptor().NumElements == expNumElements );
	CHECK( buffer->GetDescriptor().ElementSize == expElemSize );	
}
