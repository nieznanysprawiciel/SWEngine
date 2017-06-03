/**
@file UIEngine.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "EngineCore/stdafx.h"
#include "UIEngine.h"
#include "EngineCore/MainEngine/Engine.h"
#include "EngineCore/Actors/ActorObjects.h"


#include "EngineCore/EngineHelpers/PerformanceCheck.h"

#include "swCommonLib/Common/MemoryLeaks.h"



namespace sw
{



/**@brief */
UIEngine::UIEngine( Engine* engine )
	: engine( engine )
{
	m_enableInput = true;
	m_inputModule = nullptr;

	InitAbstractionLayers();
}

/**@brief */
UIEngine::~UIEngine()
{
	for( unsigned int i = 0; i < m_abstractionLayers.size(); ++i )
		delete m_abstractionLayers[ i ];

	delete m_inputModule;
}

/**@brief Ustawia nowy modu³ do obs³ugi wejœcia u¿ytkownika.

@return Zwraca poprzedni modu³. Zwolnienie modu³u nale¿y do wo³aj¹cego.*/
input::IInput*			UIEngine::ChangeInputModule					( input::IInput* module )
{
	auto lastModule = m_inputModule;
	m_inputModule = module;

	return lastModule;
}

/**@brief Zwraca aktualnie u¿ywany modu³ do obs³ugi wejœcia u¿ytkownika.*/
input::IInput*			UIEngine::GetInputModule					()
{
	return m_inputModule;
}

/**@brief Funkcja umo¿liwia edytorowi wy³¹czenie inputu, gdy kontrolka odpowiedzialna
za wyœwietlanie renderowanego obrazu nie ma focusa.*/
void					UIEngine::EnableInput						( bool val )
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
void					UIEngine::ProceedInput						( float timeInterval )
{
	START_PERFORMANCE_CHECK( INPUT_PROCESSING );

	if( m_inputModule )
	{
		m_inputModule->Update( timeInterval );

		auto& keyboards = m_inputModule->GetKeyboardDevice();
		auto& mouses = m_inputModule->GetMouseDevice();
		auto& joysticks = m_inputModule->GetJoystickDevice();

		BuiltInProcessInput( keyboards, mouses, joysticks );
		UpdateAbstractionLayer( keyboards, mouses, joysticks );
	}

	END_PERFORMANCE_CHECK( INPUT_PROCESSING );
}

/**@brief Processes raw input before passing it to abstraction layers.
@note This is place for hacks. Maybe in future we could delete this function.*/
void					UIEngine::BuiltInProcessInput				( const std::vector< input::KeyboardDeviceOPtr >& keyboards,
																	  const std::vector< input::MouseDeviceOPtr >& mouses,
																	  const std::vector< input::JoystickDeviceOPtr >& joysticks )
{
	/// @todo To jest tymczasowy hack. Trzeba to obs³u¿yæ eventami.
	auto keyboardState = keyboards[ 0 ]->GetState();

	///<@fixme Wy³¹czenia aplikacji musi siê odbywaæ w jakiœ inny sposób. Powinien byæ jakiœ domyœlny mechanizm, ¿eby u¿ytkownik nie zapomnia³ zrobiæ wy³¹czania.
	if( keyboardState[ input::Keyboard::PhysicalKeys::KEY_ESCAPE ].IsPressed() )
		engine->EndAplication();

	///<@fixme To jest hack i nie mam pojêcia jak to robiæ w wersji docelowej.
	if( keyboardState[ input::Keyboard::PhysicalKeys::KEY_F1 ].IsKeyDownEvent() )
	{
		PRINT_STATISTICS( PERFORMANCE_STATISTICS_FILE_PATH );
	}

	if( keyboardState[ input::Keyboard::PhysicalKeys::KEY_F2 ].IsKeyDownEvent() )
	{
		PERFORMACE_CHECK_CLEAR_SAMPLES;
	}
}

/** @brief Funkcja rysuj¹ca graficzny interfejs u¿ytkownika.
Jest wywo³ywana zaraz po wszystkich procedurach rysuj¹cych obiekty na scenie.

@param[in] time_interval Parametrem jest czas który up³yn¹³ od ostatniego wywo³ania.
 */
void					UIEngine::DrawGUI							( float time_interval, float time_lag )
{

}


}	// sw


