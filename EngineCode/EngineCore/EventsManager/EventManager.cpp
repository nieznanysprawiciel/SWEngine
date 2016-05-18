#include "EngineCore/stdafx.h"
#include "EngineCore/EventsManager/EventManager.h"


EventManager::EventManager( Engine* engine )
	: engine( engine )
{}

/**@brief G³ówna funkcja do rozsy³ania eventów.*/
void			EventManager::ProcessEvents	( float timeInterval )
{
	Signal signal;
	while( m_signals.TryPop( signal ) )
	{
		signal.Callback( signal.Parameters );
	}
}

/**@brief Wstawia sygna³ do kolejki, aby oczekiwa³ na wywo³anie.

Aby dowiedzieæ siê o ró¿nicy miêdzy eventami i sygna³ami przeczytaj @ref EventsSignals.*/
void			EventManager::SendSignal		( const Signal& signal )
{
	m_signals.Push( signal );
}

/**@brief Wstawia do kolejki event, który oczekuje na przetworzenie.

Aby dowiedzieæ siê o ró¿nicy miêdzy eventami i sygna³ami przeczytaj @ref EventsSignals.*/
void			EventManager::SendEvent		( Event* evt )
{
	m_events.Push( evt );
}
