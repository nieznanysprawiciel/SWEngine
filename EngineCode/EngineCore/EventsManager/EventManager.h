#pragma once

#include "EngineCore/EventsManager/Event.h"
#include "EngineCore/EventsManager/Signal.h"

#include "Common/Multithreading/SpinLockedQueue.h"

class Engine;

/**@defgroup EventsSignals Eventy i Sygna³y
@ingroup EngineCore

@brief Wysy³anie eventów i sygna³ów.

Klasa @ref EventManager pozwala na przesy³anie miêdzy actorami i innymi obiektami
eventów i sygna³ów.

@section Signals Sygna³y

Sygna³y s¹ komunikatami wysy³anymi bezpoœrednio miêdzy dwoma aktorami. Dowolny aktor
mo¿e wys³aæ sygna³ do innego aktora, którego wskaŸnik posiada. Wys³anie sygna³u
oznacza po prostu wywo³anie odpowiedniej funkcji innego aktora, która zostanie wywo³ana z opóŸnienieniem
za poœrednictwem EventManagera.
Sygna³ów nie trzeba nas³uchiwaæ, ani nie jest konieczne wykonanie ¿adnych dodatkowych operacji.

@section Events Eventy

Eventy s¹ generowane przez niektóre modu³y silnika, ale mog¹ byæ te¿ wysy³ane przez obiekty u¿ytkownika.
Po wygenerowaniu event jest przekazywany do EventManagera i obiekt, który go wygenerowa³ nie ma wp³ywu
na to, kto go otrzyma.

Inni aktorzy mog¹ rejestrowaæ funkcje obs³ugi konkretnych eventów.

*/



/**@brief Modu³ do zarz¹dzania eventami.
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


