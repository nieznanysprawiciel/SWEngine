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
Obiekt przesuwa si� tylko w wybranych osiach. Do wyboru osi s�u�� przyciski:
- PROTOTYPE_BUTTONS::ENABLE_MOVE_X
- PROTOTYPE_BUTTONS::ENABLE_MOVE_Y
- PROTOTYPE_BUTTONS::ENABLE_MOVE_Z

Przesuwa si� klikaj�c mysz� w obpowiednim miejscu przyciskiem:
PROTOTYPE_BUTTONS::LEFT_CLICK
*/
class GizmoController :	public BaseInputController
{
	RTTR_ENABLE( BaseInputController )
public:

	enum class Operation { Translate, Rotate, Scale };

	struct RotationOp
	{
		float		StartAngle;
		float		StepSize;
		bool		Started;
		bool		UseStep;
	};

private:

	StaticActor*		m_followedActor;
	Operation			m_operation;
	RotationOp			m_rotationOp;

public:
	explicit			GizmoController			( InputAbstractionLayerBase* layer );
						~GizmoController		();

	virtual void		ControlObjectPre		( DynamicActor* actor, IControllersState* globalState );
	virtual void		ControlObjectPost		( DynamicActor* actor, IControllersState* globalState );

	void				SetFollowedActor		( StaticActor* actor )		{ m_followedActor = actor; }
	StaticActor*		GetFollowedActor		()							{ return m_followedActor; }

	void				ChangeOperation			( Operation op );
	Operation			CheckOperation			();

	void				SetStepSize				( float value );
	float				GetStepSize				();

	void				UseStepRotation			( bool value );
	bool				GetUseStepRotation		();

private:

	DirectX::XMVECTOR	ComputeMouseRayDirection( IControllersState* globalState );
};

