#pragma once

#include "swCommonLib/Common/TypesDefinitions.h"
#include "PathNode.h"


/**@brief Punkt startowy dla gracza lub bota.

Punkt startowy jest jednoczeœnie PathNodem i nale¿y do grafu œcie¿ek.

Aby odró¿niæ od siebie punkty startowe mo¿na u¿yæ w³aœciwoœci SpawnGroup (zmienna m_spawnGroup).
Mo¿e to byæ przydatne, je¿eli chce siê np. zrobiæ punkty startowe dla wielu dru¿yn itp.

@ingroup Actors*/
class SpawnPoint : public PathNode
{
	RTTR_ENABLE( PathNode )
	RTTR_REGISTRATION_FRIEND
private:
protected:
	int32		m_spawnGroup;
	
public:
	explicit SpawnPoint();
	virtual ~SpawnPoint();


	static ActorBase*		Create()	{ return new SpawnPoint; }
};

