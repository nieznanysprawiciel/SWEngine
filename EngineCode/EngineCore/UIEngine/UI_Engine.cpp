#include "EngineCore/stdafx.h"
#include "UI_Engine.h"
#include "EngineCore/MainEngine/Engine.h"
#include "EngineCore/Actors/ActorObjects.h"


#include "EngineCore/EngineHelpers/PerformanceCheck.h"
bool pushedF1 = false;		///< Hack. Zapamiêtuje stan przycisku odpowiedzialnego za wys³anie statystyk do pliku, ¿eby nie by³o zdublowanych wywo³añ.


#include "Common/MemoryLeaks.h"

REGISTER_PERFORMANCE_CHECK( INPUT_PROCESSING );


/**@brief */
UI_Engine::UI_Engine( Engine* engine )
	: engine( engine )
{
	m_enableInput = true;
	m_inputModule = nullptr;

	InitAbstractionLayers();
}

/**@brief */
UI_Engine::~UI_Engine()
{
	for ( unsigned int i = 0; i < m_abstractionLayers.size(); ++i )
		delete m_abstractionLayers[i];

	delete m_inputModule;
}

/**@brief Ustawia nowy modu³ do obs³ugi wejœcia u¿ytkownika.

@return Zwraca poprzedni modu³. Zwolnienie modu³u nale¿y do wo³aj¹cego.*/
IInput*		UI_Engine::ChangeInputModule					( IInput* module )
{
	auto lastModule = m_inputModule;
	m_inputModule = module;

	return lastModule;
}

/**@brief Zwraca aktualnie u¿ywany modu³ do obs³ugi wejœcia u¿ytkownika.*/
IInput*		UI_Engine::GetInputModule					()
{
	return m_inputModule;
}

/**@brief Funkcja umo¿liwia edytorowi wy³¹czenie inputu, gdy kontrolka odpowiedzialna
za wyœwietlanie renderowanego obrazu nie ma focusa.*/
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


///@brief Funkcja wywo³ywana przez klasê Engine w ka¿dym obiegu g³ównej petli programu.
///Przechwytujemy klawiaturê, wykonujemy wszytkie funkcje obs³ugi obiektów sterowanych.
///@param[in] timeInterval Parametrem jest czas który up³yn¹³ od ostatniego wywo³ania
void UI_Engine::ProceedInput( float timeInterval )
{
	START_PERFORMANCE_CHECK( INPUT_PROCESSING );

	if( m_inputModule )
	{
		m_inputModule->Update( timeInterval );

		auto& keyboards = m_inputModule->GetKeyboardStates();
		auto& mouses = m_inputModule->GetMouseStates();
		auto& joysticks = m_inputModule->GetJoystickStates();

		/// @todo To jest tymczasowy hack. Trzeba to obs³u¿yæ eventami.
		auto keyboardState = keyboards[ 0 ]->GetKeyboardState();

		///<@fixme Wy³¹czenia aplikacji musi siê odbywaæ w jakiœ inny sposób. Powinien byæ jakiœ domyœlny mechanizm, ¿eby u¿ytkownik nie zapomnia³ zrobiæ wy³¹czania.
		if ( keyboardState[ KeyboardState::PHYSICAL_KEYS::KEY_ESCAPE ] & 0x80 )
			engine->EndAplication();

		///<@fixme To jest hack i nie mam pojêcia jak to robiæ w wersji docelowej.
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

/** @brief Funkcja rysuj¹ca graficzny interfejs u¿ytkownika.
Jest wywo³ywana zaraz po wszystkich procedurach rysuj¹cych obiekty na scenie.

@param[in] time_interval Parametrem jest czas który up³yn¹³ od ostatniego wywo³ania.
 */
void UI_Engine::DrawGUI( float time_interval, float time_lag )
{

}


