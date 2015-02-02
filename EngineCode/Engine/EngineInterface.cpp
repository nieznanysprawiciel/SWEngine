#include "..\stdafx.h"
#include "Engine.h"


#include "..\memory_leaks.h"


/*Plik zawiera implementacjê interfejsu engine_interface dla klasy Engine*/

#ifdef __TEST

#include "ControllersEngine\camera_controller_PROTOTYPE.h"

void Engine::test()
{
	const wchar_t CLONE_FIGHTER[] = L"tylko_do_testow/clone_fighter_rel.FBX";
	const wchar_t MOON[] = L"tylko_do_testow/moon/moon.FBX";
	const wchar_t NEBULON[] = L"tylko_do_testow/Nebulon/Nebulon.FBX";
	const wchar_t VADER_TIE[] = L"tylko_do_testow/VadersTIE.FBX";
	const wchar_t TIE_FIGHTER[] = L"tylko_do_testow/TIE_Fighter/TIE_Fighter.FBX";

	models_manager->load_model_from_file( CLONE_FIGHTER );
	models_manager->load_model_from_file( MOON );
	models_manager->load_model_from_file( NEBULON );
	models_manager->load_model_from_file( VADER_TIE );
	models_manager->load_model_from_file( TIE_FIGHTER );

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
	nebulon->set_scale( 0.1 );

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
	Dynamic_mesh_object* skrzynia = new Dynamic_mesh_object;
	position = XMVectorSet(0.0, 0.0, 6000.0, 0.0);
	skrzynia->set_position(position);

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
	skrzynia->set_rotation_speed(axis_angle);

	skrzynia->set_model( models_manager->get_model( CLONE_FIGHTER ) );
	//skrzynia->set_scale( 0.1 );

	object_list.push_back( skrzynia );
	display_engine->add_dynamic_mesh_object( skrzynia );
	movement_engine->add_moveable_object( skrzynia );

	//ustawienie aktywnej kamery
	Camera_object* camera = new Camera_object();
	XMVECTOR camera_pos = XMVectorSet( 0.0, 0.0, 0.0, 0.0 );
	camera->set_position( camera_pos );
	//przypisujemy kontroler ( dla kontrolerów trzeba zrobiæ jakiœ mechanizm przechowywania i zwalniania)
	camera_controller_PROTOTYPE* controller = new camera_controller_PROTOTYPE(
		ui_engine->get_standard_abstraction_layer( STANDARD_ABSTRACTION_LAYER::PROTOTYPE ) );
	camera->set_controller(controller);

	//wstawiamy kamerê do odpowiednich modu³ów
	display_engine->add_camera( camera );
	display_engine->set_current_camera( camera );
	movement_engine->add_moveable_object( camera );
	controllers_engine->add_pre_controlled( camera );


	//rzuæmy na to troszkê œwiat³a
	D3DLIGHT9 light;    // create the light struct

	ZeroMemory( &light, sizeof( light ) );    // clear out the light struct for use
	light.Type = D3DLIGHT_DIRECTIONAL;
	//light.Type = D3DLIGHT_POINT;
	light.Diffuse = D3DXCOLOR( 0.8f, 0.8f, 0.8f, 1.0f );    // set the light's color
	light.Direction = D3DXVECTOR3( 1.0f, -0.8f, 1.0f );
	light.Position = D3DXVECTOR3( -500.0f, -500.3f, -500.0f );
	light.Range = 100000;
	light.Attenuation0 = 1.0;
	light.Attenuation1 = 0.0;
	light.Attenuation2 = 0.0;

}

#endif