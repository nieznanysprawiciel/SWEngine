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
@todo Uzupe³niæ dokumentacjê sterowania.*/
class GizmoController :	public BaseInputController
{
private:

	StaticActor*		m_followedActor;

public:
	explicit			GizmoController			( InputAbstractionLayerBase* layer );
						~GizmoController		();

	virtual void		ControlObjectPre		( DynamicActor* );
	virtual void		ControlObjectPost		( DynamicActor* );

	void				SetFollowedActor		( StaticActor* actor )		{ m_followedActor = actor; }
	StaticActor*		GetFollowedActor		()							{ return m_followedActor; }
};

