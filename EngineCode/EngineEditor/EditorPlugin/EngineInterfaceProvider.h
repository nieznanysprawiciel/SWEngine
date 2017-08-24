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



/**@brief Udost�pnie wybranym klasom dost�p do wska�nika na g��wny obiekt silnika.*/
public struct EngineInterfaceProvider
{
	friend ref class EditorPlugin::EngineWrapper;

	static sw::EngineInterface*		GetEngine() { return engine; }

private:
	static sw::EngineInterface*		engine;
};



}	// sw



