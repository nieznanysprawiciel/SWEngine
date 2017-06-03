#pragma once
/**@file DisplayEngine.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Plik zawiera deklaracj� klasy DisplayEngine i funkcje pomocnicze.
*/

#include "GraphicAPI/IRenderer.h"
#include "GraphicAPI/SwapChain.h"

#include "EngineCore/Actors/ActorObjects.h"
#include "EngineCore/Actors/Extended/SkyDome.h"
#include "EngineCore/DisplayEngine/RenderPass.h"		///@deprecated
#include "EngineCore/DisplayEngine/RenderPasses/RenderPassFactory.h"

#include "EngineCore/DisplayEngine/RenderPasses/IRenderingLogicPass.h"

#include "ConstantBuffersFormat.h"
#include "LightModule.h"

#include <DirectXMath.h>

#include <queue>



/**@defgroup Rendering Rendering
@brief Object pipeline.

@ingroup EngineCore*/



void inverse_camera_position		( DirectX::XMVECTOR& result_vector );
void inverse_camera_orientation		( DirectX::XMVECTOR& result_vector );

class Engine;
class AssetsManager;
class BufferObject;

/**@brief Klasa odpowiedzialna za wy�wietlanie sceny, obs�ug� kamery oraz interpolacj� po�o�e� obiekt�w.
@ingroup EngineCore
@ingroup Rendering*/
class DisplayEngine
{
private:
	Engine*					engine;
	AssetsManager*			modelsManager;

	std::vector<IRenderer*>					m_renderers;				///< Zawiera wszystkie renderery. Ka�dy odpowiada za jeden w�tek renderuj�cy.

	ResourcePtr< BufferObject >				m_cameraConstants;			///< Camera constants buffer.
	ResourcePtr< BufferObject >				m_transformConstants;		///< Transform constants buffer.
	ResourcePtr< BufferObject >				m_materialConstants;		///< Material constants buffer. @todo Temporary.

	CameraActor*							m_currentCamera;			///<Akutalnie aktywna kamera
	SkyDome*								sky_dome;					///<Klasa odpowiedzialna za kopu�� nieba

	std::vector< StaticActor* >				meshes;						///<Modele nieanimowane
	std::vector< DirectX::XMFLOAT4X4 >		m_interpolatedMatricies;		///<Tablica macierzy interpolowanych po�o�e� obiekt�w

	std::vector<CameraActor*>				cameras;					///<Kontener zawieraj�cy kamery

	ShaderInputLayout*						defaultLayout;				///<@todo Hack. Zlikwidowa�. Silnik powinien obs�ugiwa� dowolne layouty, a przynajmniej jaki� ustalony zbi�r.
	CameraActor*							m_defaultCamera;			///< Domy�lna kamera u�ywana tylko, jezeli uzytkownik nie ustawi w�asnej.

	ResourcePtr< RenderTargetObject >		m_mainRenderTarget;			///<Render target okna aplikacji. @todo W ostatecznej wersji powinien by� render target ustawiany dla ka�dego przebiegu.
	SwapChain*								m_mainSwapChain;

	std::queue<RenderPassDepracated*>		m_renderOnceQueue;			///<Kolejka przebieg�w, kt�re maj� zosta� wyrenderowane tylko raz.
	unsigned int							m_maxQueuedPassesPerFrame;	///<Maksymalna liczba przebieg�w jaka zostanie wzi�ta z kolejki w ka�dej ramce.

	Ptr< IRenderingLogicPass >				m_mainPass;					///< Collection of rendering passes.
	RenderPassFactory						m_passFactory;				///< Factory for creating render passes.

public:
	LightModule*							lightModule;				///< Light module.

public:
	DisplayEngine( Engine* engine );
	~DisplayEngine();

	void InitRenderer( IRenderer* renderer );
	void InitDisplayer( AssetsManager* assetsManager );
	void BeginScene();
	void EndScene();

	void SetLayout							( ShaderInputLayout* layout ) { defaultLayout = layout; }	///<@todo Hack. Zlikwidowa� i zrobi� docelowy spos�b obs�ugi layot�w.
	void SetMainRenderTarget				( RenderTargetObject* renderTarget );					///<@todo Hack. Zrobi� docelowy spos�b ustawiania render target�w.

	// G��wna funkcja do wy�wietlania sceny
	void			DisplayScene					( float time_interval, float time_lag );
	// Funkcja do interpolacji pozycji obiekt�w
	void			InterpolatePositions			( float time_lag);

	// Zarz�dzanie meshami
	void			AddMeshObject					( StaticActor* object );
	void			RemoveActor						( ActorBase* actor );
	void			RemoveAllActors					();
	void			DeleteAllMeshes					();
	
	/// @todo Pobieranie meshy z DisplayEngine jest tymczasowe. Trzeba wymy�le� docelowy mechanizm.
	std::vector<StaticActor*>			GetSceneObjects	() { return meshes; }
	const std::vector<StaticActor*>&	GetMeshes		() { return meshes; }

	// �wiat�a
	/// Light module is resposible for all lights manipulations.
	LightModule*	GetLightModule					();

	// camera functions
	int				AddCamera						( CameraActor* camera );
	int				SetCurrentCamera				( CameraActor* camera );
	CameraActor*	GetCurrentCamera				();

	// Renderowanie
	void			RenderOnce						( RenderPassDepracated* pass )	{	m_renderOnceQueue.push( pass );	}

	// SkyDome
	SkyDome*		SetSkydome						( SkyDome* dome );
private:
	void			UpdateCameraBuffer				( IRenderer* renderer, float timeInterval, float timeLag );

	void realocate_interpolation_memory		( unsigned int min = 1 );
	void interpolate_object					( float timeLag, const StaticActor* object, DirectX::XMFLOAT4X4* result_matrix );
	void interpolate_object2				( float timeLag, const StaticActor* object, DirectX::XMFLOAT4X4* result_matrix );

	// Display functions
	void			DisplaySceneOld			( float timeInterval, float timeLag );
	void			DisplayDynamicObjects	( float timeInterval, float timeLag );
	void			DisplaySkyBox			( float timeInterval, float timeLag );
	void			ProcessMainPass			( float timeInterval, float timeLag );

	void			RenderFromQueue			( float timeInterval, float timeLag );
	RenderContext	CreateRenderContext		( float timeInterval, float timeLag );

};



/**@brief Funkcja odwraca wektor pozycji, aby by� zwr�cony w przeciwnym kierunku.
Potrzebne w momencie tworzenia macierzy widoku na podstawie po�o�enia kamery.

@param[inout] result_vector Wektor pozycji. Nale�y poda� tutaj wektor do odwr�cenia. Wynik zostanie zwr�cony w tej zmiennej.
*/
inline void inverse_camera_position( DirectX::XMVECTOR& result_vector )
{
	result_vector = DirectX::XMVectorNegate( result_vector );
	//result_vector = XMVectorSetW( result_vector, 0.0 );		// sk�adowa W powinna by� 0, ale funkcja XMMatrixTranslationFromVector na ni� wog�le nie patrzy
}

/**@brief Funkcja odwraca kwaternion orientacji, aby by� zwr�cony w przeciwnym kierunku.
Potrzebne w momencie tworzenia macierzy widoku na podstawie po�o�enia kamery.

@param[inout] result_vector Kwaternion orientacji. Nale�y poda� tutaj wektor do odwr�cenia. Wynik zostanie zwr�cony w tej zmiennej.
*/
inline void inverse_camera_orientation( DirectX::XMVECTOR& result_vector )
{
	result_vector = DirectX::XMVectorNegate( result_vector );
	result_vector = DirectX::XMVectorSetW( result_vector, -DirectX::XMVectorGetW( result_vector ) );
}

