#pragma once
/**
@file GizmoController.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.
*/

#include "EngineCore/Actors/BasicActors/StaticActor.h"
#include "EngineCore/ControllersEngine/BaseClasses/BaseInputController.h"


/**@brief Kontroler u¿ywany przez edytor do przesuwania obiektów.

Nale¿y stworzyæ specjalny obiekt, który bêdzie wyœwietla³ gizmo i przypisaæ mu ten kontroler.
Kontroler automatycznie ustawia gizmo w œrodku œledzonego obiektu. Przechywuje te¿ klawaiturê
i mysz aby przesuwaæ obiekt w zadane przez u¿ytkownika po³o¿enie.

Sterowanie:
Obiekt przesuwa siê tylko w wybranych osiach. Do wyboru osi s³u¿¹ przyciski:
- PROTOTYPE_BUTTONS::ENABLE_MOVE_X
- PROTOTYPE_BUTTONS::ENABLE_MOVE_Y
- PROTOTYPE_BUTTONS::ENABLE_MOVE_Z

Przesuwa siê klikaj¹c mysz¹ w obpowiednim miejscu przyciskiem:
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

