/**@file EngineInterface.cpp
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Plik zawiera implementacjê interfejsu EngineInterface dla klasy Engine
*/
#include "EngineCore/stdafx.h"
#include "Engine.h"
#include "EngineCore/EventsManager/Event.h"
#include "EngineCore/CollisionEngine/CollisionEngine.h"
#include "EngineCore/ControllersEngine/ControllersEngine.h"
#include "EngineCore/GamePlay/FableEngine.h"
#include "EngineCore/DisplayEngine/DisplayEngine.h"
#include "EngineCore/ModelsManager/ModelsManager.h"
#include "EngineCore/PhysicEngine/PhysicEngine.h"
#include "EngineCore/PhysicEngine/MovementEngine.h"
#include "EngineCore/SoundEngine/SoundEngine.h"
#include "EngineCore/UIEngine/UI_Engine.h"

#include <mutex>

#include "Common/memory_leaks.h"



Engine* EngineInterface::InterfaceGroup::m_engine = nullptr;


EngineInterface::EngineInterface()
{
	EngineInterface::InterfaceGroup::m_engine = static_cast<Engine*>( this );		// EngineInterface jest tak na prawdê klas¹ Engine, wiêc takie rzutowanie jest legalne.
}

#ifdef __TEST

#include "EngineCore/ControllersEngine/camera_controller_PROTOTYPE.h"
#include "EngineCore/Features/HosekSkylightModel/HosekSkyDome.h"

#include <math.h>

void Engine::test()
{
	const wchar_t CLONE_FIGHTER[] = L"tylko_do_testow/ARC.FBX";
	const wchar_t MOON[] = L"tylko_do_testow/moon/moon.FBX";
	const wchar_t NEBULON[] = L"tylko_do_testow/Nebulon/Nebulon.FBX";
	const wchar_t VADER_TIE[] = L"tylko_do_testow/VadersTIE.FBX";
	const wchar_t TIE_FIGHTER[] = L"tylko_do_testow/TIE_Fighter/TIE_Fighter.FBX";
	const wchar_t IMPERIAL_STAR_DESTROYER[] = L"tylko_do_testow/ImperialStarDestroyer.FBX";

	Context.modelsManager->LoadModelFromFile( CLONE_FIGHTER );
	Context.modelsManager->LoadModelFromFile( MOON );
	Context.modelsManager->LoadModelFromFile( NEBULON );
	Context.modelsManager->LoadModelFromFile( VADER_TIE );
	Context.modelsManager->LoadModelFromFile( TIE_FIGHTER );
	//modelsManager->LoadModelFromFile( IMPERIAL_STAR_DESTROYER );
	
	//modelsManager->test();			// Tu sie odbywa wczytywanie

	//dodawanie ksiê¿yca
	DynamicMeshObject* moon = new DynamicMeshObject;
	XMVECTOR position = XMVectorSet( 4000.0, 0.0, 8000.0, 0.0 );
	moon->teleport( position );

	moon->set_model( Context.modelsManager->GetModel( MOON ) );
	moon->set_scale( 30.0 );

	object_list.push_back( moon );
	Context.displayEngine->add_dynamic_mesh_object( moon );

	//dodawanie Nebulona
	DynamicMeshObject* nebulon = new DynamicMeshObject;
	position = XMVectorSet( 400.0, 0.0, -6000.0, 0.0 );
	nebulon->teleport( position );

	nebulon->set_model( Context.modelsManager->GetModel( NEBULON ) );
	nebulon->set_scale( 1 );	//Nebulon ma Ÿle ustawiony pivot, wiêc jak siê przeskaluje to odleci gdzieœ w kosmos i go nie widaæ.

	object_list.push_back( nebulon );
	Context.displayEngine->add_dynamic_mesh_object( nebulon );

	//dodawanie TIE Fightera
	DynamicMeshObject* TIE = new DynamicMeshObject;
	position = XMVectorSet( -400.0, 0.0, 800.0, 0.0 );
	TIE->teleport( position );

	TIE->set_model( Context.modelsManager->GetModel( TIE_FIGHTER ) );
	TIE->set_scale( 10 );

	object_list.push_back( TIE );
	Context.displayEngine->add_dynamic_mesh_object( TIE );


	//dodawanie TIE Fightera Vadera
	DynamicMeshObject* VaderTIE = new DynamicMeshObject;
	position = XMVectorSet( -400.0, 0.0, -3000.0, 0.0 );
	VaderTIE->teleport( position );

	VaderTIE->set_model( Context.modelsManager->GetModel( VADER_TIE ) );
	//VaderTIE->set_scale( 1.0 );

	object_list.push_back( VaderTIE );
	Context.displayEngine->add_dynamic_mesh_object( VaderTIE );

	//dodawanie myœliwca
	DynamicMeshObject* clone_fighter = new DynamicMeshObject;
	position = XMVectorSet(0.0, 0.0, 6000.0, 0.0);
	clone_fighter->teleport( position );

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
	clone_fighter->set_rotation_speed( axis_angle );

	clone_fighter->set_model( Context.modelsManager->GetModel( CLONE_FIGHTER ) );

	object_list.push_back( clone_fighter );
	Context.displayEngine->add_dynamic_mesh_object( clone_fighter );
	Context.movementEngine->add_moveable_object( clone_fighter );


	//for( unsigned int i = 0; i < 100; ++i )
	//{
	//	DynamicMeshObject* cloneFighter = new DynamicMeshObject;
	//	position = XMVectorSet( 15000.0 * cos( ( 2*i*XM_PI ) / 100 ), 0.0, 15000.0 * sin( ( 2*i*XM_PI ) / 100 ), 0.0 );
	//	
	//	cloneFighter->set_model( Context.modelsManager->GetModel( CLONE_FIGHTER ) );
	//	cloneFighter->teleport( position );
	//	object_list.push_back( cloneFighter );
	//	Context.displayEngine->add_dynamic_mesh_object( cloneFighter );
	//}


	// Dodawanie gwiezdnego niszczyciela Imperial
	/*DynamicMeshObject* Imperial = new DynamicMeshObject;
	position = XMVectorSet( -4000.0, 0.0, -3000.0, 0.0 );
	Imperial->set_position( position );

	Imperial->set_model( modelsManager->GetModel( IMPERIAL_STAR_DESTROYER ) );

	object_list.push_back( Imperial );
	displayEngine->add_dynamic_mesh_object( Imperial );
	*/

	/*
	//dodawanie skrzyni
	DynamicMeshObject* skrzynia = new DynamicMeshObject;
	position = XMVectorSet( 0.0, 0.0, 2.0, 0.0 );
	skrzynia->set_position( position );

#ifdef _QUATERNION_SPEED
	XMVECTOR rot_vector = { 1.0f, 0.0f, 0.0f, 0.0f };
	XMVECTOR axis_angle = XMQuaternionRotationAxis( rot_vector, XMConvertToRadians( 30 ) );
#else
	XMFLOAT4 axis_angle2;
	axis_angle2.x = 1.0;
	axis_angle2.y = 0.0;
	axis_angle2.z = 0.0;
	axis_angle2.w = XMConvertToRadians( 30 );
#endif
	//skrzynia->set_rotation_speed( axis_angle2 );

	Model3DFromFile* new_model = modelsManager->GetModel( L"skrzynia" );
	skrzynia->set_model( new_model );
	//skrzynia->set_scale( 0.1 );

	object_list.push_back( skrzynia );
	displayEngine->add_dynamic_mesh_object( skrzynia );
	movementEngine->add_moveable_object( skrzynia );

	*/

	//ustawienie aktywnej kamery
	CameraObject* camera = new CameraObject();
	XMVECTOR camera_pos = XMVectorSet( 0.0, 0.0, 0.0, 0.0 );
	camera->teleport( camera_pos );
	//przypisujemy kontroler ( dla kontrolerów trzeba zrobiæ jakiœ mechanizm przechowywania i zwalniania)
	camera_controller_PROTOTYPE* controller = new camera_controller_PROTOTYPE(
	Context.ui_engine->GetStandardAbstractionLayer( STANDARD_ABSTRACTION_LAYER::PROTOTYPE_BUTTONS ) );
	camera->set_controller(controller);

	//wstawiamy kamerê do odpowiednich modu³ów
	Context.displayEngine->add_camera( camera );
	Context.displayEngine->set_current_camera( camera );
	Context.movementEngine->add_moveable_object( camera );
	Context.controllersEngine->add_pre_controlled( camera );

	double albedo[3] = { 0.8, 0.8, 0.8 };
	double turbidity = 4;
	XMVECTOR sun_dir = XMVectorSet( -0.2f, 0.6f, 0.6f, 1.0f );
	HosekSkyDome* skyDome = new HosekSkyDome( Context.modelsManager );
	skyDome->init_sky_dome( sun_dir, turbidity, albedo, 101, 101, 100, 5.0 );
	Context.displayEngine->set_skydome( skyDome );

	sun_dir = XMVectorNegate( sun_dir );

	DirectX::XMFLOAT4 color( 0.8f, 0.8f, 0.8f, 1.0f );
	DirectX::XMFLOAT4 direction;
	XMStoreFloat4( &direction, sun_dir );

	// Ustawiamy œwiat³o pod indeksem 0
	Context.displayEngine->set_directional_light( direction, color, 0 );
	Context.displayEngine->set_ambient_light( DirectX::XMFLOAT4( 0.2f, 0.2f, 0.2f, 1.0f ) );



}

#endif

//=====================================================================================================================//
//								EngineInterface::Assets
//=====================================================================================================================//


/**@brief Pobiera model o podanej nazwie.

Model jest pobierany tylko, je¿eli zosta³ wczeœniej wczytany.
@param[in] name Nazwa pliku z modelem.
@return Zwraca obiekt zawieraj¹cy model lub nullptr, je¿eli model nie zosta³ wczeœniej wczytany.*/
Model3DFromFile* EngineInterface::Assets::Models::GetSync( const std::wstring& name )
{
	return m_engine->Context.modelsManager->GetModel( name );
}


/**@brief Wczytuje model z podanego pliku.

Model jest ³adowany synchronicznie, dzia³anie silnika zawiesza siê, dopóki wczytywanie nie zakoñczy siê.
@param[in] Nazwa pliku.
@return Zwraca model lub nullptr, je¿eli wczytywanie nie powiod³o siê.*/
Model3DFromFile* EngineInterface::Assets::Models::LoadSync( const std::wstring& name )
{
	auto result = m_engine->Context.modelsManager->LoadModelFromFile( name );
	if( result != MODELS_MANAGER_OK )
		return nullptr;

	return m_engine->Context.modelsManager->GetModel( name );
}


//=====================================================================================================================//
//								EngineInterface::Actors
//=====================================================================================================================//



std::vector<DynamicMeshObject*> EngineInterface::Actors::GetSceneObjects()
{ return m_engine->Context.displayEngine->GetSceneObjects(); }



//=====================================================================================================================//
//								EngineInterface::Input
//=====================================================================================================================//


/**@brief Pobiera jedn¹ ze standardowych (wbudowanych) warstw abstrakcji.

@param[in] layer Enum oznaczaj¹cy pobieran¹ warstwê.
@return Zwraca wskaŸnik na pobieran¹ warstwê.
*/
InputAbstractionLayer*		EngineInterface::Input::GetStandardAbstractionLayer		( STANDARD_ABSTRACTION_LAYER layer )
{	return m_engine->Context.ui_engine->GetStandardAbstractionLayer( layer );	}

/**@brief Ustawia podan¹ warstwê abstrakcji jako aktualn¹.

@param[in] layer WskaŸnik na warstwê abstrakcji.
*/
void						EngineInterface::Input::SetAbstractionLayer				( InputAbstractionLayer* layer )
{	m_engine->Context.ui_engine->ChangeAbstractionLayer( layer );	}

/**@brief Ustawia jedn¹ ze standardowych (wbudowanych) warstw abstrakcji.

@param[in] layer Enum oznaczaj¹cy wybran¹ warstwê.
*/
void						EngineInterface::Input::SetStandardAbstractionLayer		( STANDARD_ABSTRACTION_LAYER layer )
{	m_engine->Context.ui_engine->SetStandardAbstractionLayer( layer );	}

