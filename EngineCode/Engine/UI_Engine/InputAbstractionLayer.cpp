#include "..\..\stdafx.h"
#include "InputAbstractionLayer.h"
#include "..\Engine.h"


#include "..\..\memory_leaks.h"


//=================================================================//
//					InputAbstractionLayer_base
//=================================================================//

InputAbstractionLayer_base::InputAbstractionLayer_base()
{
	active = false;
	virtual_button = nullptr;
	virtual_axis = nullptr;
	num_buttons = 0;
	num_axis = 0;
}


InputAbstractionLayer_base::~InputAbstractionLayer_base()
{
	if (virtual_axis != nullptr)
		delete[] virtual_axis;
	if (virtual_button != nullptr)
		delete[] virtual_button;
}


//=================================================================//
//						InputAbstractionLayer
//=================================================================//

InputAbstractionLayer::InputAbstractionLayer()
{
	active_changed = false;
}


InputAbstractionLayer::~InputAbstractionLayer()
{
}


/*Funkcja zapisuje wszystkie warto�ci przycisk�w, kt�re potencjalnie mog� wys�a�
eventy. Nast�pnie tablice s� zerowane.

Po dostarczeniu klasie wszystkich informacji o urzadzeniach wej�ciowych, nast�pi
sprawdzenie, gdzie zmieni�y si� stany przycisk�w i wys�anie event�w.
Do tego nale�y wywo�a� funkcj� end_events.

Wszystkie wywo�ania funkcji ustawiaj�cych stan przycisk�w i osi powinny
znajdowa� si� pomi�dzy wywo�aniami begin_event_collection i send_events.
Inaczej nic nie zadzia�a, poniewa� funkcje zak�adaj�, �e tablice s� wyzerowane.*/
void InputAbstractionLayer::begin_event_collection()
{
	//przegl�damy tablic� event�w i zapisujemy warto�ci z poprzedniej klatki
	for ( unsigned int i = 0; i < event_mapping_table.size(); ++i)
		event_mapping_table[i].value = virtual_button[event_mapping_table[i].virtual_index];
	//zerujemy warto�ci w tablicach
	//kolejne funkcje b�d� zak�ada�y, �e tablice s� wyzerowane, wi�c nie wolno tego zmienia�
	for (int i = 0; i < num_buttons; ++i)
		virtual_button[i] = 0;
	for (int i = 0; i < num_axis; ++i)
		virtual_axis[i] = 0;

}


void InputAbstractionLayer::send_events(Engine* engine)
{
//Przechodzimy po tablicy event�w i sprawdzamy czy zapisane warto�ci si� zmieni�y.
//Je�eli si� zmieni�y, znaczy to, �e trzeba wys�a� jaki� event, bo przycisk albo zosta�
//wci�ni�ty albo puszczony.
	for( unsigned int i = 0; i < event_mapping_table.size(); ++i )
	{
		char button_state = virtual_button[event_mapping_table[i].virtual_index];
		if( event_mapping_table[i].value != button_state )
		{//zmieni�a si� warto��, by� mo�e czas na event
			if( button_state == 0 )
			{
				if( event_mapping_table[i].up_event && !active_changed )
				{//je�eli dopiero co stali�my si� aktywn� wartw� to event�w nie wysy�amy, bo prawdopodobnie wci�ni�cie
				//dotyczy�o jeszcze poprzedniej warstwy
					KeyUpEvent* event = new KeyUpEvent( event_mapping_table[i].virtual_index );
					event->mouseX = mouseX;
					event->mouseY = mouseY;
					event->layer = this;

					engine->send_event(event);
				}
			}
			else //button_state == 1
			{
				if( event_mapping_table[i].down_event && !active_changed )
				{
					KeyDownEvent* event = new KeyDownEvent( event_mapping_table[i].virtual_index );
					event->mouseX = mouseX;
					event->mouseY = mouseY;
					event->layer = this;

					engine->send_event( event );
				}
			}
		}
	}

	active_changed = false;
}

//=================================================================//
//						ustawianie stanu przycisk�w
//=================================================================//
/*Funkcja wywo�ywana w ka�dej klatce do ustawienia stanu wirtualnych przycisk�w na podstawie
wej�cia o intefejsie klawiatury. Zazwyczaj wywo�uje si� j� dla klawiatury, ale mo�e by� te� tak,
�e directX reprezentuje jakie� inne urz�dzenie takim samym interfejsem.*/

void InputAbstractionLayer::update_keyboard_device( short device_nr, const char* keyboard_state )
{
	//mamy jedynie przyciski
	for ( unsigned int i = 0; i < buttons_mapping_table.size(); ++i )
		if ( buttons_mapping_table[i].device_nr == device_nr )
		{
			short dev_index = buttons_mapping_table[i].button_axis_id;
			if ( keyboard_state[dev_index] )
			{//tablica jest wyzerowana, wi�c iteresuj� nas jedynie w��czone przyciski
			//je�eli wiele przycisk�w fizycznych odnosi si� do jednego wirtualnego, to wtedy
			//wystarczy jeden wci�ni�ty, �eby stan przycisku wirtualnego zosta� ustawiony
				short v_index = buttons_mapping_table[i].virtual_index;
				virtual_button[v_index] = 1;
			}
		}
		
}

/*Funkcja wywo�ywana w ka�dej klatce do ustawienia stanu wirtualnych przycisk�w oraz osi na podstawie
wej�cia o intefejsie myszy. Zazwyczaj wywo�uje si� j� dla myszy, ale mo�e by� te� tak,
�e directX reprezentuje jakie� inne urz�dzenie takim samym interfejsem.*/
void InputAbstractionLayer::update_mouse_device( short device_nr, const DIMOUSESTATE2* mouse_state )
{


}

/*Funkcja wywo�ywana w ka�dej klatce do ustawienia stanu wirtualnych przycisk�w oraz osi na podstawie
wej�cia o intefejsie joysticka. Zazwyczaj wywo�uje si� j� dla joysticka i wszystkich innych urz�dze�,
kt�re nie sa klawiatur� albo mysz�, ale mo�e by� te� tak, �e directX reprezentuje jakie�
inne urz�dzenie takim samym interfejsem.*/
void InputAbstractionLayer::update_joystick_device( short device_nr, const DIJOYSTATE* joystick_state )
{

}

//=================================================================//
//	w��czanie i wy��czanie event�w
//=================================================================//

void InputAbstractionLayer::demand_down_event( unsigned short v_index )
{
	unsigned int i = 0;
	for( ; i < event_mapping_table.size(); ++i )
		if( event_mapping_table[i].virtual_index == v_index )
			break;

	if( i < event_mapping_table.size() )
		event_mapping_table[i].down_event = 1;
	else
	{
		event_mapping event;
		event.down_event = 1;
		event.up_event = 0;
		event.virtual_index = v_index;
		event_mapping_table.push_back( event );
	}
}

void InputAbstractionLayer::demand_up_event( unsigned short v_index )
{
	unsigned int i = 0;
	for( ; i < event_mapping_table.size( ); ++i )
		if( event_mapping_table[i].virtual_index == v_index )
			break;

	if( i < event_mapping_table.size( ) )
		event_mapping_table[i].up_event = 1;
	else
	{
		event_mapping event;
		event.down_event = 0;
		event.up_event = 1;
		event.virtual_index = v_index;
		event_mapping_table.push_back( event );
	}
}

void InputAbstractionLayer::delete_up_event( unsigned short v_index )
{
	for( unsigned int i = 0; i < event_mapping_table.size(); ++i )
	{
		if( event_mapping_table[i].virtual_index == v_index )
		{
			event_mapping_table[i].up_event = 0;
			if( event_mapping_table[i].down_event == 0 )	//nikt nie ��da �adnego z event�w, kasujemy wpis
				event_mapping_table.erase( event_mapping_table.begin() + i );
			break;		//znalexli�my element, wi�c przerywamy p�tl�
		}
	}
}

void InputAbstractionLayer::delete_down_event( unsigned short v_index )
{
	for( unsigned int i = 0; i < event_mapping_table.size( ); ++i )
	{
		if( event_mapping_table[i].virtual_index == v_index )
		{
			event_mapping_table[i].down_event = 0;
			if( event_mapping_table[i].up_event == 0 )	//nikt nie ��da �adnego z event�w, kasujemy wpis
				event_mapping_table.erase( event_mapping_table.begin( ) + i );
			break;		//znale�li�my element, wi�c przerywamy p�tl�
		}
	}
}


//=================================================================//
//	ustawienie mapowania
//=================================================================//


void InputAbstractionLayer::setup_buttons_layer( input_mapping* mapping, unsigned int length )
{
	unsigned int max = 0;
	for ( unsigned int i = 0; i < length; ++i )
	{
		buttons_mapping_table.push_back( mapping[i] );
		if ( mapping[i].virtual_index > max )
			max = mapping[i].virtual_index;
	}

	if ( virtual_button != nullptr )
		delete[] virtual_button;
	num_buttons = max + 1;
	virtual_button = new char[num_buttons];
}

void InputAbstractionLayer::setup_axis_layer( input_mapping* mapping, unsigned int length )
{
	unsigned int max = 0;
	for ( unsigned int i = 0; i < length; ++i )
	{
		axis_mapping_table.push_back( mapping[i] );
		if ( mapping[i].virtual_index > max )
			max = mapping[i].virtual_index;
	}

	if ( virtual_axis != nullptr )
		delete[] virtual_axis;
	num_axis = max + 1;
	virtual_axis = new float[num_axis];
}

int InputAbstractionLayer::setup_layer_from_file( const std::string& file_name )
{
	return 0;
}

namespace STANDARD_LAYERS
{

	input_mapping PROTOTYPE_mapping[PROTOTYPE_MAPPING_COUNT] =
	{
		{ DEVICE_IDs::KEYBOARD, PROTOTYPE::FORWARD, DIK_W },
		{ DEVICE_IDs::KEYBOARD, PROTOTYPE::BACKWARD, DIK_S },
		{ DEVICE_IDs::KEYBOARD, PROTOTYPE::LEFT, DIK_A },
		{ DEVICE_IDs::KEYBOARD, PROTOTYPE::RIGHT, DIK_D },
		{ DEVICE_IDs::KEYBOARD, PROTOTYPE::TURN_RIGHT, DIK_X },
		{ DEVICE_IDs::KEYBOARD, PROTOTYPE::TURN_LEFT, DIK_Z },
		{ DEVICE_IDs::KEYBOARD, PROTOTYPE::UP, DIK_Q },
		{ DEVICE_IDs::KEYBOARD, PROTOTYPE::DOWN, DIK_E },
		{ DEVICE_IDs::KEYBOARD, PROTOTYPE::SHOT1, DIK_LCONTROL },
		{ DEVICE_IDs::KEYBOARD, PROTOTYPE::SHOT2, DIK_RCONTROL },
		{ DEVICE_IDs::KEYBOARD, PROTOTYPE::ESCAPE, DIK_ESCAPE }
	};

}
