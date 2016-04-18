#pragma once


#include "EngineCore/stdafx.h"
#include "InputAbstractionLayer.h"

#define DIRECT_INPUT_OK				DI_OK



class Engine;


/**@brief Modu³ do obs³ugi wejœcia wyjœcia.
@ingroup EngineCore

@todo Wydzieliæ obs³ugê DirectInput do osobnego modu³u.*/
class UI_Engine
{
private:
	Engine* engine;

	//direct input
	LPDIRECTINPUT8				direct_input;
	LPDIRECTINPUTDEVICE8		keyboard_input;
	LPDIRECTINPUTDEVICE8		mouse_input;

	//keyboard, mouse joystick
	char						keyboard_state[256];
	DIMOUSESTATE2				mouse_state;

//abstraction layers
	InputAbstractionLayer*					m_currentAbstractionLayer;
	std::vector<InputAbstractionLayer*>		m_abstractionLayers;

// hack dla edytora
	bool						m_enableInput;

private:
	//directX functions
	void clean_direct_input();
public:
	UI_Engine(Engine* engine);
	~UI_Engine();

	//directX functions
	int init_direct_input();

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
	void		UpdateAbstractionLayer();
	void		InitAbstractionLayers();
};

