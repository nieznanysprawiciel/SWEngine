#pragma once
/**
@file EventManager.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.
*/


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

#include "EngineCore/EventsManager/Signal.h"
#include "Common/Multithreading/SpinLockedQueue.h"

#include <vector>
#include <unordered_map>

class Engine;
class Event;





/**@brief Modu� do zarz�dzania eventami.
@ingroup EngineCore

Aby dowiedzie� si� o r�nicy mi�dzy eventami i sygna�ami przeczytaj @ref EventsSignals.

@todo Przerobi� EventManager, �eby by� w pe�ni wielow�tkowy.
Opr�cz synchronizacji powinien on rozdziela� otrzymane eventy na w�tki, tak �eby
jednocze�nie wykonywa�y si� tylko te zadania, kt�re nie b�d� powodowa�y b��d�w synchronizacji.

@todo Poniewa� eventy b�d� wysy�ane cz�sto, a z powodu wielow�tkowo�ci musz� zast�pi� komunikacj� mi�dzy Aktorami,
kt�ra normalnie by�aby bardzo wydajna, to trzeba zapewni� maksymaln� wydajno�� ca�ego mechanizmu.
Najwi�cej mo�na zrobi� u�ywaj�c jakiej� lepszej maszynki do zarz�dzania pami�ci�.
Kolejki m_signals i m_events mog�yby alokowa� swoje elementy przy pomocy alokatora Memory Pool.
Dla klas Event mo�na by zrobi� alokator stosowy (Stack Allocator), kt�ry by�by zwalniany po ka�dej klatce.
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


