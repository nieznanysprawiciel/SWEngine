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


/**@brief Tworzy obiekt aktora o podanej nazwie. Nazwa musi by� wcze�niej zarejestrowana.

Najlepiej, �eby nazwy klas odpowiada�y nazwom u�ywanym w c++, ale nie jest to wymagane.

@param[in] name Nazwa aktora zarejestrowana funkcj� registerClass.
@return Zwraca wska�nik na stworzony obiekt lub nullptr, je�eli indentyfikator nie zosta� zarejestrowany.*/
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

/**@brief Tworzy obiekt aktora o podanym identyfikatorze. Identyfikator mo�na pobra�
funkcj� getClassId. Nazwa funkcji tworz�cej obiekt musi zosta� najpierw zarejestrowana,
�eby mo�na by�o jej u�ywa�.

Najlepiej, �eby nazwy klas odpowiada�y nazwom u�ywanym w c++, ale nie jest to wymagane.

@param[in] name Nazwa aktora zarejestrowana funkcj� registerClass.
@return Zwraca wska�nik na stworzony obiekt lub nullptr, je�eli indentyfikator nie zosta� zarejestrowany.*/
template< typename Type = Object > Type* ActorCreator::createActor( uint16 id )
{
	if ( id < create_functions.size() )
		return create_functions[ id ]();
	return nullptr;
}

///**@brief Tworzy obiekt aktora o podanej nazwie. Nazwa musi by� wcze�niej zarejestrowana.
//Podany initializer jest u�ywany w funkcji init, kt�ra jest wywo�ywana na stworzonym obiekcie.
//
//Najlepiej, �eby nazwy klas odpowiada�y nazwom u�ywanym w c++, ale nie jest to wymagane.
//
//@param[in] name Nazwa aktora zarejestrowana funkcj� registerClass.
//@param[in] initializer Obiekt zawieraj�cy dane u�ywane do inicjacji obiektu. (Wywo�ywana jest funkcja init)
//@return Zwraca wska�nik na stworzony obiekt lub nullptr, je�eli indentyfikator nie zosta� zarejestrowany.*/
//template< typename Type = Object > Type* ActorCreator::createActor( const std::string name, const ActorInitializer initializer )
//{
//
//
//}
//
///**@brief Tworzy obiekt aktora o podanym identyfikatorze. Identyfikator mo�na pobra�
//funkcj� getClassId. Nazwa funkcji tworz�cej obiekt musi zosta� najpierw zarejestrowana,
//�eby mo�na by�o jej u�ywa�.
//
//Podany initializer jest u�ywany w funkcji init, kt�ra jest wywo�ywana na stworzonym obiekcie.
//
//Najlepiej, �eby nazwy klas odpowiada�y nazwom u�ywanym w c++, ale nie jest to wymagane.
//
//@param[in] id Identyfikator klasy, kt�ry mozna pobra� funkcj� getClassId.
//@param[in] initializer Obiekt zawieraj�cy dane u�ywane do inicjacji obiektu. (Wywo�ywana jest funkcja init)
//@param[in] initializer Obiekt zawieraj�cy dane do inicjacji klasy
//@return Zwraca wska�nik na stworzony obiekt lub nullptr, je�eli indentyfikator nie zosta� zarejestrowany.*/
//template< typename Type = Object > Type* ActorCreator::createActor( uint16 id, const ActorInitializer initializer )
//{
//
//}

