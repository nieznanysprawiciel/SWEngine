#include "..\stdafx.h"
#include "Engine.h"


#include "..\memory_leaks.h"


/*Plik zawiera implementacjê interfejsu engine_interface dla klasy Engine*/

#ifdef __TEST

#include "ControllersEngine\camera_controller_PROTOTYPE.h"
#include "Features\HosekSkylightModel\HosekSkyDome.h"

void Engine::test()
{
	const wchar_t CLONE_FIGHTER[] = L"tylko_do_testow/ARC.FBX";
	const wchar_t MOON[] = L"tylko_do_testow/moon/moon.FBX";
	const wchar_t NEBULON[] = L"tylko_do_testow/Nebulon/Nebulon.FBX";
	const wchar_t VADER_TIE[] = L"tylko_do_testow/VadersTIE.FBX";
	const wchar_t TIE_FIGHTER[] = L"tylko_do_testow/TIE_Fighter/TIE_Fighter.FBX";
	const wchar_t IMPERIAL_STAR_DESTROYER[] = L"tylko_do_testow/ImperialStarDestroyer.FBX";

	models_manager->load_model_from_file( CLONE_FIGHTER );
	models_manager->load_model_from_file( MOON );
	models_manager->load_model_from_file( NEBULON );
	models_manager->load_model_from_file( VADER_TIE );
	models_manager->load_model_from_file( TIE_FIGHTER );
	//models_manager->load_model_from_file( IMPERIAL_STAR_DESTROYER );
	
	//models_manager->test();			// Tu sie odbywa wczytywanie

	//dodawanie ksiê¿yca
	Dynamic_mesh_object* moon = new Dynamic_mesh_object;
	XMVECTOR position = XMVectorSet( 4000.0, 0.0, 8000.0, 0.0 );
	moon->set_position( position );

	moon->set_model( models_manager->get_model( MOON ) );
	moon->set_scale( 30.0 );

	object_list.push_back( moon );
	display_engine->add_dynamic_mesh_object( moon );

	//dodawanie Nebulona
	Dynamic_mesh_object* nebulon = new Dynamic_mesh_object;
	position = XMVectorSet( 400.0, 0.0, -6000.0, 0.0 );
	nebulon->set_position( position );

	nebulon->set_model( models_manager->get_model( NEBULON ) );
	nebulon->set_scale( 1 );	//Nebulon ma Ÿle ustawiony pivot, wiêc jak siê przeskaluje to odleci gdzieœ w kosmos i go nie widaæ.

	object_list.push_back( nebulon );
	display_engine->add_dynamic_mesh_object( nebulon );

	//dodawanie TIE Fightera
	Dynamic_mesh_object* TIE = new Dynamic_mesh_object;
	position = XMVectorSet( -400.0, 0.0, 800.0, 0.0 );
	TIE->set_position( position );

	TIE->set_model( models_manager->get_model( TIE_FIGHTER ) );
	TIE->set_scale( 10 );

	object_list.push_back( TIE );
	display_engine->add_dynamic_mesh_object( TIE );


	//dodawanie TIE Fightera Vadera
	Dynamic_mesh_object* VaderTIE = new Dynamic_mesh_object;
	position = XMVectorSet( -400.0, 0.0, -3000.0, 0.0 );
	VaderTIE->set_position( position );

	VaderTIE->set_model( models_manager->get_model( VADER_TIE ) );
	//VaderTIE->set_scale( 1.0 );

	object_list.push_back( VaderTIE );
	display_engine->add_dynamic_mesh_object( VaderTIE );

	//dodawanie myœliwca
	Dynamic_mesh_object* clone_fighter = new Dynamic_mesh_object;
	position = XMVectorSet(0.0, 0.0, 6000.0, 0.0);
	clone_fighter->set_position( position );

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

	clone_fighter->set_model( models_manager->get_model( CLONE_FIGHTER ) );

	object_list.push_back( clone_fighter );
	display_engine->add_dynamic_mesh_object( clone_fighter );
	movement_engine->add_moveable_object( clone_fighter );


	// Dodawanie gwiezdnego niszczyciela Imperial
	/*Dynamic_mesh_object* Imperial = new Dynamic_mesh_object;
	position = XMVectorSet( -4000.0, 0.0, -3000.0, 0.0 );
	Imperial->set_position( position );

	Imperial->set_model( models_manager->get_model( IMPERIAL_STAR_DESTROYER ) );

	object_list.push_back( Imperial );
	display_engine->add_dynamic_mesh_object( Imperial );
	*/

	/*
	//dodawanie skrzyni
	Dynamic_mesh_object* skrzynia = new Dynamic_mesh_object;
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

	Model3DFromFile* new_model = models_manager->get_model( L"skrzynia" );
	skrzynia->set_model( new_model );
	//skrzynia->set_scale( 0.1 );

	object_list.push_back( skrzynia );
	display_engine->add_dynamic_mesh_object( skrzynia );
	movement_engine->add_moveable_object( skrzynia );

	*/

	//ustawienie aktywnej kamery
	Camera_object* camera = new Camera_object();
	XMVECTOR camera_pos = XMVectorSet( 0.0, 0.0, 0.0, 0.0 );
	camera->set_position( camera_pos );
	//przypisujemy kontroler ( dla kontrolerów trzeba zrobiæ jakiœ mechanizm przechowywania i zwalniania)
	camera_controller_PROTOTYPE* controller = new camera_controller_PROTOTYPE(
		ui_engine->get_standard_abstraction_layer( STANDARD_ABSTRACTION_LAYER::PROTOTYPE_BUTTONS ) );
	camera->set_controller(controller);

	//wstawiamy kamerê do odpowiednich modu³ów
	display_engine->add_camera( camera );
	display_engine->set_current_camera( camera );
	movement_engine->add_moveable_object( camera );
	controllers_engine->add_pre_controlled( camera );

	double albedo[3] = { 0.8, 0.8, 0.8 };
	double turbidity = 5;
	XMVECTOR sun_dir = XMVectorSet( -0.2, 0.6, 0.6, 1.0 );
	HosekSkyDome* sky_dome = new HosekSkyDome(models_manager);
	sky_dome->init_sky_dome( sun_dir, turbidity, albedo, 101, 101, 100, 5.0 );
	display_engine->set_skydome( sky_dome );

	sun_dir = XMVectorNegate( sun_dir );

	DirectX::XMFLOAT4 color( 0.8f, 0.8f, 0.8f, 1.0f );
	DirectX::XMFLOAT4 direction;
	XMStoreFloat4( &direction, sun_dir );

	// Ustawiamy œwiat³o pod indeksem 0
	display_engine->set_directional_light( direction, color, 0 );
	display_engine->set_ambient_light( DirectX::XMFLOAT4( 0.2, 0.2, 0.2, 1.0f ) );
}

#endif