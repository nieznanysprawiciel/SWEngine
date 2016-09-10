#include "GenericActor.h"


RTTR_REGISTRATION
{

	rttr::registration::class_< GenericActor >( "GenericActor" )
		.property( "TypeIdx", &GenericActor::m_typeIdx )
		.property( "ActorIdx", &GenericActor::m_actorIdx );

	rttr::registration::class_< GenericObject >( "GenericObject" )
		.property( "TypeIdx", &GenericObject::m_typeIdx );
}