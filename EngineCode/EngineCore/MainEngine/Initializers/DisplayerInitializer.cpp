/**@file DisplayerInitializer.cpp
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Inicjalizacja modu��w do renderowania.*/


#include "EngineCore/stdafx.h"

#include "EngineCore/MainEngine/Engine.h"
#include "EngineCore/MainEngine/EngineContext.h"
#include "EngineCore/DisplayEngine/DisplayEngine.h"
#include "swGraphicAPI/Resources/ResourcesFactory.h"

#include "swCommonLib/Common/MemoryLeaks.h"


/**@brief Inicjalizuje DisplayEngine.

Funkcja tworzy renderery, domy�lne bufory sta�ych oraz ustawia macierz projekcji.
@return Zwraca zawsze true.*/
bool Engine::InitDisplayer()
{
	IRenderer* renderer = Context->graphicInitializer->CreateRenderer( RendererUsage::USE_AS_IMMEDIATE );
	Context->displayEngine->InitRenderer( renderer );
	Context->displayEngine->InitDisplayer( Context->modelsManager );

	return true;
}

