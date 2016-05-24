#include "EngineCore/stdafx.h"
#include "UI_Engine.h"
#include "EngineCore/MainEngine/Engine.h"
#include "EngineCore/Actors/ActorObjects.h"


#include "EngineCore/EngineHelpers/PerformanceCheck.h"
bool pushedF1 = false;		///< Hack. Zapami�tuje stan przycisku odpowiedzialnego za wys�anie statystyk do pliku, �eby nie by�o zdublowanych wywo�a�.


#include "Common/MemoryLeaks.h"

REGISTER_PERFORMANCE_CHECK( INPUT_PROCESSING );


/**@brief */
UI_Engine::UI_Engine( Engine* engine )
	: engine( engine )
{
	direct_input = nullptr;
	keyboard_input = nullptr;
	mouse_input = nullptr;

	m_enableInput = true;
	m_inputModule = nullptr;

	InitAbstractionLayers();
}

/**@brief */
UI_Engine::~UI_Engine()
{
	for ( unsigned int i = 0; i < m_abstractionLayers.size(); ++i )
		delete m_abstractionLayers[i];
	clean_direct_input();
}

/**@brief Ustawia nowy modu� do obs�ugi wej�cia u�ytkownika.

@return Zwraca poprzedni modu�. Zwolnienie modu�u nale�y do wo�aj�cego.*/
IInput*		UI_Engine::ChangeInputModule					( IInput* module )
{
	auto lastModule = m_inputModule;
	m_inputModule = module;

	return lastModule;
}

/**@brief Zwraca aktualnie u�ywany modu� do obs�ugi wej�cia u�ytkownika.*/
IInput*		UI_Engine::GetInputModule					()
{
	return m_inputModule;
}

/**@brief Funkcja umo�liwia edytorowi wy��czenie inputu, gdy kontrolka odpowiedzialna
za wy�wietlanie renderowanego obrazu nie ma focusa.*/
void		UI_Engine::EnableInput						( bool val )
{
	if( val )
	{
		m_enableInput = true;
		m_currentAbstractionLayer->SetActive( true );
	}
	else
	{
		m_enableInput = false;
		m_currentAbstractionLayer->SetActive( false );
	}
}

/**@brief Inicjalizuje obiekty Direct Inputa

@return Zwraca tak� warto�� jak funkcje Directinput.*/
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
void UI_Engine::ProceedInput( float timeInterval )
{
	START_PERFORMANCE_CHECK( INPUT_PROCESSING );

	if( m_inputModule )
	{
		m_inputModule->Update( timeInterval );

		auto keyboards = m_inputModule->GetKeyboardStates();
		auto mouses = m_inputModule->GetMouseStates();
		auto joysticks = m_inputModule->GetJoystickStates();

		/// @todo To jest tymczasowy hack. Trzeba to obs�u�y� eventami.
		auto keyboardState = keyboards[ 0 ]->GetKeyboardState();

		///<@fixme Wy��czenia aplikacji musi si� odbywa� w jaki� inny spos�b. Powinien by� jaki� domy�lny mechanizm, �eby u�ytkownik nie zapomnia� zrobi� wy��czania.
		if ( keyboardState[ KeyboardState::PHYSICAL_KEYS::KEY_ESCAPE ] & 0x80 )
			engine->EndAplication();

		///<@fixme To jest hack i nie mam poj�cia jak to robi� w wersji docelowej.
		if ( (keyboardState[ KeyboardState::PHYSICAL_KEYS::KEY_F1 ] & 0x80) && !pushedF1 )
		{
			pushedF1 = true;
			PRINT_STATISTICS( PERFORMANCE_STATISTICS_FILE_PATH );
		}
		if ( !(keyboardState[ KeyboardState::PHYSICAL_KEYS::KEY_F1 ] & 0x80) )
			pushedF1 = false;

		UpdateAbstractionLayer( keyboards, mouses, joysticks );
	}

	END_PERFORMANCE_CHECK( INPUT_PROCESSING );
}

/** @brief Funkcja rysuj�ca graficzny interfejs u�ytkownika.
Jest wywo�ywana zaraz po wszystkich procedurach rysuj�cych obiekty na scenie.

@param[in] time_interval Parametrem jest czas kt�ry up�yn�� od ostatniego wywo�ania.
 */
void UI_Engine::DrawGUI( float time_interval, float time_lag )
{

}


