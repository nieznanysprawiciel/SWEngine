#include "EngineCore/stdafx.h"
#include "ActorCreator.h"

#include "Common\memory_leaks.h"


ActorFactory::ActorFactory()
{
}


ActorFactory::~ActorFactory()
{
}

/**@brief Rejestrujê funkcjê tworz¹c¹ obiekt klasy identyfikowanej za pomoc¹ nazwy.

@param[in] name Nazwa klasy. Nie musi pokrywaæ siê z nazw¹ w C++, ale by³oby to wskazane ze wzglêdów estetycznych.
@param[in] function WskaŸnik na funkcjê tworz¹c¹ obiekt. Mo¿e to byæ funkcja globalna lub funkcja statyczna klasy.
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
