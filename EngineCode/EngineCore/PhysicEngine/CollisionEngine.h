#pragma once
/**@file CollisionEngine.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Modu³ CollisionEngine.*/

#include "Collisions/CollisionOctree.h"

class Engine;
class ActorBase;



namespace sw
{


/**@brief Modu³ obs³uguj¹cy kolizje.
@ingroup EngineCore*/
class CollisionEngine
{
private:

	Engine* engine;

	/**Kontener przechowuj¹cy i sortuaj¹cy obiekty.
	@todo W przysz³oœci warto by tutaj móc wstawiaæ ró¿ne kontenery. U¿ytkownik powinien mieæ przynajmniej mo¿liwoœæ
	wyboru zastosowanego kontenera, zmienianie w trakcie mo¿e nie koniecznie.*/
	CollisionOctree*					m_octree;

	std::vector< CollisionActor* >		m_staticActors;
	std::vector< CollisionActor* >		m_dynamicActors;

public:
	CollisionEngine						( Engine* engine );
	~CollisionEngine					();

	void		ProceedCollisions		( TimeType time, TimeDiff elapsed );

	void		RemoveActor				( ActorBase* actor );
	void		RemoveAllActors			();

};

}	// sw
