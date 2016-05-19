#pragma once
/**
@file EventManager.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.
*/


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

#include "EngineCore/EventsManager/Signal.h"
#include "Common/Multithreading/SpinLockedQueue.h"

#include <vector>
#include <unordered_map>

class Engine;
class Event;





/**@brief Modu³ do zarz¹dzania eventami.
@ingroup EngineCore

Aby dowiedzieæ siê o ró¿nicy miêdzy eventami i sygna³ami przeczytaj @ref EventsSignals.

@todo Przerobiæ EventManager, ¿eby by³ w pe³ni wielow¹tkowy.
Oprócz synchronizacji powinien on rozdzielaæ otrzymane eventy na w¹tki, tak ¿eby
jednoczeœnie wykonywa³y siê tylko te zadania, które nie bêd¹ powodowa³y b³êdów synchronizacji.

@todo Poniewa¿ eventy bêd¹ wysy³ane czêsto, a z powodu wielow¹tkowoœci musz¹ zast¹piæ komunikacjê miêdzy Aktorami,
która normalnie by³aby bardzo wydajna, to trzeba zapewniæ maksymaln¹ wydajnoœæ ca³ego mechanizmu.
Najwiêcej mo¿na zrobiæ u¿ywaj¹c jakiejœ lepszej maszynki do zarz¹dzania pamiêci¹.
Kolejki m_signals i m_events mog³yby alokowaæ swoje elementy przy pomocy alokatora Memory Pool.
Dla klas Event mo¿na by zrobiæ alokator stosowy (Stack Allocator), który by³by zwalniany po ka¿dej klatce.
*/
class EventManager
{
	typedef std::vector< EventListener >						EventListenerVec;
	typedef std::unordered_map< rttr::type, EventListenerVec >	ListenerHashMap;

private:
	Engine*								engine;

	SpinLockedQueue< Signal >			m_signals;
	SpinLockedQueue< Event* >			m_events;

	ListenerHashMap						m_listeners;

protected:
public:
					EventManager	( Engine* engine );
					~EventManager	() = default;

	void			ProcessEvents	( float timeInterval );


	void			SendSignal		( const Signal& signal );
	void			SendEvent		( Event* evt );

	void			AddListener		( rttr::type eventType, const EngineObject* handlerOwner, EventDelegate handler );
	void			AddListener		( rttr::type eventType, const EventListener& listener );
	void			RemoveListener	( rttr::type eventType, const EngineObject* handlerOwner );
	void			ClearListeners	();

	bool			CheckListener	( rttr::type eventType, const EngineObject* handlerOwner );
};


