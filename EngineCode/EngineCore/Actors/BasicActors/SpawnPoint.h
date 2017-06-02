#pragma once

#include "swCommonLib/Common/TypesDefinitions.h"
#include "PathNode.h"


/**@brief Punkt startowy dla gracza lub bota.

Punkt startowy jest jednocze�nie PathNodem i nale�y do grafu �cie�ek.

Aby odr�ni� od siebie punkty startowe mo�na u�y� w�a�ciwo�ci SpawnGroup (zmienna m_spawnGroup).
Mo�e to by� przydatne, je�eli chce si� np. zrobi� punkty startowe dla wielu dru�yn itp.

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

