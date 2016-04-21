#include "ActorWrapper.h"
#include "Common/RTTR.h"

#include "Common/EngineObject.h"


namespace EditorPlugin
{


ActorWrapper::ActorWrapper( EngineObject* actor, const ActorInfo* actorInfo )
	:	m_actorPtr( actor )
	,	m_actorInfo( actorInfo )
{
	m_name = gcnew System::String( "" );
}

/**@brief Zwraca nazwê typu pobran¹ przez rttr::type.*/
System::String^		ActorWrapper::GetTypeName()
{
	auto typeInfo = rttr::type::get( *m_actorPtr );
	return gcnew System::String( typeInfo.get_name().c_str() );
}

/**@brief Zwraca WskaŸnik na aktora przekonwertowany na IntPtr.*/
System::IntPtr		ActorWrapper::GetActorPtr()
{
	return System::IntPtr( m_actorPtr );
}

/**@brief */
int ActorWrapper::Type::get()
{
	return rttr::type::get( *m_actorPtr ).get_raw_type().get_id();
}


}	// EditorPlugin