/**@file InputAbstractionLayer.cpp
@brief Plik zawiera definicje metod klasy InputAbstractionLayerBase, InputAbstractionLayer
oraz definicje standardowych warstw abstrakcji.
*/

#include "EngineCore/stdafx.h"
#include "InputAbstractionLayer.h"
#include "EngineCore/MainEngine/Engine.h"
#include "EngineCore/EventsManager/Events/KeyUpEvent.h"
#include "EngineCore/EventsManager/Events/KeyDownEvent.h"

#include "Common/MemoryLeaks.h"


//=================================================================//
//					InputAbstractionLayerBase
//=================================================================//

InputAbstractionLayerBase::InputAbstractionLayerBase()
{
	active = false;
}


InputAbstractionLayerBase::~InputAbstractionLayerBase()
{}


//=================================================================//
//						InputAbstractionLayer
//=================================================================//

InputAbstractionLayer::InputAbstractionLayer()
{
	m_activeChanged = false;
}


InputAbstractionLayer::~InputAbstractionLayer()
{}


/**@brief Funkcja zapisuje wszystkie wartoœci przycisków, które potencjalnie mog¹ wys³aæ
eventy. Nastêpnie tablice s¹ zerowane.

Po dostarczeniu klasie wszystkich informacji o urzadzeniach wejœciowych, nast¹pi
sprawdzenie, gdzie zmieni³y siê stany przycisków i wys³anie eventów.
Do tego nale¿y wywo³aæ funkcjê end_events.

Wszystkie wywo³ania funkcji ustawiaj¹cych stan przycisków i osi powinny
znajdowaæ siê pomiêdzy wywo³aniami BeginEventCollection i SendEvents.
Inaczej nic nie zadzia³a, poniewa¿ funkcje zak³adaj¹, ¿e tablice s¹ wyzerowane.*/
void InputAbstractionLayer::BeginEventCollection()
{
	//przegl¹damy tablicê eventów i zapisujemy wartoœci z poprzedniej klatki
	for ( unsigned int i = 0; i < m_requestedEvents.size(); ++i)
		m_requestedEvents[i].Value = m_virtualButtons[m_requestedEvents[i].VirtualIndex];
	//zerujemy wartoœci w tablicach
	//kolejne funkcje bêd¹ zak³ada³y, ¿e tablice s¹ wyzerowane, wiêc nie wolno tego zmieniaæ
	for (int i = 0; i < m_virtualButtons.size(); ++i)
		m_virtualButtons[i] = 0;
	for (int i = 0; i < m_virtualAxis.size(); ++i)
		m_virtualAxis[i] = 0;

}

/**@brief Funkcja wysy³a eventy, je¿eli jakieœ przyciski wirtualne zosta³y wciœniête.
Aby tak siê sta³o, musi znajdowaæ siê odpowiedni wpis w tablicy m_requestedEvents.

@param[in] engine WskaŸnik jest aby, potrzebny móc wys³aæ event.
*/
void InputAbstractionLayer::SendEvents(Engine* engine)
{
//Przechodzimy po tablicy eventów i sprawdzamy czy zapisane wartoœci siê zmieni³y.
//Je¿eli siê zmieni³y, znaczy to, ¿e trzeba wys³aæ jakiœ event, bo przycisk albo zosta³
//wciœniêty albo puszczony.
	for( unsigned int i = 0; i < m_requestedEvents.size(); ++i )
	{
		char button_state = m_virtualButtons[m_requestedEvents[i].VirtualIndex];
		if( m_requestedEvents[i].Value != button_state )
		{//zmieni³a siê wartoœæ, byæ mo¿e czas na event
			if( button_state == 0 )
			{
				if( m_requestedEvents[i].UpEvent && !m_activeChanged )
				{//je¿eli dopiero co staliœmy siê aktywn¹ wartw¹ to eventów nie wysy³amy, bo prawdopodobnie wciœniêcie
				//dotyczy³o jeszcze poprzedniej warstwy
					KeyUpEvent* event = new KeyUpEvent( m_requestedEvents[i].VirtualIndex );
					event->MouseX = mouseX;
					event->MouseY = mouseY;
					event->Layer = this;

					engine->SendEvent(event);
				}
			}
			else //button_state == 1
			{
				if( m_requestedEvents[i].DownEvent && !m_activeChanged )
				{
					KeyDownEvent* event = new KeyDownEvent( m_requestedEvents[i].VirtualIndex );
					event->MouseX = mouseX;
					event->MouseY = mouseY;
					event->Layer = this;

					engine->SendEvent( event );
				}
			}
		}
	}

	m_activeChanged = false;
}

//=================================================================//
//						ustawianie stanu przycisków
//=================================================================//

/**@brief Funkcja wywo³ywana w ka¿dej klatce do ustawienia stanu wirtualnych przycisków na podstawie
wejœcia o intefejsie klawiatury. Zazwyczaj wywo³uje siê j¹ dla klawiatury, ale mo¿e byæ te¿ tak,
¿e directX reprezentuje jakieœ inne urz¹dzenie takim samym interfejsem.

@param[in] DeviceNr Identyfikator urz¹dzenia, które aktualizujemy, jedna z wartoœci @ref DEVICE_IDs.
@param[in] keyboard_state Tablica 255 elementów opisuj¹ca stan klawiatury.*/

void InputAbstractionLayer::UpdateKeyboardDevice( DeviceNumber DeviceNr, KeyboardState* keyboardState )
{
	auto state = keyboardState->GetKeyboardState();

	// Mamy jedynie przyciski
	for( unsigned int i = 0; i < m_buttonsMapping.size(); ++i )
	{
		if( m_buttonsMapping[ i ].DeviceNr == DeviceNr )
		{
			short dev_index = m_buttonsMapping[ i ].PhysicalIndex;
			if( state[ dev_index ] )
			{//tablica jest wyzerowana, wiêc iteresuj¹ nas jedynie w³¹czone przyciski
			//je¿eli wiele przycisków fizycznych odnosi siê do jednego wirtualnego, to wtedy
			//wystarczy jeden wciœniêty, ¿eby stan przycisku wirtualnego zosta³ ustawiony
				short v_index = m_buttonsMapping[ i ].VirtualIndex;
				m_virtualButtons[ v_index ] = 1;
			}
		}
	}
}

/**@brief Funkcja wywo³ywana w ka¿dej klatce do ustawienia stanu wirtualnych przycisków oraz osi na podstawie
wejœcia o intefejsie myszy. Zazwyczaj wywo³uje siê j¹ dla myszy, ale mo¿e byæ te¿ tak,
¿e directX reprezentuje jakieœ inne urz¹dzenie takim samym interfejsem.

@param[in] DeviceNr Identyfikator urz¹dzenia, które aktualizujemy, jedna z wartoœci @ref DEVICE_IDs.
@param[in] mouse_state Struktura opisuj¹ca stan urz¹dzenia, które da siê opisaæ takim interfejsem.
@param[in] window_width Szerokość okna
@param[in] window_height Wysokość okna
@todo Zastanowić się czy wysokość i szerokość okna jest wogóle potrzebna (i dlaczego nie).
@todo Uzupełnić implementację dla pozostałych osi myszy.
*/
void InputAbstractionLayer::UpdateMouseDevice( DeviceNumber DeviceNr, MouseState* mouseState, int window_width, int window_height)
{
	auto buttons = mouseState->GetButtonsState();

	for( unsigned int i = 0; i < m_buttonsMapping.size(); ++i )
	{
		if( m_buttonsMapping[ i ].DeviceNr == DeviceNr )
		{
			short devIndex = m_buttonsMapping[ i ].PhysicalIndex;
			if( buttons[ devIndex ] )
			{//tablica jest wyzerowana, wiêc iteresuj¹ nas jedynie w³¹czone przyciski
			//je¿eli wiele przycisków fizycznych odnosi siê do jednego wirtualnego, to wtedy
			//wystarczy jeden wciœniêty, ¿eby stan przycisku wirtualnego zosta³ ustawiony
				short v_index = m_buttonsMapping[ i ].VirtualIndex;
				m_virtualButtons[ v_index ] = 1;
			}
		}
	}

	mouseX = mouseState->GetPositionX();
	mouseY = mouseState->GetPositionY();

	//osie
	auto axes = mouseState->GetAxesState();
	for ( unsigned int i = 0; i < m_axisMapping.size(); ++i )
	{
		if (m_axisMapping[i].DeviceNr == DeviceNr)
		{
			unsigned int devIndex = m_axisMapping[i].PhysicalIndex;

			switch ( devIndex )
			{
			case MouseState::PHYSICAL_AXES::X_AXIS:
			{
				short v_x_index = m_axisMapping[i].VirtualIndex;
				float virtual_x_axis_value = axes[ MouseState::PHYSICAL_AXES::X_AXIS ] / (float)window_width;
				virtual_x_axis_value = (virtual_x_axis_value > 1.0f ? 1.0f : virtual_x_axis_value);
				m_virtualAxis[v_x_index] = virtual_x_axis_value;
				break;
			}
			case MouseState::PHYSICAL_AXES::Y_AXIS:
			{
				short v_y_index = m_axisMapping[i].VirtualIndex;
				float virtual_y_axis_value = axes[ MouseState::PHYSICAL_AXES::Y_AXIS ] / (float)window_width;
				virtual_y_axis_value = (virtual_y_axis_value > 1.0f ? 1.0f : virtual_y_axis_value);
				m_virtualAxis[v_y_index] = virtual_y_axis_value;
				break;
			}
			case MouseState::PHYSICAL_AXES::WHEEL:
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

@param[in] DeviceNr Identyfikator urz¹dzenia, które aktualizujemy, jedna z wartoœci @ref DEVICE_IDs.
@param[in] joystick_state Struktura opisuj¹ca stan urz¹dzenia, które da siê opisaæ takim interfejsem.*/
void InputAbstractionLayer::UpdateJoystickDevice( DeviceNumber DeviceNr, JoystickState* joystickState )
{

}

//=================================================================//
//	włączanie i wyłączanie eventów
//=================================================================//

/**@brief Żąda wysyłania eventów o wciśnięciu wirtualnego przycisku 
o podanym indeksie.

@todo Po napisaniu porządnego sposobu obsługi eventów należy umożliwić tutaj
wstawienie konkretnego delegata, który zostanie wywołany jako obsługa eventu.
Obecnie mechanizm jest bardzo niewygodny. W funkcji obsługi eventu trzeba sprawdzać
jaki przycisk został wciśnięty faktycznie.

@param[in] virtualIdx Indeks wirtualnego przycisku w tablicy.*/
void InputAbstractionLayer::DemandDownEvent( VirtualKeyIndex virtualIdx )
{
	unsigned int i = 0;
	for( ; i < m_requestedEvents.size(); ++i )
		if( m_requestedEvents[i].VirtualIndex == virtualIdx )
			break;

	if( i < m_requestedEvents.size() )
		m_requestedEvents[i].DownEvent = 1;
	else
	{
		EventMapping event;
		event.DownEvent = 1;
		event.UpEvent = 0;
		event.VirtualIndex = virtualIdx;
		m_requestedEvents.push_back( event );
	}
}

/**@brief ¯¹da wysy³ania eventów o puszczeniu wirtualnego przycisku
o podanym indeksie.
@param[in] virtualIdx Indeks wirtualnego przycisku w tablicy.*/
void InputAbstractionLayer::DemandUpEvent( VirtualKeyIndex virtualIdx )
{
	unsigned int i = 0;
	for( ; i < m_requestedEvents.size( ); ++i )
		if( m_requestedEvents[i].VirtualIndex == virtualIdx )
			break;

	if( i < m_requestedEvents.size( ) )
		m_requestedEvents[i].UpEvent = 1;
	else
	{
		EventMapping event;
		event.DownEvent = 0;
		event.UpEvent = 1;
		event.VirtualIndex = virtualIdx;
		m_requestedEvents.push_back( event );
	}
}

/**@brief ¯¹da zaprzestania wysy³ania eventów o wciœniêciu wirtualnego przycisku
o podanym indeksie.
@param[in] virtualIdx Indeks wirtualnego przycisku w tablicy.*/
void InputAbstractionLayer::DeleteUpEvent( VirtualKeyIndex virtualIdx )
{
	for( unsigned int i = 0; i < m_requestedEvents.size(); ++i )
	{
		if( m_requestedEvents[i].VirtualIndex == virtualIdx )
		{
			m_requestedEvents[i].UpEvent = 0;
			if( m_requestedEvents[i].DownEvent == 0 )	//nikt nie ¿¹da ¿adnego z eventów, kasujemy wpis
				m_requestedEvents.erase( m_requestedEvents.begin() + i );
			break;		//znalexliœmy element, wiêc przerywamy pêtlê
		}
	}
}

/**@brief ¯¹da zaprzestania wysy³ania eventów o puszczeniu wirtualnego przycisku
o podanym indeksie.
@param[in] virtualIdx Indeks wirtualnego przycisku w tablicy.*/
void InputAbstractionLayer::DeleteDownEvent( VirtualKeyIndex virtualIdx )
{
	for( unsigned int i = 0; i < m_requestedEvents.size( ); ++i )
	{
		if( m_requestedEvents[i].VirtualIndex == virtualIdx )
		{
			m_requestedEvents[i].DownEvent = 0;
			if( m_requestedEvents[i].UpEvent == 0 )	//nikt nie ¿¹da ¿adnego z eventów, kasujemy wpis
				m_requestedEvents.erase( m_requestedEvents.begin( ) + i );
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
void InputAbstractionLayer::SetupButtonsLayer( std::vector< InputMapping >&& mapping )
{
	unsigned int max = 0;
	for ( unsigned int i = 0; i < mapping.size(); ++i )
	{
		// Szukamy maksymalnego indexu virtualnego przycisku, żeby wiedzieć ile miejsca potrzeba w tablicy m_virtualButtons.
		if ( mapping[i].VirtualIndex > max )
			max = mapping[i].VirtualIndex;
	}

	m_buttonsMapping = mapping;
	m_virtualButtons.resize( max + 1 );
}


/**@brief Funkcja s³u¿y do zdefiniowania wartstwy abstrakcji dla osi.

@param[in] mapping Tablica struktur, która mapuj¹ osie fizyczne na osie wirtualne.
@param[in] length D³ugoœæ podanej w pierwszym parametrze tablicy
*/
void InputAbstractionLayer::SetupAxisLayer( std::vector< InputMapping >&& mapping )
{
	unsigned int max = 0;
	for ( unsigned int i = 0; i < mapping.size(); ++i )
	{
		// Szukamy maksymalnego indexu virtualnego przycisku, żeby wiedzieć ile miejsca potrzeba w tablicy m_virtualAxis.
		if ( mapping[i].VirtualIndex > max )
			max = mapping[i].VirtualIndex;
	}

	m_axisMapping = mapping;
	m_virtualAxis.resize( max + 1 );
}

/**@brief Funkcja tworz¹ca warstwê abstrakcji na podstawie podanego pliku.
@todo Funkcja do napisania, struktura pliku musi zostaæ okreœlona.

@param[in] file_name Nazwa pliku do wczytania.
@return Zwraca 0 w przypadku poprawnego wczytania.
*/
int InputAbstractionLayer::SetupLayerFromFile( const std::string& file_name )
{
	return 0;
}


