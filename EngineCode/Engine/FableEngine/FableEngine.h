#pragma once
#include "stdafx.h"
#include "Events\Event.h"
#include "Interfaces\GamePlay.h"


class Engine;



class FableEngine
{
	friend Engine;
private:
	Engine*						engine;
	GamePlay*					game_play;

	//kolejka eventów
	std::queue<Event*>			events_queue;

	//GamePlay - tablica obs³ugi Eventów
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

	inline void set_game_play( GamePlay* gameplay ) { game_play = gameplay; }	//tylko dla Engine
};

