#pragma once
/**
@file GamePlay.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.
*/

#include "EngineInterface.h"


class GamePlay;
class FableEngine;

typedef void (*EventDelegate)(GamePlay*, Event*);

/*Klasa jest interfejsem dla klasy GamePlaya, kt�r� powinien stworzy� u�ytkownik silnika.
 *Zadaniem klasy jest implementowanie funkcji obs�ugi event�w, startowanie i ko�czenie level�w,
 *dodawanie, modyfikowanie i usuwanie obiekt�w, zmiana stan�w silnika i wszystkie inne dzia�ania
 *podchodz�ce pod gameplay.
 *
 *Klasa komunikuje si� z silnikiem za pomoc� interfejsu EngineInterface.
 *
 *W ka�dym obiegu g��wnej p�tli silnika wywo�ywane s� funkcje dla poszczeg�lnych modu��w.
 *Obiekt GamePlay nalezy do klasy FableEngine. Klasa ta najpierw wywo�uje funkcje obs�ugi dla 
 *poszczeg�lnych event�w, kt�re znajduj� si� w kolejce. Funkcja proceed_game_play() jest wywo�ywana
 *gdy wszystkie komunikaty zostan� juz obs�u�one (je�eli nie istnieje funkcja obs�ugi, zostaj� skasowane).
 *Nale�y pami�ta�, �eby nie wykonywa� zbyt wielu kosztownych czasowo operacji w tej funkcji, aby nie spowolni�
 *dzia�ania silnika.
 *
 *W momencie przetwarzania funkcji GamePlaya po�o�enia wszystkich obiekt�w s� ju� policzone, podobnie jak kolizje,
 *oddzia�ywania fizyczne, zachowania AI oraz dane z wej�cia.*/

class GamePlay
{
private:

protected:
	EngineInterface*				engine;
	FableEngine*					fable_engine;
public:
	GamePlay() = default;
	virtual ~GamePlay() = default;

	virtual void set_engine_and_fable( EngineInterface* game_engine, FableEngine* fable ) { engine = game_engine; fable_engine = fable; }

	virtual void proceed_game_play(float time_interval) = 0;
	virtual int load_level() = 0;
	virtual int unload_level() = 0;
};