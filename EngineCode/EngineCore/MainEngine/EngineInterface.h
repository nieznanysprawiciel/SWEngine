#pragma once
/**@file EngineInterface.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Plik zawiera deklaracj� klasy EngineInterface.*/

#include "EngineCore/MainEngine/TimeManager.h"
#include "Common/Multithreading/SpinLock.h"
#include "EngineCore/UIEngine/StandardAbstractionLayers.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <queue>
#include <string>


class ControllersEngine;
struct IndexPrediction;
class Event;
class Command;
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
class IGraphicAPIInitializer;

class Model3DFromFile;
struct MaterialObject;
class PixelShaderObject;
class VertexShaderObject;
class TextureObject;
class BufferObject;
class RenderTargetObject;
class InputAbstractionLayer;
class ShaderInputLayoutObject;
class InputLayoutDescriptor;
struct RenderTargetDescriptor;
struct MaterialObject;

class MemoryChunk;

class Object;
class DynamicMeshObject;
class RenderPass;

/**@brief Przechowuje wszystkie zmienne, kt�re powinny znale�� si� w klasie EngineInterface.

Struktura jest potrzebna, �eby komendy mog�y mie� dost�p do stanu silnika.
@see EngineInterfaceCommands
*/
struct EngineContext
{
	//directX and windows variables
	bool						engineReady;			///<Je�eli zmienna jest niepoprawna, nie renderujemy

	bool						fullScreen;				///<Pe�ny ekran lub renderowanie w oknie
	HWND						windowHandler;			///<Uchwyt okna aplikacji
	HINSTANCE					instanceHandler;		///<Uchwyt instancji procesu
	int							windowWidth;			///<Szeroko�� okna/ekranu
	int							windowHeight;			///<Wysoko�� okna/ekranu

	IGraphicAPIInitializer*		graphicInitializer;		///<Obs�uguje u�ywane API graficzne.

	//Modules
	ControllersEngine*			controllersEngine;		///<Kontroluje AI (klawiatur� trzeba przenie��)
	MovementEngine*				movementEngine;			///<Przelicza pozycj� obiekt�w w nast�pnej klatce
	DisplayEngine*				displayEngine;			///<Wy�wietla obiekty na scenie
	CollisionEngine*			collisionEngine;		///<Liczy kolizje
	PhysicEngine*				physicEngine;			///<Liczy oddzia�ywania fizyczne
	ModelsManager*				modelsManager;			///<Zarz�dza modelami, teksturami i materia�ami
	FableEngine*				fableEngine;			///<Zarz�dza fabu�� gry, interakcjami obiekt�w itd. Odpowiada za tre��
	SoundEngine*				soundEngine;			///<Zarz�dza muzyk� i d�wi�kami
	UI_Engine*					ui_engine;				///<Interfejs u�ytkownika (tak�e graficzny)

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
	std::queue<Event*>*			eventsQueue;	///<Wska�nik na kolejk� komunikat�w w celu szybszego dost�pu
	std::queue<Command*>		commandQueue;	///<Kolejka komend wykonywanych po zako�czeniu wszystkich oblicze� w danej klatce. Aktualizuje stan silnika.


#ifdef __TEST
	std::vector<Object*>	object_list;
#endif
};


/**@brief Klasa jest interfejsem dla u�ytkownika u�ytkuj�cego silnik.

Taki interfejs b�dzie widzia� u�ytkownik silnika z wn�trza swoich klas odpowiedzialnych za fabu��.
U�ytkownik musi mie� ograniczone mo�liwo�ci, �eby przypadkiem nie namiesza�.

Funkcje s� pogrupowane tematycznie przy pomocy klas zagnie�d�onych.
Wszystkie funkcje jakie powinien mie� dost�pne nale�y deklarowa� w�asnie w klasach zagnie�d�onych.
Zasadniczo klasa EngineInterface nie powinna mie� opr�cz tego �adnych funkcji API.

Zmienne i sk�adniki silnika nale�y deklarowa� w strukturze EngineContext.
Dzi�ki temu b�dzie do nich dost�p z poziomu komend, poniewa�
ka�da komenda dostaje referencj� na t� struktur�.
@see EngineInterfaceCommands, Command

Funkcje umieszczone w sekcji prywatnej klasy EngineInterface b�d� mog�y by� wywo�ane z klas
zagnie�d�onych, ale nie b�dzie do nich dost�pu w komendach.
*/
class EngineInterface
{
protected:
	EngineContext		Context;		///<Zawiera wska�niki na wszystkie modu�y i zmienne silnika.

protected:
	EngineInterface();
	~EngineInterface() { };

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

			Model3DFromFile*		GetSync( const std::wstring& name );
			Model3DFromFile*		LoadSync( const std::wstring& name );
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

			VertexShaderObject*		GetVertexShaderSync( const std::wstring& name );
			VertexShaderObject*		LoadVertexShaderSync( const std::wstring& name );
			VertexShaderObject*		LoadVertexShaderSync( const std::wstring& name, ShaderInputLayoutObject** layout, InputLayoutDescriptor* layoutDesc );
			PixelShaderObject*		GetPixelShaderSync( const std::wstring& name );
			PixelShaderObject*		LoadPixelShaderSync( const std::wstring& name );
		};
		/**@brief Zapewnia dost�p do tekstur w klasie ModelsManager.*/
		class Textures : public InterfaceGroup
		{
		private:
		protected:
		public:
			Textures() = default;
			~Textures() = default;

			TextureObject*			GetTexture( const std::wstring& name );
		};
		/**@brief Zapewnia dost�p do bufor�w w klasie ModelsManager.*/
		class Buffers : public InterfaceGroup
		{
		private:
		protected:
		public:
			Buffers() = default;
			~Buffers() = default;

			BufferObject*			CreateVertexBufferSync( const std::wstring& name, MemoryChunk& data, unsigned int vertCount );
		};
		/**@brief Zapewnia dost�p do render target�w w klasie ModelsManager.*/
		class RenderTargets : public InterfaceGroup
		{
		private:
		protected:
		public:
			RenderTargets() = default;
			~RenderTargets() = default;

			RenderTargetObject*		CreateSync( const std::wstring& name, const RenderTargetDescriptor& renderTargetDescriptor );
		};
		/**@brief */
		class Materials
		{
		private:
		protected:
		public:
			Materials() = default;
			~Materials() = default;

			MaterialObject*			GetSync( const std::wstring& name );
		};

#pragma endregion
	protected:
	public:
		Assets() = default;
		~Assets() = default;
	public:
		Models			models;
		Animations		animations;
		Sprites			sprites;
		Shaders			shaders;
		Textures		textures;
		Buffers			buffers;
		RenderTargets	renderTargets;
		Materials		materials;
	};

	/**@brief Zawiera funkcje do zarz�dzania aktorami.*/
	class Actors : public InterfaceGroup
	{
	private:
#pragma region EngineInterface.Actors
		/**@brief Pozwala ustawia� assety u�ywane przez aktor�w.*/
		class Assets : public InterfaceGroup
		{
		private:
		protected:
		public:
			Assets() = default;
			~Assets() = default;

			void		ChangeTextures( DynamicMeshObject* mesh, TextureObject* newTex, uint16 beginPart, uint16 endPart, uint8 texIndex );

			void		ChangePixelShaders( DynamicMeshObject* mesh, PixelShaderObject* newShader, uint16 beginPart, uint16 endPart );
			void		ChangeVertexShaders( DynamicMeshObject* mesh, VertexShaderObject* newShader, uint16 beginPart, uint16 endPart );
		};
#pragma endregion
	protected:
	public:
		Actors() = default;
		~Actors() = default;
	public:
		std::vector<DynamicMeshObject*>		GetSceneObjects();	///<@todo Zlikwidowa� GetSceneObjects. Stworzone tylko dla LightmapsTool.

		void								AddDynamicMesh	( DynamicMeshObject* object );
		void								CleanScene		();
	public:
		Assets		assets;			///< Pozwala na ustawianie asset�w aktorom.
	};

	/**@brief Zawiera funkcje do obs�ugi wej�cia u�ytkownika.*/
	class Input : public InterfaceGroup
	{
	private:
	protected:
	public:
		Input() = default;
		~Input() = default;
	public:
		InputAbstractionLayer*		GetStandardAbstractionLayer( STANDARD_ABSTRACTION_LAYER layer );
		void						SetAbstractionLayer( InputAbstractionLayer* layer );
		void						SetStandardAbstractionLayer( STANDARD_ABSTRACTION_LAYER layer );
	};

	/**@brief Funkcje do obs�ugi renderowania.*/
	class Rendering : public InterfaceGroup
	{
	private:
	protected:
	public:
		Rendering() = default;
		~Rendering() = default;
	public:
		void		RenderOnce( RenderPass* );
	};

#pragma endregion


public:
	Assets			assets;		///<Funkcje do zarz�dzania assetami.
	Actors			actors;		///<Funkcje do zarz�dzania aktorami.
	Input			input;		///<Funkcje do zarz�dzania wej�ciem od u�ytkownika.
	Rendering		rendering;	///<Funkcje do zarzadzania renderowaniem.
};


