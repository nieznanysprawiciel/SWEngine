#pragma once

#include "swCommonLib/Common/EngineObject.h"


class GenericActor : public EngineObject
{
	RTTR_ENABLE( EngineObject );
	RTTR_REGISTRATION_FRIEND;
private:
	int		m_typeIdx;
	int		m_actorIdx;

public:
	GenericActor() {}
};


class GenericObject : public EngineObject
{
	RTTR_ENABLE( EngineObject );
	RTTR_REGISTRATION_FRIEND;
private:
	int		m_typeIdx;

public:
	GenericObject() {}
};
