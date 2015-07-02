#include "stdafx.h"
#include "ActorCreator.h"

#include "memory_leaks.h"


ActorCreator::ActorCreator()
{
}


ActorCreator::~ActorCreator()
{
}

/**@brief Rejestruj� funkcj� tworz�c� obiekt klasy identyfikowanej za pomoc� nazwy.

@param[in] name Nazwa klasy. Nie musi pokrywa� si� z nazw� w C++, ale by�oby to wskazane ze wzgl�d�w estetycznych.
@param[in] function Wska�nik na funkcj� tworz�c� obiekt. Mo�e to by� funkcja globalna lub funkcja statyczna klasy.
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
