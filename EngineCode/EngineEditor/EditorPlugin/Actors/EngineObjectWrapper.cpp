#include "EngineObjectWrapper.h"
#include "swCommonLib/Common/RTTR.h"

#include "swCommonLib/Common/EngineObject.h"


namespace sw {
namespace EditorPlugin
{



EngineObjectWrapper::EngineObjectWrapper( EngineObject* actor )
	: m_actorPtr( actor )
{}

/**@brief Zwraca nazwÍ typu pobranπ przez rttr::type.*/
System::String^		EngineObjectWrapper::GetTypeName()
{
	auto typeInfo = rttr::type::get( *m_actorPtr );
	return gcnew System::String( typeInfo.get_name().c_str() );
}

/**@brief Zwraca Wskaünik na aktora przekonwertowany na IntPtr.*/
System::IntPtr		EngineObjectWrapper::GetActorPtr()
{
	return System::IntPtr( m_actorPtr );
}

/**@brief Zwraca natywny wskaünik na obiekt.*/
EngineObject*		EngineObjectWrapper::GetPtr		()
{
	return m_actorPtr;
}


/**@brief Zwraca liczbowy identyfikator typu aktora.*/
int					EngineObjectWrapper::Type::get()
{
	return rttr::type::get( *m_actorPtr ).get_raw_type().get_id();
}


}	// EditorPlugin
}	// sw
