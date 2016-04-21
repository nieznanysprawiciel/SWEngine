#pragma once
/**@file ActorBase.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief */

#include "Common/RTTR.h"


/**@brief Klasa bazowa dla obiekt�w, kt�re maj� by� dost�pne w edytorze
poprzez rttr. Nale�y po niej odziedziczy� i doda� odpowiednie deklaracje.

@ingroup Reflection*/
class EngineObject
{
	RTTR_ENABLE()
};
