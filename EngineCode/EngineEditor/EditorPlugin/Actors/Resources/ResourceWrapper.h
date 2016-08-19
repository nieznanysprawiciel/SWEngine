#pragma once

#include "EngineEditor/EditorPlugin/Actors/EngineObjectWrapper.h"
#include "EngineEditor/EditorPlugin/Properties/ResourcePropertyWrapper.h"
#include "GraphicAPI/ResourceObject.h"

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
