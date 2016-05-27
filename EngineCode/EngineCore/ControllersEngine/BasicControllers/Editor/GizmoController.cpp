#include "EngineCore/stdafx.h"
#include "GizmoController.h"
#include "EngineCore/UIEngine/InputAbstractionLayer.h"

#include "EngineCore/Actors/BasicActors/DynamicActor.h"


RTTR_REGISTRATION
{
	rttr::registration::class_< GizmoController >( "GizmoController" );
}


/**@brief */
GizmoController::GizmoController( InputAbstractionLayerBase* layer )
	:	BaseInputController( layer )
	,	m_followedActor( nullptr )
{}

/**@brief */
GizmoController::~GizmoController()
{}

/**@brief Przemieszcza kontrolowanego aktora.

�ledzony aktor jest przemieszczany dopiero przez funkcj� ControlObjectPost.*/
void GizmoController::ControlObjectPre		( DynamicActor* actor, IControllersState* globalState  )
{
	if ( !m_abstractionLayer->IsActive() )
		return;

	auto buttons = m_abstractionLayer->GetButtonsTable();
	auto mouseX = m_abstractionLayer->GetMouseX();
	auto mouseY = m_abstractionLayer->GetMouseY();


}

/**@brief �ledzi podanego aktora.*/
void GizmoController::ControlObjectPost		( DynamicActor* actor, IControllersState* globalState )
{
	if( m_followedActor )
	{
		auto position = m_followedActor->GetPosition();
		/// @todo Zamiast wywo�ywa� teleport, nale�y wys�a� sygna�.
		actor->Teleport( position );
	}
}

