#pragma once

#include "EngineCore/EventsManager/Event.h"
#include "EngineCore/EventsManager/Signal.h"

#include "Common/Multithreading/SpinLockedQueue.h"

class Engine;

/**@defgroup EventsSignals Eventy i Sygna�y
@ingroup EngineCore

@brief Wysy�anie event�w i sygna��w.

Klasa @ref EventManager pozwala na przesy�anie mi�dzy actorami i innymi obiektami
event�w i sygna��w.

@section Signals Sygna�y

Sygna�y s� komunikatami wysy�anymi bezpo�rednio mi�dzy dwoma aktorami. Dowolny aktor
mo�e wys�a� sygna� do innego aktora, kt�rego wska�nik posiada. Wys�anie sygna�u
oznacza po prostu wywo�anie odpowiedniej funkcji innego aktora, kt�ra zostanie wywo�ana z op�nienieniem
za po�rednictwem EventManagera.
Sygna��w nie trzeba nas�uchiwa�, ani nie jest konieczne wykonanie �adnych dodatkowych operacji.

@section Events Eventy

Eventy s� generowane przez niekt�re modu�y silnika, ale mog� by� te� wysy�ane przez obiekty u�ytkownika.
Po wygenerowaniu event jest przekazywany do EventManagera i obiekt, kt�ry go wygenerowa� nie ma wp�ywu
na to, kto go otrzyma.

Inni aktorzy mog� rejestrowa� funkcje obs�ugi konkretnych event�w.

*/



/**@brief Modu� do zarz�dzania eventami.
@ingroup EngineCore*/
class EventManager
{
private:
	Engine*								engine;

	SpinLockedQueue< Signal >			m_signals;
	SpinLockedQueue< Event* >			m_events;

protected:
public:
					EventManager	( Engine* engine );
					~EventManager	() = default;

	void			ProcessEvents	( float timeInterval );


	void			SendSignal		( const Signal& signal );
	void			SendEvent		( Event* evt );
};


