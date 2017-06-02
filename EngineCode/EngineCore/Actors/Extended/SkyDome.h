#pragma once
/**
@file SkyDome.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "EngineCore/Actors/BasicActors/DynamicActor.h"


/**@brief Base class for sky domes, sky boxes and so on.*/
class SkyDome : public DynamicActor
{
	RTTR_ENABLE( DynamicActor );
private:
protected:
public:
	explicit		SkyDome		() = default;
	virtual			~SkyDome	() = default;


	virtual void		UpdateBuffers	( IRenderer* renderer ) = 0;

};


