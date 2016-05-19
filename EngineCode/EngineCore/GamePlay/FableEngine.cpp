#include "EngineCore/stdafx.h"
#include "FableEngine.h"
#include "EngineCore/MainEngine/Engine.h"


#include "Common/MemoryLeaks.h"

FableEngine::FableEngine( Engine* engine )
	: m_engine( engine )
{
	//klasa m_engine musi mie� �atwy dost�p do naszej kolejki, wi�c mu j� przypisujemy
	engine->SetEventsQueue( &m_eventsQueue );

	m_gamePlay = nullptr;

	//wype�niamy tablic� delegat�w nullptrami
	for( unsigned int i = 0; i < BUILD_IN_EVENTS; ++i )
		m_eventDelegates.push_back( nullptr );

	m_delegatesCount = BUILD_IN_EVENTS;
}


FableEngine::~FableEngine()
{
	//nasza klasa jest odpowiedzialna za zwalnianie event�w
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
	//na ko�cu po wykonaniu obs�ugi wszystkich event�w
	//wywo�ujemy funkcj� g��wn� GamePlaya
	if( m_gamePlay != nullptr )
		m_gamePlay->ProceedGameLogic( timeInterval );
}

void FableEngine::ProceedEvents( float timeInterval )
{
	while( !m_eventsQueue.empty() )
	{
		//pobieramy z kolejki kolejny event i wywo�ujemy dla niego funkcj� obs�ugi
		Event* current_event = m_eventsQueue.front();
		m_eventsQueue.pop();
		if( current_event->type < m_delegatesCount )		//kto� m�g� wpisa� niezarejestrowany Event
			if( m_eventDelegates[ current_event->type ] != nullptr )	//mo�e nie by� delegata
				m_eventDelegates[ current_event->type ]( current_event );
		//FableEngine jest odpowiedzialna za kasowanie
		delete current_event;
	}
}

/**Funkcja s�u�y do dodania funkcji obs�ugi dla nowego eventu stworzonego przez u�ytkownika.
Warto�ci� zwracan� jest identyfikator, kt�ry nale�y wpisywa� w polu type klasy Event.
Identyfikator jest nast�pn� w kolejce liczb� po ostatnim zarejestrowanym Evencie.

@param[in] eventDelegate Delegat, kt�ry zostanie wywo�any w przypadku pojawienia si� eventu.*/
unsigned int FableEngine::RegisterEvent( EventDelegateTmp eventDelegate )
{
	m_eventDelegates.push_back( eventDelegate );
	++m_delegatesCount;
	return ( m_delegatesCount - 1 );
}

/**Zmieniamy delegate pod indeksem id. Je�eli delegat o takim indeksie nie istnieje nic z�ego si�
nie stanie, ale funkcja nie poinformuje w �aden spos�b o b��dzie.*/
void FableEngine::ChangeDelegate( unsigned int id, EventDelegateTmp eventDelegate )
{
	if( id >= m_delegatesCount )
		return;		//nie istnieje event
	m_eventDelegates[ id ] = eventDelegate;
}

/**Zamieniamy wska�nik funkcji pod adresem id na nullptr.*/
void FableEngine::DeleteDelegate( unsigned int id )
{
	if( id < m_delegatesCount )
		return;		//nie istnieje event
	m_eventDelegates[ id ] = nullptr;
}