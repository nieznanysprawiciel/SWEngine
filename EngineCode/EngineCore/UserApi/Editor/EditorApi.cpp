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
EngineInterface*		EditorApi::CreateEngine			( gui::INativeGUI* gui )
{
	sw::Engine* engine = new sw::Engine( 0, nullptr, gui );

	engine->StartEditorMode();

	if( engine->Init() )
	{
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
void*					EditorApi::GetRenderTargetHandle( uint16 width, uint16 height )
{
	RenderTargetDescriptor descriptor;
	descriptor.AllowShareResource = 1;
	descriptor.TextureWidth = width;
	descriptor.TextureHeight = height;
	descriptor.ColorBuffFormat = ResourceFormat::RESOURCE_FORMAT_B8G8R8A8_UNORM;
	descriptor.TextureType = TextureType::TEXTURE_TYPE_TEXTURE2D;
	descriptor.DepthStencilFormat = DepthStencilFormat::DEPTH_STENCIL_FORMAT_D24_UNORM_S8_UINT;
	descriptor.Usage = ResourceUsage::RESOURCE_USAGE_DEFAULT;

	RenderTargetObject* renderTarget = Context->modelsManager->CreateRenderTarget( DefaultAssets::EDITOR_RENDERTARGET_STRING, descriptor );
	Context->displayEngine->SetMainRenderTarget( renderTarget );

	Context->windowHeight = height;
	Context->windowWidth = width;

	return Context->graphicInitializer->GetRenderTargetHandle( renderTarget );
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
void					EditorApi::UpdateScene			( float& lag, float timeInterval )
{
	static_cast< sw::Engine* >( Engine )->UpdateScene( lag, timeInterval );
}

// ================================ //
//
void					EditorApi::RenderScene			( float lag, float timeInterval )
{
	static_cast< sw::Engine* >( Engine )->RenderScene( lag, timeInterval );
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
void EditorApi::SetSkydomeAndCamera()
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


