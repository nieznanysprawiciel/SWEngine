#pragma once
/**
@file EngineContext.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "EngineCore/MainEngine/TimeManager.h"
#include "swCommonLib/Common/Multithreading/SpinLock.h"
#include "EngineCore/UIEngine/StandardAbstractionLayers.h"


#include <queue>
#include <string>

#include "EngineCore/PhysicEngine/CollisionEngine.h"
#include "EngineCore/ControllersEngine/ControllersEngine.h"
#include "EngineCore/GamePlay/FableEngine.h"
#include "EngineCore/DisplayEngine/DisplayEngine.h"
#include "EngineCore/ModelsManager/AssetsManager.h"
#include "EngineCore/PhysicEngine/PhysicEngine.h"
#include "EngineCore/PhysicEngine/MovementEngine.h"
#include "EngineCore/SoundEngine/SoundEngine.h"
#include "EngineCore/UIEngine/UIEngine.h"
#include "EngineCore/Actors/ActorsManager.h"
#include "EngineCore/EventsManager/EventManager.h"


#include "swInputLibrary/InputCore/IInput.h"


class MemoryChunk;

class PixelShader;
class VertexShader;
class TextureObject;
class BufferObject;
class RenderTargetObject;

class ShaderInputLayout;
class InputLayoutDescriptor;
struct RenderTargetDescriptor;

class IGraphicAPIInitializer;



namespace sw
{

class Event;
class Command;

class Config;

class InputAbstractionLayer;

class ActorBase;
class StaticActor;
class DynamicActor;
class RenderPassDepracated;

/**@brief Przechowuje wszystkie zmienne, które powinny znaleŸæ siê w klasie EngineInterface.
@ingroup EngineCore

Struktura jest potrzebna, ¿eby komendy mog³y mieæ dostêp do stanu silnika.
@see EngineInterfaceCommands
*/
struct EngineContext
{
	//directX and windows variables
	bool						engineReady;			///<Je¿eli zmienna jest niepoprawna, nie renderujemy

	bool						fullScreen;				///<Pe³ny ekran lub renderowanie w oknie
	WindowHandle				windowHandler;			///<Uchwyt okna aplikacji. @deprecated Get from native window instead.
	AppInstanceHandle			instanceHandler;		///<Uchwyt instancji procesu. @deprecated Get from native window instead.
	int							windowWidth;			///<Szerokoœæ okna/ekranu. @deprecated Get from native window instead.
	int							windowHeight;			///<Wysokoœæ okna/ekranu. @deprecated Get from native window instead.

	Config*						config;					///< Engine start configuration.

	IGraphicAPIInitializer*		graphicInitializer;

	//Modules
	ControllersEngine*			controllersEngine;
	MovementEngine*				movementEngine;
	DisplayEngine*				displayEngine;
	CollisionEngine*			collisionEngine;
	PhysicEngine*				physicEngine;
	AssetsManager*				modelsManager;
	FableEngine*				fableEngine;
	SoundEngine*				soundEngine;
	UIEngine*					ui_engine;
	ActorsManager*				actorsManager;
	EventManager*				eventsManager;

	/**@brief SpinLock do synchronizacji komunikacji miêdzy GamePlayem a silnikiem.

	SpinLock wykorzystuje aktywne oczekiwanie w dostêpie do zasobów. Z za³o¿enia operacje modyfikacji stanu silnika
	zachodz¹ rzadko i nie powinny byæ d³ugie. Lepiej jest poczekaæ w pêtli ni¿ zawieszaæ w¹tek za poœrednictwem
	systemu operacyjnego.

	Istnieje tylko jeden SpinLock, ale je¿eli pojawi sie mo¿liwoœæ logicznego wydzielenia niezale¿nych od siebie
	zadañ, to trzeba rozwa¿yæ dodanie kolejnych SpinLocków.*/
	SpinLock					engineLock;

	TimeManager					timeManager;	///<Obiekt do zarz¹dzania czasem.

	bool						pause;			///<Pauza

	//queue
	std::queue<Command*>		commandQueue;	///<Kolejka komend wykonywanych po zakoñczeniu wszystkich obliczeñ w danej klatce. Aktualizuje stan silnika.
	std::vector<ActorBase*>		objectList;
};

}	// sw

