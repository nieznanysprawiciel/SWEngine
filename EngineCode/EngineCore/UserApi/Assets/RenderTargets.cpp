#include "EngineCore/stdafx.h"
#include "RenderTargets.h"

#include "EngineCore/MainEngine/EngineContext.h"

namespace Api { namespace Assets
{


RenderTargetObject*		RenderTargets::CreateSync	( const std::wstring& name, const RenderTargetDescriptor& renderTargetDescriptor )
{
	return Context->modelsManager->CreateRenderTarget( name, renderTargetDescriptor );
}

/**@copydoc ModelsManager::ListRenderTargets.*/
std::vector< ResourcePtr< RenderTargetObject > > RenderTargets::List()
{
	return Context->modelsManager->ListRenderTargets();
}


}	// Assets
}	// Api