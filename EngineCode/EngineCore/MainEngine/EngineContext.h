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

/**@brief Przechowuje wszystkie zmienne, kt�re powinny znale�� si� w klasie EngineInterface.
@ingroup EngineCore

Struktura jest potrzebna, �eby komendy mog�y mie� dost�p do stanu silnika.
@see EngineInterfaceCommands
*/
struct EngineContext
{
	//directX and windows variables
	bool						engineReady;			///<Je�eli zmienna jest niepoprawna, nie renderujemy

	bool						fullScreen;				///<Pe�ny ekran lub renderowanie w oknie
	WindowHandle				windowHandler;			///<Uchwyt okna aplikacji
	AppInstanceHandle			instanceHandler;		///<Uchwyt instancji procesu
	int							windowWidth;			///<Szeroko�� okna/ekranu
	int							windowHeight;			///<Wysoko�� okna/ekranu

	Config*						config;					///< Engine start configuration.

	IGraphicAPIInitializer*		graphicInitializer;		///<Obs�uguje u�ywane API graficzne.

	//Modules
	ControllersEngine*			controllersEngine;		///<Kontroluje AI (klawiatur� trzeba przenie��)
	MovementEngine*				movementEngine;			///<Przelicza pozycj� obiekt�w w nast�pnej klatce
	DisplayEngine*				displayEngine;			///<Wy�wietla obiekty na scenie
	CollisionEngine*			collisionEngine;		///<Liczy kolizje
	PhysicEngine*				physicEngine;			///<Liczy oddzia�ywania fizyczne
	AssetsManager*				modelsManager;			///<Zarz�dza modelami, teksturami i materia�ami
	FableEngine*				fableEngine;			///<Zarz�dza fabu�� gry, interakcjami obiekt�w itd. Odpowiada za tre��
	SoundEngine*				soundEngine;			///<Zarz�dza muzyk� i d�wi�kami
	sw::UIEngine*				ui_engine;				///<Interfejs u�ytkownika (tak�e graficzny)
	ActorsManager*				actorsManager;			///<Zarz�dzanie aktorami.
	EventManager*				eventsManager;			///<Zarz�dzanie eventami.

	/**@brief SpinLock do synchronizacji komunikacji mi�dzy GamePlayem a silnikiem.

	SpinLock wykorzystuje aktywne oczekiwanie w dost�pie do zasob�w. Z za�o�enia operacje modyfikacji stanu silnika
	zachodz� rzadko i nie powinny by� d�ugie. Lepiej jest poczeka� w p�tli ni� zawiesza� w�tek za po�rednictwem
	systemu operacyjnego.

	Istnieje tylko jeden SpinLock, ale je�eli pojawi sie mo�liwo�� logicznego wydzielenia niezale�nych od siebie
	zada�, to trzeba rozwa�y� dodanie kolejnych SpinLock�w.*/
	SpinLock					engineLock;

	TimeManager					timeManager;	///<Obiekt do zarz�dzania czasem.

	bool						pause;			///<Pauza

	//queue
	std::queue<Command*>		commandQueue;	///<Kolejka komend wykonywanych po zako�czeniu wszystkich oblicze� w danej klatce. Aktualizuje stan silnika.
	std::vector<ActorBase*>		objectList;
};


