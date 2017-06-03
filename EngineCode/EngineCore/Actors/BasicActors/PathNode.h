#pragma once
/**
@file PathNode.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/



#include "StaticActor.h"

#include <vector>



namespace sw
{

/**@brief Punkt kontrolny dla grafów œcie¿ek dla AI.

Koncepcja jest podobna jak PathNody z Unreal Engine 3.

@ingroup Actors*/
class PathNode : public StaticActor
{
	RTTR_ENABLE( StaticActor )
		RTTR_REGISTRATION_FRIEND
private:
protected:
	std::vector< PathNode* >		m_connectedNodes;

public:
	explicit PathNode();
	virtual ~PathNode();


	static ActorBase*		Create() { return new PathNode; }
};


}	// sw