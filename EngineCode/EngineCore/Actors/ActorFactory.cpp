/**
@file HosekSykDome.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "EngineCore/stdafx.h"
#include "ActorFactory.h"

#include "swCommonLib/Common/MemoryLeaks.h"



namespace sw
{


/**@brief */
ActorFactory::ActorFactory()
{}

/**@brief */
ActorFactory::~ActorFactory()
{}


/**@brief Pobiera identyfikator klasy aktora.

Przy pomocy tego identyfikatora mo¿na tworzyæ instancje tej klasy bez koniecznoœci
tworzenia stringów.*/
ActorType ActorFactory::GetClassId( const std::string& name )
{
	auto element = m_classNames.find( name );
	if( element == m_classNames.end() )
	{
		assert( false );
		return rttr::type::get( nullptr );
	}

	return element->second;
}

/**@brief Pobiera mapê zarejestrowanych typów.

@return Pierszy element tablicy, to zarejestrowana nazwa klasy, a drugi identyfikator
typu aktora.*/
const ActorFactory::StringTypeMapping&	ActorFactory::GetRegisteredClasses	()
{
	return m_classNames;
}


}	// sw

