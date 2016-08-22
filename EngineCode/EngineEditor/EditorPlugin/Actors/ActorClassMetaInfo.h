#pragma once

#include "Common/RTTR.h"

#include "EngineEditor/EditorPlugin/Properties/PropertyWrapper.h"
#include "EngineEditor/EditorPlugin/Actors/ActorWrapper.h"
#include "EngineEditor/EditorPlugin/EditorApp/IDragable.h"


class EngineObject;

namespace EditorPlugin
{
using namespace System::Collections::Generic;

/**@brief Klasa przechowuje metadane na temat typów actorów silnikowych.

Klasa mo¿e byc u¿yta nie tylko do wydobywania metainformacji o typie
oraz wydobywania poszczególnych parametrów dla aktora.
*/
public ref class ActorClassMetaInfo : public EditorApp::GUI::IDragable
{
private:

	// Metadata
	System::String^						m_actorClassName;
	rttr::type::type_id					m_type;

	List< PropertyWrapper^ >^			m_properties;

	// Object data
	ActorWrapper^						m_actorPtr;		///< WskaŸnik u¿ywany do pobierania parametrów obiektu klasy.

public:

	explicit	ActorClassMetaInfo	( rttr::type classType );
	explicit	ActorClassMetaInfo	( ActorWrapper^ actor );


	void		ResetActor		( ActorWrapper^ objectPtr );

public:

	void		BuildHierarchy( ActorWrapper^ objectPtr, rttr::type classType );


	/**@brief Zwraca typ obiektu, którym jest @ref rttr::type zwracany przez bibliotekê rttr.*/
	property int	Type
	{
		int get()	{ return (int)m_type; }
	}

	/**@brief Nazwa klasy.*/
	property System::String^			TypeName
	{
		System::String^					get()	{ return m_actorClassName; }
	}

	/**@brief Lista wszystkich Property danej klasy.*/
	property List< PropertyWrapper^ >^	Properties
	{
		List< PropertyWrapper^ >^		get()	{ return m_properties; }
	}

	property ActorWrapper^				Actor
	{
		ActorWrapper^					get ()	{ return m_actorPtr; }
	}


	property System::Type^		DataType
	{
		virtual System::Type^		get ()
		{
			return ActorClassMetaInfo::typeid;
		}
	}

	virtual void				Remove( System::Object^ i )
	{
		// Nic nie robi. Funkcja remove powinna zostaæ usuniêta.
	}

};

}	//	EditorPlugin
