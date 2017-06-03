#pragma once
/**@file JeyDownEvent.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief */


#include "EngineCore/EventsManager/Event.h"
#include "EngineCore/UIEngine/InputAbstractionLayer.h"

class InputAbstractionLayer;

namespace sw
{

/**@brief Event wysy�any w momencie wci�ni�cia przycisku.*/
class KeyDownEvent : public Event
{
	RTTR_ENABLE( Event )
public:
	InputAbstractionLayer*		Layer;				///< Wska�nik na aktualn� warstw� abstrakcji
	VirtualKeyIndex				VirtualIndex;		///< Indeks przycisku w tablicy
	short						MouseX;
	short						MouseY;

	KeyDownEvent( short v_index );
	~KeyDownEvent() = default;
};

}	// sw

