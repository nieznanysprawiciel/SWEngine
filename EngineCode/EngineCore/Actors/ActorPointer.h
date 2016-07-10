#pragma once

#include "EngineCore/EventsManager/Signal.h"
#include "EngineCore/Actors/BasicActors/ActorBase.h"
#include "EngineCore/MainEngine/EngineInterface.h"


/**@brief Klucz dost�pu do klasy ActorPtr.

Tylko wybrane klasy mog� stworzy� ten obiekt i mie� bezpo�redni dost�p do wska�nika w klasie ActorPtr.
Zasadniczo aktorzy nie powinni wywo�ywa� swoich funkcji bezo�rednio. Zamiast tego musz� wymienia� si�
sygna�ami i eventami. Klasa ActorPtrKey umo�liwia tylko wywo�anie takich funkcji, jakie s� bezieczne
w danym kontek�cie.*/
class ActorPtrKey
{
private:
	ActorPtrKey() = default;					///<Tylko klasa zaprzyja�niona mo�e stworzy� obiekt.
	ActorPtrKey( const ActorPtrKey& ) {}		///<Tylko klasa zaprzyja�niona mo�e stworzy� obiekt.
	ActorPtrKey( const ActorPtrKey&& ) {}		///<Tylko klasa zaprzyja�niona mo�e stworzy� obiekt.
public:
};


/**@brief Klasa trzymaj�ca wska�nik na aktora.

Na aktorze mo�na wywo�ywa� jedynie niekt�re funkcje.
Dost�p do pozosta�ych jest uniemo�liwiony celowo.

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
	bool		operator==			( const Type* ptr ) const;
	bool		operator==			( const ActorPtr< Type >& ptr ) const;
	bool		operator<			( const ActorPtr< Type >& ptr ) const;

	///@name Funkcje og�lnie dost�pne
	///@{
	template< void (Type::*signalPtr)( const EngineObject* sender, Event* params ) >
	bool					SendSignal	( const EngineObject* sender, Event* parameters );

	template< void (Type::*signalPtr)( const EngineObject* sender, Event* params ) >
	bool					SendSignal	( const EngineObject* sender, Event* parameters, SignalDelegate onExecuted );

	bool					IsValid		()	{ return m_actorPointer != nullptr; }

	template< typename CastType >
	ActorPtr< CastType >		StaticCast	();

	template< typename CastType >
	const ActorPtr< CastType >	StaticCast	() const;

	template< typename CastType >
	ActorPtr< CastType >		DynamicCast	();

	template< typename CastType >
	const ActorPtr< CastType >	DynamicCast	() const;
	///@}

public:
	///@name Funkcje o ograniczonym dost�pie
	///@{
	inline Type*		Get		( const ActorPtrKey );
	///@}
};

/**@brief Tworzy aktora ze wska�nikiem na nullptr.,*/
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
inline bool				ActorPtr< Type >::operator==( const Type* ptr ) const
{	return m_actorPointer == ptr;	}

template< typename Type >
inline bool				ActorPtr< Type >::operator==( const ActorPtr< Type >& ptr ) const
{	return m_actorPointer == ptr.m_actorPointer;	}

template<typename Type>
inline bool				ActorPtr< Type >::operator<( const ActorPtr< Type >& ptr ) const
{	return m_actorPointer < ptr.m_actorPointer;		}


/**@brief Pobiera wska�nik na aktora.

Dost�p ograniczony.*/
template< typename Type >
inline Type*		ActorPtr< Type >::Get		( const ActorPtrKey )
{
	return m_actorPointer; 
}

/**@brief Wysy�a sygna� do aktora.

W parametrze szablonu trzeba poda� wska�nik na funkcj�, kt�ra ma zosta� wywo�ana.
Funkcja ta musi by� oznaczona jako sygna�.

@param[in] sender Obiekt, kt�ry wysy�a sygna�. Nale�y poda� wska�nik this.

@param[in] parameters Struktura z parametrami. Nale�y odziedziczyc po klasie event i stworzy� w�asny
zestaw parametr�w lub u�y� wbudowanej klasy.*/
template< typename Type >
template< void( Type::* signalPtr )( const EngineObject* sender, Event* params ) >
inline bool				ActorPtr< Type >::SendSignal	( const EngineObject* sender, Event* parameters )
{
	return SendSignal< signalPtr >( sender, parameters, SignalDelegate() );
}

/**@brief Wysy�a sygna� do aktora. 

W parametrze szablonu trzeba poda� wska�nik na funkcj�, kt�ra ma zosta� wywo�ana.
Funkcja ta musi by� oznaczona jako sygna�.

@param[in] sender Obiekt, kt�ry wysy�a sygna�. Nale�y poda� wska�nik this.

@param[in] parameters Struktura z parametrami. Nale�y odziedziczyc po klasie event i stworzy� w�asny
zestaw parametr�w lub u�y� wbudowanej klasy.

@param[in] onExecuted Podana funkcja zostanie wywo�ana po przetworzeniu sygna�u przez aktora.
W parametrze zostanie zwr�cona ta sama struktura parameters, kt�r� podano rzy wywo�aniu.
Aktor otrzymuj�cy sygna� mo�e zmodyfikowa� t� struktur�.*/
template<typename Type>
template<void( Type::* signalPtr )( const EngineObject* sender, Event* params )>
inline bool				ActorPtr< Type >::SendSignal	( const EngineObject* sender, Event* parameters, SignalDelegate onExecuted )
{
	if( IsValid() )
	{
		auto engine = ActorBase::GetEngineInterface();

		EventDelegate delegate;
		delegate.bind( m_actorPointer, static_cast< void (EngineObject::*)( const EngineObject*, Event* ) >( signalPtr ) );

		Signal sigParams;
		sigParams.Sender = sender;
		sigParams.Receiver = m_actorPointer;
		sigParams.Parameters = parameters;
		sigParams.SignalHandler = delegate;
		sigParams.OnExecuted = onExecuted;

		engine->Actors.Communication.SendSignal( sigParams );

		return true;
	}
	return false;
}

template< typename Type >
template< typename CastType >
inline ActorPtr< CastType >			ActorPtr< Type >::StaticCast()
{
	return ActorPtr< CastType >( static_cast< CastType* >( m_actorPointer ) );
}

template< typename Type >
template< typename CastType >
inline const ActorPtr< CastType >	ActorPtr< Type >::StaticCast() const
{
	static_assert( std::is_const< CastType >::value, "You are casting constant object. Add const keyword in template parameter." );
	return ActorPtr< CastType >( static_cast< CastType* >( m_actorPointer ) );
}

template< typename Type >
template< typename CastType >
inline ActorPtr< CastType >			ActorPtr< Type >::DynamicCast()
{
	return ActorPtr< CastType >( rttr::rttr_cast< CastType* >( m_actorPointer ) );
}

template<typename Type>
template<typename CastType>
inline const ActorPtr<CastType> ActorPtr<Type>::DynamicCast() const
{
	static_assert( std::is_const< CastType >::value, "You are casting constant object. Add const keyword in template parameter." );
	return ActorPtr< CastType >( rttr::rttr_cast< CastType* >( m_actorPointer ) );
}
