#pragma once
/**@file BaseAIController.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Klasa bazowa dla kontroler�w steruj�cych AI.*/

#include "IController.h"



namespace sw
{

/**@brief Klasa bazowa dla wszystkich kontroler�w sztucznej inteligencji.*/
class BaseAIController : public IController
{
	RTTR_ENABLE( IController )
private:
protected:
public:
};

}	// sw

