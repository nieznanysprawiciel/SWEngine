#pragma once
/**
@file ResourceMetaInfo.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/



#include "EngineEditor/EditorPlugin/Actors/EngineObjectMetaInfo.h"
#include "ResourceWrapper.h"




namespace sw {
namespace EditorPlugin
{



public ref class ResourceMetaInfo : public EngineObjectMetaInfo
{
private:
protected:
public:
	ResourceMetaInfo( ResourceWrapper^ resource );



	property ResourceWrapper^			Resource
	{
		ResourceWrapper^				get () { return static_cast<ResourceWrapper^>( m_actorPtr ); }
	}
};


}	// EditorPlugin
}	// sw


