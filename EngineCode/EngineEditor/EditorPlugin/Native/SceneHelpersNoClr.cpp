#pragma unmanaged


#include "SceneHelpers.h"

#include "Common/Serialization/Serializer.h"
#include "Common/Serialization/SW/EngineSerializationContext.h"



ISerializer* SceneHelpers::CreateSerializerWithContext()
{
	auto context = std::make_unique< EngineSerializationContext >();
	context->SaveWholeMap = true;

	ISerializer* ser = new ISerializer( std::move( context ) );
	return ser;
}

