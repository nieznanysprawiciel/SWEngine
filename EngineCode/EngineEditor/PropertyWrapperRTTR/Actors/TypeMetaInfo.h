#pragma once
/**
@file TypeMetaInfo.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/



#include "EngineEditor/PropertyWrapperRTTR/Build/IncludeRTTR.h"
#include "EngineEditor/EditorPlugin/EditorApp/IDragable.h"



namespace sw {
namespace EditorPlugin
{


using namespace System::Collections::Generic;



/**@brief This class wraps rttr::type.*/
public ref class TypeMetaInfo : public EditorApp::GUI::IDragable
{
private:
protected:

	System::String^				m_typeName;
	TypeID::type_id				m_typeId;

public:

	explicit		TypeMetaInfo		( const TypeID& type );

	
	/**@brief Type identifier.*/
	property uint16						Type
	{
		uint16							get() { return m_typeId; }
	}

	/**@brief Registered type name.*/
	property System::String^			TypeName
	{
		System::String^					get() { return m_typeName; }
	}

	/**@brief Size of type.*/
	property uint32						TypeSize
	{
		uint32							get();
	}

	/**@brief List of base classes if this type is object type.*/
	property List< System::String^ >^	BaseClasses
	{
		List< System::String^ >^		get();
	}


public:
	// ================================ //
	// EditorApp::GUI::IDragable

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

public:

	TypeID					GetType		();
};



}	// EditorPlugin
}	// sw




