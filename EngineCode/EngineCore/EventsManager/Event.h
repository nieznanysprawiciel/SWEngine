#pragma once
/**@file Event.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.
*/

#include "Common/RTTR.h"

class InputAbstractionLayer;
class RenderPass;


#define	BUILD_IN_EVENTS				3

typedef enum class EventType
{
	KeyDownEvent,
	KeyUpEvent,
	RenderOnceEndedEvent
} EventType;



//Klasa bazowa dla wszystkich eventów w silniku
class Event
{
	RTTR_ENABLE()
public:
	unsigned int				type;		// @deprecated Zmienna okreœlaj¹ca typ zostanie zast¹piona przez RTTR.

	Event( unsigned int type );
	virtual ~Event() = default;
};


class KeyDownEvent	:	public Event
{
	RTTR_ENABLE( Event )
public:
	InputAbstractionLayer*	layer;		//wskaŸnik na aktualn¹ warstwê abstrakcji
	short VirtualIndex;				//indeks przycisku w tablicy
	short mouseX;
	short mouseY;

	KeyDownEvent( short v_index );
	~KeyDownEvent() = default;
};

class KeyUpEvent	:	public Event
{
	RTTR_ENABLE( Event )
public:
	InputAbstractionLayer*	layer;		//wskaŸnik na aktualn¹ warstwê abstrakcji
	short VirtualIndex;				//indeks przycisku w tablicy
	short mouseX;
	short mouseY;

	KeyUpEvent( short v_index );
	~KeyUpEvent() = default;
};


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
