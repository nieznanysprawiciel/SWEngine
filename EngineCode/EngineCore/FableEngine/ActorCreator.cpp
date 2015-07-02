#include "stdafx.h"
#include "ActorCreator.h"

#include "memory_leaks.h"


ActorCreator::ActorCreator()
{
}


ActorCreator::~ActorCreator()
{
}

/**@brief Rejestrujê funkcjê tworz¹c¹ obiekt klasy identyfikowanej za pomoc¹ nazwy.

@param[in] name Nazwa klasy. Nie musi pokrywaæ siê z nazw¹ w C++, ale by³oby to wskazane ze wzglêdów estetycznych.
@param[in] function WskaŸnik na funkcjê tworz¹c¹ obiekt. Mo¿e to byæ funkcja globalna lub funkcja statyczna klasy.
*/
void ActorCreator::registerClass( const std::string name, CreateActorFunction function )
{
	auto element = class_names.find( name );
	if ( element == class_names.end() )
	{
		assert( false );
		return;
	}

	create_functions.push_back( function );
	class_names[name] = create_functions.size() - 1;
}

int16 ActorCreator::getClassId( const std::string name )
{
	auto element = class_names.find( name );
	if ( element == class_names.end() )
	{
		assert( false );
		return -1;
	}

	return element->second;
}
