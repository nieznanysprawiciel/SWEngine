#pragma once

#include "EngineCore/GamePlay/IGamePlay.h"

#include <queue>


class Engine;
class Event;


/**@brief Modu� do zarz�dzania fabu��.
@ingroup EngineCore*/
class FableEngine
{
	friend Engine;
private:
	Engine*						m_engine;
	IGamePlay*					m_gamePlay;

	//kolejka event�w
	std::queue<Event*>			m_eventsQueue;

	//IGamePlay - tablica obs�ugi Event�w
	std::vector<EventDelegateTmp>	m_eventDelegates;
	unsigned int				m_delegatesCount;

public:
	FableEngine(Engine* engine);
	~FableEngine();

	unsigned int		RegisterEvent		( EventDelegateTmp eventDelegate );
	void				ChangeDelegate		( unsigned int id, EventDelegateTmp eventDelegate );
	void				DeleteDelegate		( unsigned int id );


protected:		//u�ytkownik nie mo�e dotyka�
	void				ProceedFable		( float time_interval );
	void				ProceedEvents		( float time_interval );

	inline void SetGamePlay( IGamePlay* gameplay ) { m_gamePlay = gameplay; }	//tylko dla Engine
};

