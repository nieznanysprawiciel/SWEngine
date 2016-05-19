#pragma once
/**@file KeyUpEvent.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief*/


#include "EngineCore/EventsManager/Event.h"
#include "EngineCore/UIEngine/InputAbstractionLayer.h"


/**@brief Event wysy�any w momencie puszczenia przycisku.*/
class KeyUpEvent	:	public Event
{
	RTTR_ENABLE( Event )
public:
	InputAbstractionLayer*		Layer;				///< Wska�nik na aktualn� warstw� abstrakcji
	VirtualKeyIndex				VirtualIndex;		///< Indeks przycisku w tablicy
	short						MouseX;
	short						MouseY;

	KeyUpEvent( short v_index );
	~KeyUpEvent() = default;
};

