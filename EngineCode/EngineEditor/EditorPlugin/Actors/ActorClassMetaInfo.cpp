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

/**@brief Ustawia instancjê aktora. Umo¿liwia to pobieranie wartoœci jego parametrów.*/
void		ActorClassMetaInfo::ResetActor		( System::IntPtr objectPtr )
{
	m_actorPtr = static_cast< EngineObject* >( objectPtr.ToPointer() );

	for each( PropertyWrapper^ prop in m_properties )
		prop->ResetActor( objectPtr );
}

/**@brief Tworzy metadane od podanej klasie.*/
void		ActorClassMetaInfo::BuildHierarchy	( rttr::type classType )
{
	// Tworzymy obiekt, kradniemy od niego pole Properties i o nim zapominamy, ¿eby zosta³ zwolniony.
	CategoryPropertyWrapper^ actorClassMetaData = gcnew CategoryPropertyWrapper( "" );;
	
	actorClassMetaData->BuildHierarchy( classType );
	m_properties = actorClassMetaData->Properties;
}



}	//	EditorPlugin
