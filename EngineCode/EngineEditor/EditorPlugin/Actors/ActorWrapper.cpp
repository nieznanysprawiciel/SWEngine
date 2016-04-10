#include "ActorWrapper.h"
#include "Common/RTTR.h"

#include "EngineCore/Actors/BasicActors/EngineObject.h"


namespace EditorPlugin
{


ActorWrapper::ActorWrapper( EngineObject* actor, const ActorInfo* actorInfo )
	:	m_actorPtr( actor )
	,	m_actorInfo( actorInfo )
{
	m_name = gcnew System::String( "" );
}

/**@brief Zwraca nazwê typu pobran¹ przez rttr::type.*/
System::String^ ActorWrapper::GetTypeName()
{
	auto typeInfo = rttr::type::get( *m_actorPtr );
	return gcnew System::String( typeInfo.get_name().c_str() );
}


}	// EditorPlugin