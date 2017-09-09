#pragma once
/**@file EngineInterface.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Plik zawiera deklaracjê klasy EngineInterface.*/

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

Taki interfejs bêdzie widzia³ u¿ytkownik silnika z wnêtrza swoich klas odpowiedzialnych za fabu³ê.
U¿ytkownik musi mieæ ograniczone mo¿liwoœci, ¿eby przypadkiem nie namiesza³.

Funkcje s¹ pogrupowane tematycznie przy pomocy klas zagnie¿d¿onych.
Wszystkie funkcje jakie powinien mieæ dostêpne nale¿y deklarowaæ w³asnie w klasach zagnie¿d¿onych.
Zasadniczo klasa EngineInterface nie powinna mieæ oprócz tego ¿adnych funkcji API.

Zmienne i sk³adniki silnika nale¿y deklarowaæ w strukturze EngineContext.
Dziêki temu bêdzie do nich dostêp z poziomu komend, poniewa¿
ka¿da komenda dostaje referencjê na tê strukturê.
@see EngineInterfaceCommands, Command

Funkcje umieszczone w sekcji prywatnej klasy EngineInterface bêd¹ mog³y byæ wywo³ane z klas
zagnie¿d¿onych, ale nie bêdzie do nich dostêpu w komendach.
*/
class EngineInterface
{
protected:
	EngineContext*		Context;		///<Zawiera wskaŸniki na wszystkie modu³y i zmienne silnika.

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
