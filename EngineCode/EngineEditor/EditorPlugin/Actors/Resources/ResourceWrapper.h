#pragma once
/**
@file ResourceWrapper.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/



#include "EngineEditor/EditorPlugin/Actors/EngineObjectWrapper.h"
#include "EngineEditor/EditorPlugin/Properties/ResourcePropertyWrapper.h"
#include "swGraphicAPI/Resources/ResourceObject.h"

namespace sw {
namespace EditorPlugin
{



public ref class ResourceWrapper : EngineObjectWrapper
{
private:

	ResourcePropertyType			m_resourceType;

public:
	ResourceWrapper			( ResourceObject* resource );



	property ResourcePropertyType	ResourceType
	{
		ResourcePropertyType		get ()
		{
			return m_resourceType;
		}

		void						set ( ResourcePropertyType type )
		{
			m_resourceType = type;
		}
	}

	property System::String^		ResourceName
	{
		System::String^				get();
	}

	property int					ID
	{
		int							get();
	}

};


}	// EditorPlugin
}	// sw
