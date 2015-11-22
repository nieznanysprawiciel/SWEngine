/**@file EngineInterface.cpp
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Plik zawiera implementacj� interfejsu EngineInterface dla klasy Engine
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

#include "EngineCore/ModelsManager/ResourceHelpers.h"

#include <mutex>

#include "Common/memory_leaks.h"

using namespace DirectX;

Engine* EngineInterface::InterfaceGroup::m_engine = nullptr;


EngineInterface::EngineInterface()
{
	EngineInterface::InterfaceGroup::m_engine = static_cast<Engine*>( this );		// EngineInterface jest tak na prawd� klas� Engine, wi�c takie rzutowanie jest legalne.
}

#ifdef __TEST

#include "EngineCore/ControllersEngine/camera_controller_PROTOTYPE.h"
#include "EngineCore/Features/HosekSkylightModel/HosekSkyDome.h"

#include <math.h>
#include "EngineInterface.h"

void Engine::test()
{
//	const wchar_t CLONE_FIGHTER[] = L"tylko_do_testow/ARC.FBX";
//	const wchar_t MOON[] = L"tylko_do_testow/moon/moon.FBX";
//	const wchar_t NEBULON[] = L"tylko_do_testow/Nebulon/Nebulon.FBX";
//	const wchar_t VADER_TIE[] = L"tylko_do_testow/VadersTIE.FBX";
//	const wchar_t TIE_FIGHTER[] = L"tylko_do_testow/TIE_Fighter/TIE_Fighter.FBX";
//	const wchar_t IMPERIAL_STAR_DESTROYER[] = L"tylko_do_testow/ImperialStarDestroyer.FBX";
//
//	Context.modelsManager->LoadModelFromFile( CLONE_FIGHTER );
//	Context.modelsManager->LoadModelFromFile( MOON );
//	Context.modelsManager->LoadModelFromFile( NEBULON );
//	Context.modelsManager->LoadModelFromFile( VADER_TIE );
//	Context.modelsManager->LoadModelFromFile( TIE_FIGHTER );
//	//modelsManager->LoadModelFromFile( IMPERIAL_STAR_DESTROYER );
//	
//	//modelsManager->test();			// Tu sie odbywa wczytywanie
//
//	//dodawanie ksi�yca
//	DynamicMeshObject* moon = new DynamicMeshObject;
//	XMVECTOR position = XMVectorSet( 4000.0, 0.0, 8000.0, 0.0 );
//	moon->Teleport( position );
//
//	moon->SetModel( Context.modelsManager->GetModel( MOON ) );
//	moon->set_scale( 30.0 );
//
//	object_list.push_back( moon );
//	Context.displayEngine->AddDynamicMeshObject( moon );
//
//	//dodawanie Nebulona
//	DynamicMeshObject* nebulon = new DynamicMeshObject;
//	position = XMVectorSet( 400.0, 0.0, -6000.0, 0.0 );
//	nebulon->Teleport( position );
//
//	nebulon->SetModel( Context.modelsManager->GetModel( NEBULON ) );
//	nebulon->set_scale( 1 );	//Nebulon ma �le ustawiony pivot, wi�c jak si� przeskaluje to odleci gdzie� w kosmos i go nie wida�.
//
//	object_list.push_back( nebulon );
//	Context.displayEngine->AddDynamicMeshObject( nebulon );
//
//	//dodawanie TIE Fightera
//	DynamicMeshObject* TIE = new DynamicMeshObject;
//	position = XMVectorSet( -400.0, 0.0, 800.0, 0.0 );
//	TIE->Teleport( position );
//
//	TIE->SetModel( Context.modelsManager->GetModel( TIE_FIGHTER ) );
//	TIE->set_scale( 10 );
//
//	object_list.push_back( TIE );
//	Context.displayEngine->AddDynamicMeshObject( TIE );
//
//
//	//dodawanie TIE Fightera Vadera
//	DynamicMeshObject* VaderTIE = new DynamicMeshObject;
//	position = XMVectorSet( -400.0, 0.0, -3000.0, 0.0 );
//	VaderTIE->Teleport( position );
//
//	VaderTIE->SetModel( Context.modelsManager->GetModel( VADER_TIE ) );
//	//VaderTIE->set_scale( 1.0 );
//
//	object_list.push_back( VaderTIE );
//	Context.displayEngine->AddDynamicMeshObject( VaderTIE );
//
//	//dodawanie my�liwca
//	DynamicMeshObject* clone_fighter = new DynamicMeshObject;
//	position = XMVectorSet(0.0, 0.0, 6000.0, 0.0);
//	clone_fighter->Teleport( position );
//
//#ifdef _QUATERNION_SPEED
//	XMVECTOR rot_vector = { 1.0f, 0.0f, 0.0f, 0.0f };
//	XMVECTOR axis_angle = XMQuaternionRotationAxis(rot_vector, XMConvertToRadians(30));
//#else
//	XMFLOAT4 axis_angle;
//	axis_angle.x = 1.0;
//	axis_angle.y = 0.0;
//	axis_angle.z = 0.0;
//	axis_angle.w = XMConvertToRadians(30);
//#endif
//	clone_fighter->SetRotationSpeed( axis_angle );
//
//	clone_fighter->SetModel( Context.modelsManager->GetModel( CLONE_FIGHTER ) );
//
//	object_list.push_back( clone_fighter );
//	Context.displayEngine->AddDynamicMeshObject( clone_fighter );
//	Context.movementEngine->add_moveable_object( clone_fighter );





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
	camera->Teleport( camera_pos );
	//przypisujemy kontroler ( dla kontroler�w trzeba zrobi� jaki� mechanizm przechowywania i zwalniania)
	camera_controller_PROTOTYPE* controller = new camera_controller_PROTOTYPE(
	Context.ui_engine->GetStandardAbstractionLayer( STANDARD_ABSTRACTION_LAYER::PROTOTYPE_BUTTONS ) );
	camera->SetController(controller);

	//wstawiamy kamer� do odpowiednich modu��w
	Context.displayEngine->AddCamera( camera );
	Context.displayEngine->SetCurrentCamera( camera );
	Context.movementEngine->add_moveable_object( camera );
	Context.controllersEngine->add_pre_controlled( camera );

	double albedo[3] = { 0.8, 0.8, 0.8 };
	double turbidity = 4;
	XMVECTOR sun_dir = XMVectorSet( -0.2f, 0.6f, 0.6f, 1.0f );
	HosekSkyDome* skyDome = new HosekSkyDome( Context.modelsManager );
	skyDome->init_sky_dome( sun_dir, turbidity, albedo, 101, 101, 100, 5.0 );
	Context.displayEngine->SetSkydome( skyDome );

	sun_dir = XMVectorNegate( sun_dir );

	DirectX::XMFLOAT4 color( 0.8f, 0.8f, 0.8f, 1.0f );
	DirectX::XMFLOAT4 direction;
	XMStoreFloat4( &direction, sun_dir );

	// Ustawiamy �wiat�o pod indeksem 0
	Context.displayEngine->SetDirectionalLight( direction, color, 0 );
	Context.displayEngine->SetAmbientLight( DirectX::XMFLOAT4( 0.2f, 0.2f, 0.2f, 1.0f ) );



}

#endif

//=====================================================================================================================//
//								EngineInterface::Assets::Models
//=====================================================================================================================//


/**@brief Pobiera model o podanej nazwie.

Model jest pobierany tylko, je�eli zosta� wcze�niej wczytany.
@param[in] name Nazwa pliku z modelem.
@return Zwraca obiekt zawieraj�cy model lub nullptr, je�eli model nie zosta� wcze�niej wczytany.*/
Model3DFromFile* EngineInterface::Assets::Models::GetSync( const std::wstring& name )
{
	return m_engine->Context.modelsManager->GetModel( name );
}


/**@brief Wczytuje model z podanego pliku.

Model jest �adowany synchronicznie, dzia�anie silnika zawiesza si�, dop�ki wczytywanie nie zako�czy si�.
@param[in] name Nazwa pliku.
@return Zwraca model lub nullptr, je�eli wczytywanie nie powiod�o si�.*/
Model3DFromFile* EngineInterface::Assets::Models::LoadSync( const std::wstring& name )
{
	auto result = m_engine->Context.modelsManager->LoadModelFromFile( name );
	if( result != MODELS_MANAGER_OK )
		return nullptr;

	return m_engine->Context.modelsManager->GetModel( name );
}


//=====================================================================================================================//
//								EngineInterface::Assets::Shaders
//=====================================================================================================================//

/**@brief Pobiera model o podanej nazwie.

Model jest pobierany tylko, je�eli zosta� wcze�niej wczytany.
@param[in] name Nazwa pliku z modelem.
@return Zwraca obiekt zawieraj�cy model lub nullptr, je�eli model nie zosta� wcze�niej wczytany.*/
VertexShaderObject* EngineInterface::Assets::Shaders::GetVertexShaderSync( const std::wstring& name )
{
	return m_engine->Context.modelsManager->GetVertexShader( name );
}

/**@brief Wczytuje shader z podanego pliku.

Model jest �adowany synchronicznie, dzia�anie silnika zawiesza si�, dop�ki wczytywanie nie zako�czy si�.
@param[in] name Nazwa pliku.
@return Zwraca model lub nullptr, je�eli wczytywanie nie powiod�o si�.*/
VertexShaderObject* EngineInterface::Assets::Shaders::LoadVertexShaderSync( const std::wstring& name )
{
	return m_engine->Context.modelsManager->AddVertexShader( name, DEFAULT_VERTEX_SHADER_ENTRY );
}

/**@brief Wczytuje shader z podanego pliku i tworzy layout wierzcho�ka.

Model jest �adowany synchronicznie, dzia�anie silnika zawiesza si�, dop�ki wczytywanie nie zako�czy si�.
@param[in] name Nazwa pliku.
@param[in] layout Wska�nik do kt�rego zostanie zwr�cony layout.
@param[in] layoutDesc Deskryptor layoutu do stworzenia.
@return Zwraca model lub nullptr, je�eli wczytywanie nie powiod�o si�.*/
VertexShaderObject* EngineInterface::Assets::Shaders::LoadVertexShaderSync( const std::wstring& name, ShaderInputLayoutObject** layout, InputLayoutDescriptor* layoutDesc )
{
	return m_engine->Context.modelsManager->AddVertexShader( name, DEFAULT_VERTEX_SHADER_ENTRY, layout, layoutDesc );
}

/**@brief Pobiera model o podanej nazwie.

Model jest pobierany tylko, je�eli zosta� wcze�niej wczytany.
@param[in] name Nazwa pliku z modelem.
@return Zwraca obiekt zawieraj�cy model lub nullptr, je�eli model nie zosta� wcze�niej wczytany.*/
PixelShaderObject* EngineInterface::Assets::Shaders::GetPixelShaderSync( const std::wstring& name )
{
	return m_engine->Context.modelsManager->GetPixelShader( name );
}

/**@brief Wczytuje shader z podanego pliku.

Model jest �adowany synchronicznie, dzia�anie silnika zawiesza si�, dop�ki wczytywanie nie zako�czy si�.
@param[in] name Nazwa pliku.
@return Zwraca model lub nullptr, je�eli wczytywanie nie powiod�o si�.*/
PixelShaderObject* EngineInterface::Assets::Shaders::LoadPixelShaderSync( const std::wstring& name )
{
	return m_engine->Context.modelsManager->AddPixelShader( name, DEFAULT_PIXEL_SHADER_ENTRY );
}


//=====================================================================================================================//
//								EngineInterface::Assets::Shaders
//=====================================================================================================================//

/**@brief Pobiera tekstur� o podanej nazwie.

Tekxtura jest pobierana tylko, je�eli zosta�a wcze�niej wczytana.
@param[in] name Nazwa pliku z tekstur� lub po prostu nazwa tekstury, je�eli zosta�a wygenerowana.
@return Zwraca obiekt zawieraj�cy tekstur� lub nullptr, je�eli tekstura nie zosta�a wcze�niej wczytana.*/
TextureObject* EngineInterface::Assets::Textures::GetTexture( const std::wstring& name )
{
	return m_engine->Context.modelsManager->GetTexture( name );
}

//=====================================================================================================================//
//								EngineInterface::Assets::Buffers
//=====================================================================================================================//

/**@brief Tworzy bufor wierzcho�k�w.

@param[in] name Nazwa bufora.
@param[in] data MemoryChunk zawieraj�cy obszar pami�ci do przekopiowania do bufora.
@param[in] vertCount Liczba wierzcho�k�w w buforze. Rozmiar pojedynczego wierzcho�ka zostanie wydedukowany z pozosta�ych danyhch.
@return Zwraca bufor wierzcho�k�w lub nullptr.*/
BufferObject* EngineInterface::Assets::Buffers::CreateVertexBufferSync( const std::wstring& name, MemoryChunk& data, unsigned int vertCount )
{
	return m_engine->Context.modelsManager->AddVertexBuffer( name, data.GetMemory<void>(), data.GetMemorySize() / vertCount, vertCount );
}

//=====================================================================================================================//
//								EngineInterface::Assets::RenderTargets
//=====================================================================================================================//

RenderTargetObject* EngineInterface::Assets::RenderTargets::CreateSync( const std::wstring& name, const RenderTargetDescriptor& renderTargetDescriptor )
{
	return m_engine->Context.modelsManager->CreateRenderTarget( name, renderTargetDescriptor );
}


//=====================================================================================================================//
//								EngineInterface::Assets::Materials
//=====================================================================================================================//


MaterialObject* EngineInterface::Assets::Materials::GetSync( const std::wstring& name )
{
	return m_engine->Context.modelsManager->GetMaterial( name );
}

//=====================================================================================================================//
//								EngineInterface::Actors
//=====================================================================================================================//



std::vector<DynamicMeshObject*> EngineInterface::Actors::GetSceneObjects()
{ return m_engine->Context.displayEngine->GetSceneObjects(); }

void EngineInterface::Actors::AddDynamicMesh( DynamicMeshObject* object )
{
	m_engine->Context.displayEngine->AddDynamicMeshObject( object );
	m_engine->Context.object_list.push_back( object );
}

void EngineInterface::Actors::CleanScene()
{	m_engine->Context.displayEngine->DeleteAllMeshes();	}

//=====================================================================================================================//
//								EngineInterface::Actors::Assets
//=====================================================================================================================//

template<typename Resource>
void ChangeResource( DynamicMeshObject* mesh, Resource* newResource, uint16 beginPart, uint16 endPart, uint8 index )
{
	if( !newResource )
		return;

	auto& parts = mesh->GetModelParts();
	int size = parts.size();
	int max = endPart > size ? size : endPart;
	for( int i = beginPart; i < max; ++i )
	{
		auto& resource = Get<Resource>( parts[ i ], index );
		if( resource )
			resource->DeleteObjectReference();
		resource = newResource;
		newResource->AddObjectReference();
	}
}

/**@brief Zmienia tekstur� w podanym obiekcie.

Zmieniany jest zestaw tekstur mi�dzy beginPart a endPart (wy��cznie). Pod uwag� s� brane tekstury o indeksie texIndex.
Mo�na wstawi� sta�� @ref TextureType.

@param[in] mesh Obiekt, do kt�rego zostanie wstawiona nowa tekstura.
@param[in] newTex Tekstura do ustawienia.
@param[in] beginPart Indeks pierwszego podmesha, w kt�rym zmieniamy zas�b.
@param[in] endPart Indeks za ostatnim elementem, kt�ry zostanie zmieniony.
@param[in] texIndex Indeks w tablicy tekstur [0,ENGINE_MAX_TEXTURES]. Mo�e to by� jedna ze sta�ych TextureType.*/
void EngineInterface::Actors::Assets::ChangeTextures( DynamicMeshObject* mesh, TextureObject* newTex, uint16 beginPart, uint16 endPart, uint8 texIndex )
{
	ChangeResource<TextureObject>( mesh, newTex, beginPart, endPart, texIndex );
}

/**@brief Zmienia pixel shader w podanym obiekcie.

Zmieniany jest zestaw shader�w mi�dzy beginPart a endPart (wy��cznie).

@param[in] mesh Obiekt, do kt�rego zostanie wstawiona nowy shader.
@param[in] newShader Shader do ustawienia.
@param[in] beginPart Indeks pierwszego podmesha, w kt�rym zmieniamy zas�b.
@param[in] endPart Indeks za ostatnim elementem, kt�ry zostanie zmieniony.*/
void EngineInterface::Actors::Assets::ChangePixelShaders( DynamicMeshObject* mesh, PixelShaderObject* newShader, uint16 beginPart, uint16 endPart )
{
	ChangeResource<PixelShaderObject>( mesh, newShader, beginPart, endPart, 0 );
}

/**@brief Zmienia vertex shader w podanym obiekcie.

Zmieniany jest zestaw shader�w mi�dzy beginPart a endPart (wy��cznie).

@param[in] mesh Obiekt, do kt�rego zostanie wstawiona nowy shader.
@param[in] newShader Shader do ustawienia.
@param[in] beginPart Indeks pierwszego podmesha, w kt�rym zmieniamy zas�b.
@param[in] endPart Indeks za ostatnim elementem, kt�ry zostanie zmieniony.*/
void EngineInterface::Actors::Assets::ChangeVertexShaders( DynamicMeshObject* mesh, VertexShaderObject* newShader, uint16 beginPart, uint16 endPart )
{
	ChangeResource<VertexShaderObject>( mesh, newShader, beginPart, endPart, 0 );
}


//=====================================================================================================================//
//								EngineInterface::Input
//=====================================================================================================================//


/**@brief Pobiera jedn� ze standardowych (wbudowanych) warstw abstrakcji.

@param[in] layer Enum oznaczaj�cy pobieran� warstw�.
@return Zwraca wska�nik na pobieran� warstw�.
*/
InputAbstractionLayer*		EngineInterface::Input::GetStandardAbstractionLayer		( STANDARD_ABSTRACTION_LAYER layer )
{	return m_engine->Context.ui_engine->GetStandardAbstractionLayer( layer );	}

/**@brief Ustawia podan� warstw� abstrakcji jako aktualn�.

@param[in] layer Wska�nik na warstw� abstrakcji.
*/
void						EngineInterface::Input::SetAbstractionLayer				( InputAbstractionLayer* layer )
{	m_engine->Context.ui_engine->ChangeAbstractionLayer( layer );	}

/**@brief Ustawia jedn� ze standardowych (wbudowanych) warstw abstrakcji.

@param[in] layer Enum oznaczaj�cy wybran� warstw�.
*/
void						EngineInterface::Input::SetStandardAbstractionLayer		( STANDARD_ABSTRACTION_LAYER layer )
{	m_engine->Context.ui_engine->SetStandardAbstractionLayer( layer );	}


//=====================================================================================================================//
//								EngineInterface::Rendering
//=====================================================================================================================//

/**@brief Dorzuca do kolejki renderingu nowy przebieg, kt�ry zostanie wyrenderowany tylko raz.

Funkcja jest przydatna, je�eli chcemy wyrenderowa� tekstur�, kt�ra b�dzie niezmienna przez ca�� gr�.

Po wyrenderowaniu wysy�any jest event RenderOnceEndedEvent. Nie ma gwarancji, �e renderowanie odb�dzie
si� w tej samej klatce, w kt�rej zosta�o wys�ane ��danie. Je�eli p�tla renderuj�ca b�dzie za bardzo obci��ona,
to z kolejki b�dzie wybieranych tylko kilka ��da�, a pozosta�e zostan� od�o�one do kolejnych klatek.

@attention W obecnej wersji event musi zostac odebrany, �eby zwolni� obiekt RenderPass i wszystkie zasoby,
kt�re zostan� w nim umieszczone. W przysz�o�ci trzeba zrobi� jaki� m�drzejszy mechanizm.

@param[in] pass Dane przebiegu uzywane przy renderowaniu.*/
void EngineInterface::Rendering::RenderOnce( RenderPass* pass )
{
	m_engine->Context.displayEngine->RenderOnce( pass );
}

