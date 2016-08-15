#pragma warning(disable : 4561)	// DirectXMath converting fastcall to stdcall
#include "ActorClassMetaInfo.h"

// Bardzo wa�ny include. Je�eli ten plik nie zostanie gdzie� zaincludowany, to wszystko si� skompiluje
// ale interfejs nie b�dzie widoczny poza dllk�.
#include "EngineEditor/EditorPlugin/EditorApp/IDragable.h"

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
