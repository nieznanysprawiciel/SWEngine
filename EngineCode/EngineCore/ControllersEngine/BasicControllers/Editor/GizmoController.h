#pragma once
/**
@file GizmoController.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.
*/

#include "EngineCore/Actors/BasicActors/StaticActor.h"
#include "EngineCore/ControllersEngine/BaseClasses/BaseInputController.h"


/**@brief Kontroler u�ywany przez edytor do przesuwania obiekt�w.

Nale�y stworzy� specjalny obiekt, kt�ry b�dzie wy�wietla� gizmo i przypisa� mu ten kontroler.
Kontroler automatycznie ustawia gizmo w �rodku �ledzonego obiektu. Przechywuje te� klawaitur�
i mysz aby przesuwa� obiekt w zadane przez u�ytkownika po�o�enie.

Sterowanie:
@todo Uzupe�ni� dokumentacj� sterowania.*/
class GizmoController :	public BaseInputController
{
	RTTR_ENABLE( BaseInputController )
private:

	StaticActor*		m_followedActor;

public:
	explicit			GizmoController			( InputAbstractionLayerBase* layer );
						~GizmoController		();

	virtual void		ControlObjectPre		( DynamicActor* actor, IControllersState* globalState );
	virtual void		ControlObjectPost		( DynamicActor* actor, IControllersState* globalState );

	void				SetFollowedActor		( StaticActor* actor )		{ m_followedActor = actor; }
	StaticActor*		GetFollowedActor		()							{ return m_followedActor; }

private:

	DirectX::XMVECTOR	ComputeMouseRayDirection( IControllersState* globalState );
};

