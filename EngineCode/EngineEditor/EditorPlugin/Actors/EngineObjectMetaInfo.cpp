#pragma warning(disable : 4561)	// DirectXMath converting fastcall to stdcall

#include "EngineObjectMetaInfo.h"



namespace EditorPlugin
{

EngineObjectMetaInfo::EngineObjectMetaInfo( EngineObjectWrapper^ objectPtr )
{
	rttr::type classType = objectPtr->GetPtr()->GetType();

	m_type = classType.get_id();
	m_actorPtr = objectPtr;
	m_actorClassName = gcnew System::String( classType.get_name().c_str() );

	BuildHierarchy( objectPtr, classType );
}

/**@brief */
void		EngineObjectMetaInfo::ResetActor		( EngineObjectWrapper^ objectPtr )
{
	m_actorPtr = objectPtr;
	BuildHierarchy( m_actorPtr, m_actorPtr->GetPtr()->GetType() );
}

/**@brief */
void		EngineObjectMetaInfo::BuildHierarchy	( EngineObjectWrapper^ objectPtr, rttr::type classType )
{
	// Tworzymy obiekt, kradniemy od niego pole Properties i o nim zapominamy, ¿eby zosta³ zwolniony.
	CategoryLessPropertyWrapper^ actorClassMetaData = gcnew CategoryLessPropertyWrapper( objectPtr->GetPtr(), "" );
	
	actorClassMetaData->BuildHierarchy( objectPtr->GetPtr(), classType );
	m_properties = actorClassMetaData->Properties;
}



}	// EditorPlugin
