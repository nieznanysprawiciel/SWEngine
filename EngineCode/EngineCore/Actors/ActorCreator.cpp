#include "EngineCore/stdafx.h"
#include "ActorCreator.h"

#include "Common\memory_leaks.h"


ActorFactory::ActorFactory()
{
}


ActorFactory::~ActorFactory()
{
}

/**@brief Rejestruj� funkcj� tworz�c� obiekt klasy identyfikowanej za pomoc� nazwy.

@param[in] name Nazwa klasy. Nie musi pokrywa� si� z nazw� w C++, ale by�oby to wskazane ze wzgl�d�w estetycznych.
@param[in] function Wska�nik na funkcj� tworz�c� obiekt. Mo�e to by� funkcja globalna lub funkcja statyczna klasy.
*/
void ActorFactory::RegisterClass( const std::string& name, CreateActorFunction function )
{
	auto element = m_classNames.find( name );
	if ( element == m_classNames.end() )
	{
		assert( false );
		return;
	}

	m_createFunctions.push_back( function );
	m_classNames[name] = static_cast<short>( m_createFunctions.size() - 1 );
}

/**@brief */
int16 ActorFactory::GetClassId( const std::string& name )
{
	auto element = m_classNames.find( name );
	if ( element == m_classNames.end() )
	{
		assert( false );
		return -1;
	}

	return element->second;
}
