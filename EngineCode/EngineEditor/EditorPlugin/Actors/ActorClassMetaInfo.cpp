#include "ActorClassMetaInfo.h"



namespace EditorPlugin
{


/**@brief */
ActorClassMetaInfo::ActorClassMetaInfo			( rttr::type classType )
{
	m_type = classType.get_id();
	m_actorClassName = gcnew System::String( classType.get_name().c_str() );

	BuildHierarchy( classType );
}

/**@brief Ustawia instancj� aktora. Umo�liwia to pobieranie warto�ci jego parametr�w.*/
void		ActorClassMetaInfo::ResetActor		( ActorWrapper^ objectPtr )
{
	m_actorPtr = objectPtr;

	for each( PropertyWrapper^ prop in m_properties )
		prop->ResetActor( m_actorPtr->GetActorPtr() );
}

/**@brief Tworzy metadane od podanej klasie.*/
void		ActorClassMetaInfo::BuildHierarchy	( rttr::type classType )
{
	// Tworzymy obiekt, kradniemy od niego pole Properties i o nim zapominamy, �eby zosta� zwolniony.
	CategoryPropertyWrapper^ actorClassMetaData = gcnew CategoryPropertyWrapper( "" );;
	
	actorClassMetaData->BuildHierarchy( classType );
	m_properties = actorClassMetaData->Properties;
}



}	//	EditorPlugin
