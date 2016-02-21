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
template< typename Type = Object > Type* ActorFactory::CreateActor( uint16 id )
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

