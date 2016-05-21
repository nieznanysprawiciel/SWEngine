#include "EngineCore/stdafx.h"
#include "EngineCore/EventsManager/EventManager.h"


/**@brief */
EventManager::EventManager( Engine* engine )
	: engine( engine )
{}

/**@brief G��wna funkcja do rozsy�ania event�w.*/
void			EventManager::ProcessEvents	( float timeInterval )
{
	// W funkcji obs�ugi sygna�u lub eventu kto� mo�e wys�a� kolejny event,
	// dlatego trzeba zrobi� tak� p�tl�.
	/// @todo Trzeba si� zabezpieczy� w jaki� spos�b przed zep�tleniem event�w przez u�ytkownika!!!
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
			// Zast�pi� alokatorem stosowym.
			delete evt;
		}

		// Sygna�y
		Signal signal;
		while( m_signals.TryPop( signal ) )
		{
			signal.SignalHandler( signal.Sender, signal.Parameters );
			if( !signal.OnExecuted.empty() )
				signal.OnExecuted( signal.Receiver, signal.Parameters );

			// Zast�pi� alokatorem stosowym.
			delete signal.Parameters;
		}
	}
}

/**@brief Wstawia sygna� do kolejki, aby oczekiwa� na wywo�anie.

Aby dowiedzie� si� o r�nicy mi�dzy eventami i sygna�ami przeczytaj @ref EventsSignals.*/
void			EventManager::SendSignal		( const Signal& signal )
{
	m_signals.Push( signal );
}

/**@brief Wstawia do kolejki event, kt�ry oczekuje na przetworzenie.

Aby dowiedzie� si� o r�nicy mi�dzy eventami i sygna�ami przeczytaj @ref EventsSignals.
*/
void			EventManager::SendEvent		( Event* evt )
{
	m_events.Push( evt );
}

/**@brief Dodaje funkcje obs�ugi do eventu podanego typu.

W trybie debug funkcja zatrzymuje si� na assercie, je�eli dany obiekt zarejestrowa� ju� swoj� funkcj�
obs�ugi do danego eventu. W trybie release taka funkcja zostanie zarejestrowana ponownie i wywo�ana
wielokrotnie.

@attention Funkcja nie synchronizuje dost�pu do tablicy handler�w.
Zak�adam, �e w silniku b�dzie moment w potoku przetwarzania, w kt�ym zadania b�d� wykonywane szeregowo 
zamiast wsp�bie�nie. Modyfikacje tablicy handler�w nale�y op�ni� do takiego momentu.

@param[in] handlerOwner Obiekt, kt�ry jest w�a�cicielem handlera. Potrzebne do zidentyfikowania
delegata w przypadku kasowania nas�uchiwacza oraz u�ywane do prawid�owego rozdzielania funkcji obs�ugi mi�dzy w�tki.*/
void			EventManager::AddListener	( rttr::type eventType, const EngineObject* handlerOwner, EventDelegate handler )
{
	EventListener listener;
	listener.EventHandler = handler;
	listener.Receiver = handlerOwner;
	AddListener( eventType, listener );
}

/**@brief Dodaje funkcje obs�ugi do eventu podanego typu.

W trybie debug funkcja zatrzymuje si� na assercie, je�eli dany obiekt zarejestrowa� ju� swoj� funkcj�
obs�ugi do danego eventu. W trybie release taka funkcja zostanie zarejestrowana ponownie i wywo�ana
wielokrotnie.

@attention Funkcja nie synchronizuje dost�pu do tablicy handler�w.
Zak�adam, �e w silniku b�dzie moment w potoku przetwarzania, w kt�ym zadania b�d� wykonywane szeregowo 
zamiast wsp�bie�nie. Modyfikacje tablicy handler�w nale�y op�ni� do takiego momentu.
*/
void			EventManager::AddListener	( rttr::type eventType, const EventListener& listener )
{
	assert( !CheckListener( eventType, listener.Receiver ) );

	auto & listenerVec = m_listeners[ eventType ];
	listenerVec.push_back( listener );
}

/**@brief Usuwa funkcj� obs�ugi eventu.

Je�eli jeden obiekt zarejestrowa� wiele funkcji obs�ugi tego samego eventu
(@see AddListener w trybie release) to zostanie skasowana tylko pierwsza w kolejno�ci.
Je�eli nie zostanie znaleziona funkcja obs�ugi, nie stanie si� nic z�ego.*/
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

/**@brief Usuwa wszystkie funkcje obs�ugi.*/
void			EventManager::ClearListeners	()
{
	m_listeners.clear();
}

/**@brief Sprawdza czy funkcja obs�ugi jest ju� zarejestrowana.*/
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
