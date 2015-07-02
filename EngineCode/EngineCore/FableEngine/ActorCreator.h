#pragma once

#include "stdafx.h"
#include "Interfaces\basic_interfaces.h"



class ActorInitializer;

class ActorCreator
{
	typedef Object* (*CreateActorFunction)();
private:
	std::unordered_map<std::string, int16> class_names;
	std::vector<CreateActorFunction> create_functions;
public:
	ActorCreator();
	~ActorCreator();

	void registerClass( const std::string name, CreateActorFunction function );
	int16 getClassId( const std::string name );
	template< typename Type = Object > Type* createActor( const std::string name );
	template< typename Type = Object > Type* createActor( uint16 id );
	//template< typename Type = Object > Type* createActor( const std::string name, const ActorInitializer initializer );
	//template< typename Type = Object > Type* createActor( uint16 id, const ActorInitializer initializer );
};


/**@brief Tworzy obiekt aktora o podanej nazwie. Nazwa musi byæ wczeœniej zarejestrowana.

Najlepiej, ¿eby nazwy klas odpowiada³y nazwom u¿ywanym w c++, ale nie jest to wymagane.

@param[in] name Nazwa aktora zarejestrowana funkcj¹ registerClass.
@return Zwraca wskaŸnik na stworzony obiekt lub nullptr, je¿eli indentyfikator nie zosta³ zarejestrowany.*/
template< typename Type = Object > Type* ActorCreator::createActor( const std::string name )
{
	auto index = class_name.find( name );
	if ( index == class_name.end() )
	{
		assert( false );
		return nullptr;
	}

	return createActor( index.second );

}

/**@brief Tworzy obiekt aktora o podanym identyfikatorze. Identyfikator mo¿na pobraæ
funkcj¹ getClassId. Nazwa funkcji tworz¹cej obiekt musi zostaæ najpierw zarejestrowana,
¿eby mo¿na by³o jej u¿ywaæ.

Najlepiej, ¿eby nazwy klas odpowiada³y nazwom u¿ywanym w c++, ale nie jest to wymagane.

@param[in] name Nazwa aktora zarejestrowana funkcj¹ registerClass.
@return Zwraca wskaŸnik na stworzony obiekt lub nullptr, je¿eli indentyfikator nie zosta³ zarejestrowany.*/
template< typename Type = Object > Type* ActorCreator::createActor( uint16 id )
{
	if ( id < create_functions.size() )
		return create_functions[ id ]();
	return nullptr;
}

///**@brief Tworzy obiekt aktora o podanej nazwie. Nazwa musi byæ wczeœniej zarejestrowana.
//Podany initializer jest u¿ywany w funkcji init, która jest wywo³ywana na stworzonym obiekcie.
//
//Najlepiej, ¿eby nazwy klas odpowiada³y nazwom u¿ywanym w c++, ale nie jest to wymagane.
//
//@param[in] name Nazwa aktora zarejestrowana funkcj¹ registerClass.
//@param[in] initializer Obiekt zawieraj¹cy dane u¿ywane do inicjacji obiektu. (Wywo³ywana jest funkcja init)
//@return Zwraca wskaŸnik na stworzony obiekt lub nullptr, je¿eli indentyfikator nie zosta³ zarejestrowany.*/
//template< typename Type = Object > Type* ActorCreator::createActor( const std::string name, const ActorInitializer initializer )
//{
//
//
//}
//
///**@brief Tworzy obiekt aktora o podanym identyfikatorze. Identyfikator mo¿na pobraæ
//funkcj¹ getClassId. Nazwa funkcji tworz¹cej obiekt musi zostaæ najpierw zarejestrowana,
//¿eby mo¿na by³o jej u¿ywaæ.
//
//Podany initializer jest u¿ywany w funkcji init, która jest wywo³ywana na stworzonym obiekcie.
//
//Najlepiej, ¿eby nazwy klas odpowiada³y nazwom u¿ywanym w c++, ale nie jest to wymagane.
//
//@param[in] id Identyfikator klasy, który mozna pobraæ funkcj¹ getClassId.
//@param[in] initializer Obiekt zawieraj¹cy dane u¿ywane do inicjacji obiektu. (Wywo³ywana jest funkcja init)
//@param[in] initializer Obiekt zawieraj¹cy dane do inicjacji klasy
//@return Zwraca wskaŸnik na stworzony obiekt lub nullptr, je¿eli indentyfikator nie zosta³ zarejestrowany.*/
//template< typename Type = Object > Type* ActorCreator::createActor( uint16 id, const ActorInitializer initializer )
//{
//
//}

