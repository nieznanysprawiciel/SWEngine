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


/**@brief Funkcja podtrzymuje stan przycisków, ale kasuje z nich informacje o eventach.*/
void InputAbstractionLayer::BeginEventCollection()
{
	for( Size i = 0; i < m_virtualButtons.size(); ++i )
		m_virtualButtons[ i ].HoldState();
}

/**@brief Funkcja wysyła eventy, jeżeli jakieś przyciski wirtualne zostały wciśnięte.
Aby tak się stało, musi znajdować się odpowiedni wpis z żądaniem w tablicy m_requestedEvents.

@param[in] engine Wskaźnik na silnik jest potrzebny, aby móc wysłać event.
*/
void InputAbstractionLayer::SendEvents(Engine* engine)
{
	for( unsigned int i = 0; i < m_requestedEvents.size(); ++i )
	{
		KeyState state = m_virtualButtons[ m_requestedEvents[ i ].VirtualIndex ];

		if( state.IsKeyDownEvent() && !m_activeChanged )
		{
			KeyDownEvent* event = new KeyDownEvent( m_requestedEvents[i].VirtualIndex );
			event->MouseX = mouseX;
			event->MouseY = mouseY;
			event->Layer = this;

			engine->SendEvent( event );
		}
		else if( state.IsKeyUpEvent() && !m_activeChanged )
		{
			KeyUpEvent* event = new KeyUpEvent( m_requestedEvents[i].VirtualIndex );
			event->MouseX = mouseX;
			event->MouseY = mouseY;
			event->Layer = this;

			engine->SendEvent(event);
		}
	}
		
	m_activeChanged = false;
}

//=================================================================//
//						ustawianie stanu przycisków
//=================================================================//

/**@brief Funkcja wywoływana w ka¿dej klatce do ustawienia stanu wirtualnych przycisków na podstawie
wejścia o intefejsie klawiatury. Zazwyczaj wywłuje się ją dla klawiatury, ale może byæ teć tak,
że direct input albo inne api wejścia reprezentuje jakieś inne urządzenie takim samym interfejsem.

@param[in] DeviceNr Identyfikator urządzenia, które aktualizujemy, jedna z wartoœci @ref DEVICE_IDs.
@param[in] keyboardState Tablica 255 elementów opisująca stan klawiatury.

@todo Rozwiązać problemy z przypadkami, gdy kilka przycisków mapuje sie na jeden wirtualny.
Problematyczna sekwencja Press1 -> Press2 -> UnPress1 -> Problem -> UnPress2.
Być może wirtualny przycisk powinien byś w tym miejscu nadal wciśnięty.*/
void InputAbstractionLayer::UpdateKeyboardDevice( DeviceNumber DeviceNr, KeyboardState* keyboardState )
{
	auto state = keyboardState->GetKeyboardState();

	// Mamy jedynie przyciski
	for( unsigned int i = 0; i < m_buttonsMapping.size(); ++i )
	{
		if( m_buttonsMapping[ i ].DeviceNr == DeviceNr )
		{
			short devIndex = m_buttonsMapping[ i ].PhysicalIndex;
			if( state[ devIndex ].IsKeyDownEvent() )
			{
				short virtualIndex = m_buttonsMapping[ i ].VirtualIndex;
				m_virtualButtons[ virtualIndex ].Press();
			}
			else if( state[ devIndex ].IsKeyUpEvent() )
			{
				short virtualIndex = m_buttonsMapping[ i ].VirtualIndex;
				m_virtualButtons[ virtualIndex ].UnPress();				
			}
		}
	}
}

/**@brief Funkcja wywoływana w ka¿dej klatce do ustawienia stanu wirtualnych przycisków oraz osi na podstawie
wejœcia o intefejsie myszy. Zazwyczaj wywołuje się ją dla myszy, ale może być też tak,
że direct input albo inne api wejścia reprezentuje jakieœ inne urz¹dzenie takim samym interfejsem.

@param[in] DeviceNr Identyfikator urządzenia, które aktualizujemy, jedna z wartości @ref DEVICE_IDs.
@param[in] mouseState Struktura opisująca stan urządzenia, które da się opisać takim interfejsem.
@param[in] windowWidth Szerokość okna
@param[in] windowHeight Wysokość okna
@todo Zastanowić się czy wysokość i szerokość okna jest wogóle potrzebna (i dlaczego nie).
@todo Uzupełnić implementację dla pozostałych osi myszy.
*/
void InputAbstractionLayer::UpdateMouseDevice( DeviceNumber DeviceNr, MouseState* mouseState, int windowWidth, int windowHeight)
{
	auto buttons = mouseState->GetButtonsState();

	for( unsigned int i = 0; i < m_buttonsMapping.size(); ++i )
	{
		if( m_buttonsMapping[ i ].DeviceNr == DeviceNr )
		{
			short devIndex = m_buttonsMapping[ i ].PhysicalIndex;
			if( buttons[ devIndex ].IsKeyDownEvent() )
			{
				short virtualIndex = m_buttonsMapping[ i ].VirtualIndex;
				m_virtualButtons[ virtualIndex ].Press();
			}
			else if( buttons[ devIndex ].IsKeyUpEvent() )
			{
				short virtualIndex = m_buttonsMapping[ i ].VirtualIndex;
				m_virtualButtons[ virtualIndex ].UnPress();				
			}
		}
	}

	mouseX = mouseState->GetPositionX();
	mouseY = mouseState->GetPositionY();

	//osie
	auto axes = mouseState->GetAxesState();
	for( unsigned int i = 0; i < m_axisMapping.size(); ++i )
	{
		if( m_axisMapping[ i ].DeviceNr == DeviceNr )
		{
			unsigned int devIndex = m_axisMapping[ i ].PhysicalIndex;

			switch( devIndex )
			{
			case MouseState::PHYSICAL_AXES::X_AXIS:
			{
				short v_x_index = m_axisMapping[ i ].VirtualIndex;
				float virtual_x_axis_value = axes[ MouseState::PHYSICAL_AXES::X_AXIS ] / (float)windowWidth;
				virtual_x_axis_value = ( virtual_x_axis_value > 1.0f ? 1.0f : virtual_x_axis_value );
				m_virtualAxis[ v_x_index ] = virtual_x_axis_value;
				break;
			}
			case MouseState::PHYSICAL_AXES::Y_AXIS:
			{
				short v_y_index = m_axisMapping[ i ].VirtualIndex;
				float virtual_y_axis_value = axes[ MouseState::PHYSICAL_AXES::Y_AXIS ] / (float)windowWidth;
				virtual_y_axis_value = ( virtual_y_axis_value > 1.0f ? 1.0f : virtual_y_axis_value );
				m_virtualAxis[ v_y_index ] = virtual_y_axis_value;
				break;
			}
			case MouseState::PHYSICAL_AXES::WHEEL:
			case MouseState::PHYSICAL_AXES::W_AXIS:
			{
				short virtualIdx = m_axisMapping[ i ].VirtualIndex;
				m_virtualAxis[ virtualIdx ] = axes[ devIndex ];
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


