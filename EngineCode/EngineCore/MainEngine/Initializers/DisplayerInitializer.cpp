/**@file DisplayerInitializer.cpp
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Inicjalizacja modu³ów do renderowania.*/


#include "EngineCore/stdafx.h"

#include "EngineCore/MainEngine/Engine.h"
#include "EngineCore/MainEngine/EngineContext.h"
#include "EngineCore/DisplayEngine/DisplayEngine.h"
#include "GraphicAPI/ResourcesFactory.h"


/**@brief Inicjalizuje DisplayEngine.

Funkcja tworzy renderery, domyœlne bufory sta³ych oraz ustawia macierz projekcji.
@return Zwraca zawsze true.*/
bool Engine::InitDisplayer()
{
	IRenderer* renderer = Context->graphicInitializer->CreateRenderer( RendererUsage::USE_AS_IMMEDIATE );
	Context->displayEngine->InitRenderer( renderer );
	Context->displayEngine->InitDisplayer( Context->modelsManager );

	Context->displayEngine->SetProjectionMatrix( DirectX::XMConvertToRadians( 45 ),
										   (float)Context->windowWidth / (float)Context->windowHeight, 1, 100000 );

	return true;
}

