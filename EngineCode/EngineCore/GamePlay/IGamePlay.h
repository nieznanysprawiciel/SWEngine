#pragma once
/**
@file IGamePlay.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.
*/

#include "EngineCore/MainEngine/EngineInterface.h"
#include "FastDelegate.h"


class IGamePlay;
class FableEngine;

//typedef void (*EventDelegate)(IGamePlay*, Event*);
typedef fastdelegate::FastDelegate1<Event*> EventDelegate;

/*Klasa jest interfejsem dla klasy GamePlaya, któr¹ powinien stworzyæ u¿ytkownik silnika.
Zadaniem klasy jest implementowanie funkcji obs³ugi eventów, startowanie i koñczenie levelów,
dodawanie, modyfikowanie i usuwanie obiektów, zmiana stanów silnika i wszystkie inne dzia³ania
podchodz¹ce pod gameplay.

Klasa komunikuje siê z silnikiem za pomoc¹ interfejsu EngineInterface.

W ka¿dym obiegu g³ównej pêtli silnika wywo³ywane s¹ funkcje dla poszczególnych modu³ów.
Obiekt IGamePlay nalezy do klasy FableEngine. Klasa ta najpierw wywo³uje funkcje obs³ugi dla 
poszczególnych eventów, które znajduj¹ siê w kolejce. Funkcja ProceedGameLogic() jest wywo³ywana
gdy wszystkie komunikaty zostan¹ juz obs³u¿one (je¿eli nie istnieje funkcja obs³ugi, zostaj¹ skasowane).
Nale¿y pamiêtaæ, ¿eby nie wykonywaæ zbyt wielu kosztownych czasowo operacji w tej funkcji, aby nie spowolniæ
dzia³ania silnika.

W momencie przetwarzania funkcji GamePlaya po³o¿enia wszystkich obiektów s¹ ju¿ policzone, podobnie jak kolizje,
oddzia³ywania fizyczne, zachowania AI oraz dane z wejœcia.*/

class IGamePlay
{
private:

protected:
	EngineInterface*				m_engine;
	FableEngine*					m_fableEngine;
public:
	IGamePlay() = default;
	virtual ~IGamePlay() = default;

	virtual void		SetEngineReference			( EngineInterface* game_engine, FableEngine* fable )
													{ m_engine = game_engine; m_fableEngine = fable; }

	virtual void		ProceedGameLogic			( float time ) = 0;
	virtual int			LoadLevel					() = 0;
	virtual int			UnloadLevel					() = 0;
};