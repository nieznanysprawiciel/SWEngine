#pragma once
/**
@file EngineObjectWrapper.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

class EngineObject;

#include "EngineEditor/PropertyWrapperRTTR/Build/IncludeRTTR.h"
#include "swCommonLib/Common/TypesDefinitions.h"



namespace sw {
namespace EditorPlugin
{


/**@brief Class wrappes native objects derived from EngineObject.*/
public ref class EngineObjectWrapper
{
private:
protected:
	EngineObject*		m_actorPtr;		///< WskaŸnik na aktora.

public:
	EngineObjectWrapper( EngineObject* actor );

	System::String^		GetTypeName	();
	System::IntPtr		GetActorPtr	();
	EngineObject*		GetPtr		();

	TypeID				GetTypeID	();
	rttr::instance		GetInstance	();

	property uint32				Type
	{
		uint32				get ();
	}

	property System::String^	TypeName
	{
		System::String^		get () { return GetTypeName(); }
	}
};



}	// EditorPlugin
}	// sw
