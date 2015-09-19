#pragma once
#include "EngineCore/stdafx.h"
#include "EngineCore/EventsManager/Event.h"
#include "EngineCore/GamePlay/IGamePlay.h"


class Engine;



class FableEngine
{
	friend Engine;
private:
	Engine*						engine;
	IGamePlay*					game_play;

	//kolejka eventów
	std::queue<Event*>			events_queue;

	//IGamePlay - tablica obs³ugi Eventów
	std::vector<EventDelegate>	event_delegates;
	unsigned int				delegates_count;

public:
	FableEngine(Engine* engine);
	~FableEngine();

	unsigned int register_event(EventDelegate event_delegate);
	void change_delegate(unsigned int id, EventDelegate event_delegate);
	void delete_delegate(unsigned int id);


protected:		//u¿ytkownik nie mo¿e dotykaæ
	void proceed_fable(float time_interval);
	void proceed_events( float time_interval );

	inline void set_game_play( IGamePlay* gameplay ) { game_play = gameplay; }	//tylko dla Engine
};

