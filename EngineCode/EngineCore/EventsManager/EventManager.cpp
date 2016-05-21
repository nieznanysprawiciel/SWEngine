#include "EngineCore/stdafx.h"
#include "EngineCore/EventsManager/EventManager.h"


/**@brief */
EventManager::EventManager( Engine* engine )
	: engine( engine )
{}

/**@brief G³ówna funkcja do rozsy³ania eventów.*/
void			EventManager::ProcessEvents	( float timeInterval )
{
	// W funkcji obs³ugi sygna³u lub eventu ktoœ mo¿e wys³aæ kolejny event,
	// dlatego trzeba zrobiæ tak¹ pêtlê.
	/// @todo Trzeba siê zabezpieczyæ w jakiœ sposób przed zepêtleniem eventów przez u¿ytkownika!!!
	while( !m_events.IsEmpty() || !m_signals.IsEmpty() )
	{
		// Eventy
		Event* evt;
		while( m_events.TryPop( evt ) )
		{
			rttr::type eventType = rttr::type::get( *evt );
			auto& listenersVec = m_listeners.find( eventType );
			if( listenersVec != m_listeners.end() )
			{
				for( auto & listener : listenersVec->second )
				{
					listener.EventHandler( listener.Receiver, evt );
				}
			}
			// Zast¹piæ alokatorem stosowym.
			delete evt;
		}

		// Sygna³y
		Signal signal;
		while( m_signals.TryPop( signal ) )
		{
			signal.SignalHandler( signal.Sender, signal.Parameters );
			if( !signal.OnExecuted.empty() )
				signal.OnExecuted( signal.Receiver, signal.Parameters );

			// Zast¹piæ alokatorem stosowym.
			delete signal.Parameters;
		}
	}
}

/**@brief Wstawia sygna³ do kolejki, aby oczekiwa³ na wywo³anie.

Aby dowiedzieæ siê o ró¿nicy miêdzy eventami i sygna³ami przeczytaj @ref EventsSignals.*/
void			EventManager::SendSignal		( const Signal& signal )
{
	m_signals.Push( signal );
}

/**@brief Wstawia do kolejki event, który oczekuje na przetworzenie.

Aby dowiedzieæ siê o ró¿nicy miêdzy eventami i sygna³ami przeczytaj @ref EventsSignals.
*/
void			EventManager::SendEvent		( Event* evt )
{
	m_events.Push( evt );
}

/**@brief Dodaje funkcje obs³ugi do eventu podanego typu.

W trybie debug funkcja zatrzymuje siê na assercie, je¿eli dany obiekt zarejestrowa³ ju¿ swoj¹ funkcjê
obs³ugi do danego eventu. W trybie release taka funkcja zostanie zarejestrowana ponownie i wywo³ana
wielokrotnie.

@attention Funkcja nie synchronizuje dostêpu do tablicy handlerów.
Zak³adam, ¿e w silniku bêdzie moment w potoku przetwarzania, w któym zadania bêd¹ wykonywane szeregowo 
zamiast wspó³bie¿nie. Modyfikacje tablicy handlerów nale¿y opóŸniæ do takiego momentu.

@param[in] handlerOwner Obiekt, który jest w³aœcicielem handlera. Potrzebne do zidentyfikowania
delegata w przypadku kasowania nas³uchiwacza oraz u¿ywane do prawid³owego rozdzielania funkcji obs³ugi miêdzy w¹tki.*/
void			EventManager::AddListener	( rttr::type eventType, const EngineObject* handlerOwner, EventDelegate handler )
{
	EventListener listener;
	listener.EventHandler = handler;
	listener.Receiver = handlerOwner;
	AddListener( eventType, listener );
}

/**@brief Dodaje funkcje obs³ugi do eventu podanego typu.

W trybie debug funkcja zatrzymuje siê na assercie, je¿eli dany obiekt zarejestrowa³ ju¿ swoj¹ funkcjê
obs³ugi do danego eventu. W trybie release taka funkcja zostanie zarejestrowana ponownie i wywo³ana
wielokrotnie.

@attention Funkcja nie synchronizuje dostêpu do tablicy handlerów.
Zak³adam, ¿e w silniku bêdzie moment w potoku przetwarzania, w któym zadania bêd¹ wykonywane szeregowo 
zamiast wspó³bie¿nie. Modyfikacje tablicy handlerów nale¿y opóŸniæ do takiego momentu.
*/
void			EventManager::AddListener	( rttr::type eventType, const EventListener& listener )
{
	assert( !CheckListener( eventType, listener.Receiver ) );

	auto & listenerVec = m_listeners[ eventType ];
	listenerVec.push_back( listener );
}

/**@brief Usuwa funkcjê obs³ugi eventu.

Je¿eli jeden obiekt zarejestrowa³ wiele funkcji obs³ugi tego samego eventu
(@see AddListener w trybie release) to zostanie skasowana tylko pierwsza w kolejnoœci.
Je¿eli nie zostanie znaleziona funkcja obs³ugi, nie stanie siê nic z³ego.*/
void			EventManager::RemoveListener( rttr::type eventType, const EngineObject* handlerOwner )
{
	auto & listenerVec = m_listeners[ eventType ];
	for( auto iter = listenerVec.begin(); iter != listenerVec.end(); ++iter )
	{
		if( iter->Receiver == handlerOwner )
		{
			iter = listenerVec.erase( iter );
			break;
		}
	}

	if( listenerVec.empty() )
		m_listeners.erase( eventType );
}

/**@brief Usuwa wszystkie funkcje obs³ugi.*/
void			EventManager::ClearListeners	()
{
	m_listeners.clear();
}

/**@brief Sprawdza czy funkcja obs³ugi jest ju¿ zarejestrowana.*/
bool			EventManager::CheckListener	( rttr::type eventType, const EngineObject* handlerOwner )
{
	auto& listenerVecIter = m_listeners.find( eventType );
	if( listenerVecIter != m_listeners.end() )
	{
		for( auto& listener : listenerVecIter->second )
		{
			if( listener.Receiver == handlerOwner )
				return true;
		}
	}

	return false;
}
