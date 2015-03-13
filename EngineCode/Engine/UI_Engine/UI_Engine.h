#pragma once


#include "..\..\stdafx.h"
#include "InputAbstractionLayer.h"

#define DIRECT_INPUT_OK				DI_OK



class Engine;

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
	InputAbstractionLayer*					current_abstraction_layer;
	std::vector<InputAbstractionLayer*>		abstraction_layers;

public:
	UI_Engine(Engine* engine);
	~UI_Engine();

	//directX functions
	int init_direct_input();

	//called by Engine in main loop
	void proceed_input(float time_interval);
	void draw_GUI( float time_interval, float time_lag );

	//abstraction layers
	int change_abstraction_layer( InputAbstractionLayer* next_layer );
	void add_abstraction_layer( InputAbstractionLayer* new_layer );
	int delete_abstraction_layer( InputAbstractionLayer* layer );
	int set_standard_abstraction_layer(STANDARD_ABSTRACTION_LAYER layer);
	void clear_abstraction_layers();	
	inline InputAbstractionLayer* get_standard_abstraction_layer( STANDARD_ABSTRACTION_LAYER layer )
	{
		if ( layer >= STANDARD_ABSTRACTION_LAYER_COUNT )
			return nullptr;
		return abstraction_layers[layer];
	};

private:
	//abstraction layers
	void update_abstraction_layer();
	void init_abstraction_layers();
};

