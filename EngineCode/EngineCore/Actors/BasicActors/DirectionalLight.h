#pragma once


#include "LightBase.h"


/**@brief DirectionalL light.
@ingroup Lights*/
class DirectionalLight	: public LightBase
{
	RTTR_ENABLE( LightBase )
	RTTR_REGISTRATION_FRIEND
private:
protected:
public:
	explicit DirectionalLight();
	virtual ~DirectionalLight() = default;

	static ActorBase*		Create()	{ return new DirectionalLight; }
};

