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
	EnumPropertyWrapper( HierarchicalPropertyWrapper^ parent, rttr::property prop );

public:
	property System::String^		Value
	{
		System::String^		get	();
		void				set	( System::String^ value );
	}

	property List< System::String^ >^	EnumStrings
	{
		List< System::String^ >^		get()
		{
			return m_enumStrings;
		}
	}

private:

	System::String^		GetValue		( const rttr::instance& refObject );
	void				SetValue		( rttr::instance& refObject, System::String^ newValue );
};


}
}
