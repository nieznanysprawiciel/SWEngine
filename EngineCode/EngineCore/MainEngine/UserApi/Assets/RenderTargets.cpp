#include "EngineCore/stdafx.h"
#include "RenderTargets.h"

#include "EngineCore/MainEngine/EngineContext.h"

namespace Api { namespace Assets
{


RenderTargetObject*		RenderTargets::CreateSync	( const std::wstring& name, const RenderTargetDescriptor& renderTargetDescriptor )
{
	return Context->modelsManager->CreateRenderTarget( name, renderTargetDescriptor );
}


}	// Assets
}	// Api