#pragma once
/**@file EngineInterface.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Plik zawiera deklaracj� klasy EngineInterface.*/

#include "UserApi/Input/InputApi.h"
#include "UserApi/Rendering/RenderApi.h"
#include "UserApi/Actors/ActorsApi.h"
#include "UserApi/Assets/AssetsApi.h"

struct EngineContext;
class Engine;


/**
@defgroup UserAPI API dla u�ytkownika silnika.
*/



/**@brief Klasa jest interfejsem dla u�ytkownika u�ytkuj�cego silnik.
@ingroup EngineCore
@ingroup UserAPI

Taki interfejs b�dzie widzia� u�ytkownik silnika z wn�trza swoich klas odpowiedzialnych za fabu��.
U�ytkownik musi mie� ograniczone mo�liwo�ci, �eby przypadkiem nie namiesza�.

Funkcje s� pogrupowane tematycznie przy pomocy klas zagnie�d�onych.
Wszystkie funkcje jakie powinien mie� dost�pne nale�y deklarowa� w�asnie w klasach zagnie�d�onych.
Zasadniczo klasa EngineInterface nie powinna mie� opr�cz tego �adnych funkcji API.

Zmienne i sk�adniki silnika nale�y deklarowa� w strukturze EngineContext.
Dzi�ki temu b�dzie do nich dost�p z poziomu komend, poniewa�
ka�da komenda dostaje referencj� na t� struktur�.
@see EngineInterfaceCommands, Command

Funkcje umieszczone w sekcji prywatnej klasy EngineInterface b�d� mog�y by� wywo�ane z klas
zagnie�d�onych, ale nie b�dzie do nich dost�pu w komendach.
*/
class EngineInterface
{
protected:
	EngineContext*		Context;		///<Zawiera wska�niki na wszystkie modu�y i zmienne silnika.

protected:
	EngineInterface();
	~EngineInterface();


public:
	Api::AssetsApi			assets;		///<Funkcje do zarz�dzania assetami.
	Api::ActorsApi			actors;		///<Funkcje do zarz�dzania aktorami.
	Api::RenderingApi		rendering;	///<Funkcje do zarzadzania renderowaniem.
	Api::InputApi			input;		///<Funkcje do zarz�dzania wej�ciem od u�ytkownika.

};


