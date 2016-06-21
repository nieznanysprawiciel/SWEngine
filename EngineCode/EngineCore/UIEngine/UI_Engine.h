#pragma once
/**
@file UI_Engine.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.
*/


#include "InputLibrary/IInput.h"
#include "InputAbstractionLayer.h"



class Engine;


/**@brief Modu³ do obs³ugi wejœcia wyjœcia.
@ingroup EngineCore

@todo Wydzieliæ obs³ugê DirectInput do osobnego modu³u.*/
class UI_Engine
{
private:
	Engine* engine;

	IInput*						m_inputModule;

// Abstraction layers
	InputAbstractionLayer*					m_currentAbstractionLayer;
	std::vector<InputAbstractionLayer*>		m_abstractionLayers;

// Hack dla edytora
	bool						m_enableInput;

public:
				UI_Engine						(Engine* engine);
				~UI_Engine						();


	IInput*		ChangeInputModule				( IInput* module );
	IInput*		GetInputModule					();

	//called by Engine in main loop
	void		ProceedInput					( float time_interval );
	void		DrawGUI							( float time_interval, float time_lag );

	//abstraction layers
	int			ChangeAbstractionLayer			( InputAbstractionLayer* next_layer );
	void		AddAbstractionLayer				( InputAbstractionLayer* new_layer );
	int			DeleteAbstractionLayer			( InputAbstractionLayer* layer );
	int			SetStandardAbstractionLayer		( STANDARD_ABSTRACTION_LAYER layer );
	void		ClearAbstractionLayers			();

	inline InputAbstractionLayer*	GetStandardAbstractionLayer( STANDARD_ABSTRACTION_LAYER layer )
	{
		if ( layer >= STANDARD_ABSTRACTION_LAYER_COUNT )
			return nullptr;
		return m_abstractionLayers[layer];
	};

	void		EnableInput						( bool val );

private:
	//abstraction layers
	void		UpdateAbstractionLayer			( const std::vector< KeyboardState* >& keyboards,
												  const std::vector< MouseState* >& mouses,
												  const std::vector< JoystickState* >& jousticks );
	void		InitAbstractionLayers();
};

