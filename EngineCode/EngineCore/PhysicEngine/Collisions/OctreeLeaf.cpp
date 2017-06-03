#include "EngineCore/stdafx.h"
#include "OctreeLeaf.h"


namespace sw
{



/**@brief Domyœlny konstruktor.*/
OctreeLeaf::OctreeLeaf()
	: IOctreeNode( OctreeNodeType::LeafNode )
	, m_staticActorsCount( 0 )
{}

/**@brief Kasuje wszystkich aktorów.*/
void		OctreeLeaf::ClearAllActors()
{
	m_actors.clear();
	m_staticActorsCount = 0;
}

/**@brief Kasuje jedynie aktórów dynamicznych, którzy zmieniaj¹
swoja pozycj¹ z klatki na klatkê.*/
void		OctreeLeaf::ClearDynamicActors	()
{
	m_actors.erase( m_actors.begin() + m_staticActorsCount, m_actors.end() );
}

/**@brief Dodaje dynamicznego aktora.*/
void		OctreeLeaf::AddDynamicActor		( CollisionActor* actor )
{
	m_actors.push_back( actor );
}

/**@brief Dodaje statycznego aktora.*/
void		OctreeLeaf::AddStaticActor		( CollisionActor* actor )
{
	m_actors.insert( m_actors.begin() + m_staticActorsCount, actor );
	++m_staticActorsCount;
}

/**@brief Kasuje aktora dynamicznego.

@note Je¿eli mamy pewnoœæ, ¿e aktor nale¿y do grupy aktorów dynamicznych,
to ta funkcja pozwala przeszukaæ listê elementów szybciej, poniewa¿ pomija
elementy statyczne.

@return Zwraca false, je¿eli nie znaleziono elementu.*/
bool		OctreeLeaf::RemoveDynamicActor	( CollisionActor* actor )
{
	for( LeafElementsNum index = m_staticActorsCount; index < m_actors.size(); ++index )
	{
		if( m_actors[ index ] == actor )
		{
			m_actors.erase( m_actors.begin() + index );
			return true;
		}
	}
	return false;
}

/**@brief Kasuje aktora statycznego.

@note Je¿eli mamy pewnoœæ, ¿e aktor nale¿y do grupy aktorów statycznych,
to ta funkcja pozwala przeszukaæ listê elementów szybciej, poniewa¿ pomija
elementy dynamiczne.

@return Zwraca false, je¿eli nie znaleziono elementu.*/
bool		OctreeLeaf::RemoveStaticActor	( CollisionActor* actor )
{
	for( LeafElementsNum index = 0; index < m_staticActorsCount; ++index )
	{
		if( m_actors[ index ] == actor )
		{
			m_actors.erase( m_actors.begin() + index );

			--m_staticActorsCount;
			return true;
		}
	}
	return false;
}

/**@brief Kasuje aktora z liœcia.
@return Zwraca false, je¿eli nie znaleziono elementu.*/
bool		OctreeLeaf::RemoveActor			( CollisionActor* actor )
{
	bool result;
	result = RemoveStaticActor( actor );
	result |= RemoveDynamicActor( actor );

	return result;
}



}	// sw