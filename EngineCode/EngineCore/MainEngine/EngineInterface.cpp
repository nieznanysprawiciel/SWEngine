/**@file EngineInterface.cpp
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Plik zawiera implementacjê interfejsu EngineInterface dla klasy Engine
*/
#include "EngineCore/stdafx.h"

#include "EngineContext.h"
#include "EngineCore/MainEngine/Engine.h"
#include "GraphicAPI/ResourcesFactory.h"

#include <mutex>

#include "Common/MemoryLeaks.h"

using namespace DirectX;


EngineInterface::EngineInterface()
{
	Context = new EngineContext;

	Api::ApiGroup::Engine = static_cast< Engine* >( this );		// EngineInterface jest tak na prawdê klas¹ Engine, wiêc takie rzutowanie jest legalne.
	Api::ApiGroup::Context = Context;
}

EngineInterface::~EngineInterface()
{
	delete Context;
}

#ifdef __TEST

#include "EngineCore/ControllersEngine/BasicControllers/SpectatorCameraController.h"
#include "EngineCore/Features/HosekSkylightModel/HosekSkyDome.h"

#include <math.h>
#include "EngineInterface.h"
#include "EngineCore/ModelsManager/Assets/Meshes/MeshAsset.h"
#include "EngineCore/ModelsManager/Assets/Materials/MaterialAsset.h"
#include "EngineCore/ModelsManager/Assets/Materials/PhongMaterialData.h"
#include "Common/System/Path.h"



/**@brief */
void Engine::test()
{
	const wchar_t CLONE_FIGHTER[] = L"tylko_do_testow/ARC.FBX";
	const wchar_t MOON[] = L"tylko_do_testow/moon/moon.FBX";
	const wchar_t NEBULON[] = L"tylko_do_testow/Nebulon/Nebulon.FBX";
	const wchar_t VADER_TIE[] = L"tylko_do_testow/VadersTIE.FBX";
	const wchar_t TIE_FIGHTER[] = L"tylko_do_testow/TIE_Fighter/TIE_Fighter.FBX";
	const wchar_t IMPERIAL_STAR_DESTROYER[] = L"tylko_do_testow/ImperialStarDestroyer.FBX";
	const wchar_t CHURCH[] = L"tylko_do_testow/Church/AbandonedChurch.FBX";
	const wchar_t GATE[] = L"meshes/Wall/Castle wall/Castle wall.FBX";

	Context->modelsManager->LoadModelFromFile( CLONE_FIGHTER );
	Context->modelsManager->LoadModelFromFile( MOON );
	Context->modelsManager->LoadModelFromFile( NEBULON );
	Context->modelsManager->LoadModelFromFile( VADER_TIE );
	Context->modelsManager->LoadModelFromFile( TIE_FIGHTER );
	Context->modelsManager->LoadModelFromFile( GATE );
	//Context->modelsManager->LoadModelFromFile( CHURCH );
	//modelsManager->LoadModelFromFile( IMPERIAL_STAR_DESTROYER );
	

	//dodawanie ksiê¿yca
	StaticActor* moon = Actors.CreateActor< StaticActor >( GetTypeidName< StaticActor >(), EnableDisplay );
	XMVECTOR position = XMVectorSet( 4000.0, 0.0, 8000.0, 0.0 );
	moon->Teleport( position );

	moon->SetModel( Context->modelsManager->GetModel( MOON ) );
	moon->SetScale( 30.0 );


	//dodawanie Nebulona
	StaticActor* nebulon = Actors.CreateActor< StaticActor >( GetTypeidName< StaticActor >(), EnableDisplay );
	position = XMVectorSet( 400.0, 0.0, -6000.0, 0.0 );
	nebulon->Teleport( position );

	nebulon->SetModel( Context->modelsManager->GetModel( NEBULON ) );
	nebulon->SetScale( 1 );	//Nebulon ma Ÿle ustawiony pivot, wiêc jak siê przeskaluje to odleci gdzieœ w kosmos i go nie widaæ.


	//dodawanie TIE Fightera
	StaticActor* TIE = Actors.CreateActor< StaticActor >( GetTypeidName< StaticActor >(), EnableDisplay );
	position = XMVectorSet( -400.0, 0.0, 800.0, 0.0 );
	TIE->Teleport( position );

	TIE->SetModel( Context->modelsManager->GetModel( TIE_FIGHTER ) );
	TIE->SetScale( 10 );


	//dodawanie TIE Fightera Vadera
	StaticActor* VaderTIE = Actors.CreateActor< StaticActor >( GetTypeidName< StaticActor >(), EnableDisplay );
	position = XMVectorSet( -400.0, 0.0, -3000.0, 0.0 );
	VaderTIE->Teleport( position );

	VaderTIE->SetModel( Context->modelsManager->GetModel( VADER_TIE ) );
	//VaderTIE->SetScale( 1.0 );


	//dodawanie myœliwca
	DynamicActor* cloneFighter = Actors.CreateActor< DynamicActor >( GetTypeidName< DynamicActor >(), EnableDisplay | EnableMovement );

	position = XMVectorSet(0.0, 0.0, 6000.0, 0.0);
	cloneFighter->Teleport( position );

#ifdef _QUATERNION_SPEED
	XMVECTOR rot_vector = { 1.0f, 0.0f, 0.0f, 0.0f };
	XMVECTOR axis_angle = XMQuaternionRotationAxis(rot_vector, XMConvertToRadians(30));
#else
	XMFLOAT4 axis_angle;
	axis_angle.x = 1.0;
	axis_angle.y = 0.0;
	axis_angle.z = 0.0;
	axis_angle.w = XMConvertToRadians(30);
#endif
	cloneFighter->SetRotationSpeed( axis_angle );

	cloneFighter->SetModel( Context->modelsManager->GetModel( CLONE_FIGHTER ) );


	////dodawanie koœcio³a
	//StaticActor* church = Actors.CreateActor< StaticActor >( GetTypeidName< StaticActor >(), EnableDisplay );
	//position = XMVectorSet( -300.0, 0.0, 500.0, 0.0 );
	//church->Teleport( position );

	//church->SetModel( Context->modelsManager->GetModel( CHURCH ) );


	//dodawanie koœcio³a
	StaticActor* gate = Actors.CreateActor< StaticActor >( GetTypeidName< StaticActor >(), EnableDisplay );
	position = XMVectorSet( -300.0, 0.0, 500.0, 0.0 );
	gate->Teleport( position );

	gate->SetModel( Context->modelsManager->GetModel( GATE ) );

	//for( unsigned int i = 0; i < 100; ++i )
	//{
	//	DynamicActor* cloneFighter = Actors.CreateActor< DynamicActor >( "DynamicActor", EnableDisplay );
	//	position = XMVectorSet( 15000.0 * cos( ( 2*i*XM_PI ) / 100 ), 0.0, 15000.0 * sin( ( 2*i*XM_PI ) / 100 ), 0.0 );
	//	
	//	cloneFighter->set_model( Context->modelsManager->GetModel( CLONE_FIGHTER ) );
	//	cloneFighter->teleport( position );
	//}


	// Dodawanie gwiezdnego niszczyciela Imperial
	/*StaticActor* Imperial = Actors.CreateActor< StaticActor >( "StaticActor", EnableDisplay );
	position = XMVectorSet( -4000.0, 0.0, -3000.0, 0.0 );
	Imperial->set_position( position );

	Imperial->set_model( modelsManager->GetModel( IMPERIAL_STAR_DESTROYER ) );
	*/

	
	//dodawanie skrzyni
//	StaticActor* skrzynia = Actors.CreateActor< StaticActor >( "StaticActor", EnableDisplay | EnableMovement );
//	position = XMVectorSet( 0.0, 0.0, 2.0, 0.0 );
//	skrzynia->Teleport( position );
//
//#ifdef _QUATERNION_SPEED
//	XMVECTOR rot_vector = { 1.0f, 0.0f, 0.0f, 0.0f };
//	XMVECTOR axis_angle = XMQuaternionRotationAxis( rot_vector, XMConvertToRadians( 30 ) );
//#else
//	XMFLOAT4 axis_angle2;
//	axis_angle2.x = 1.0;
//	axis_angle2.y = 0.0;
//	axis_angle2.z = 0.0;
//	axis_angle2.w = XMConvertToRadians( 30 );
//#endif
//	skrzynia->SetRotationSpeed( axis_angle2 );
//
//	Model3DFromFile* new_model = Context->modelsManager->GetModel( L"skrzynia" );
//	skrzynia->SetModel( new_model );
//	//skrzynia->SetScale( 0.1 );

	SetSkydomeAndCamera();

	//testMaterial( Context->modelsManager->GetModel( CLONE_FIGHTER ) );


	int actorInfoSize = sizeof( ActorInfo );
	int actorDataSize = sizeof( ActorData );
	int engineInterfaceSize = sizeof( EngineInterface );
	int actorApiSize = sizeof( Api::AssetsApi );
	int vectorSize = sizeof( std::vector< int > );
	int stringSize = sizeof( std::string );
	int wstringSize = sizeof( std::wstring );
	int mapSize = sizeof( std::map< int, int > );
	int hashMapSize = sizeof( std::unordered_map< int, int > );

	int model3DFromFileSize = sizeof( Model3DFromFile );
	int meshAssetSize = sizeof( MeshAsset );
	int materialObjectSize = sizeof( MaterialObject );
	int materialInfoSize = sizeof( MaterialInfo );
	int materialAssetSize = sizeof( MaterialAsset );
	int phongMaterialSize = sizeof( PhongMaterial );

	int pathSize = sizeof( filesystem::Path );

	Size VadersTieSize = VaderTIE->MemorySize();
	Size StaticActorSize = sizeof( StaticActor );
	Size dynamicActorSize = sizeof( DynamicActor );
	Size complexActorSize = sizeof( ComplexActor );
	Size ChildActorSize = sizeof( ChildActor );
}

/**@brief */
void Engine::SetSkydomeAndCamera()
{
	// Ustawienie aktywnej kamery
	CameraActor* camera = Actors.CreateActor< CameraActor >( GetTypeidName< CameraActor >(), AsCamera | EnablePreController | EnableMovement );
	XMVECTOR cameraPos = XMVectorSet( 0.0, 0.0, 0.0, 0.0 );
	camera->Teleport( cameraPos );

	// Przypisujemy kontroler ( dla kontrolerów trzeba zrobiæ jakiœ mechanizm przechowywania i zwalniania)
	SpectatorCameraController* controller = new SpectatorCameraController(
	Context->ui_engine->GetStandardAbstractionLayer( STANDARD_ABSTRACTION_LAYER::PROTOTYPE_BUTTONS ) );
	camera->SetController( controller );

	// Wstawiamy kamerê do odpowiednich modu³ów
	Context->displayEngine->SetCurrentCamera( camera );

	SetSkydome();
}

/**@brief */
void Engine::SetSkydome()
{
	double albedo[3] = { 0.8, 0.8, 0.8 };
	double turbidity = 4;
	XMVECTOR sunDir = XMVectorSet( -0.2f, 0.6f, 0.6f, 1.0f );
	HosekSkyDome* skyDome = new HosekSkyDome( Context->modelsManager );
	skyDome->init_sky_dome( sunDir, turbidity, albedo, 101, 101, 100, 5.0 );
	Context->displayEngine->SetSkydome( skyDome );

	sunDir = XMVectorNegate( sunDir );

	DirectX::XMFLOAT3 color( 0.8f, 0.8f, 0.8f );

	// Ustawiamy œwiat³o pod indeksem 0
	DirectionalLight* light = Actors.CreateActor< DirectionalLight >( GetTypeidName< DirectionalLight >(), AsLight );
	light->SetLightDirection( sunDir );
	light->SetLightColor( color );

	Context->displayEngine->lightModule->SetAmbientLight( DirectX::XMFLOAT3( 0.2f, 0.2f, 0.2f ) );
}

#include "EngineCore/ModelsManager/Assets/Materials/MaterialAsset.h"
#include "EngineCore/ModelsManager/Assets/Materials/MaterialAssetInitData.h"
#include "EngineCore/ModelsManager/Assets/Materials/PhongMaterialData.h"
#include "EngineCore/ModelsManager/Loaders/Material/SWMat/SWMaterialLoader.h"

#include "Common/Serialization/Serializer.h"
#include "Common/Serialization/SW/EngineSerializationContext.h"

void Engine::testMaterial( Model3DFromFile* model )
{
	auto part = model->get_part( 1 );

	MaterialCreateData init;
	init.Data.VertexShader = part->vertex_shader;
	init.Data.PixelShader = part->pixel_shader;
	init.Data.Textures[ 0 ] = part->GetTexture1();

	// Shading model data

	PhongMaterial phongMaterial;
	phongMaterial.Diffuse = DirectX::XMFLOAT4( 1.0f, 0.4f, 2.0f, 1.0f );
	phongMaterial.Ambient = DirectX::XMFLOAT4( 0.0f, 0.4f, 0.0f, 0.0f );
	phongMaterial.Specular = DirectX::XMFLOAT4( 1.0f, 1.0f, 0.5f, 0.0f );
	phongMaterial.Emissive = DirectX::XMFLOAT3( 0.0f, 0.01f, 0.0f );
	phongMaterial.Power = 128.0;

	ShadingModelData< PhongMaterial >* shadingData = new ShadingModelData< PhongMaterial >();
	shadingData->Data = phongMaterial;
	init.Data.ShadingData = UPtr< ShadingModelBase >( shadingData );


	// Additional buffers.
	AdditionalBufferInfo addBuff;
	addBuff.BufferSize = sizeof( PhongMaterial );
	addBuff.BufferType = TypeID::get< PhongMaterial >();
	addBuff.ShaderType = ShaderType::PixelShader;
	
	init.Data.AdditionalBuffers.push_back( addBuff );

	// Memory leak!!
	MaterialAsset* newMaterial = new MaterialAsset( L"::Generated", std::move( init ) );

	SWMaterialLoader loader( Context->modelsManager );
	loader.SaveMaterial( "tylko_do_testow/serialization/materials/materialSerialize.swmat", newMaterial );

	auto mat = loader.LoadMaterial( "tylko_do_testow/serialization/materials/materialDeserialize.swmat" );


	//ISerializer ser( std::make_unique< EngineSerializationContext >() );
	//newMaterial->Serialize( &ser );
	//ser.SaveFile( "tylko_do_testow/serialization/materialBruteSerialize.swmat", WritingMode::Readable );
}

#endif

