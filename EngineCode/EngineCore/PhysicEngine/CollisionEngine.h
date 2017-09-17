#pragma once
/**@file CollisionEngine.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Modu� CollisionEngine.*/

#include "Collisions/CollisionOctree.h"

class Engine;
class ActorBase;



namespace sw
{


/**@brief Modu� obs�uguj�cy kolizje.
@ingroup EngineCore*/
class CollisionEngine
{
private:

	Engine* engine;

	/**Kontener przechowuj�cy i sortuaj�cy obiekty.
	@todo W przysz�o�ci warto by tutaj m�c wstawia� r�ne kontenery. U�ytkownik powinien mie� przynajmniej mo�liwo��
	wyboru zastosowanego kontenera, zmienianie w trakcie mo�e nie koniecznie.*/
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
