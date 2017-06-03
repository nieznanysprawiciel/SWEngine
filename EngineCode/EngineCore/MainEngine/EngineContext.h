#pragma once

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



class Event;
class Command;

class Config;

class IGraphicAPIInitializer;

class Model3DFromFile;
struct MaterialObject;
class PixelShader;
class VertexShader;
class TextureObject;
class BufferObject;
class RenderTargetObject;
class InputAbstractionLayer;
class ShaderInputLayout;
class InputLayoutDescriptor;
struct RenderTargetDescriptor;
struct MaterialObject;

class MemoryChunk;

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
	WindowHandle				windowHandler;			///<Uchwyt okna aplikacji
	AppInstanceHandle			instanceHandler;		///<Uchwyt instancji procesu
	int							windowWidth;			///<Szerokoœæ okna/ekranu
	int							windowHeight;			///<Wysokoœæ okna/ekranu

	Config*						config;					///< Engine start configuration.

	IGraphicAPIInitializer*		graphicInitializer;		///<Obs³uguje u¿ywane API graficzne.

	//Modules
	ControllersEngine*			controllersEngine;		///<Kontroluje AI (klawiaturê trzeba przenieœæ)
	MovementEngine*				movementEngine;			///<Przelicza pozycjê obiektów w nastêpnej klatce
	DisplayEngine*				displayEngine;			///<Wyœwietla obiekty na scenie
	CollisionEngine*			collisionEngine;		///<Liczy kolizje
	PhysicEngine*				physicEngine;			///<Liczy oddzia³ywania fizyczne
	AssetsManager*				modelsManager;			///<Zarz¹dza modelami, teksturami i materia³ami
	FableEngine*				fableEngine;			///<Zarz¹dza fabu³¹ gry, interakcjami obiektów itd. Odpowiada za treœæ
	SoundEngine*				soundEngine;			///<Zarz¹dza muzyk¹ i dŸwiêkami
	sw::UIEngine*				ui_engine;				///<Interfejs u¿ytkownika (tak¿e graficzny)
	ActorsManager*				actorsManager;			///<Zarz¹dzanie aktorami.
	EventManager*				eventsManager;			///<Zarz¹dzanie eventami.

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


