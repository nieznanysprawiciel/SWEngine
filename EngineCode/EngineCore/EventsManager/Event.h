#pragma once
/**@file Event.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.
*/

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
public:
	unsigned int				type;

	Event( unsigned int type );
	virtual ~Event() = default;
};


class KeyDownEvent	:	public Event
{
public:
	InputAbstractionLayer*	layer;		//wskaŸnik na aktualn¹ warstwê abstrakcji
	short virtual_index;				//indeks przycisku w tablicy
	short mouseX;
	short mouseY;

	KeyDownEvent( short v_index );
	~KeyDownEvent() = default;
};

class KeyUpEvent	:	public Event
{
public:
	InputAbstractionLayer*	layer;		//wskaŸnik na aktualn¹ warstwê abstrakcji
	short virtual_index;				//indeks przycisku w tablicy
	short mouseX;
	short mouseY;

	KeyUpEvent( short v_index );
	~KeyUpEvent() = default;
};


class RenderOnceEndedEvent	:	public Event
{
private:
protected:
public:
	RenderOnceEndedEvent();
	~RenderOnceEndedEvent() = default;
public:
	RenderPass*		renderPass;
};
