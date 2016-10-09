#include "catch.hpp"

#include "EngineCore/MainEngine/Engine.h"
#include "EngineCore/ModelsManager/AssetsManager.h"


#define WINDOW_WIDTH		1280
#define WINDOW_HEIGHT		800


TEST_CASE( "Loading assets", "[FBXLoader]" )
{
	Engine* engine = new Engine();
	AssetsManager& assetsManager = *engine->GetAssetsManager();

	const filesystem::Path CLONE_FIGHTER = "tylko_do_testow/ARC.FBX";
	const filesystem::Path MOON = "tylko_do_testow/moon/moon.FBX";
	const filesystem::Path NEBULON = "tylko_do_testow/Nebulon/Nebulon.FBX";
	const filesystem::Path VADER_TIE = "tylko_do_testow/VadersTIE.FBX";
	const filesystem::Path TIE_FIGHTER = "tylko_do_testow/TIE_Fighter/TIE_Fighter.FBX";
	const filesystem::Path IMPERIAL_STAR_DESTROYER = "tylko_do_testow/ImperialStarDestroyer.FBX";
	const filesystem::Path CHURCH = "tylko_do_testow/Church/AbandonedChurch.FBX";
	const filesystem::Path GATE = "meshes/Wall/Castle wall/Castle wall.FBX";

	SECTION( "Loading Clone Fighter" )
	{
		auto cloneFighterMesh = assetsManager.LoadMesh( CLONE_FIGHTER );
		REQUIRE( cloneFighterMesh != nullptr );
	}

	SECTION( "Loading Moon")
	{
		auto moonMesh = assetsManager.LoadMesh( MOON );
		REQUIRE( moonMesh != nullptr );
	}

	SECTION( "Loading Nebulon")
	{
		auto nebulonMesh = assetsManager.LoadMesh( MOON );
		REQUIRE( nebulonMesh != nullptr );
	}

	SECTION( "Loading vader TIE")
	{
		auto vaderTIEMesh = assetsManager.LoadMesh( MOON );
		REQUIRE( vaderTIEMesh != nullptr );
	}

	SECTION( "Loading TIE Fighter")
	{
		auto tieMesh = assetsManager.LoadMesh( MOON );
		REQUIRE( tieMesh != nullptr );
	}

	SECTION( "Loading Gate")
	{
		auto gateMesh = assetsManager.LoadMesh( MOON );
		REQUIRE( gateMesh != nullptr );
	}
	
}

