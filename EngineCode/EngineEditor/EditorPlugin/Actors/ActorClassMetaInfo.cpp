#pragma warning(disable : 4561)	// DirectXMath converting fastcall to stdcall

#include "EngineEditor/EditorPlugin/stdafx.h"
#include "ActorClassMetaInfo.h"



namespace sw {
namespace EditorPlugin
{


/**@brief Creates actor meta info class but doesn't build properties hierarchy.

You can use ResetActor to build actor info with all properties.*/
ActorClassMetaInfo::ActorClassMetaInfo			( TypeID classType )
	:	EngineObjectMeta( classType )
{
	m_type = classType.get_id();
	m_actorClassName = gcnew System::String( classType.get_name().to_string().c_str() );
}

/**@brief Creates object and fills it with meta data about properties.*/
ActorClassMetaInfo::ActorClassMetaInfo			( ActorWrapper^ actor )
	:	EngineObjectMeta( actor )
{
	rttr::type classType = actor->GetTypeID();
	m_type = classType.get_id();
}




}	//	EditorPlugin
}	// sw
