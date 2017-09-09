#pragma once
/**@file EngineInterface.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Plik zawiera deklaracj� klasy EngineInterface.*/

#include "EngineCore/UserApi/Input/InputApi.h"
#include "EngineCore/UserApi/Rendering/RenderApi.h"
#include "EngineCore/UserApi/Actors/ActorsApi.h"
#include "EngineCore/UserApi/Assets/AssetsApi.h"
#include "EngineCore/UserApi/Level/LevelApi.h"



namespace sw
{


struct EngineContext;
class Engine;


/**
@defgroup UserAPI API for engine user (game creator).
*/



/**@brief This class is interface for game creator user.
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
	Api::AssetsApi			Assets;		///< Assets managing functions.
	Api::ActorsApi			Actors;		///< Actors managing functions.
	Api::RenderingApi		Rendering;	///< Rendering managing functions.
	Api::InputApi			Input;		///< User input managing functions.
	Api::LevelApi			Level;		///< Levels managing functions.

};


}	// sw
