#include "stdafx.h"
#include "UI_Engine.h"
#include "Engine.h"
#include "Interfaces\basic_interfaces.h"


#include "EngineHelpers\PerformanceCheck.h"
bool pushedF1 = false;		///< Zapami�tuje stan przycisku odpowiedzialnego za wys�anie statystyk do pliku, �eby nie by�o zdublowanych wywo�a�.


#include "Common\memory_leaks.h"


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

@return Zwraca tak� warto�� jak funkcje Directinput.*/
int UI_Engine::init_direct_input()
{
	int result = DirectInput8Create(engine->get_instance_handler(),
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
	keyboard_input->SetCooperativeLevel(engine->get_window_handler(), DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	//tworzymy obiekt myszy
	result = direct_input->CreateDevice(GUID_SysMouse, &mouse_input, nullptr);
	if (result != DIRECT_INPUT_OK)
	{
		keyboard_input->Release();
		direct_input->Release();
		return result;
	}
	mouse_input->SetDataFormat(&c_dfDIMouse2);
	mouse_input->SetCooperativeLevel(engine->get_window_handler(), DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	//todo: sprawdzi� czy jest w systemie joystick i go wczyta�

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

///@brief Funkcja wywo�ywana przez klas� Engine w ka�dym obiegu g��wnej petli programu.
///Przechwytujemy klawiatur�, wykonujemy wszytkie funkcje obs�ugi obiekt�w sterowanych.
///@param[in] time_interval Parametrem jest czas kt�ry up�yn�� od ostatniego wywo�ania
void UI_Engine::proceed_input(float time_interval)
{
	keyboard_input->GetDeviceState(256, keyboard_state);
	mouse_input->GetDeviceState(sizeof(mouse_state), &mouse_state);

	///<@fixme Wy��czenia aplikacji musi si� odbywa� w jaki� inny spos�b. Powinien by� jaki� domy�lny mechanizm, �eby u�ytkownik nie zapomnia� zrobi� wy��czania.
	if ( keyboard_state[DIK_ESCAPE] & 0x80 )
		engine->end_aplication();

	///<@fixme To jest hack i nie mam poj�cia jak to robi� w wersji docelowej.
	if ( (keyboard_state[DIK_F1] & 0x80) && !pushedF1 )
	{
		pushedF1 = true;
		PRINT_STATISTICS( PERFORMANCE_STATISTICS_FILE_PATH );
	}
	if ( !(keyboard_state[DIK_F1] & 0x80) )
		pushedF1 = false;

	update_abstraction_layer();
}

/** @brief Funkcja rysuj�ca graficzny interfejs u�ytkownika.
Jest wywo�ywana zaraz po wszystkich procedurach rysuj�cych obiekty na scenie.

@param[in] time_interval Parametrem jest czas kt�ry up�yn�� od ostatniego wywo�ania.
 */
void UI_Engine::draw_GUI( float time_interval, float time_lag )
{

}


