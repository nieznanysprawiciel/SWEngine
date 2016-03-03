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

/**@brief Rejestruj� funkcj� tworz�c� obiekt klasy identyfikowanej za pomoc� nazwy.

@param[in] name Nazwa klasy. Nie musi pokrywa� si� z nazw� w C++, ale by�oby to wskazane ze wzgl�d�w estetycznych.
@param[in] function Wska�nik na funkcj� tworz�c� obiekt. Mo�e to by� funkcja globalna lub funkcja statyczna klasy.
@return Zwraca identyfikator przy pomocy kt�rego mo�na tworzy� instancje obiektu.
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

Przy pomocy tego identyfikatora mo�na tworzy� instancje tej klasy bez konieczno�ci
tworzenia string�w.*/
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
