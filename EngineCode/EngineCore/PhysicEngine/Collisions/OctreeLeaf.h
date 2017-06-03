#pragma once
/**@file OctreeLeaf.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief*/

#include "IOctreeNode.h"

#include <vector>



namespace sw
{


class CollisionActor;

typedef uint16 LeafElementsNum;

/**@brief Li�� drzewa zawieraj�cy obiekty b�d�ce w najbli�szym s�siedztwie.

W li�ciu znajduj� si� obiekty czysto statyczne, kt�rych pozycja nie zmienia si�
co klatk�, s� one wtedy umieszczane na pocz�tku wektora m_actors. Pozosta�e obiekty
s� wysy�ane na koniec.*/
class OctreeLeaf : public IOctreeNode
{
private:

	LeafElementsNum						m_staticActorsCount;	///< Liczba aktor�w o niezmiennej pozycji.
	std::vector< CollisionActor* >		m_actors;				///< Wektor aktor�w.

public:
	OctreeLeaf();
	~OctreeLeaf() = default;

	void		ClearAllActors		();
	void		ClearDynamicActors	();

	void		AddDynamicActor		( CollisionActor* actor );
	void		AddStaticActor		( CollisionActor* actor );

	bool		RemoveActor			( CollisionActor* actor );
	bool		RemoveDynamicActor	( CollisionActor* actor );
	bool		RemoveStaticActor	( CollisionActor* actor );
};


}	// sw


