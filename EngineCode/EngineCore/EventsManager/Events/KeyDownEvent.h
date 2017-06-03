#pragma once
/**@file JeyDownEvent.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief */


#include "EngineCore/EventsManager/Event.h"
#include "EngineCore/UIEngine/InputAbstractionLayer.h"

class InputAbstractionLayer;

namespace sw
{

/**@brief Event wysy³any w momencie wciœniêcia przycisku.*/
class KeyDownEvent : public Event
{
	RTTR_ENABLE( Event )
public:
	InputAbstractionLayer*		Layer;				///< WskaŸnik na aktualn¹ warstwê abstrakcji
	VirtualKeyIndex				VirtualIndex;		///< Indeks przycisku w tablicy
	short						MouseX;
	short						MouseY;

	KeyDownEvent( short v_index );
	~KeyDownEvent() = default;
};

}	// sw

