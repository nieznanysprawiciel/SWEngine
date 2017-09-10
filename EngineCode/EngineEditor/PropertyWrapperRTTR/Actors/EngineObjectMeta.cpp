/**
@file EngineObjectMeta.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#pragma warning(disable : 4561)	// DirectXMath converting fastcall to stdcall
#include "EngineEditor/PropertyWrapperRTTR/stdafx.h"

#include "EngineObjectMeta.h"


#include "swCommonLib/Common/EngineObject.h"


// ================================ //
// Important include. Without it averything would compile but type wouldn't be visible
// outside of assembly.
#include "EngineEditor/EditorPlugin/EditorApp/IDragable.h"


namespace sw {
namespace EditorPlugin
{

// ================================ //
//
EngineObjectMeta::EngineObjectMeta( EngineObjectWrapper^ objectPtr )
	:	m_actorPtr( objectPtr )
	,	m_properties( nullptr )
{
	m_actorPtr = objectPtr;
	m_actorClassName = gcnew System::String( m_actorPtr->GetTypeName() );

	BuildHierarchy( objectPtr, m_actorPtr->GetTypeID() );
}

// ================================ //
//
EngineObjectMeta::EngineObjectMeta( const TypeID& classType )
	:	m_actorPtr( nullptr )
	,	m_properties( nullptr )
{
	m_actorClassName = gcnew System::String( classType.get_name().to_string().c_str() );
}

/**@brief */
void						EngineObjectMeta::ResetActor		( EngineObjectWrapper^ objectPtr )
{
	if( objectPtr )
	{
		m_actorPtr = objectPtr;
		BuildHierarchy( m_actorPtr, m_actorPtr->GetTypeID() );
	}
}

/**@brief */
void						EngineObjectMeta::BuildHierarchy	( EngineObjectWrapper^ objectPtr, rttr::type classType )
{
	rttr::instance instance = objectPtr->GetPtr();

	HierarchicalPropertyWrapper^ actorClassMetaData = gcnew HierarchicalPropertyWrapper( instance, "" );

	BuildContext context;

	actorClassMetaData->BuildHierarchy( context );
	m_properties = actorClassMetaData;
}

/**@brief */
List< System::String^ >^	EngineObjectMeta::BaseClasses::get()
{
	if( m_actorPtr )
	{
		List< System::String^ >^ classesList = gcnew List< System::String^ >();

		auto baseClassesVec = m_actorPtr->GetTypeID().get_base_classes();
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
uint32						EngineObjectMeta::TypeSize::get()
{
	if( m_actorPtr )
	{
		return (uint32)m_actorPtr->GetTypeID().get_sizeof();
	}
	return 0;
}

/**@brief */
uint32						EngineObjectMeta::MemorySize::get()
{
	if( m_actorPtr )
	{
		return (uint32)m_actorPtr->GetPtr()->MemorySize();
	}
	return 0;
}

}	// EditorPlugin
}	// sw

