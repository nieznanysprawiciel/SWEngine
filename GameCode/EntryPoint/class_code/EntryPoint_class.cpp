
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

/*Funkcja wywo�ywana w momencie wczytywania levelu.
Zako�czona poprawnie powinna zwr�ci� warto�� 0, w innych wypadkach inn�.*/
int EntryPoint::load_level( )
{
	return 0;
}

/*Funkcja wywo�ywana w momencie zako�czenia levelu do usuni�cia niepotrzebnych obiekt�w.
Zako�czona poprawnie powinna zwr�ci� warto�� 0, w innych wypadkach inn�.*/
int EntryPoint::unload_level( )
{
	return 0;
}
