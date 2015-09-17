#pragma once
/**@file Event.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.
*/

class InputAbstractionLayer;



#define	BUILD_IN_EVENTS				2

typedef enum
{
	KEYDOWNEVENT,
	KEYUPEVENT
}EVENT_TYPE;



//Klasa bazowa dla wszystkich eventów w silniku
class Event
{
public:
	unsigned int				type;

	Event(int type);
	virtual ~Event();
};


class KeyDownEvent	:	public Event
{
public:
	InputAbstractionLayer*	layer;		//wskaŸnik na aktualn¹ warstwê abstrakcji
	short virtual_index;				//indeks przycisku w tablicy
	short mouseX;
	short mouseY;

	KeyDownEvent(short v_index);
	~KeyDownEvent();
};

class KeyUpEvent	:	public Event
{
public:
	InputAbstractionLayer*	layer;		//wskaŸnik na aktualn¹ warstwê abstrakcji
	short virtual_index;				//indeks przycisku w tablicy
	short mouseX;
	short mouseY;

	KeyUpEvent(short v_index);
	~KeyUpEvent();
};

