#pragma once
/**@file ActorFactory.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Fabryka aktorów.*/

#include "EngineCore/stdafx.h"

#include "EngineCore/Actors/ActorObjects.h"
#include <typeinfo>

#include "FastDelegate.h"


#undef RegisterClass

class ActorInitializer;

typedef fastdelegate::FastDelegate0< Object* > CreateActorFunction;

/**@brief Zwraca nazwê klasy zapisan¹ w RTTI z pominiêciem s³ówka class.*/
template< typename ClassType >
const char*			GetTypeidName()
{
	return typeid( ClassType ).name() + 6;
}



/**@brief Fabryka aktorów silnika.

Przechowywana jest mapa stringów, które mapuj¹ siê na identyfikatory klas aktorów
oraz wektor funkcji do tworzenia obiektów.

Obiekty mo¿na tworzyæ podaj¹c stringi z nazw¹ lub identyfikatory.
Dostêp przy pomocy identyfikatoów jest szybszy, bo nie wymaga przeszukiwania mapy.
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


/**@brief Tworzy obiekt aktora o podanej nazwie. Nazwa musi byæ wczeœniej zarejestrowana.

Najlepiej, ¿eby nazwy klas odpowiada³y nazwom u¿ywanym w c++, ale nie jest to wymagane.

@param[in] name Nazwa aktora zarejestrowana funkcj¹ RegisterClass.
@return Zwraca wskaŸnik na stworzony obiekt lub nullptr, je¿eli indentyfikator nie zosta³ zarejestrowany.*/
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

/**@brief Tworzy obiekt aktora o podanym identyfikatorze. Identyfikator mo¿na pobraæ
funkcj¹ GetClassId. Nazwa funkcji tworz¹cej obiekt musi zostaæ najpierw zarejestrowana,
¿eby mo¿na by³o jej u¿ywaæ.

Najlepiej, ¿eby nazwy klas odpowiada³y nazwom u¿ywanym w c++, ale nie jest to wymagane.

@param[in] name Nazwa aktora zarejestrowana funkcj¹ RegisterClass.
@return Zwraca wskaŸnik na stworzony obiekt lub nullptr, je¿eli indentyfikator nie zosta³ zarejestrowany.*/
template< typename Type = Object > Type* ActorFactory::CreateActor( ActorType id )
{
	if ( id < m_createFunctions.size() )
		return m_createFunctions[ id ]();
	return nullptr;
}

///**@brief Tworzy obiekt aktora o podanej nazwie. Nazwa musi byæ wczeœniej zarejestrowana.
//Podany initializer jest u¿ywany w funkcji Init, która jest wywo³ywana na stworzonym obiekcie.
//
//Najlepiej, ¿eby nazwy klas odpowiada³y nazwom u¿ywanym w c++, ale nie jest to wymagane.
//
//@param[in] name Nazwa aktora zarejestrowana funkcj¹ RegisterClass.
//@param[in] initializer Obiekt zawieraj¹cy dane u¿ywane do inicjacji obiektu. (Wywo³ywana jest funkcja Init)
//@return Zwraca wskaŸnik na stworzony obiekt lub nullptr, je¿eli indentyfikator nie zosta³ zarejestrowany.*/
//template< typename Type = Object > Type* ActorFactory::CreateActor( const std::string name, const ActorInitializer initializer )
//{
//
//
//}
//
///**@brief Tworzy obiekt aktora o podanym identyfikatorze. Identyfikator mo¿na pobraæ
//funkcj¹ GetClassId. Nazwa funkcji tworz¹cej obiekt musi zostaæ najpierw zarejestrowana,
//¿eby mo¿na by³o jej u¿ywaæ.
//
//Podany initializer jest u¿ywany w funkcji Init, która jest wywo³ywana na stworzonym obiekcie.
//
//Najlepiej, ¿eby nazwy klas odpowiada³y nazwom u¿ywanym w c++, ale nie jest to wymagane.
//
//@param[in] id Identyfikator klasy, który mozna pobraæ funkcj¹ GetClassId.
//@param[in] initializer Obiekt zawieraj¹cy dane u¿ywane do inicjacji obiektu. (Wywo³ywana jest funkcja Init)
//@param[in] initializer Obiekt zawieraj¹cy dane do inicjacji klasy
//@return Zwraca wskaŸnik na stworzony obiekt lub nullptr, je¿eli indentyfikator nie zosta³ zarejestrowany.*/
//template< typename Type = Object > Type* ActorFactory::CreateActor( uint16 id, const ActorInitializer initializer )
//{
//
//}

