/**
@file EditorApi.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "EngineCore/stdafx.h"
#include "EditorApi.h"

#include "EngineCore/MainEngine/EngineContext.h"
#include "EngineCore/MainEngine/Engine.h"


namespace sw {
namespace Api
{


// ================================ //
//
EngineInterface*		EditorApi::CreateEngine			( gui::INativeGUI* gui, input::IInput* input, uint16 width, uint16 height )
{
	sw::Engine* engine = new sw::Engine( gui );

	engine->StartEditorMode();

	if( engine->Init() )
	{
		if( engine->InitEditorWindow( input, width, height ) )
			return engine;
	}

	return nullptr;
}

// ================================ //
//
void					EditorApi::ReleaseEngine		()
{
	sw::Engine* engine = static_cast< sw::Engine* >( Engine );

	// We must call close logic manually, because editor uses MainLoopCore to make main loop step.
	// MainLoopCore doesn't call CloseLogic.
	engine->CloseLogic();
	delete engine;

	Engine = nullptr;
	Context = nullptr;
}

// ================================ //
//
void*					EditorApi::GetMainRenderTargetHandle()
{
	RenderTargetObject* renderTarget = Context->modelsManager->GetRenderTarget( DefaultAssets::EDITOR_RENDERTARGET_STRING );

	if( renderTarget )
		return Context->graphicInitializer->GetRenderTargetHandle( renderTarget );
	return nullptr;
}

// ================================ //
//
CameraData&				EditorApi::GetMainCamera		()
{
	return Context->controllersEngine->GetGlobalState()->Camera;
}

// ================================ //
//
EngineInterface*		EditorApi::GetEngine			()
{
	return Engine;
}

// ================================ //
//
void					EditorApi::EnableInput			( bool val )
{
	Context->ui_engine->EnableInput( val );
}

// ================================ //
//
input::IInput*			EditorApi::ChangeInputModule	( input::IInput* newModule )
{
	return Context->ui_engine->ChangeInputModule( newModule );
}

//====================================================================================//
//			Main loop and object pipeline	
//====================================================================================//


// ================================ //
//
void					EditorApi::MainLoopStep			()
{
	static_cast< sw::Engine* >( Engine )->MainLoopCore();
}

// ================================ //
//
void					EditorApi::Flush				()
{
	Context->displayEngine->FlushCommands();
}


//====================================================================================//
//			Test functions hopefully temporary	
//====================================================================================//

// ================================ //
//
void					EditorApi::Test					()
{
	static_cast< sw::Engine* >( Engine )->test();
}

// ================================ //
//
void					EditorApi::SetSkydomeAndCamera	()
{
	static_cast< sw::Engine* >( Engine )->SetSkydomeAndCamera();
}

// ================================ //
//
void					EditorApi::CreateDefaultScene	()
{
	static_cast< sw::Engine* >( Engine )->CreateDefaultScene();
}


}	// Api
}	// sw


