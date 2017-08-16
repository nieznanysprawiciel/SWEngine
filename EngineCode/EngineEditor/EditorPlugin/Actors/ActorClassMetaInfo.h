#pragma once
/**
@file ActorClassMetaInfo.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/



#include "EngineEditor/PropertyWrapperRTTR/Build/IncludeRTTR.h"

#include "EngineEditor/EditorPlugin/Actors/ActorWrapper.h"



class EngineObject;


namespace sw {
namespace EditorPlugin
{
using namespace System::Collections::Generic;

/**@brief Klasa przechowuje metadane na temat typów actorów silnikowych.

Klasa mo¿e byc u¿yta nie tylko do wydobywania metainformacji o typie
oraz wydobywania poszczególnych parametrów dla aktora.
*/
public ref class ActorClassMetaInfo : public EngineObjectMeta
{
private:

	rttr::type::type_id					m_type;

public:

	explicit	ActorClassMetaInfo	( rttr::type classType );
	explicit	ActorClassMetaInfo	( ActorWrapper^ actor );


public:


	/**@brief Zwraca typ obiektu, którym jest @ref rttr::type zwracany przez bibliotekê rttr.*/
	property int	Type
	{
		int get() { return (int)m_type; }
	}

	/**@brief Nazwa klasy.*/
	property System::String^			TypeName
	{
		System::String^					get() { return m_actorClassName; }
	}

	property ActorWrapper^				Actor
	{
		ActorWrapper^					get () { return static_cast< ActorWrapper^ >( m_actorPtr ); }
	}

};

}	//	EditorPlugin

}	// sw