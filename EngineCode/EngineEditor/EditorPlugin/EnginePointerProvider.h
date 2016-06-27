#pragma once

#include "EngineCore/MainEngine/Engine.h"

class Engine;
namespace EditorPlugin
{
	ref class EngineWrapper;
}

/**@brief Udost�pnie wybranym klasom dost�p do wska�nika na g��wny obiekt silnika.*/
public struct EnginePointerProvider
{
	friend ref class EditorPlugin::EngineWrapper;

	static Engine*		GetEngine()			{ return engine; }

private:
	static Engine*		engine;
};


