#pragma once
/**
@file StringPropertyWrapper.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "EngineEditor/PropertyWrapperRTTR/Properties/Base/PropertyWrapper.h"


namespace sw {
namespace EditorPlugin
{


//====================================================================================//
//				StringPropertyWrapper
//====================================================================================//



/**@brief Property typu std::string.*/
public ref class StringPropertyWrapper : PropertyWrapper
{
public:
	StringPropertyWrapper( PropertyWrapper^ parent, rttr::property prop )
		: PropertyWrapper( parent, PropertyType::PropertyString, prop, prop.get_name().to_string().c_str() )
	{}

public:
	property System::String^		Value
	{
		System::String^		get	()
		{
			if( m_actorPtr )
				return GetValue( m_actorPtr );
			return gcnew System::String( "" );
		}

		void				set	( System::String^ value )
		{
			if( m_actorPtr )
				SetValue( m_actorPtr, value );
		}
	}

private:
	System::String^		GetValue( void* refObject );
	void				SetValue( void* refObject, System::String^ newValue );
};


//====================================================================================//
//				WStringPropertyWrapper
//====================================================================================//



/**@brief Property typu std::wstring.*/
public ref class WStringPropertyWrapper : PropertyWrapper
{
public:
	WStringPropertyWrapper( PropertyWrapper^ parent, rttr::property prop )
		: PropertyWrapper( parent, PropertyType::PropertyWString, prop, prop.get_name().to_string().c_str() )
	{}

public:
	property System::String^		Value
	{
		System::String^		get	()
		{
			if( m_actorPtr )
				return GetValue( m_actorPtr );
			return gcnew System::String( "" );
		}

		void				set	( System::String^ value )
		{
			if( m_actorPtr )
				SetValue( m_actorPtr, value );
		}
	}

private:
	System::String^		GetValue( void* refObject );
	void				SetValue( void* refObject, System::String^ newValue );
};



}
}	// sw


