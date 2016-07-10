#pragma once

#include "EngineCore/EventsManager/Signal.h"
#include "EngineCore/Actors/BasicActors/ActorBase.h"


/**@brief Klucz dostêpu do klasy ActorPtr.

Tylko wybrane klasy mog¹ stworzyæ ten obiekt i mieæ bezpoœredni dostêp do wskaŸnika w klasie ActorPtr.
Zasadniczo aktorzy nie powinni wywo³ywaæ swoich funkcji bezoœrednio. Zamiast tego musz¹ wymieniaæ siê
sygna³ami i eventami. Klasa ActorPtrKey umo¿liwia tylko wywo³anie takich funkcji, jakie s¹ bezieczne
w danym kontekœcie.*/
class ActorPtrKey
{
private:
	ActorPtrKey() = default;					///<Tylko klasa zaprzyjaŸniona mo¿e stworzyæ obiekt.
	ActorPtrKey( const ActorPtrKey& ) {}		///<Tylko klasa zaprzyjaŸniona mo¿e stworzyæ obiekt.
	ActorPtrKey( const ActorPtrKey&& ) {}		///<Tylko klasa zaprzyjaŸniona mo¿e stworzyæ obiekt.
public:
};


/**@brief Klasa trzymaj¹ca wskaŸnik na aktora.

Na aktorze mo¿na wywo³ywaæ jedynie niektóre funkcje.
Dostêp do pozosta³ych jest uniemo¿liwiony celowo.

@see ActorPtrKey*/
template< typename Type >
class ActorPtr
{
private:
	Type*		m_actorPointer;

public:
	ActorPtr	();
	ActorPtr	( Type* ptr );
	ActorPtr	( const ActorPtr< Type >& other );

	void		operator=			( Type* ptr );
	void		operator=			( ActorPtr< Type >& ptr );
	bool		operator==			( Type* ptr );
	bool		operator==			( ActorPtr< Type >& ptr );

	///@name Funkcje ogólnie dostêpne
	///@{
	template< void (Type::*signalPtr)() >
	bool					Signal	( const EngineObject* sender, Event* parameters );

	template< void (Type::*signalPtr)() >
	bool					Signal	( const EngineObject* sender, Event* parameters, SignalDelegate onExecuted );
	///@}

public:
	///@name Funkcje o ograniczonym dostêpie
	///@{
	inline Type*		Get		( const ActorPtrKey );
	///@}
};

/**@brief Tworzy aktora ze wskaŸnikiem na nullptr.,*/
template< typename Type >
inline					ActorPtr< Type >::ActorPtr()
	:	m_actorPointer( nullptr )
{ }

/**@brief */
template< typename Type >
inline					ActorPtr< Type >::ActorPtr	( Type* ptr )
	:	m_actorPointer( ptr )
{ }

/**@brief */
template< typename Type >
inline					ActorPtr< Type >::ActorPtr	( const ActorPtr< Type >& other )
	:	m_actorPointer( other.m_actorPointer )
{ }



template< typename Type >
inline void				ActorPtr< Type >::operator=( Type* ptr )
{	m_actorPointer = ptr;	}

template< typename Type >
inline void				ActorPtr< Type >::operator=( ActorPtr< Type >& ptr )
{	m_actorPointer = ptr.m_actorPointer; }

template< typename Type >
inline bool				ActorPtr< Type >::operator==( Type* ptr )
{	return m_actorPointer == ptr;	}

template< typename Type >
inline bool				ActorPtr< Type >::operator==( ActorPtr< Type >& ptr )
{	return m_actorPointer == ptr.m_actorPointer;}


/**@brief Pobiera wskaŸnik na aktora.

Dostêp ograniczony.*/
template< typename Type >
inline Type*		ActorPtr< Type >::Get		( const ActorPtrKey )
{
	return m_actorPointer; 
}

/**@brief Wysy³a sygna³ do aktora.

W parametrze szablonu trzeba podaæ wskaŸnik na funkcjê, która ma zostaæ wywo³ana.
Funkcja ta musi byæ oznaczona jako sygna³.

@param[in] sender Obiekt, który wysy³a sygna³. Nale¿y podaæ wskaŸnik this.

@param[in] parameters Struktura z parametrami. Nale¿y odziedziczyc po klasie event i stworzyc w³asny
zestaw parametrów lub u¿yæ wbudowanej klasy.*/
template< typename Type >
template< void( Type::* signalPtr )( ) >
inline bool				ActorPtr< Type >::Signal	( const EngineObject* sender, Event* parameters )
{
	return false;
}

/**@brief Wysy³a sygna³ do aktora. 

W parametrze szablonu trzeba podaæ wskaŸnik na funkcjê, która ma zostaæ wywo³ana.
Funkcja ta musi byæ oznaczona jako sygna³.

@param[in] sender Obiekt, który wysy³a sygna³. Nale¿y podaæ wskaŸnik this.

@param[in] parameters Struktura z parametrami. Nale¿y odziedziczyc po klasie event i stworzyc w³asny
zestaw parametrów lub u¿yæ wbudowanej klasy.

@param[in] onExecuted Podana funkcja zostanie wywo³ana po przetworzeniu sygna³u przez aktora.
W parametrze zostanie zwrócona ta sama struktura parameters, któr¹ podano rzy wywo³aniu.
Aktor otrzymuj¹cy sygna³ mo¿e zmodyfikowaæ tê strukturê.*/
template<typename Type>
template<void( Type::* signalPtr )( )>
inline bool				ActorPtr< Type >::Signal	( const EngineObject* sender, Event* parameters, SignalDelegate onExecuted )
{
	return false;
}
