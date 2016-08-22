#pragma once

#include "Common/RTTR.h"

#include "EngineEditor/EditorPlugin/Properties/PropertyWrapper.h"
#include "EngineEditor/EditorPlugin/Actors/EngineObjectWrapper.h"
#include "EngineEditor/EditorPlugin/EditorApp/IDragable.h"


class EngineObject;

namespace EditorPlugin
{
using namespace System::Collections::Generic;



public ref class EngineObjectMetaInfo : public EditorApp::GUI::IDragable
{
private:
protected:
	// Metadata
	System::String^						m_actorClassName;
	rttr::type::type_id					m_type;

	List< PropertyWrapper^ >^			m_properties;

	// Object data
	EngineObjectWrapper^				m_actorPtr;		///< WskaŸnik u¿ywany do pobierania parametrów obiektu klasy.

public:
	EngineObjectMetaInfo( EngineObjectWrapper^ objectPtr );



	virtual void		ResetActor		( EngineObjectWrapper^ objectPtr );

public:

	/**@brief Zwraca typ obiektu, którym jest @ref rttr::type zwracany przez bibliotekê rttr.*/
	property int						Type
	{
		int								get()	{ return (int)m_type; }
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

	property EngineObjectWrapper^		Actor
	{
		EngineObjectWrapper^			get ()	{ return m_actorPtr; }
	}


	property System::Type^				DataType
	{
		virtual System::Type^			get ()
		{
			return nullptr;
		}
	}

	virtual void				Remove( System::Object^ i )
	{
		// Nic nie robi. Funkcja remove powinna zostaæ usuniêta.
	}

protected:

	void				BuildHierarchy	( EngineObjectWrapper^ objectPtr, rttr::type classType );

};


}	// EditorPlugin
