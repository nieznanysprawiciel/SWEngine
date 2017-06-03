#pragma once
/**@file BaseAIController.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Klasa bazowa dla kontrolerów steruj¹cych AI.*/

#include "IController.h"



namespace sw
{

/**@brief Klasa bazowa dla wszystkich kontrolerów sztucznej inteligencji.*/
class BaseAIController : public IController
{
	RTTR_ENABLE( IController )
private:
protected:
public:
};

}	// sw

