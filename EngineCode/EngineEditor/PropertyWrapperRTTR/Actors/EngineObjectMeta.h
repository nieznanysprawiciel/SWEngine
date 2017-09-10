#pragma once
/**
@file EngineObjectMetaInfo.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/



#include "EngineEditor/PropertyWrapperRTTR/Build/IncludeRTTR.h"

#include "EngineEditor/PropertyWrapperRTTR/Properties/Complex/HierarchicalPropertyWrapper.h"
#include "EngineEditor/PropertyWrapperRTTR/Actors/EngineObjectWrapper.h"
#include "EngineEditor/EditorPlugin/EditorApp/IDragable.h"


class EngineObject;


namespace sw {
namespace EditorPlugin
{


using namespace System::Collections::Generic;



public ref class EngineObjectMeta : public EditorApp::GUI::IDragable
{
private:
protected:
	// Metadata
	System::String^						m_actorClassName;
	
	// Object data
	EngineObjectWrapper^				m_actorPtr;		///< WskaŸnik u¿ywany do pobierania parametrów obiektu klasy.
	HierarchicalPropertyWrapper^		m_properties;	///< Hierarchy of properties for wrapped object.

public:

	explicit	EngineObjectMeta( EngineObjectWrapper^ objectPtr );
	explicit	EngineObjectMeta( const TypeID& classType );
	

	virtual void		ResetActor		( EngineObjectWrapper^ objectPtr );

public:

	/**@brief Zwraca typ obiektu, którym jest @ref rttr::type zwracany przez bibliotekê rttr.*/
	property uint16						Type
	{
		uint16							get() { return m_actorPtr->Type; }
	}

	/**@brief Nazwa klasy.*/
	property System::String^			TypeName
	{
		System::String^					get() { return m_actorClassName; }
	}

	/**@brief Rozmiar klasy (operator sizeof).*/
	property uint32						TypeSize
	{
		uint32							get();
	}

	/**@brief Rozmiar zajmowany przez klasê razem z alokowan¹ pamiêci¹. (deklarowane przez klasê)*/
	property uint32						MemorySize
	{
		uint32							get();
	}

	/**@brief Lista wszystkich Property danej klasy.*/
	property List< PropertyWrapper^ >^	Properties
	{
		List< PropertyWrapper^ >^		get() { return m_properties->Properties; }
	}

	property EngineObjectWrapper^		Actor
	{
		EngineObjectWrapper^			get () { return m_actorPtr; }
	}

	property List< System::String^ >^	BaseClasses
	{
		List< System::String^ >^		get();
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
}	// sw
