#pragma once
/**@file InputApi.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Deklaracja klasy InputApi.*/



#include "EngineCore/UserApi/ApiGroup.h"
#include "EngineCore/UIEngine/StandardAbstractionLayers.h"


namespace sw
{

class InputAbstractionLayer;

namespace Api
{

/**@defgroup InputAPI Managing input/output.
@ingroup UserAPI
@brief Functions for user input.*/


/**@brief Consists of functions for managing user input.
@ingroup UserAPI
@ingroup InputAPI*/
class InputApi : public ApiGroup
{
private:
protected:
public:
	InputApi() = default;
	~InputApi() = default;
public:
	InputAbstractionLayer*		GetStandardAbstractionLayer		( STANDARD_ABSTRACTION_LAYER layer );
	void						SetAbstractionLayer				( InputAbstractionLayer* layer );
	void						SetStandardAbstractionLayer		( STANDARD_ABSTRACTION_LAYER layer );
};



}	// Api
}	// sw
