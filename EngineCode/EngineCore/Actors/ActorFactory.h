#pragma once
/**
@file ActorFactory.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "EngineCore/Actors/ActorObjects.h"
#include "swCommonLib/Common/RTTR.h"

#include "swCommonLib/External/FastDelegate/FastDelegate.h"

#include <unordered_map>
#include <map>


#undef RegisterClass


namespace sw
{


class ActorInitializer;

typedef fastdelegate::FastDelegate0< ActorBase* > CreateActorFunction;



/**@brief Zwraca nazw� klasy zapisan� w RTTI z pomini�ciem s��wka class.*/
template< typename ClassType >
std::string			GetTypeidName()
{
	return rttr::type::get< ClassType >().get_name().to_string();
}



/**@brief Fabryka aktor�w silnika.

Przechowywana jest mapa string�w, kt�re mapuj� si� na identyfikatory klas aktor�w
oraz wektor funkcji do tworzenia obiekt�w.

Obiekty mo�na tworzy� podaj�c stringi z nazw� lub identyfikatory.
Dost�p przy pomocy identyfikato�w jest szybszy, bo nie wymaga przeszukiwania mapy.
*/
class ActorFactory
{
	typedef std::map< std::string, ActorType >				StringTypeMapping;
	typedef std::map< ActorType, CreateActorFunction >		TypeCreateFunMapping;
private:

	StringTypeMapping				m_classNames;
	TypeCreateFunMapping			m_createFunctions;

public:
	ActorFactory();
	~ActorFactory();

	template< typename Type >
	ActorType					RegisterClass			( const std::string& name, CreateActorFunction function );
	ActorType					GetClassId				( const std::string& name );
	const StringTypeMapping&	GetRegisteredClasses	();


	template< typename Type = ActorBase >	Type*		CreateActor				( const std::string& name );
	template< typename Type = ActorBase >	Type*		CreateActor				( ActorType id );
};

/**@brief Rejestruj� funkcj� tworz�c� obiekt klasy aktora identyfikowanej za pomoc� nazwy.

@note W trybie debug funkcja assertuje, je�eli rejestrowana klasa nie dziedziczy po ActorBase.

@param[in] name Nazwa klasy. Nie musi pokrywa� si� z nazw� w C++, ale by�oby to wskazane ze wzgl�d�w estetycznych.
@param[in] function Wska�nik na funkcj� tworz�c� obiekt. Mo�e to by� funkcja globalna lub funkcja statyczna klasy.
@return Zwraca identyfikator przy pomocy kt�rego mo�na tworzy� instancje obiektu.
*/
template< typename Type >
ActorType ActorFactory::RegisterClass( const std::string& name, CreateActorFunction function )
{
	auto element = m_classNames.find( name );
	if( element != m_classNames.end() )
	{
		assert( false );
		return element->second;
	}

	assert( rttr::type::get< Type >().is_derived_from< ActorBase >() );

	auto inserted = m_createFunctions.insert( std::make_pair( rttr::type::get< Type >(), function ) );

	ActorType newActorId = inserted.first->first;
	m_classNames.insert( std::make_pair( name, newActorId ) );

	return newActorId;
}


/**@brief Tworzy obiekt aktora o podanej nazwie. Nazwa musi by� wcze�niej zarejestrowana.

Najlepiej, �eby nazwy klas odpowiada�y nazwom u�ywanym w c++, ale nie jest to wymagane.

@param[in] name Nazwa aktora zarejestrowana funkcj� RegisterClass.
@return Zwraca wska�nik na stworzony obiekt lub nullptr, je�eli indentyfikator nie zosta� zarejestrowany.*/
template< typename Type >
Type* ActorFactory::CreateActor( const std::string& name )
{
	auto index = m_classNames.find( name );
	if( index == m_classNames.end() )
	{
		assert( false );
		return nullptr;
	}

	ActorBase* newActor = CreateActor< Type >( index->second );
	return static_cast<Type*>( newActor );
}

/**@brief Tworzy obiekt aktora o podanym identyfikatorze. Identyfikator mo�na pobra�
funkcj� GetClassId. Nazwa funkcji tworz�cej obiekt musi zosta� najpierw zarejestrowana,
�eby mo�na by�o jej u�ywa�.

Najlepiej, �eby nazwy klas odpowiada�y nazwom u�ywanym w c++, ale nie jest to wymagane.

@param[in] name Nazwa aktora zarejestrowana funkcj� RegisterClass.
@return Zwraca wska�nik na stworzony obiekt lub nullptr, je�eli indentyfikator nie zosta� zarejestrowany.*/
template< typename Type >
Type* ActorFactory::CreateActor( ActorType id )
{
	auto function = m_createFunctions.find( id );

	if( function != m_createFunctions.end() )
	{
		ActorBase* newActor = function->second();

		assert( rttr::rttr_cast<Type*>( newActor ) );
		return static_cast<Type*>( newActor );
	}
	return nullptr;
}

}	// sw
