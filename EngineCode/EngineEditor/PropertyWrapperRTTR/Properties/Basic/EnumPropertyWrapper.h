#pragma once
/**
@file EnumPropertyWrapper.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "EngineEditor/PropertyWrapperRTTR/Properties/Base/PropertyWrapper.h"



namespace sw {
namespace EditorPlugin
{



/**@brief Property enum.*/
public ref class EnumPropertyWrapper : PropertyWrapper
{
private:
protected:

	List< System::String^ >^		m_enumStrings;

public:
	EnumPropertyWrapper( void* parent, rttr::property prop );

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

	property List< System::String^ >^	EnumStrings
	{
		List< System::String^ >^		get()
		{
			return m_enumStrings;
		}
	}

private:
	System::String^		GetValue( void* refObject );
	void				SetValue( void* refObject, System::String^ newValue );
};


}
}
