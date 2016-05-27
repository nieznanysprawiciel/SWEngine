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

Œledzony aktor jest przemieszczany dopiero przez funkcjê ControlObjectPost.*/
void GizmoController::ControlObjectPre		( DynamicActor* actor, IControllersState* globalState  )
{
	if ( !m_abstractionLayer->IsActive() )
		return;

	auto buttons = m_abstractionLayer->GetButtonsTable();
	auto mouseX = m_abstractionLayer->GetMouseX();
	auto mouseY = m_abstractionLayer->GetMouseY();


}

/**@brief Œledzi podanego aktora.*/
void GizmoController::ControlObjectPost		( DynamicActor* actor, IControllersState* globalState )
{
	if( m_followedActor )
	{
		auto position = m_followedActor->GetPosition();
		/// @todo Zamiast wywo³ywaæ teleport, nale¿y wys³aæ sygna³.
		actor->Teleport( position );
	}
}

