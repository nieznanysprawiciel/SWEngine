#pragma once
/**@file EngineInterface.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Plik zawiera deklaracjê klasy EngineInterface.*/

#include "EngineCore/MainEngine/TimeManager.h"
#include "Common/Multithreading/SpinLock.h"

#include <queue>
#include <string>


class ControllersEngine;
struct IndexPrediction;
class Event;
class Engine;

class ControllersEngine;
class ModelsManager;
class PhysicEngine;
class FableEngine;
class MovementEngine;
class DisplayEngine;
class CollisionEngine;
class SoundEngine;
class UI_Engine;
class Object;
class IGraphicAPIInitializer;

class Model3DFromFile;
struct MaterialObject;
class PixelShaderObject;
class VertexShaderObject;
class TextureObject;
class RenderTargetObject;


/**@brief Klasa jest interfejsem dla u¿ytkownika u¿ytkuj¹cego silnik.

Taki interfejs bêdzie widzia³ u¿ytkownik silnika z wnêtrza swoich klas odpowiedzialnych za fabu³ê.
U¿ytkownik musi mieæ ograniczone mo¿liwoœci, ¿eby przypadkiem nie namiesza³.
Szczególnie nie powinien mieæ mo¿liwoœci wysy³ania eventów, bo mog³oby to doprowadziæ do zapêtlenia.
Wszystkie funkcje jakie powinien mieæ dostêpne nale¿y deklarowaæ w tej klasie w sekcji publicznej.
Wszystkie pozosta³e funkcje powinny siê znaleŸæ w deklaracji klasy Engine.
Wszystkie zmienne silnika powinny byæ deklarowane na tym poziomie, aby równie¿ metody tej klasy dostêpne
dla u¿ytkownika mia³y do nich dostêp.

Funkcje s¹ pogrupowane tematycznie przy pomocy klas zagnie¿d¿onych.
*/
class EngineInterface
{
protected:
	//directX and windows variables
	bool						m_engineReady;			///<Je¿eli zmienna jest niepoprawna, nie renderujemy

	bool						full_screen;			///<Pe³ny ekran lub renderowanie w oknie
	HWND						window_handler;			///<Uchwyt okna aplikacji
	HINSTANCE					instance_handler;		///<Uchwyt instancji procesu
	int							window_width;			///<Szerokoœæ okna/ekranu
	int							window_height;			///<Wysokoœæ okna/ekranu

	IGraphicAPIInitializer*		m_graphicInitializer;	///<Obs³uguje u¿ywane API graficzne.

	//Modules
	ControllersEngine*			controllers_engine;		///<Kontroluje AI (klawiaturê trzeba przenieœæ)
	MovementEngine*				movement_engine;		///<Przelicza pozycjê obiektów w nastêpnej klatce
	DisplayEngine*				display_engine;			///<Wyœwietla obiekty na scenie
	CollisionEngine*			collision_engine;		///<Liczy kolizje
	PhysicEngine*				physic_engine;			///<Liczy oddzia³ywania fizyczne
	ModelsManager*				models_manager;			///<Zarz¹dza modelami, teksturami i materia³ami
	FableEngine*				fable_engine;			///<Zarz¹dza fabu³¹ gry, interakcjami obiektów itd. Odpowiada za treœæ
	SoundEngine*				sound_engine;			///<Zarz¹dza muzyk¹ i dŸwiêkami
	UI_Engine*					ui_engine;				///<Interfejs u¿ytkownika (tak¿e graficzny)

	/**@brief SpinLock do synchronizacji komunikacji miêdzy GamePlayem a silnikiem.
	
	SpinLock wykorzystuje aktywne oczekiwanie w dostêpie do zasobów. Z za³o¿enia operacje modyfikacji stanu silnika
	zachodz¹ rzadko i nie powinny byæ d³ugie. Lepiej jest poczekaæ w pêtli ni¿ zawieszaæ w¹tek za poœrednictwem
	systemu operacyjnego.

	Istnieje tylko jeden SpinLock, ale je¿eli pojawi sie mo¿liwoœæ logicznego wydzielenia niezale¿nych od siebie
	zadañ, to trzeba rozwa¿yæ dodanie kolejnych SpinLocków.*/
	SpinLock					m_engineAccess;

	//Objects
	//std::vector<IndexPrediction>		index_predictor;

	TimeManager					m_timeManager;	///<Obiekt do zarz¹dzania czasem.

	bool						pause;			///<Pauza

	//queue
	std::queue<Event*>*			events_queue;	///<WskaŸnik na kolejke komunikatów w celu szybszego dostêpu

protected:
	~EngineInterface(){};

	//FableEngine - ta klasa jest dostêpna bezpoœrednio z IGamePlay

private:
	class Assets;
	class Actors;

};


class EngineInterface::Actors
{
private:
	Engine*			m_engine;
protected:
public:
	Actors( Engine* engine )
		: m_engine( engine )
	{}
	~Actors() = default;
public:

};



class EngineInterface::Assets
{
private:
	Engine*			m_engine;
protected:
public:
	Assets( Engine* engine )
		: m_engine( engine )
	{}
	~Assets() = default;
public:
	Model3DFromFile*		GetModel				( const std::wstring& name );
	VertexShaderObject*		GetVertexShader			( const std::wstring& name );

};

