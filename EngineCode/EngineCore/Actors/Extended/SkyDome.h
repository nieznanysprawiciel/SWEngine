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

	bool			m_needsBufferUpdate;

protected:

	void			RequestUpdate			()								{ m_needsBufferUpdate = true; }
	void			Updated					()								{ m_needsBufferUpdate = false; }

public:
	explicit		SkyDome		() = default;
	virtual			~SkyDome	() = default;


	virtual void		UpdateBuffers		( IRenderer* renderer ) = 0;
	bool				NeedsBufferUpdate	()								{ return m_needsBufferUpdate;  }
};


