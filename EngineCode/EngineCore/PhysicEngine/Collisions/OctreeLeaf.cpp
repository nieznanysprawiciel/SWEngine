#include "EngineCore/stdafx.h"
#include "OctreeLeaf.h"


namespace sw
{



/**@brief Domy�lny konstruktor.*/
OctreeLeaf::OctreeLeaf()
	: IOctreeNode( OctreeNodeType::LeafNode )
	, m_staticActorsCount( 0 )
{}

/**@brief Kasuje wszystkich aktor�w.*/
void		OctreeLeaf::ClearAllActors()
{
	m_actors.clear();
	m_staticActorsCount = 0;
}

/**@brief Kasuje jedynie akt�r�w dynamicznych, kt�rzy zmieniaj�
swoja pozycj� z klatki na klatk�.*/
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

@note Je�eli mamy pewno��, �e aktor nale�y do grupy aktor�w dynamicznych,
to ta funkcja pozwala przeszuka� list� element�w szybciej, poniewa� pomija
elementy statyczne.

@return Zwraca false, je�eli nie znaleziono elementu.*/
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

@note Je�eli mamy pewno��, �e aktor nale�y do grupy aktor�w statycznych,
to ta funkcja pozwala przeszuka� list� element�w szybciej, poniewa� pomija
elementy dynamiczne.

@return Zwraca false, je�eli nie znaleziono elementu.*/
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

/**@brief Kasuje aktora z li�cia.
@return Zwraca false, je�eli nie znaleziono elementu.*/
bool		OctreeLeaf::RemoveActor			( CollisionActor* actor )
{
	bool result;
	result = RemoveStaticActor( actor );
	result |= RemoveDynamicActor( actor );

	return result;
}



}	// sw