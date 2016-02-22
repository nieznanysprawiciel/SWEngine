#pragma once
/**@file ActorFactory.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Fabryka aktor�w.*/

#include "EngineCore/stdafx.h"

#include "EngineCore/Actors/ActorObjects.h"
#include <typeinfo>

#include "FastDelegate.h"


#undef RegisterClass

class ActorInitializer;

typedef fastdelegate::FastDelegate0< Object* > CreateActorFunction;

/**@brief Zwraca nazw� klasy zapisan� w RTTI z pomini�ciem s��wka class.*/
template< typename ClassType >
const char*			GetTypeidName()
{
	return typeid( ClassType ).name() + 6;
}



/**@brief Fabryka aktor�w silnika.

Przechowywana jest mapa string�w, kt�re mapuj� si� na identyfikatory klas aktor�w
oraz wektor funkcji do tworzenia obiekt�w.

Obiekty mo�na tworzy� podaj�c stringi z nazw� lub identyfikatory.
Dost�p przy pomocy identyfikato�w jest szybszy, bo nie wymaga przeszukiwania mapy.
*/
class ActorFactory
{
private:
	std::unordered_map< std::string, ActorType >	m_classNames;
	std::vector< CreateActorFunction >				m_createFunctions;
public:
	ActorFactory();
	~ActorFactory();

	ActorType	RegisterClass			( const std::string& name, CreateActorFunction function );
	ActorType	GetClassId				( const std::string& name );

	template< typename Type = Object >	Type*		CreateActor				( const std::string& name );
	template< typename Type = Object >	Type*		CreateActor				( ActorType id );
	//template< typename Type = Object > Type* CreateActor( const std::string name, const ActorInitializer initializer );
	//template< typename Type = Object > Type* CreateActor( uint16 id, const ActorInitializer initializer );
};


/**@brief Tworzy obiekt aktora o podanej nazwie. Nazwa musi by� wcze�niej zarejestrowana.

Najlepiej, �eby nazwy klas odpowiada�y nazwom u�ywanym w c++, ale nie jest to wymagane.

@param[in] name Nazwa aktora zarejestrowana funkcj� RegisterClass.
@return Zwraca wska�nik na stworzony obiekt lub nullptr, je�eli indentyfikator nie zosta� zarejestrowany.*/
template< typename Type = Object > Type* ActorFactory::CreateActor( const std::string& name )
{
	auto index = m_classNames.find( name );
	if ( index == m_classNames.end() )
	{
		assert( false );
		return nullptr;
	}

	return CreateActor( index.second );

}

/**@brief Tworzy obiekt aktora o podanym identyfikatorze. Identyfikator mo�na pobra�
funkcj� GetClassId. Nazwa funkcji tworz�cej obiekt musi zosta� najpierw zarejestrowana,
�eby mo�na by�o jej u�ywa�.

Najlepiej, �eby nazwy klas odpowiada�y nazwom u�ywanym w c++, ale nie jest to wymagane.

@param[in] name Nazwa aktora zarejestrowana funkcj� RegisterClass.
@return Zwraca wska�nik na stworzony obiekt lub nullptr, je�eli indentyfikator nie zosta� zarejestrowany.*/
template< typename Type = Object > Type* ActorFactory::CreateActor( ActorType id )
{
	if ( id < m_createFunctions.size() )
		return m_createFunctions[ id ]();
	return nullptr;
}

///**@brief Tworzy obiekt aktora o podanej nazwie. Nazwa musi by� wcze�niej zarejestrowana.
//Podany initializer jest u�ywany w funkcji Init, kt�ra jest wywo�ywana na stworzonym obiekcie.
//
//Najlepiej, �eby nazwy klas odpowiada�y nazwom u�ywanym w c++, ale nie jest to wymagane.
//
//@param[in] name Nazwa aktora zarejestrowana funkcj� RegisterClass.
//@param[in] initializer Obiekt zawieraj�cy dane u�ywane do inicjacji obiektu. (Wywo�ywana jest funkcja Init)
//@return Zwraca wska�nik na stworzony obiekt lub nullptr, je�eli indentyfikator nie zosta� zarejestrowany.*/
//template< typename Type = Object > Type* ActorFactory::CreateActor( const std::string name, const ActorInitializer initializer )
//{
//
//
//}
//
///**@brief Tworzy obiekt aktora o podanym identyfikatorze. Identyfikator mo�na pobra�
//funkcj� GetClassId. Nazwa funkcji tworz�cej obiekt musi zosta� najpierw zarejestrowana,
//�eby mo�na by�o jej u�ywa�.
//
//Podany initializer jest u�ywany w funkcji Init, kt�ra jest wywo�ywana na stworzonym obiekcie.
//
//Najlepiej, �eby nazwy klas odpowiada�y nazwom u�ywanym w c++, ale nie jest to wymagane.
//
//@param[in] id Identyfikator klasy, kt�ry mozna pobra� funkcj� GetClassId.
//@param[in] initializer Obiekt zawieraj�cy dane u�ywane do inicjacji obiektu. (Wywo�ywana jest funkcja Init)
//@param[in] initializer Obiekt zawieraj�cy dane do inicjacji klasy
//@return Zwraca wska�nik na stworzony obiekt lub nullptr, je�eli indentyfikator nie zosta� zarejestrowany.*/
//template< typename Type = Object > Type* ActorFactory::CreateActor( uint16 id, const ActorInitializer initializer )
//{
//
//}

