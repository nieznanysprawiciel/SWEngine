#pragma once
/**@file EngineInterface.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Plik zawiera deklaracj� klasy EngineInterface.*/

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


/**@brief Klasa jest interfejsem dla u�ytkownika u�ytkuj�cego silnik.

Taki interfejs b�dzie widzia� u�ytkownik silnika z wn�trza swoich klas odpowiedzialnych za fabu��.
U�ytkownik musi mie� ograniczone mo�liwo�ci, �eby przypadkiem nie namiesza�.
Szczeg�lnie nie powinien mie� mo�liwo�ci wysy�ania event�w, bo mog�oby to doprowadzi� do zap�tlenia.
Wszystkie funkcje jakie powinien mie� dost�pne nale�y deklarowa� w tej klasie w sekcji publicznej.
Wszystkie pozosta�e funkcje powinny si� znale�� w deklaracji klasy Engine.
Wszystkie zmienne silnika powinny by� deklarowane na tym poziomie, aby r�wnie� metody tej klasy dost�pne
dla u�ytkownika mia�y do nich dost�p.

Funkcje s� pogrupowane tematycznie przy pomocy klas zagnie�d�onych.
*/
class EngineInterface
{
protected:
	//directX and windows variables
	bool						m_engineReady;			///<Je�eli zmienna jest niepoprawna, nie renderujemy

	bool						full_screen;			///<Pe�ny ekran lub renderowanie w oknie
	HWND						window_handler;			///<Uchwyt okna aplikacji
	HINSTANCE					instance_handler;		///<Uchwyt instancji procesu
	int							window_width;			///<Szeroko�� okna/ekranu
	int							window_height;			///<Wysoko�� okna/ekranu

	IGraphicAPIInitializer*		m_graphicInitializer;	///<Obs�uguje u�ywane API graficzne.

	//Modules
	ControllersEngine*			controllers_engine;		///<Kontroluje AI (klawiatur� trzeba przenie��)
	MovementEngine*				movement_engine;		///<Przelicza pozycj� obiekt�w w nast�pnej klatce
	DisplayEngine*				display_engine;			///<Wy�wietla obiekty na scenie
	CollisionEngine*			collision_engine;		///<Liczy kolizje
	PhysicEngine*				physic_engine;			///<Liczy oddzia�ywania fizyczne
	ModelsManager*				models_manager;			///<Zarz�dza modelami, teksturami i materia�ami
	FableEngine*				fable_engine;			///<Zarz�dza fabu�� gry, interakcjami obiekt�w itd. Odpowiada za tre��
	SoundEngine*				sound_engine;			///<Zarz�dza muzyk� i d�wi�kami
	UI_Engine*					ui_engine;				///<Interfejs u�ytkownika (tak�e graficzny)

	/**@brief SpinLock do synchronizacji komunikacji mi�dzy GamePlayem a silnikiem.
	
	SpinLock wykorzystuje aktywne oczekiwanie w dost�pie do zasob�w. Z za�o�enia operacje modyfikacji stanu silnika
	zachodz� rzadko i nie powinny by� d�ugie. Lepiej jest poczeka� w p�tli ni� zawiesza� w�tek za po�rednictwem
	systemu operacyjnego.

	Istnieje tylko jeden SpinLock, ale je�eli pojawi sie mo�liwo�� logicznego wydzielenia niezale�nych od siebie
	zada�, to trzeba rozwa�y� dodanie kolejnych SpinLock�w.*/
	SpinLock					m_engineAccess;

	//Objects
	//std::vector<IndexPrediction>		index_predictor;

	TimeManager					m_timeManager;	///<Obiekt do zarz�dzania czasem.

	bool						pause;			///<Pauza

	//queue
	std::queue<Event*>*			events_queue;	///<Wska�nik na kolejke komunikat�w w celu szybszego dost�pu

protected:
	EngineInterface();
	~EngineInterface(){};

	//FableEngine - ta klasa jest dost�pna bezpo�rednio z IGamePlay

private:
#pragma region EngineInterface.UserInterface

/**@brief Klasa bazowa dla klas zagnie�d�onych w EngineInterface s�u��cych do grupowania
funkcji interfejsu dla u�ytkownika (klasy GamePlay i obiekty aktor�w).

Klasa posiada statyczny wska�nik na Engine. Dzi�ki temu nie trzeba go przekazywa�
do ka�dej klasy z osobna. Dodatkowo mo�na w tej klasie umieszcza� wska�niki na inne
obiekty, kt�re wymagaj� bezpo�redniego dost�pu.*/
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

/**@brief Zawiera funkcje do zarz�dzania assetami.*/
class Assets : public InterfaceGroup
{
private:
#pragma region EngineInterface.Assets.NestedClasses
/**@brief Zapewnia dost�p do modeli w klasie ModelsManager.*/
class Models : public InterfaceGroup
{
private:
protected:
public:
	Models() = default;
	~Models() = default;

};
/**@brief Zapewnia dost�p do animacji w klasie ModelsManager.*/
class Animations : public InterfaceGroup
{
private:
protected:
public:
	Animations() = default;
	~Animations() = default;

};
/**@brief Zapewnia dost�p do sprit�w w klasie ModelsManager.*/
class Sprites : public InterfaceGroup
{
private:
protected:
public:
	Sprites() = default;
	~Sprites() = default;

};
/**@brief Zapewnia dost�p do shader�w w klasie ModelsManager.*/
class Shaders : public InterfaceGroup
{
private:
protected:
public:
	Shaders() = default;
	~Shaders() = default;

};
/**@brief Zapewnia dost�p do tekstur w klasie ModelsManager.*/
class Textures : public InterfaceGroup
{
private:
protected:
public:
	Textures() = default;
	~Textures() = default;

};
/**@brief Zapewnia dost�p do bufor�w w klasie ModelsManager.*/
class Buffers : public InterfaceGroup
{
private:
protected:
public:
	Buffers() = default;
	~Buffers() = default;

};
/**@brief Zapewnia dost�p do render target�w w klasie ModelsManager.*/
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

/**@brief Zawiera funkcje do zarz�dzania aktorami.*/
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


