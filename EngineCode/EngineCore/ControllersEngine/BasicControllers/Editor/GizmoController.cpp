#include "EngineCore/stdafx.h"
#include "GizmoController.h"

#include "EngineCore/Actors/BasicActors/DynamicActor.h"


/**@brief */
GizmoController::GizmoController( InputAbstractionLayerBase* layer )
	:	BaseInputController( layer )
	,	m_followedActor( nullptr )
{}

/**@brief */
GizmoController::~GizmoController()
{}

/**@brief */
void GizmoController::ControlObjectPre		( DynamicActor* actor )
{

}

/**@brief �ledzi podanego aktora.*/
void GizmoController::ControlObjectPost		( DynamicActor* actor )
{
	if( m_followedActor )
	{
		auto position = m_followedActor->GetPosition();
		/// @todo Zamiast wywo�ywa� teleport, nale�y wys�a� sygna�.
		actor->Teleport( position );
	}
}

