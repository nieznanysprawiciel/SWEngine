#pragma once
/**@file RenderOnceEndedEvent.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief */

#include "EngineCore/EventsManager/Event.h"


class RenderPass;


/**@brief Event wysy�any w momencie zako�czenia zleconego renderowania.*/
class RenderOnceEndedEvent	:	public Event
{
	RTTR_ENABLE( Event )
private:
protected:
public:
	RenderOnceEndedEvent();
	~RenderOnceEndedEvent() = default;
public:
	RenderPass*		renderPass;
};

