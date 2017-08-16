/**
@file SceneHelpersNoClr.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "EngineEditor/EditorPlugin/stdafx.h"


#pragma unmanaged

#include "SceneHelpers.h"

#include "swCommonLib/Serialization/Serializer.h"
#include "swCommonLib/Serialization/PropertySerialization/EngineSerializationContext.h"



namespace sw
{

// ================================ //
//
ISerializer*		SceneHelpers::CreateSerializerWithContext()
{
	auto context = std::make_unique< EngineSerializationContext >();
	context->SaveWholeMap = true;

	ISerializer* ser = new ISerializer( std::move( context ) );
	return ser;
}

}	// sw
