#pragma once


class Engine;
namespace EditorPlugin
{
	ref class EngineWrapper;
}

/**@brief Udost�pnie wybranym klasom dost�p do wska�nika na g��wny obiekt silnika.*/
struct EnginePointerProvider
{
	friend ref class EditorPlugin::EngineWrapper;

	static Engine*		GetEngine()			{ return engine; }

private:
	static Engine*		engine;
};


