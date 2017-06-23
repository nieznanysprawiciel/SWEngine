#pragma warning(disable : 4561)	// DirectXMath converting fastcall to stdcall

#include "EngineObjectMetaInfo.h"



namespace sw {
namespace EditorPlugin
{

// ================================ //
//
EngineObjectMetaInfo::EngineObjectMetaInfo( EngineObjectWrapper^ objectPtr )
{
	rttr::type classType = objectPtr->GetPtr()->GetType();

	m_type = classType.get_id();
	m_actorPtr = objectPtr;
	m_actorClassName = gcnew System::String( classType.get_name().to_string().c_str() );

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

/**@brief */
List< System::String^ >^	EngineObjectMetaInfo::BaseClasses::get()
{
	if( m_actorPtr )
	{
		List< System::String^ >^ classesList = gcnew List< System::String^ >();

		auto baseClassesVec = m_actorPtr->GetPtr()->GetType().get_base_classes();
		for each( auto baseClass in baseClassesVec )
		{
			System::String^ className = gcnew System::String( baseClass.get_name().to_string().c_str() );
			classesList->Add( className );
		}

		return classesList;
	}
	else
		return nullptr;
}

/**@brief */
uint32						EngineObjectMetaInfo::TypeSize::get()
{
	if( m_actorPtr )
	{
		return (uint32)m_actorPtr->GetPtr()->GetType().get_sizeof();
	}
	return 0;
}

/**@brief */
uint32						EngineObjectMetaInfo::MemorySize::get()
{
	if( m_actorPtr )
	{
		return (uint32)m_actorPtr->GetPtr()->MemorySize();
	}
	return 0;
}

}	// EditorPlugin
}	// sw

