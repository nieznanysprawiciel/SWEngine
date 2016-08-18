#pragma warning(disable : 4561)	// DirectXMath converting fastcall to stdcall
#include "ActorClassMetaInfo.h"

// Bardzo wa�ny include. Je�eli ten plik nie zostanie gdzie� zaincludowany, to wszystko si� skompiluje
// ale interfejs nie b�dzie widoczny poza dllk�.
#include "EngineEditor/EditorPlugin/EditorApp/IDragable.h"

namespace EditorPlugin
{


/**@brief Tworzy obiekt metadanych, ale nie wype�nia ich rekurencyjnie.

Aby wype�ni� klas� metadanymi nale�y wywo�ac funkcj� BuildHierarchy.*/
ActorClassMetaInfo::ActorClassMetaInfo			( rttr::type classType )
{
	m_type = classType.get_id();
	m_actorClassName = gcnew System::String( classType.get_name().c_str() );
}

/**@brief Tworzy obiekt i wype�nia go metadanymi.*/
ActorClassMetaInfo::ActorClassMetaInfo			( ActorWrapper^ actor )
{
	rttr::type classType = actor->Ptr()->GetType();

	m_type = classType.get_id();
	m_actorPtr = actor;
	m_actorClassName = gcnew System::String( classType.get_name().c_str() );

	BuildHierarchy( actor, classType );
}

/**@brief Ustawia instancj� aktora. Umo�liwia to pobieranie warto�ci jego parametr�w.*/
void		ActorClassMetaInfo::ResetActor		( ActorWrapper^ objectPtr )
{
	m_actorPtr = objectPtr;
	BuildHierarchy( m_actorPtr, m_actorPtr->Ptr()->GetType() );
}

/**@brief Tworzy metadane od podanej klasie.*/
void		ActorClassMetaInfo::BuildHierarchy	( ActorWrapper^ objectPtr, rttr::type classType )
{
	// Tworzymy obiekt, kradniemy od niego pole Properties i o nim zapominamy, �eby zosta� zwolniony.
	CategoryPropertyWrapper^ actorClassMetaData = gcnew CategoryPropertyWrapper( objectPtr->Ptr(), "" );
	
	actorClassMetaData->BuildHierarchy( objectPtr->Ptr(), classType );
	m_properties = actorClassMetaData->Properties;
}



}	//	EditorPlugin
