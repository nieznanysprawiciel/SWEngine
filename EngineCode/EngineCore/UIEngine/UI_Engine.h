#pragma once


#include "EngineCore/stdafx.h"
#include "InputAbstractionLayer.h"

#define DIRECT_INPUT_OK				DI_OK



class Engine;


/**@brief Modu� do obs�ugi wej�cia wyj�cia.
@ingroup EngineCore*/
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

	//directX functions
	void clean_direct_input();

//abstraction layers
	InputAbstractionLayer*					m_currentAbstractionLayer;
	std::vector<InputAbstractionLayer*>		m_abstractionLayers;

public:
	UI_Engine(Engine* engine);
	~UI_Engine();

	//directX functions
	int init_direct_input();

	//called by Engine in main loop
	void		ProceedInput(float time_interval);
	void		DrawGUI( float time_interval, float time_lag );

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

private:
	//abstraction layers
	void		UpdateAbstractionLayer();
	void		InitAbstractionLayers();
};

