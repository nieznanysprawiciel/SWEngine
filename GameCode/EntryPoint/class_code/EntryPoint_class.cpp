
#include "..\stdafx.h"
#include "EntryPoint_class.h"

EntryPoint::EntryPoint()
{
}


EntryPoint::~EntryPoint()
{
}


void EntryPoint::proceed_game_play( float time_interval )
{

}

/*Funkcja wywo³ywana w momencie wczytywania levelu.
Zakoñczona poprawnie powinna zwróciæ wartoœæ 0, w innych wypadkach inn¹.*/
int EntryPoint::load_level( )
{
	return 0;
}

/*Funkcja wywo³ywana w momencie zakoñczenia levelu do usuniêcia niepotrzebnych obiektów.
Zakoñczona poprawnie powinna zwróciæ wartoœæ 0, w innych wypadkach inn¹.*/
int EntryPoint::unload_level( )
{
	return 0;
}
