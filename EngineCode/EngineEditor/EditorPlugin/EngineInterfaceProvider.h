#pragma once
/**
@file EngineInterfaceProvider.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "EngineCore/MainEngine/EngineInterface.h"



namespace sw
{

class EngineInterface;
namespace EditorPlugin
{
	ref class EngineWrapper;
}



/**@brief Udostêpnie wybranym klasom dostêp do wskaŸnika na g³ówny obiekt silnika.*/
public struct EngineInterfaceProvider
{
	friend ref class EditorPlugin::EngineWrapper;

	static sw::EngineInterface*		GetEngine() { return engine; }

private:
	static sw::EngineInterface*		engine;
};



}	// sw



