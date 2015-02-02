#include "..\..\stdafx.h"
#include "FableEngine.h"
#include "..\Engine.h"


#include "..\..\memory_leaks.h"

FableEngine::FableEngine(Engine* engine)
	: engine(engine)
{
	//klasa engine musi mieæ latwy dostêp do naszej kolejki, wiêc mu j¹ przypisujemy
	engine->events_queue = &events_queue;

	game_play = nullptr;

	//wype³niamy tablicê delegatów nullptrami
	for (unsigned int i = 0; i < BUILD_IN_EVENTS; ++i)
		event_delegates.push_back(nullptr);

	delegates_count = BUILD_IN_EVENTS;
}


FableEngine::~FableEngine()
{
	//nasza klasa jest odpowiedzialna za zwalnianie eventów
	while (!events_queue.empty())
	{
		Event* cur_event = events_queue.front();
		delete cur_event;
	}

	if ( game_play )
		delete game_play;
}


void FableEngine::proceed_fable(float time_interval)
{
	proceed_events( time_interval );
	//na koñcu po wykonaniu obs³ugi wszystkich eventów
	//wywo³ujemy funkcjê g³ówn¹ GamePlaya
	if ( game_play != nullptr )
		game_play->proceed_game_play(time_interval);
}

void FableEngine::proceed_events( float time_interval )
{
	while ( !events_queue.empty() )
	{
		//pobieramy z kolejki kolejny event i wywo³ujemy dla niego funkcjê obs³ugi
		Event* current_event = events_queue.front();
		if ( current_event->type < delegates_count )		//ktoœ móg³ wpisaæ niezarejestrowany Event
			if ( event_delegates[current_event->type] != nullptr )	//mo¿e nie byæ delegata
				event_delegates[current_event->type]( game_play, current_event );
		//FableEngine jest odpowiedzialna za kasowanie
		delete current_event;
	}
}

/*Funkcja s³u¿y do dodania funkcji obs³ugi dla nowego eventu stworzonego przez u¿ytkownika.
 *Wartoœci¹ zwracan¹ jest identyfikator, który nale¿y wpisywaæ w polu type klasy Event.
 *Identyfikator jest nastêpn¹ w kolejce liczb¹ po ostatnim zarejestrowanym Evencie.*/
unsigned int FableEngine::register_event(EventDelegate event_delegate)
{
	event_delegates.push_back(event_delegate);
	++delegates_count;
	return (delegates_count - 1);
}

/*Zmieniamy delegate pod indeksem id. Je¿eli delegat o takim indeksie nie istnieje nic z³ego siê
 *nie stanie, ale funkcja nie poinformuje w ¿aden sposób o b³êdzie.*/
void FableEngine::change_delegate(unsigned int id, EventDelegate event_delegate)
{
	if (id < delegates_count)
		return;		//nie istnieje event
	event_delegates[id] = event_delegate;
}

/*Zamieniamy wskaŸnik funkcji pod adresem id na nullptr.*/
void FableEngine::delete_delegate(unsigned int id)
{
	if (id < delegates_count)
		return;		//nie istnieje event
	event_delegates[id] = nullptr;
}