#include "EngineCore/stdafx.h"
#include "EngineCore/EventsManager/EventManager.h"


EventManager::EventManager( Engine* engine )
	: engine( engine )
{}

/**@brief G��wna funkcja do rozsy�ania event�w.*/
void			EventManager::ProcessEvents	( float timeInterval )
{
	Signal signal;
	while( m_signals.TryPop( signal ) )
	{
		signal.Callback( signal.Parameters );
	}
}

/**@brief Wstawia sygna� do kolejki, aby oczekiwa� na wywo�anie.

Aby dowiedzie� si� o r�nicy mi�dzy eventami i sygna�ami przeczytaj @ref EventsSignals.*/
void			EventManager::SendSignal		( const Signal& signal )
{
	m_signals.Push( signal );
}

/**@brief Wstawia do kolejki event, kt�ry oczekuje na przetworzenie.

Aby dowiedzie� si� o r�nicy mi�dzy eventami i sygna�ami przeczytaj @ref EventsSignals.*/
void			EventManager::SendEvent		( Event* evt )
{
	m_events.Push( evt );
}
