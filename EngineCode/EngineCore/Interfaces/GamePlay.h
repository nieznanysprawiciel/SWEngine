#pragma once
/**
@file GamePlay.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.
*/

#include "EngineInterface.h"


class GamePlay;
class FableEngine;

typedef void (*EventDelegate)(GamePlay*, Event*);

/*Klasa jest interfejsem dla klasy GamePlaya, któr¹ powinien stworzyæ u¿ytkownik silnika.
 *Zadaniem klasy jest implementowanie funkcji obs³ugi eventów, startowanie i koñczenie levelów,
 *dodawanie, modyfikowanie i usuwanie obiektów, zmiana stanów silnika i wszystkie inne dzia³ania
 *podchodz¹ce pod gameplay.
 *
 *Klasa komunikuje siê z silnikiem za pomoc¹ interfejsu EngineInterface.
 *
 *W ka¿dym obiegu g³ównej pêtli silnika wywo³ywane s¹ funkcje dla poszczególnych modu³ów.
 *Obiekt GamePlay nalezy do klasy FableEngine. Klasa ta najpierw wywo³uje funkcje obs³ugi dla 
 *poszczególnych eventów, które znajduj¹ siê w kolejce. Funkcja proceed_game_play() jest wywo³ywana
 *gdy wszystkie komunikaty zostan¹ juz obs³u¿one (je¿eli nie istnieje funkcja obs³ugi, zostaj¹ skasowane).
 *Nale¿y pamiêtaæ, ¿eby nie wykonywaæ zbyt wielu kosztownych czasowo operacji w tej funkcji, aby nie spowolniæ
 *dzia³ania silnika.
 *
 *W momencie przetwarzania funkcji GamePlaya po³o¿enia wszystkich obiektów s¹ ju¿ policzone, podobnie jak kolizje,
 *oddzia³ywania fizyczne, zachowania AI oraz dane z wejœcia.*/

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