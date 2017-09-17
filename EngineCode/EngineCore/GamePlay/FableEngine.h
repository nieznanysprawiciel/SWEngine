#pragma once

#include "EngineCore/GamePlay/IGamePlay.h"
#include "EngineCore/MainEngine/Types.h"

#include <queue>



namespace sw
{

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
	FableEngine( Engine* engine );
	~FableEngine();

protected:		//u¿ytkownik nie mo¿e dotykaæ
	void				ProceedFable		( TimeType time, TimeDiff elapsed );
	void				ProceedEvents		( TimeType time, TimeDiff elapsed );

	inline void SetGamePlay( IGamePlay* gameplay ) { m_gamePlay = gameplay; }	//tylko dla Engine
};

}	// sw
