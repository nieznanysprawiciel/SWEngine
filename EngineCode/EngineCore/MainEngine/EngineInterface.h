#pragma once
/**@file EngineInterface.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Plik zawiera deklaracjê klasy EngineInterface.*/

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

/**@brief Przechowuje wszystkie zmienne, które powinny znaleŸæ siê w klasie EngineInterface.

Struktura jest potrzebna, ¿eby komendy mog³y mieæ dostêp do stanu silnika.
@see EngineInterfaceCommands
*/
struct EngineContext
{
	//directX and windows variables
	bool						engineReady;			///<Je¿eli zmienna jest niepoprawna, nie renderujemy

	bool						fullScreen;				///<Pe³ny ekran lub renderowanie w oknie
	HWND						windowHandler;			///<Uchwyt okna aplikacji
	HINSTANCE					instanceHandler;		///<Uchwyt instancji procesu
	int							windowWidth;			///<Szerokoœæ okna/ekranu
	int							windowHeight;			///<Wysokoœæ okna/ekranu

	IGraphicAPIInitializer*		graphicInitializer;		///<Obs³uguje u¿ywane API graficzne.

	//Modules
	ControllersEngine*			controllersEngine;		///<Kontroluje AI (klawiaturê trzeba przenieœæ)
	MovementEngine*				movementEngine;			///<Przelicza pozycjê obiektów w nastêpnej klatce
	DisplayEngine*				displayEngine;			///<Wyœwietla obiekty na scenie
	CollisionEngine*			collisionEngine;		///<Liczy kolizje
	PhysicEngine*				physicEngine;			///<Liczy oddzia³ywania fizyczne
	ModelsManager*				modelsManager;			///<Zarz¹dza modelami, teksturami i materia³ami
	FableEngine*				fableEngine;			///<Zarz¹dza fabu³¹ gry, interakcjami obiektów itd. Odpowiada za treœæ
	SoundEngine*				soundEngine;			///<Zarz¹dza muzyk¹ i dŸwiêkami
	UI_Engine*					ui_engine;				///<Interfejs u¿ytkownika (tak¿e graficzny)

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
	std::queue<Event*>*			eventsQueue;	///<WskaŸnik na kolejkê komunikatów w celu szybszego dostêpu
	std::queue<Command*>		commandQueue;	///<Kolejka komend wykonywanych po zakoñczeniu wszystkich obliczeñ w danej klatce. Aktualizuje stan silnika.


#ifdef __TEST
	std::vector<Object*>	object_list;
#endif
};


/**@brief Klasa jest interfejsem dla u¿ytkownika u¿ytkuj¹cego silnik.

Taki interfejs bêdzie widzia³ u¿ytkownik silnika z wnêtrza swoich klas odpowiedzialnych za fabu³ê.
U¿ytkownik musi mieæ ograniczone mo¿liwoœci, ¿eby przypadkiem nie namiesza³.

Funkcje s¹ pogrupowane tematycznie przy pomocy klas zagnie¿d¿onych.
Wszystkie funkcje jakie powinien mieæ dostêpne nale¿y deklarowaæ w³asnie w klasach zagnie¿d¿onych.
Zasadniczo klasa EngineInterface nie powinna mieæ oprócz tego ¿adnych funkcji API.

Zmienne i sk³adniki silnika nale¿y deklarowaæ w strukturze EngineContext.
Dziêki temu bêdzie do nich dostêp z poziomu komend, poniewa¿
ka¿da komenda dostaje referencjê na tê strukturê.
@see EngineInterfaceCommands, Command

Funkcje umieszczone w sekcji prywatnej klasy EngineInterface bêd¹ mog³y byæ wywo³ane z klas
zagnie¿d¿onych, ale nie bêdzie do nich dostêpu w komendach.
*/
class EngineInterface
{
protected:
	EngineContext		Context;		///<Zawiera wskaŸniki na wszystkie modu³y i zmienne silnika.

protected:
	EngineInterface();
	~EngineInterface() { };

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

			Model3DFromFile*		GetSync( const std::wstring& name );
			Model3DFromFile*		LoadSync( const std::wstring& name );
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

			VertexShaderObject*		GetVertexShaderSync( const std::wstring& name );
			VertexShaderObject*		LoadVertexShaderSync( const std::wstring& name );
			VertexShaderObject*		LoadVertexShaderSync( const std::wstring& name, ShaderInputLayoutObject** layout, InputLayoutDescriptor* layoutDesc );
			PixelShaderObject*		GetPixelShaderSync( const std::wstring& name );
			PixelShaderObject*		LoadPixelShaderSync( const std::wstring& name );
		};
		/**@brief Zapewnia dostêp do tekstur w klasie ModelsManager.*/
		class Textures : public InterfaceGroup
		{
		private:
		protected:
		public:
			Textures() = default;
			~Textures() = default;

			TextureObject*			GetTexture( const std::wstring& name );
		};
		/**@brief Zapewnia dostêp do buforów w klasie ModelsManager.*/
		class Buffers : public InterfaceGroup
		{
		private:
		protected:
		public:
			Buffers() = default;
			~Buffers() = default;

			BufferObject*			CreateVertexBufferSync( const std::wstring& name, MemoryChunk& data, unsigned int vertCount );
		};
		/**@brief Zapewnia dostêp do render targetów w klasie ModelsManager.*/
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

	/**@brief Zawiera funkcje do zarz¹dzania aktorami.*/
	class Actors : public InterfaceGroup
	{
	private:
#pragma region EngineInterface.Actors
		/**@brief Pozwala ustawiaæ assety u¿ywane przez aktorów.*/
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
		std::vector<DynamicMeshObject*>		GetSceneObjects();	///<@todo Zlikwidowaæ GetSceneObjects. Stworzone tylko dla LightmapsTool.

		void								AddDynamicMesh	( DynamicMeshObject* object );
		void								CleanScene		();
	public:
		Assets		assets;			///< Pozwala na ustawianie assetów aktorom.
	};

	/**@brief Zawiera funkcje do obs³ugi wejœcia u¿ytkownika.*/
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

	/**@brief Funkcje do obs³ugi renderowania.*/
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
	Assets			assets;		///<Funkcje do zarz¹dzania assetami.
	Actors			actors;		///<Funkcje do zarz¹dzania aktorami.
	Input			input;		///<Funkcje do zarz¹dzania wejœciem od u¿ytkownika.
	Rendering		rendering;	///<Funkcje do zarzadzania renderowaniem.
};


