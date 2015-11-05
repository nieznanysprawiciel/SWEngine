#pragma once
/**@file Event.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.
*/

class InputAbstractionLayer;



#define	BUILD_IN_EVENTS				2

typedef enum class EventType
{
	KeyDownEvent,
	KeyUpEvent
} EventType;



//Klasa bazowa dla wszystkich event�w w silniku
class Event
{
public:
	unsigned int				type;

	Event( unsigned int type );
	virtual ~Event();
};


class KeyDownEvent	:	public Event
{
public:
	InputAbstractionLayer*	layer;		//wska�nik na aktualn� warstw� abstrakcji
	short virtual_index;				//indeks przycisku w tablicy
	short mouseX;
	short mouseY;

	KeyDownEvent(short v_index);
	~KeyDownEvent();
};

class KeyUpEvent	:	public Event
{
public:
	InputAbstractionLayer*	layer;		//wska�nik na aktualn� warstw� abstrakcji
	short virtual_index;				//indeks przycisku w tablicy
	short mouseX;
	short mouseY;

	KeyUpEvent(short v_index);
	~KeyUpEvent();
};

