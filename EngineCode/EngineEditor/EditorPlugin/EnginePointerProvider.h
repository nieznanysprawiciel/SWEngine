#pragma once
/**
@file EnginePointerProvider.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "EngineCore/MainEngine/Engine.h"



namespace sw
{

class Engine;
namespace EditorPlugin
{
	ref class EngineWrapper;
}



/**@brief Udostêpnie wybranym klasom dostêp do wskaŸnika na g³ówny obiekt silnika.*/
public struct EnginePointerProvider
{
	friend ref class EditorPlugin::EngineWrapper;

	static sw::Engine*		GetEngine() { return engine; }

private:
	static sw::Engine*		engine;
};



}	// sw

