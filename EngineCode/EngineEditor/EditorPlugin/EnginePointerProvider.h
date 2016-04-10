#pragma once


class Engine;
namespace EditorPlugin
{
	ref class EngineWrapper;
}

/**@brief Udostêpnie wybranym klasom dostêp do wskaŸnika na g³ówny obiekt silnika.*/
struct EnginePointerProvider
{
	friend ref class EditorPlugin::EngineWrapper;

	static Engine*		GetEngine()			{ return engine; }

private:
	static Engine*		engine;
};


