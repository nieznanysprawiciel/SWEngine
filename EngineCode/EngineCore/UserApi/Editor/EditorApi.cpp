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
EngineInterface*		EditorApi::CreateEngine			()
{
	return nullptr;
}

// ================================ //
//
void					EditorApi::ReleaseEngine		()
{
	delete static_cast< sw::Engine* >( Engine );

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


}	// Api
}	// sw


