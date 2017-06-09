#pragma once
/**
@file UIEngine.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/



#include "swInputLibrary/InputCore/IInput.h"
#include "InputAbstractionLayer.h"






namespace sw
{


class Engine;


/**@brief Modu³ do obs³ugi wejœcia wyjœcia.
@ingroup EngineCore

@todo Wydzieliæ obs³ugê DirectInput do osobnego modu³u.
@todo Refactor this whole code, add unique pointers where needed, handle devices better.*/
class UIEngine
{
private:
	Engine* engine;

	input::IInput*							m_inputModule;

// Abstraction layers
	InputAbstractionLayer*					m_currentAbstractionLayer;
	std::vector< InputAbstractionLayer* >	m_abstractionLayers;

// Hack dla edytora
	bool						m_enableInput;

public:
	UIEngine						( Engine* engine );
	~UIEngine						();


	input::IInput*		ChangeInputModule				( input::IInput* module );
	input::IInput*		GetInputModule					();

	//called by Engine in main loop
	void		ProceedInput					( float time_interval );
	void		DrawGUI							( float time_interval, float time_lag );

	//abstraction layers
	int			ChangeAbstractionLayer			( InputAbstractionLayer* next_layer );
	void		AddAbstractionLayer				( InputAbstractionLayer* new_layer );
	int			DeleteAbstractionLayer			( InputAbstractionLayer* layer );
	int			SetStandardAbstractionLayer		( STANDARD_ABSTRACTION_LAYER layer );
	void		ClearAbstractionLayers			();

	InputAbstractionLayer*		GetStandardAbstractionLayer	( STANDARD_ABSTRACTION_LAYER layer );;

	void		EnableInput						( bool val );

private:
	//abstraction layers
	void		UpdateAbstractionLayer			( const std::vector< input::KeyboardDeviceOPtr >& keyboards,
												  const std::vector< input::MouseDeviceOPtr >& mouses,
												  const std::vector< input::JoystickDeviceOPtr >& joysticks );
	void		InitAbstractionLayers();

	void		BuiltInProcessInput				( const std::vector< input::KeyboardDeviceOPtr >& keyboards,
												  const std::vector< input::MouseDeviceOPtr >& mouses,
												  const std::vector< input::JoystickDeviceOPtr >& joysticks );
};

}	// sw
