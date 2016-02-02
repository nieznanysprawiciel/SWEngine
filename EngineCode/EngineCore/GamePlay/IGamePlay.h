#pragma once
/**
@file IGamePlay.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.
*/

#include "EngineCore/MainEngine/EngineInterface.h"
#include "FastDelegate.h"


class IGamePlay;
class FableEngine;

//typedef void (*EventDelegate)(IGamePlay*, Event*);
typedef fastdelegate::FastDelegate1<Event*> EventDelegate;

/*Klasa jest interfejsem dla klasy GamePlaya, kt�r� powinien stworzy� u�ytkownik silnika.
Zadaniem klasy jest implementowanie funkcji obs�ugi event�w, startowanie i ko�czenie level�w,
dodawanie, modyfikowanie i usuwanie obiekt�w, zmiana stan�w silnika i wszystkie inne dzia�ania
podchodz�ce pod gameplay.

Klasa komunikuje si� z silnikiem za pomoc� interfejsu EngineInterface.

W ka�dym obiegu g��wnej p�tli silnika wywo�ywane s� funkcje dla poszczeg�lnych modu��w.
Obiekt IGamePlay nalezy do klasy FableEngine. Klasa ta najpierw wywo�uje funkcje obs�ugi dla 
poszczeg�lnych event�w, kt�re znajduj� si� w kolejce. Funkcja ProceedGameLogic() jest wywo�ywana
gdy wszystkie komunikaty zostan� juz obs�u�one (je�eli nie istnieje funkcja obs�ugi, zostaj� skasowane).
Nale�y pami�ta�, �eby nie wykonywa� zbyt wielu kosztownych czasowo operacji w tej funkcji, aby nie spowolni�
dzia�ania silnika.

W momencie przetwarzania funkcji GamePlaya po�o�enia wszystkich obiekt�w s� ju� policzone, podobnie jak kolizje,
oddzia�ywania fizyczne, zachowania AI oraz dane z wej�cia.*/

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