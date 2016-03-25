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
std::string			GetTypeidName()
{
	return RTTR::TypeInfo::get< ClassType >().getName();
}



/**@brief Fabryka aktorów silnika.

Przechowywana jest mapa stringów, które mapuj¹ siê na identyfikatory klas aktorów
oraz wektor funkcji do tworzenia obiektów.

Obiekty mo¿na tworzyæ podaj¹c stringi z nazw¹ lub identyfikatory.
Dostêp przy pomocy identyfikatoów jest szybszy, bo nie wymaga przeszukiwania mapy.

@todo Ze wzglêdu na pojawienie siê biblioteki RTTR, byæ mo¿e zamiast vectora funkcji
m_createFunctions, warto by by³o, aby ActorType to by³ identyfikator generowany przez
RTTR.
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
};


/**@brief Tworzy obiekt aktora o podanej nazwie. Nazwa musi byæ wczeœniej zarejestrowana.

Najlepiej, ¿eby nazwy klas odpowiada³y nazwom u¿ywanym w c++, ale nie jest to wymagane.

@param[in] name Nazwa aktora zarejestrowana funkcj¹ RegisterClass.
@return Zwraca wskaŸnik na stworzony obiekt lub nullptr, je¿eli indentyfikator nie zosta³ zarejestrowany.*/
template< typename Type > Type* ActorFactory::CreateActor( const std::string& name )
{
	auto index = m_classNames.find( name );
	if ( index == m_classNames.end() )
	{
		assert( false );
		return nullptr;
	}

	Object* newActor = CreateActor< Type >( index->second );
	return static_cast< Type* >( newActor );
}

/**@brief Tworzy obiekt aktora o podanym identyfikatorze. Identyfikator mo¿na pobraæ
funkcj¹ GetClassId. Nazwa funkcji tworz¹cej obiekt musi zostaæ najpierw zarejestrowana,
¿eby mo¿na by³o jej u¿ywaæ.

Najlepiej, ¿eby nazwy klas odpowiada³y nazwom u¿ywanym w c++, ale nie jest to wymagane.

@param[in] name Nazwa aktora zarejestrowana funkcj¹ RegisterClass.
@return Zwraca wskaŸnik na stworzony obiekt lub nullptr, je¿eli indentyfikator nie zosta³ zarejestrowany.*/
template< typename Type > Type* ActorFactory::CreateActor( ActorType id )
{
	if ( id < m_createFunctions.size() )
	{
		Object* newActor = m_createFunctions[ id ]();
		
		assert( rttr_cast< Type* >( newActor ) );
		return static_cast< Type* >( newActor );
	}
	return nullptr;
}

