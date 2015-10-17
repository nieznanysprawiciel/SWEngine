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
	EngineInterface();
	~EngineInterface(){};

	//FableEngine - ta klasa jest dostêpna bezpoœrednio z IGamePlay

private:
#pragma region EngineInterface.UserInterface

/**@brief Klasa bazowa dla klas zagnie¿d¿onych w EngineInterface s³u¿¹cych do grupowania
funkcji interfejsu dla u¿ytkownika (klasy GamePlay i obiekty aktorów).

Klasa posiada statyczny wskaŸnik na Engine. Dziêki temu nie trzeba go przekazywaæ
do ka¿dej klasy z osobna. Dodatkowo mo¿na w tej klasie umieszczaæ wskaŸniki na inne
obiekty, które wymagaj¹ bezpoœredniego dostêpu.*/
class InterfaceGroup
{
	friend class EngineInterface;
private:
protected:
	static Engine*			m_engine;
public:
	InterfaceGroup() = default;
	~InterfaceGroup() = default;
};

/**@brief Zawiera funkcje do zarz¹dzania assetami.*/
class Assets : public InterfaceGroup
{
private:
#pragma region EngineInterface.Assets.NestedClasses
/**@brief Zapewnia dostêp do modeli w klasie ModelsManager.*/
class Models : public InterfaceGroup
{
private:
protected:
public:
	Models() = default;
	~Models() = default;

};
/**@brief Zapewnia dostêp do animacji w klasie ModelsManager.*/
class Animations : public InterfaceGroup
{
private:
protected:
public:
	Animations() = default;
	~Animations() = default;

};
/**@brief Zapewnia dostêp do spritów w klasie ModelsManager.*/
class Sprites : public InterfaceGroup
{
private:
protected:
public:
	Sprites() = default;
	~Sprites() = default;

};
/**@brief Zapewnia dostêp do shaderów w klasie ModelsManager.*/
class Shaders : public InterfaceGroup
{
private:
protected:
public:
	Shaders() = default;
	~Shaders() = default;

};
/**@brief Zapewnia dostêp do tekstur w klasie ModelsManager.*/
class Textures : public InterfaceGroup
{
private:
protected:
public:
	Textures() = default;
	~Textures() = default;

};
/**@brief Zapewnia dostêp do buforów w klasie ModelsManager.*/
class Buffers : public InterfaceGroup
{
private:
protected:
public:
	Buffers() = default;
	~Buffers() = default;

};
/**@brief Zapewnia dostêp do render targetów w klasie ModelsManager.*/
class RenderTargets : public InterfaceGroup
{
private:
protected:
public:
	RenderTargets() = default;
	~RenderTargets() = default;

};
#pragma endregion
protected:
public:
	Assets() = default;
	~Assets() = default;
public:
	Model3DFromFile*		GetModel				( const std::wstring& name );
	VertexShaderObject*		GetVertexShader			( const std::wstring& name );

	Models			models;
	Animations		animations;
	Sprites			sprites;
	Shaders			shaders;
	Textures		textures;
	Buffers			buffers;
	RenderTargets	renderTargets;
};

/**@brief Zawiera funkcje do zarz¹dzania aktorami.*/
class Actors : public InterfaceGroup
{
private:
protected:
public:
	Actors() = default;
	~Actors() = default;
public:

};

#pragma endregion

public:
	Assets			assets;
	Actors			actors;

};


