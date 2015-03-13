#include "..\..\stdafx.h"
#include "UI_Engine.h"
#include "..\Engine.h"
#include "..\..\Interfaces\basic_interfaces.h"



#include "..\..\memory_leaks.h"


UI_Engine::UI_Engine(Engine* engine)
	: engine(engine)
{
	direct_input = nullptr;
	keyboard_input = nullptr;
	mouse_input = nullptr;

	init_abstraction_layers( );
}


UI_Engine::~UI_Engine()
{
	for ( unsigned int i = 0; i < abstraction_layers.size(); ++i )
		delete abstraction_layers[i];
	clean_direct_input();
}



/**@brief Inicjalizuje obiekty Direct Inputa

@return Zwraca tak¹ wartoœæ jak funkcje Directinput.*/
int UI_Engine::init_direct_input()
{
	int result = DirectInput8Create(engine->instance_handler,
		DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&direct_input, NULL);
	if (result != DIRECT_INPUT_OK)
		return result;

	//tworzymy obiekt klawiatury
	result = direct_input->CreateDevice(GUID_SysKeyboard, &keyboard_input, nullptr);
	if (result != DIRECT_INPUT_OK)
	{
		direct_input->Release();
		return result;
	}
	keyboard_input->SetDataFormat(&c_dfDIKeyboard);
	keyboard_input->SetCooperativeLevel(engine->window_handler, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	//tworzymy obiekt myszy
	result = direct_input->CreateDevice(GUID_SysMouse, &mouse_input, nullptr);
	if (result != DIRECT_INPUT_OK)
	{
		keyboard_input->Release();
		direct_input->Release();
		return result;
	}
	mouse_input->SetDataFormat(&c_dfDIMouse2);
	mouse_input->SetCooperativeLevel(engine->window_handler, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	//todo: sprawdziæ czy jest w systemie joystick i go wczytaæ

	keyboard_input->Acquire();
	mouse_input->Acquire();

	return result;
}


/**@brief Zwalnia obiekty Direct Inputa.*/
void UI_Engine::clean_direct_input()
{
	if (direct_input != nullptr)
	{
		if (keyboard_input != nullptr)
		{
			keyboard_input->Unacquire();
			keyboard_input->Release();
		}

		if (mouse_input != nullptr)
		{
			mouse_input->Unacquire();
			mouse_input->Release();
		}
		direct_input->Release();
	}
}

///@brief Funkcja wywo³ywana przez klasê Engine w ka¿dym obiegu g³ównej petli programu.
///Przechwytujemy klawiaturê, wykonujemy wszytkie funkcje obs³ugi obiektów sterowanych.
///@param[in] time_interval Parametrem jest czas który up³yn¹³ od ostatniego wywo³ania
void UI_Engine::proceed_input(float time_interval)
{
	keyboard_input->GetDeviceState(256, keyboard_state);
	mouse_input->GetDeviceState(sizeof(mouse_state), &mouse_state);

	if ( keyboard_state[DIK_ESCAPE] & 0x80 )
		engine->end_aplication();

	update_abstraction_layer();
}

/** @brief Funkcja rysuj¹ca graficzny interfejs u¿ytkownika.
Jest wywo³ywana zaraz po wszystkich procedurach rysuj¹cych obiekty na scenie.

@param[in] time_interval Parametrem jest czas który up³yn¹³ od ostatniego wywo³ania
 */
void UI_Engine::draw_GUI( float time_interval, float time_lag )
{

}


