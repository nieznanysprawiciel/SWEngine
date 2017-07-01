/**
@file EngineObjectWrapper.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/
#include "EngineEditor/PropertyWrapperRTTR/stdafx.h"


#include "EngineObjectWrapper.h"
#include "EngineEditor/PropertyWrapperRTTR/Build/IncludeRTTR.h"

#include "swCommonLib/Common/EngineObject.h"


namespace sw {
namespace EditorPlugin
{


// ================================ //
//
EngineObjectWrapper::EngineObjectWrapper( EngineObject* actor )
	: m_actorPtr( actor )
{}

/**@brief Zwraca nazwê typu pobran¹ przez rttr::type.*/
System::String^		EngineObjectWrapper::GetTypeName()
{
	auto typeName = m_actorPtr->GetTypeName();
	return gcnew System::String( typeName.c_str() );
}

/**@brief Zwraca WskaŸnik na aktora przekonwertowany na IntPtr.*/
System::IntPtr		EngineObjectWrapper::GetActorPtr()
{
	return System::IntPtr( m_actorPtr );
}

/**@brief Zwraca natywny wskaŸnik na obiekt.*/
EngineObject*		EngineObjectWrapper::GetPtr		()
{
	return m_actorPtr;
}

// ================================ //
//
TypeID				EngineObjectWrapper::GetTypeID	()
{
	return m_actorPtr->GetType();
}

// ================================ //
//
rttr::instance		EngineObjectWrapper::GetInstance()
{
	rttr::variant object = *m_actorPtr;
	return object;
}


/**@brief Zwraca liczbowy identyfikator typu aktora.*/
uint32				EngineObjectWrapper::Type::get	()
{
	return rttr::type::get( *m_actorPtr ).get_raw_type().get_id();
}


}	// EditorPlugin
}	// sw
