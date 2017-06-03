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

/**@brief Ustawia nowy modu� do obs�ugi wej�cia u�ytkownika.

@return Zwraca poprzedni modu�. Zwolnienie modu�u nale�y do wo�aj�cego.*/
input::IInput*			UIEngine::ChangeInputModule					( input::IInput* module )
{
	auto lastModule = m_inputModule;
	m_inputModule = module;

	return lastModule;
}

/**@brief Zwraca aktualnie u�ywany modu� do obs�ugi wej�cia u�ytkownika.*/
input::IInput*			UIEngine::GetInputModule					()
{
	return m_inputModule;
}

/**@brief Funkcja umo�liwia edytorowi wy��czenie inputu, gdy kontrolka odpowiedzialna
za wy�wietlanie renderowanego obrazu nie ma focusa.*/
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


///@brief Funkcja wywo�ywana przez klas� Engine w ka�dym obiegu g��wnej petli programu.
///Przechwytujemy klawiatur�, wykonujemy wszytkie funkcje obs�ugi obiekt�w sterowanych.
///@param[in] timeInterval Parametrem jest czas kt�ry up�yn�� od ostatniego wywo�ania
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
	/// @todo To jest tymczasowy hack. Trzeba to obs�u�y� eventami.
	auto keyboardState = keyboards[ 0 ]->GetState();

	///<@fixme Wy��czenia aplikacji musi si� odbywa� w jaki� inny spos�b. Powinien by� jaki� domy�lny mechanizm, �eby u�ytkownik nie zapomnia� zrobi� wy��czania.
	if( keyboardState[ input::Keyboard::PhysicalKeys::KEY_ESCAPE ].IsPressed() )
		engine->EndAplication();

	///<@fixme To jest hack i nie mam poj�cia jak to robi� w wersji docelowej.
	if( keyboardState[ input::Keyboard::PhysicalKeys::KEY_F1 ].IsKeyDownEvent() )
	{
		PRINT_STATISTICS( PERFORMANCE_STATISTICS_FILE_PATH );
	}

	if( keyboardState[ input::Keyboard::PhysicalKeys::KEY_F2 ].IsKeyDownEvent() )
	{
		PERFORMACE_CHECK_CLEAR_SAMPLES;
	}
}

/** @brief Funkcja rysuj�ca graficzny interfejs u�ytkownika.
Jest wywo�ywana zaraz po wszystkich procedurach rysuj�cych obiekty na scenie.

@param[in] time_interval Parametrem jest czas kt�ry up�yn�� od ostatniego wywo�ania.
 */
void					UIEngine::DrawGUI							( float time_interval, float time_lag )
{

}


}	// sw


