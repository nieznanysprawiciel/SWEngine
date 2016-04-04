/**@file ActorFactory.cpp
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Implementacja fabryki aktor�w.*/
#include "EngineCore/stdafx.h"
#include "ActorFactory.h"

#include "Common/MemoryLeaks.h"

#undef RegisterClass

/**@brief */
ActorFactory::ActorFactory()
{}

/**@brief */
ActorFactory::~ActorFactory()
{}


/**@brief Pobiera identyfikator klasy aktora.

Przy pomocy tego identyfikatora mo�na tworzy� instancje tej klasy bez konieczno�ci
tworzenia string�w.*/
ActorType ActorFactory::GetClassId( const std::string& name )
{
	auto element = m_classNames.find( name );
	if ( element == m_classNames.end() )
	{
		assert( false );
		return rttr::type::get( nullptr );
	}

	return element->second;
}

/**@brief Pobiera map� zarejestrowanych typ�w.

@return Pierszy element tablicy, to zarejestrowana nazwa klasy, a drugi identyfikator
typu aktora.*/
const ActorFactory::StringTypeMapping&	ActorFactory::GetRegisteredClasses	()
{
	return m_classNames;
}