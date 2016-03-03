/**@file ActorFactory.cpp
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Implementacja fabryki aktorów.*/
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

/**@brief Rejestrujê funkcjê tworz¹c¹ obiekt klasy identyfikowanej za pomoc¹ nazwy.

@param[in] name Nazwa klasy. Nie musi pokrywaæ siê z nazw¹ w C++, ale by³oby to wskazane ze wzglêdów estetycznych.
@param[in] function WskaŸnik na funkcjê tworz¹c¹ obiekt. Mo¿e to byæ funkcja globalna lub funkcja statyczna klasy.
@return Zwraca identyfikator przy pomocy którego mo¿na tworzyæ instancje obiektu.
*/
ActorType ActorFactory::RegisterClass( const std::string& name, CreateActorFunction function )
{
	auto element = m_classNames.find( name );
	if ( element != m_classNames.end() )
	{
		assert( false );
		return element->second;
	}

	m_createFunctions.push_back( function );

	ActorType newActorId = static_cast< ActorType >( m_createFunctions.size() - 1 );
	m_classNames[ name ] = newActorId;

	return newActorId;
}

/**@brief Pobiera identyfikator klasy aktora.

Przy pomocy tego identyfikatora mo¿na tworzyæ instancje tej klasy bez koniecznoœci
tworzenia stringów.*/
ActorType ActorFactory::GetClassId( const std::string& name )
{
	auto element = m_classNames.find( name );
	if ( element == m_classNames.end() )
	{
		assert( false );
		return -1;
	}

	return element->second;
}
