#pragma once

#include "EngineCore/GamePlay/IGamePlay.h"

#include <queue>


class Engine;
class Event;


/**@brief Modu³ do zarz¹dzania fabu³¹.
@ingroup EngineCore*/
class FableEngine
{
	friend Engine;
private:
	Engine*						m_engine;
	IGamePlay*					m_gamePlay;

public:
	FableEngine(Engine* engine);
	~FableEngine();

protected:		//u¿ytkownik nie mo¿e dotykaæ
	void				ProceedFable		( float time_interval );
	void				ProceedEvents		( float time_interval );

	inline void SetGamePlay( IGamePlay* gameplay ) { m_gamePlay = gameplay; }	//tylko dla Engine
};

