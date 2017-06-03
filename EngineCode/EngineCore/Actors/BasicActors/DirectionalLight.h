#pragma once
/**
@file DirectionalLight.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "LightBase.h"


namespace sw
{


/**@brief DirectionalL light.

@ingroup Actors
@ingroup Lights*/
class DirectionalLight : public LightBase
{
	RTTR_ENABLE( LightBase )
		RTTR_REGISTRATION_FRIEND
private:
protected:
public:
	explicit DirectionalLight();
	virtual ~DirectionalLight() = default;

	static ActorBase*		Create() { return new DirectionalLight; }
};

}	// sw
