/**@file InputAbstractionLayer.cpp
@brief Plik zawiera definicje metod klasy InputAbstractionLayer_base, InputAbstractionLayer
oraz definicje standardowych warstw abstrakcji.
*/

#include "EngineCore/stdafx.h"
#include "InputAbstractionLayer.h"
#include "EngineCore/MainEngine/Engine.h"
#include "EngineCore/EventsManager/Event.h"

#include "Common\memory_leaks.h"


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


/**@brief Funkcja zapisuje wszystkie wartoœci przycisków, które potencjalnie mog¹ wys³aæ
eventy. Nastêpnie tablice s¹ zerowane.

Po dostarczeniu klasie wszystkich informacji o urzadzeniach wejœciowych, nast¹pi
sprawdzenie, gdzie zmieni³y siê stany przycisków i wys³anie eventów.
Do tego nale¿y wywo³aæ funkcjê end_events.

Wszystkie wywo³ania funkcji ustawiaj¹cych stan przycisków i osi powinny
znajdowaæ siê pomiêdzy wywo³aniami begin_event_collection i send_events.
Inaczej nic nie zadzia³a, poniewa¿ funkcje zak³adaj¹, ¿e tablice s¹ wyzerowane.*/
void InputAbstractionLayer::begin_event_collection()
{
	//przegl¹damy tablicê eventów i zapisujemy wartoœci z poprzedniej klatki
	for ( unsigned int i = 0; i < event_mapping_table.size(); ++i)
		event_mapping_table[i].value = virtual_button[event_mapping_table[i].virtual_index];
	//zerujemy wartoœci w tablicach
	//kolejne funkcje bêd¹ zak³ada³y, ¿e tablice s¹ wyzerowane, wiêc nie wolno tego zmieniaæ
	for (int i = 0; i < num_buttons; ++i)
		virtual_button[i] = 0;
	for (int i = 0; i < num_axis; ++i)
		virtual_axis[i] = 0;

}

/**@brief Funkcja wysy³a eventy, je¿eli jakieœ przyciski wirtualne zosta³y wciœniête.
Aby tak siê sta³o, musi znajdowaæ siê odpowiedni wpis w tablicy event_mapping_table.

@param[in] engine WskaŸnik jest aby, potrzebny móc wys³aæ event.
*/
void InputAbstractionLayer::send_events(Engine* engine)
{
//Przechodzimy po tablicy eventów i sprawdzamy czy zapisane wartoœci siê zmieni³y.
//Je¿eli siê zmieni³y, znaczy to, ¿e trzeba wys³aæ jakiœ event, bo przycisk albo zosta³
//wciœniêty albo puszczony.
	for( unsigned int i = 0; i < event_mapping_table.size(); ++i )
	{
		char button_state = virtual_button[event_mapping_table[i].virtual_index];
		if( event_mapping_table[i].value != button_state )
		{//zmieni³a siê wartoœæ, byæ mo¿e czas na event
			if( button_state == 0 )
			{
				if( event_mapping_table[i].up_event && !active_changed )
				{//je¿eli dopiero co staliœmy siê aktywn¹ wartw¹ to eventów nie wysy³amy, bo prawdopodobnie wciœniêcie
				//dotyczy³o jeszcze poprzedniej warstwy
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
//						ustawianie stanu przycisków
//=================================================================//

/**@brief Funkcja wywo³ywana w ka¿dej klatce do ustawienia stanu wirtualnych przycisków na podstawie
wejœcia o intefejsie klawiatury. Zazwyczaj wywo³uje siê j¹ dla klawiatury, ale mo¿e byæ te¿ tak,
¿e directX reprezentuje jakieœ inne urz¹dzenie takim samym interfejsem.

@param[in] device_nr Identyfikator urz¹dzenia, które aktualizujemy, jedna z wartoœci @ref DEVICE_IDs.
@param[in] keyboard_state Tablica 255 elementów opisuj¹ca stan klawiatury.*/

void InputAbstractionLayer::update_keyboard_device( short device_nr, const char* keyboard_state )
{
	//mamy jedynie przyciski
	for ( unsigned int i = 0; i < buttons_mapping_table.size(); ++i )
		if ( buttons_mapping_table[i].device_nr == device_nr )
		{
			short dev_index = buttons_mapping_table[i].button_axis_id;
			if ( keyboard_state[dev_index] )
			{//tablica jest wyzerowana, wiêc iteresuj¹ nas jedynie w³¹czone przyciski
			//je¿eli wiele przycisków fizycznych odnosi siê do jednego wirtualnego, to wtedy
			//wystarczy jeden wciœniêty, ¿eby stan przycisku wirtualnego zosta³ ustawiony
				short v_index = buttons_mapping_table[i].virtual_index;
				virtual_button[v_index] = 1;
			}
		}
		
}

/**@brief Funkcja wywo³ywana w ka¿dej klatce do ustawienia stanu wirtualnych przycisków oraz osi na podstawie
wejœcia o intefejsie myszy. Zazwyczaj wywo³uje siê j¹ dla myszy, ale mo¿e byæ te¿ tak,
¿e directX reprezentuje jakieœ inne urz¹dzenie takim samym interfejsem.

@param[in] device_nr Identyfikator urz¹dzenia, które aktualizujemy, jedna z wartoœci @ref DEVICE_IDs.
@param[in] mouse_state Struktura opisuj¹ca stan urz¹dzenia, które da siê opisaæ takim interfejsem.
@param[in] window_width Szerokość okna
@param[in] window_height Wysokość okna
*/
void InputAbstractionLayer::update_mouse_device( short device_nr, const DIMOUSESTATE2* mouse_state, int window_width, int window_height)
{
	//TODO:	przyciski

	//osie
	for ( unsigned int i = 0; i < axis_mapping_table.size(); ++i )
	{
		if (axis_mapping_table[i].device_nr == device_nr)
		{
			unsigned int dev_index = axis_mapping_table[i].button_axis_id;

			switch (dev_index)
			{
			case DIMOUSE_XAXIS:
			{
				short v_x_index = axis_mapping_table[i].virtual_index;
				float virtual_x_axis_value = (float)mouse_state->lX / (float)window_width;
				virtual_x_axis_value = (virtual_x_axis_value > 1.0f ? 1.0f : virtual_x_axis_value);
				virtual_axis[v_x_index] = virtual_x_axis_value;
				break;
			}
			case DIMOUSE_YAXIS:
			{
				short v_y_index = axis_mapping_table[i].virtual_index;
				float virtual_y_axis_value = (float)mouse_state->lY / (float)window_width;
				virtual_y_axis_value = (virtual_y_axis_value > 1.0f ? 1.0f : virtual_y_axis_value);
				virtual_axis[v_y_index] = virtual_y_axis_value;
				break;
			}
			case DIMOUSE_WHEEL:
			{
				//TODO: wheel
				break;
			}
			}
		}

	}

}

/**@brief Funkcja wywo³ywana w ka¿dej klatce do ustawienia stanu wirtualnych przycisków oraz osi na podstawie
wejœcia o intefejsie joysticka.

Zazwyczaj wywo³uje siê j¹ dla joysticka i wszystkich innych urz¹dzeñ,
które nie sa klawiatur¹ albo mysz¹, ale mo¿e byæ te¿ tak, ¿e directX reprezentuje jakieœ
inne urz¹dzenie takim samym interfejsem.

@param[in] device_nr Identyfikator urz¹dzenia, które aktualizujemy, jedna z wartoœci @ref DEVICE_IDs.
@param[in] joystick_state Struktura opisuj¹ca stan urz¹dzenia, które da siê opisaæ takim interfejsem.*/
void InputAbstractionLayer::update_joystick_device( short device_nr, const DIJOYSTATE* joystick_state )
{

}

//=================================================================//
//	w³¹czanie i wy³¹czanie eventów
//=================================================================//

/**@brief ¯¹da wysy³ania eventów o wciœniêciu wirtualnego przycisku 
o podanym indeksie.
@param[in] v_index Indeks wirtualnego przycisku w tablicy.*/
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

/**@brief ¯¹da wysy³ania eventów o puszczeniu wirtualnego przycisku
o podanym indeksie.
@param[in] v_index Indeks wirtualnego przycisku w tablicy.*/
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

/**@brief ¯¹da zaprzestania wysy³ania eventów o wciœniêciu wirtualnego przycisku
o podanym indeksie.
@param[in] v_index Indeks wirtualnego przycisku w tablicy.*/
void InputAbstractionLayer::delete_up_event( unsigned short v_index )
{
	for( unsigned int i = 0; i < event_mapping_table.size(); ++i )
	{
		if( event_mapping_table[i].virtual_index == v_index )
		{
			event_mapping_table[i].up_event = 0;
			if( event_mapping_table[i].down_event == 0 )	//nikt nie ¿¹da ¿adnego z eventów, kasujemy wpis
				event_mapping_table.erase( event_mapping_table.begin() + i );
			break;		//znalexliœmy element, wiêc przerywamy pêtlê
		}
	}
}

/**@brief ¯¹da zaprzestania wysy³ania eventów o puszczeniu wirtualnego przycisku
o podanym indeksie.
@param[in] v_index Indeks wirtualnego przycisku w tablicy.*/
void InputAbstractionLayer::delete_down_event( unsigned short v_index )
{
	for( unsigned int i = 0; i < event_mapping_table.size( ); ++i )
	{
		if( event_mapping_table[i].virtual_index == v_index )
		{
			event_mapping_table[i].down_event = 0;
			if( event_mapping_table[i].up_event == 0 )	//nikt nie ¿¹da ¿adnego z eventów, kasujemy wpis
				event_mapping_table.erase( event_mapping_table.begin( ) + i );
			break;		//znaleŸliœmy element, wiêc przerywamy pêtlê
		}
	}
}


//=================================================================//
//	ustawienie mapowania
//=================================================================//

/**@brief Funkcja s³u¿y do zdefiniowania wartstwy abstrakcji dla przycisków.

@param[in] mapping Tablica struktur, która mapuj¹ przyciski fizyczne na przyciski wirtualne.
@param[in] length D³ugoœæ podanej w pierwszym parametrze tablicy
*/
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


/**@brief Funkcja s³u¿y do zdefiniowania wartstwy abstrakcji dla osi.

@param[in] mapping Tablica struktur, która mapuj¹ osie fizyczne na osie wirtualne.
@param[in] length D³ugoœæ podanej w pierwszym parametrze tablicy
*/
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

/**@brief Funkcja tworz¹ca warstwê abstrakcji na podstawie podanego pliku.
@todo Funkcja do napisania, struktura pliku musi zostaæ okreœlona.

@param[in] file_name Nazwa pliku do wczytania.
@return Zwraca 0 w przypadku poprawnego wczytania.
*/
int InputAbstractionLayer::setup_layer_from_file( const std::string& file_name )
{
	return 0;
}


