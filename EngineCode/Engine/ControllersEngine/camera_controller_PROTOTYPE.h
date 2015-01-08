#pragma once
#include "..\..\Interfaces\basic_interfaces.h"



class camera_controller_PROTOTYPE	:	public Base_input_controller
{
private:
	float move_speed;
	float rot_speed;
public:
	camera_controller_PROTOTYPE( InputAbstractionLayer_base* layer );
	virtual ~camera_controller_PROTOTYPE();

	virtual void control_object( Dynamic_object* );
};

