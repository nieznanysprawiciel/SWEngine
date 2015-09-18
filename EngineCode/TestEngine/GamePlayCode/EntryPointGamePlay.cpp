
#include "../stdafx.h"
#include "EntryPointGamePlay.h"

EntryPointGamePlay::EntryPointGamePlay()
{
}


EntryPointGamePlay::~EntryPointGamePlay()
{
}


void EntryPointGamePlay::proceed_game_play( float time_interval )
{

}

/*Funkcja wywo³ywana w momencie wczytywania levelu.
Zakoñczona poprawnie powinna zwróciæ wartoœæ 0, w innych wypadkach inn¹.*/
int EntryPointGamePlay::load_level( )
{
	return 0;
}

/*Funkcja wywo³ywana w momencie zakoñczenia levelu do usuniêcia niepotrzebnych obiektów.
Zakoñczona poprawnie powinna zwróciæ wartoœæ 0, w innych wypadkach inn¹.*/
int EntryPointGamePlay::unload_level( )
{
	return 0;
}
