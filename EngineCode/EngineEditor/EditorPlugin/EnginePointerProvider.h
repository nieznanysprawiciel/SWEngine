#pragma once

#include "EngineCore/MainEngine/Engine.h"

class Engine;
namespace EditorPlugin
{
	ref class EngineWrapper;
}

/**@brief Udostêpnie wybranym klasom dostêp do wskaŸnika na g³ówny obiekt silnika.*/
public struct EnginePointerProvider
{
	friend ref class EditorPlugin::EngineWrapper;

	static Engine*		GetEngine()			{ return engine; }

private:
	static Engine*		engine;
};


