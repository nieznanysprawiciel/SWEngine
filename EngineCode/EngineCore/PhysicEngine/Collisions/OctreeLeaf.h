#pragma once
/**@file OctreeLeaf.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief*/

#include "IOctreeNode.h"

#include <vector>



namespace sw
{


class CollisionActor;

typedef uint16 LeafElementsNum;

/**@brief Liœæ drzewa zawieraj¹cy obiekty bêd¹ce w najbli¿szym s¹siedztwie.

W liœciu znajduj¹ siê obiekty czysto statyczne, których pozycja nie zmienia siê
co klatkê, s¹ one wtedy umieszczane na pocz¹tku wektora m_actors. Pozosta³e obiekty
s¹ wysy³ane na koniec.*/
class OctreeLeaf : public IOctreeNode
{
private:

	LeafElementsNum						m_staticActorsCount;	///< Liczba aktorów o niezmiennej pozycji.
	std::vector< CollisionActor* >		m_actors;				///< Wektor aktorów.

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


