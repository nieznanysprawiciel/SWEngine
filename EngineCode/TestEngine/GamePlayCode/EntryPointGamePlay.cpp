#include "TestEngine/stdafx.h"
#include "EntryPointGamePlay.h"

#include "Common/SWEngineInclude.h"
#include "PlayerSignalTest.h"


EntryPointGamePlay::EntryPointGamePlay()
{
}


EntryPointGamePlay::~EntryPointGamePlay()
{
}


void EntryPointGamePlay::ProceedGameLogic( float time_interval )
{

}

/*Funkcja wywo³ywana w momencie wczytywania levelu.
Zakoñczona poprawnie powinna zwróciæ wartoœæ 0, w innych wypadkach inn¹.*/
int EntryPointGamePlay::LoadLevel( )
{
	const wchar_t CLONE_FIGHTER[] = L"tylko_do_testow/ARC.FBX";
	m_engine->Assets.Models.LoadSync( CLONE_FIGHTER );
	
	m_engine->Actors.RegisterClass< PlayerSignalTest >( GetTypeidName< PlayerSignalTest >(), PlayerSignalTest::Create );

	auto actor1 = m_engine->Actors.CreateActor< PlayerSignalTest >( GetTypeidName< PlayerSignalTest >(), EnableDisplay );
	auto actor2 = m_engine->Actors.CreateActor< PlayerSignalTest >( GetTypeidName< PlayerSignalTest >(), EnableDisplay );

	actor1->SetOtherPlayer( actor2 );
	actor2->SetOtherPlayer( actor1 );


	DirectX::XMVECTOR position1 = DirectX::XMVectorSet( 4000.0, 0.0, 8000.0, 0.0 );
	actor1->Teleport( position1 );
	actor1->SetModel( m_engine->Assets.Models.GetSync( CLONE_FIGHTER ) );

	DirectX::XMVECTOR position2 = DirectX::XMVectorSet( 8000.0f, 0.0, 4000.0, 0.0 );
	actor2->Teleport( position2 );
	actor2->SetModel( m_engine->Assets.Models.GetSync( CLONE_FIGHTER ) );

	return 0;
}

/*Funkcja wywo³ywana w momencie zakoñczenia levelu do usuniêcia niepotrzebnych obiektów.
Zakoñczona poprawnie powinna zwróciæ wartoœæ 0, w innych wypadkach inn¹.*/
int EntryPointGamePlay::UnloadLevel( )
{



	return 0;
}
