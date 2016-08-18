#pragma warning(disable : 4561)	// DirectXMath converting fastcall to stdcall
#include "ActorClassMetaInfo.h"

// Bardzo wa¿ny include. Je¿eli ten plik nie zostanie gdzieœ zaincludowany, to wszystko siê skompiluje
// ale interfejs nie bêdzie widoczny poza dllk¹.
#include "EngineEditor/EditorPlugin/EditorApp/IDragable.h"

namespace EditorPlugin
{


/**@brief Tworzy obiekt metadanych, ale nie wype³nia ich rekurencyjnie.

Aby wype³niæ klasê metadanymi nale¿y wywo³ac funkcjê BuildHierarchy.*/
ActorClassMetaInfo::ActorClassMetaInfo			( rttr::type classType )
{
	m_type = classType.get_id();
	m_actorClassName = gcnew System::String( classType.get_name().c_str() );
}

/**@brief Tworzy obiekt i wype³nia go metadanymi.*/
ActorClassMetaInfo::ActorClassMetaInfo			( ActorWrapper^ actor )
{
	rttr::type classType = actor->Ptr()->GetType();

	m_type = classType.get_id();
	m_actorPtr = actor;
	m_actorClassName = gcnew System::String( classType.get_name().c_str() );

	BuildHierarchy( actor, classType );
}

/**@brief Ustawia instancjê aktora. Umo¿liwia to pobieranie wartoœci jego parametrów.*/
void		ActorClassMetaInfo::ResetActor		( ActorWrapper^ objectPtr )
{
	m_actorPtr = objectPtr;
	BuildHierarchy( m_actorPtr, m_actorPtr->Ptr()->GetType() );
}

/**@brief Tworzy metadane od podanej klasie.*/
void		ActorClassMetaInfo::BuildHierarchy	( ActorWrapper^ objectPtr, rttr::type classType )
{
	// Tworzymy obiekt, kradniemy od niego pole Properties i o nim zapominamy, ¿eby zosta³ zwolniony.
	CategoryPropertyWrapper^ actorClassMetaData = gcnew CategoryPropertyWrapper( objectPtr->Ptr(), "" );
	
	actorClassMetaData->BuildHierarchy( objectPtr->Ptr(), classType );
	m_properties = actorClassMetaData->Properties;
}



}	//	EditorPlugin
