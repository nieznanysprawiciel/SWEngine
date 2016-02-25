/**@file EngineInterface.cpp
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Plik zawiera implementacj� interfejsu EngineInterface dla klasy Engine
*/
#include "EngineCore/stdafx.h"

#include "EngineContext.h"
#include "EngineCore/MainEngine/Engine.h"
#include "GraphicAPI/ResourcesFactory.h"

#include <mutex>

#include "Common/memory_leaks.h"

using namespace DirectX;


EngineInterface::EngineInterface()
{
	Context = new EngineContext;

	Api::ApiGroup::Engine = static_cast< Engine* >( this );		// EngineInterface jest tak na prawd� klas� Engine, wi�c takie rzutowanie jest legalne.
	Api::ApiGroup::Context = Context;
}

EngineInterface::~EngineInterface()
{
	delete Context;
}

#ifdef __TEST

#include "EngineCore/ControllersEngine/camera_controller_PROTOTYPE.h"
#include "EngineCore/Features/HosekSkylightModel/HosekSkyDome.h"

#include <math.h>
#include "EngineInterface.h"

void Engine::test()
{
	const wchar_t CLONE_FIGHTER[] = L"tylko_do_testow/ARC.FBX";
	const wchar_t MOON[] = L"tylko_do_testow/moon/moon.FBX";
	const wchar_t NEBULON[] = L"tylko_do_testow/Nebulon/Nebulon.FBX";
	const wchar_t VADER_TIE[] = L"tylko_do_testow/VadersTIE.FBX";
	const wchar_t TIE_FIGHTER[] = L"tylko_do_testow/TIE_Fighter/TIE_Fighter.FBX";
	const wchar_t IMPERIAL_STAR_DESTROYER[] = L"tylko_do_testow/ImperialStarDestroyer.FBX";

	Context->modelsManager->LoadModelFromFile( CLONE_FIGHTER );
	Context->modelsManager->LoadModelFromFile( MOON );
	Context->modelsManager->LoadModelFromFile( NEBULON );
	Context->modelsManager->LoadModelFromFile( VADER_TIE );
	Context->modelsManager->LoadModelFromFile( TIE_FIGHTER );
	//modelsManager->LoadModelFromFile( IMPERIAL_STAR_DESTROYER );
	
	Context->modelsManager->test();			// Tu sie odbywa wczytywanie

	//dodawanie ksi�yca
	DynamicMeshObject* moon = actors.CreateActor< DynamicMeshObject >( "DynamicMeshObject", EnableDisplay );
	XMVECTOR position = XMVectorSet( 4000.0, 0.0, 8000.0, 0.0 );
	moon->Teleport( position );

	moon->SetModel( Context->modelsManager->GetModel( MOON ) );
	moon->set_scale( 30.0 );


	//dodawanie Nebulona
	DynamicMeshObject* nebulon = actors.CreateActor< DynamicMeshObject >( "DynamicMeshObject", EnableDisplay );
	position = XMVectorSet( 400.0, 0.0, -6000.0, 0.0 );
	nebulon->Teleport( position );

	nebulon->SetModel( Context->modelsManager->GetModel( NEBULON ) );
	nebulon->set_scale( 1 );	//Nebulon ma �le ustawiony pivot, wi�c jak si� przeskaluje to odleci gdzie� w kosmos i go nie wida�.


	//dodawanie TIE Fightera
	DynamicMeshObject* TIE = actors.CreateActor< DynamicMeshObject >( "DynamicMeshObject", EnableDisplay );
	position = XMVectorSet( -400.0, 0.0, 800.0, 0.0 );
	TIE->Teleport( position );

	TIE->SetModel( Context->modelsManager->GetModel( TIE_FIGHTER ) );
	TIE->set_scale( 10 );


	//dodawanie TIE Fightera Vadera
	DynamicMeshObject* VaderTIE = actors.CreateActor< DynamicMeshObject >( "DynamicMeshObject", EnableDisplay );
	position = XMVectorSet( -400.0, 0.0, -3000.0, 0.0 );
	VaderTIE->Teleport( position );

	VaderTIE->SetModel( Context->modelsManager->GetModel( VADER_TIE ) );
	//VaderTIE->set_scale( 1.0 );


	//dodawanie my�liwca
	DynamicMeshObject* cloneFighter = actors.CreateActor< DynamicMeshObject >( "DynamicMeshObject", EnableDisplay | EnableMovement );

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





	//for( unsigned int i = 0; i < 100; ++i )
	//{
	//	DynamicMeshObject* cloneFighter = actors.CreateActor< DynamicMeshObject >( "DynamicMeshObject", EnableDisplay );
	//	position = XMVectorSet( 15000.0 * cos( ( 2*i*XM_PI ) / 100 ), 0.0, 15000.0 * sin( ( 2*i*XM_PI ) / 100 ), 0.0 );
	//	
	//	cloneFighter->set_model( Context->modelsManager->GetModel( CLONE_FIGHTER ) );
	//	cloneFighter->teleport( position );
	//}


	// Dodawanie gwiezdnego niszczyciela Imperial
	/*DynamicMeshObject* Imperial = actors.CreateActor< DynamicMeshObject >( "DynamicMeshObject", EnableDisplay );
	position = XMVectorSet( -4000.0, 0.0, -3000.0, 0.0 );
	Imperial->set_position( position );

	Imperial->set_model( modelsManager->GetModel( IMPERIAL_STAR_DESTROYER ) );
	*/

	
	//dodawanie skrzyni
//	DynamicMeshObject* skrzynia = actors.CreateActor< DynamicMeshObject >( "DynamicMeshObject", EnableDisplay | EnableMovement );
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
//	//skrzynia->set_scale( 0.1 );

	

	//ustawienie aktywnej kamery
	CameraObject* camera = actors.CreateActor< CameraObject >( "CameraObject", AsCamera | EnablePreController | EnableMovement );
	XMVECTOR camera_pos = XMVectorSet( 0.0, 0.0, 0.0, 0.0 );
	camera->Teleport( camera_pos );
	//przypisujemy kontroler ( dla kontroler�w trzeba zrobi� jaki� mechanizm przechowywania i zwalniania)
	camera_controller_PROTOTYPE* controller = new camera_controller_PROTOTYPE(
	Context->ui_engine->GetStandardAbstractionLayer( STANDARD_ABSTRACTION_LAYER::PROTOTYPE_BUTTONS ) );
	camera->SetController( controller );

	//wstawiamy kamer� do odpowiednich modu��w
	Context->displayEngine->SetCurrentCamera( camera );


	double albedo[3] = { 0.8, 0.8, 0.8 };
	double turbidity = 4;
	XMVECTOR sun_dir = XMVectorSet( -0.2f, 0.6f, 0.6f, 1.0f );
	HosekSkyDome* skyDome = new HosekSkyDome( Context->modelsManager );
	skyDome->init_sky_dome( sun_dir, turbidity, albedo, 101, 101, 100, 5.0 );
	Context->displayEngine->SetSkydome( skyDome );

	sun_dir = XMVectorNegate( sun_dir );

	DirectX::XMFLOAT4 color( 0.8f, 0.8f, 0.8f, 1.0f );
	DirectX::XMFLOAT4 direction;
	XMStoreFloat4( &direction, sun_dir );

	// Ustawiamy �wiat�o pod indeksem 0
	Context->displayEngine->SetDirectionalLight( direction, color, 0 );
	Context->displayEngine->SetAmbientLight( DirectX::XMFLOAT4( 0.2f, 0.2f, 0.2f, 1.0f ) );



}

#endif

