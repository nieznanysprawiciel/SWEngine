/**
@file FableEngine.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "EngineCore/stdafx.h"
#include "FableEngine.h"
#include "EngineCore/MainEngine/Engine.h"


#include "swCommonLib/Common/MemoryLeaks.h"



namespace sw
{


 // ================================ //
//
FableEngine::FableEngine( Engine* engine )
	: m_engine( engine )
{
	m_gamePlay = nullptr;
}

// ================================ //
//
FableEngine::~FableEngine()
{
	if( m_gamePlay )
		delete m_gamePlay;
}

// ================================ //
//
void FableEngine::ProceedFable( float timeInterval )
{
	ProceedEvents( timeInterval );
	//na ko�cu po wykonaniu obs�ugi wszystkich event�w
	//wywo�ujemy funkcj� g��wn� GamePlaya
	if( m_gamePlay != nullptr )
		m_gamePlay->ProceedGameLogic( timeInterval );
}


// ================================ //
//
void FableEngine::ProceedEvents( float timeInterval )
{}

}	// sw


