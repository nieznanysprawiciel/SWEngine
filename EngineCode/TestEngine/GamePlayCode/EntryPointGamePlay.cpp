
#include "../stdafx.h"
#include "EntryPointGamePlay.h"

EntryPointGamePlay::EntryPointGamePlay()
{
}


EntryPointGamePlay::~EntryPointGamePlay()
{
}


void EntryPointGamePlay::ProceedGameLogic( float time_interval )
{

}

/*Funkcja wywo�ywana w momencie wczytywania levelu.
Zako�czona poprawnie powinna zwr�ci� warto�� 0, w innych wypadkach inn�.*/
int EntryPointGamePlay::LoadLevel( )
{
	return 0;
}

/*Funkcja wywo�ywana w momencie zako�czenia levelu do usuni�cia niepotrzebnych obiekt�w.
Zako�czona poprawnie powinna zwr�ci� warto�� 0, w innych wypadkach inn�.*/
int EntryPointGamePlay::UnloadLevel( )
{
	return 0;
}
