#pragma once

#include "EngineCore/stdafx.h"
#include "EngineCore/Actors/ActorObjects.h"



class ActorInitializer;

class ActorFactory
{
public:
	typedef Object* (*CreateActorFunction)();
private:
	std::unordered_map<std::string, int16>		m_classNames;
	std::vector<CreateActorFunction>			m_createFunctions;
public:
	ActorFactory();
	~ActorFactory();

	void		RegisterClass			( const std::string& name, CreateActorFunction function );
	int16		GetClassId				( const std::string& name );

	template< typename Type = Object >	Type*		CreateActor				( const std::string& name );
	template< typename Type = Object >	Type*		CreateActor				( uint16 id );
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
template< typename Type = Object > Type* ActorFactory::CreateActor( uint16 id )
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

