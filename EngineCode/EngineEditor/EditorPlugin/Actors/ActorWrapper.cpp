#include "ActorWrapper.h"


namespace EditorPlugin
{


ActorWrapper::ActorWrapper( EngineObject* actor, const ActorInfo* actorInfo )
	:	m_actorPtr( actor )
	,	m_actorInfo( actorInfo )
{
	m_name = gcnew System::String( "" );
}


}	// EditorPlugin