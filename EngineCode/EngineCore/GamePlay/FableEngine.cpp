#include "EngineCore/stdafx.h"
#include "FableEngine.h"
#include "EngineCore/MainEngine/Engine.h"


#include "Common/MemoryLeaks.h"

FableEngine::FableEngine( Engine* engine )
	: m_engine( engine )
{
	//klasa m_engine musi mieæ ³atwy dostêp do naszej kolejki, wiêc mu j¹ przypisujemy
	engine->SetEventsQueue( &m_eventsQueue );

	m_gamePlay = nullptr;

	//wype³niamy tablicê delegatów nullptrami
	for( unsigned int i = 0; i < BUILD_IN_EVENTS; ++i )
		m_eventDelegates.push_back( nullptr );

	m_delegatesCount = BUILD_IN_EVENTS;
}


FableEngine::~FableEngine()
{
	//nasza klasa jest odpowiedzialna za zwalnianie eventów
	while( !m_eventsQueue.empty() )
	{
		Event* cur_event = m_eventsQueue.front();
		delete cur_event;
	}

	if( m_gamePlay )
		delete m_gamePlay;
}


void FableEngine::ProceedFable( float timeInterval )
{
	ProceedEvents( timeInterval );
	//na koñcu po wykonaniu obs³ugi wszystkich eventów
	//wywo³ujemy funkcjê g³ówn¹ GamePlaya
	if( m_gamePlay != nullptr )
		m_gamePlay->ProceedGameLogic( timeInterval );
}

void FableEngine::ProceedEvents( float timeInterval )
{
	while( !m_eventsQueue.empty() )
	{
		//pobieramy z kolejki kolejny event i wywo³ujemy dla niego funkcjê obs³ugi
		Event* current_event = m_eventsQueue.front();
		m_eventsQueue.pop();
		if( current_event->type < m_delegatesCount )		//ktoœ móg³ wpisaæ niezarejestrowany Event
			if( m_eventDelegates[ current_event->type ] != nullptr )	//mo¿e nie byæ delegata
				m_eventDelegates[ current_event->type ]( current_event );
		//FableEngine jest odpowiedzialna za kasowanie
		delete current_event;
	}
}

/**Funkcja s³u¿y do dodania funkcji obs³ugi dla nowego eventu stworzonego przez u¿ytkownika.
Wartoœci¹ zwracan¹ jest identyfikator, który nale¿y wpisywaæ w polu type klasy Event.
Identyfikator jest nastêpn¹ w kolejce liczb¹ po ostatnim zarejestrowanym Evencie.

@param[in] eventDelegate Delegat, który zostanie wywo³any w przypadku pojawienia siê eventu.*/
unsigned int FableEngine::RegisterEvent( EventDelegateTmp eventDelegate )
{
	m_eventDelegates.push_back( eventDelegate );
	++m_delegatesCount;
	return ( m_delegatesCount - 1 );
}

/**Zmieniamy delegate pod indeksem id. Je¿eli delegat o takim indeksie nie istnieje nic z³ego siê
nie stanie, ale funkcja nie poinformuje w ¿aden sposób o b³êdzie.*/
void FableEngine::ChangeDelegate( unsigned int id, EventDelegateTmp eventDelegate )
{
	if( id >= m_delegatesCount )
		return;		//nie istnieje event
	m_eventDelegates[ id ] = eventDelegate;
}

/**Zamieniamy wskaŸnik funkcji pod adresem id na nullptr.*/
void FableEngine::DeleteDelegate( unsigned int id )
{
	if( id < m_delegatesCount )
		return;		//nie istnieje event
	m_eventDelegates[ id ] = nullptr;
}