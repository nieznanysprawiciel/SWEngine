#include "EngineCore/stdafx.h"
#include "UI_Engine.h"
#include "EngineCore/MainEngine/Engine.h"
#include "EngineCore/Actors/ActorObjects.h"


#include "EngineCore/EngineHelpers/PerformanceCheck.h"
bool pushedF1 = false;		///< Hack. Zapamiêtuje stan przycisku odpowiedzialnego za wys³anie statystyk do pliku, ¿eby nie by³o zdublowanych wywo³añ.


#include "Common/MemoryLeaks.h"


UI_Engine::UI_Engine( Engine* engine )
	: engine( engine )
{
	direct_input = nullptr;
	keyboard_input = nullptr;
	mouse_input = nullptr;

	m_enableInput = true;

	InitAbstractionLayers();
}


UI_Engine::~UI_Engine()
{
	for ( unsigned int i = 0; i < m_abstractionLayers.size(); ++i )
		delete m_abstractionLayers[i];
	clean_direct_input();
}

/**@brief Funkcja umo¿liwia edytorowi wy³¹czenie inputu, gdy kontrolka odpowiedzialna
za wyœwietlanie renderowanego obrazu nie ma focusa.*/
void		UI_Engine::EnableInput						( bool val )
{
	if( val )
	{
		m_enableInput = true;
		m_currentAbstractionLayer->set_active( true );
	}
	else
	{
		m_enableInput = false;
		m_currentAbstractionLayer->set_active( false );
	}
}

/**@brief Inicjalizuje obiekty Direct Inputa

@return Zwraca tak¹ wartoœæ jak funkcje Directinput.*/
int UI_Engine::init_direct_input()
{
	int result = DirectInput8Create(engine->GetInstanceHandler(),
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
	keyboard_input->SetCooperativeLevel(engine->GetWindowHandler(), DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	//tworzymy obiekt myszy
	result = direct_input->CreateDevice(GUID_SysMouse, &mouse_input, nullptr);
	if (result != DIRECT_INPUT_OK)
	{
		keyboard_input->Release();
		direct_input->Release();
		return result;
	}
	mouse_input->SetDataFormat(&c_dfDIMouse2);
	mouse_input->SetCooperativeLevel(engine->GetWindowHandler(), DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

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
void UI_Engine::ProceedInput(float time_interval)
{
	keyboard_input->GetDeviceState(256, keyboard_state);
	mouse_input->GetDeviceState(sizeof(mouse_state), &mouse_state);

	///<@fixme Wy³¹czenia aplikacji musi siê odbywaæ w jakiœ inny sposób. Powinien byæ jakiœ domyœlny mechanizm, ¿eby u¿ytkownik nie zapomnia³ zrobiæ wy³¹czania.
	if ( keyboard_state[DIK_ESCAPE] & 0x80 )
		engine->EndAplication();

	///<@fixme To jest hack i nie mam pojêcia jak to robiæ w wersji docelowej.
	if ( (keyboard_state[DIK_F1] & 0x80) && !pushedF1 )
	{
		pushedF1 = true;
		PRINT_STATISTICS( PERFORMANCE_STATISTICS_FILE_PATH );
	}
	if ( !(keyboard_state[DIK_F1] & 0x80) )
		pushedF1 = false;

	UpdateAbstractionLayer();
}

/** @brief Funkcja rysuj¹ca graficzny interfejs u¿ytkownika.
Jest wywo³ywana zaraz po wszystkich procedurach rysuj¹cych obiekty na scenie.

@param[in] time_interval Parametrem jest czas który up³yn¹³ od ostatniego wywo³ania.
 */
void UI_Engine::DrawGUI( float time_interval, float time_lag )
{

}


