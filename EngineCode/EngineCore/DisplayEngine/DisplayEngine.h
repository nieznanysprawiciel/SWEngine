#pragma once
/**@file DisplayEngine.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Plik zawiera deklaracj� klasy DisplayEngine i funkcje pomocnicze.
*/

#include "GraphicAPI/IRenderer.h"
#include "EngineCore/Actors/ActorObjects.h"
#include "ConstantBuffersFormat.h"
#include "EngineCore/Features/SkyDome.h"
#include "EngineCore/DisplayEngine/RenderPass.h"
#include <DirectXMath.h>

#include <queue>

void interpolate_position			( float time_lag, const DynamicActor* object, DirectX::XMVECTOR& result_vector );
void interpolate_orientation		( float time_lag, const DynamicActor* object, DirectX::XMVECTOR& result_vector );
void inverse_camera_position		( DirectX::XMVECTOR& result_vector );
void inverse_camera_orientation		( DirectX::XMVECTOR& result_vector );

class Engine;
class ModelsManager;
class BufferObject;

/**@brief Klasa odpowiedzialna za wy�wietlanie sceny, obs�ug� kamery oraz interpolacj� po�o�e� obiekt�w.
@ingroup EngineCore


Obiekty s� rozdzielone na kilka grup:
- Meshe instancjonowane
- Meshe dynamiczne
- Meshe dynamiczne o kr�tkim czasie �ycia
- Cz�steczki
- Skyboxy, sky sphere'y, sky plane'y i inne obiekty s�u��ce za t�o
Ka�da z tych grup ma sw�j w�asny spos�b reprezentacji i wy�wietlania ze wzgl�du na odmienny charakter
i inne mo�liwo�ci optymalizacji.
*/
class DisplayEngine
{
private:
	Engine*					engine;
	ModelsManager*			modelsManager;

	std::vector<IRenderer*>					m_renderers;					///< Zawiera wszystkie renderery. Ka�dy odpowiada za jeden w�tek renderuj�cy.

	ConstantPerFrame						shader_data_per_frame;		///<Bufor sta�ych zmiennych co ramk� animacji.
	BufferObject*							m_constantsPerFrame;		///<Bufor sta�ych zmiennych co ramk� animacji.
	BufferObject*							m_constantsPerMesh;			///<Bufor sta�ych zmiennych dla ka�dego fragmentu mesha.

	CameraActor*							current_camera;				///<Akutalnie aktywna kamera
	SkyDome*								sky_dome;					///<Klasa odpowiedzialna za kopu�� nieba

	std::vector<DynamicMeshActor*>			meshes;						///<Modele nieanimowane
	DirectX::XMFLOAT4X4*					interpolated_matrixes;		///<Tablica macierzy interpolowanych po�o�e� obiekt�w
	unsigned int							interpol_matrixes_count;	///<Liczba macierzy interpolowanych

	std::vector<CameraActor*>				cameras;					///<Kontener zawieraj�cy kamery

	ShaderInputLayoutObject*				defaultLayout;				///<@todo Hack. Zlikwidowa�. Silnik powinien obs�ugiwa� dowolne layouty, a przynajmniej jaki� ustalony zbi�r.
	RenderTargetObject*						m_mainRenderTarget;			///<Render target okna aplikacji. @todo W ostatecznej wersji powinien by� render target ustawiany dla ka�dego przebiegu.

	std::queue<RenderPass*>					m_renderOnceQueue;			///<Kolejka przebieg�w, kt�re maj� zosta� wyrenderowane tylko raz.
	unsigned int							m_maxQueuedPassesPerFrame;	///<Maksymalna liczba przebieg�w jaka zostanie wzi�ta z kolejki w ka�dej ramce.
public:
	DisplayEngine( Engine* engine );
	~DisplayEngine();

	void InitRenderer( IRenderer* renderer );
	void InitDisplayer( ModelsManager* assetsManager );
	void BeginScene();
	void EndScene();

	void SetLayout							( ShaderInputLayoutObject* layout ) { defaultLayout = layout; }	///<@todo Hack. Zlikwidowa� i zrobi� docelowy spos�b obs�ugi layot�w.
	void SetMainRenderTarget				( RenderTargetObject* renderTarget );					///<@todo Hack. Zrobi� docelowy spos�b ustawiania render target�w.

	// G��wna funkcja do wy�wietlania sceny
	void			DisplayScene					( float time_interval, float time_lag );
	// Funkcja do interpolacji pozycji obiekt�w
	void			InterpolatePositions			( float time_lag);
	void			SetProjectionMatrix				( float angle, float X_to_Y, float near_plane, float far_plane);

	// Zarz�dzanie meshami
	void			AddDynamicMeshObject			( DynamicMeshActor* object );
	void			DeleteAllMeshes					();
	
	/// @todo Pobieranie meshy z DisplayEngine jest tymczasowe. Trzeba wymy�le� docelowy mechanizm.
	std::vector<DynamicMeshActor*>		GetSceneObjects() { return meshes; }

	// �wiat�a
	int				SetDirectionalLight				( const DirectX::XMFLOAT4& direction, const DirectX::XMFLOAT4& color, unsigned int index );
	void			SetAmbientLight					( const DirectX::XMFLOAT4& color );

	// camera functions
	int				AddCamera						( CameraActor* camera );
	int				SetCurrentCamera				( CameraActor* camera );
	CameraActor*	GetCurrentCamera				();

	// Renderowanie
	void			RenderOnce						( RenderPass* pass )	{	m_renderOnceQueue.push( pass );	}

	// SkyDome
	SkyDome*		SetSkydome						( SkyDome* dome );
private:
	void SetViewMatrix						( float time_lag );

	void realocate_interpolation_memory		(unsigned int min = 1);
	void interpolate_object					( float timeLag, const DynamicActor* object, DirectX::XMFLOAT4X4* result_matrix );
	void interpolate_object2				( float timeLag, const DynamicActor* object, DirectX::XMFLOAT4X4* result_matrix );

	// Wy�wietlanie (funkcje wewn�trzne)
	void DisplayInstancedMeshes				( float timeInterval, float timeLag );
	void DisplayDynamicObjects				( float timeInterval, float timeLag );
	void DisplayParticles					( float timeInterval, float timeLag );
	void DisplayShortLiveObjects			( float timeInterval, float timeLag );
	void DisplaySkyBox						( float timeInterval, float timeLag );
	void DisplaySkeletons					( float timeInterval, float timeLag );
	void DisplaySelfDrawingObjects			( float timeInterval, float timeLag );

	void RenderFromQueue					( float timeInterval, float timeLag );

	// Funkcje pomocnicze do renderingu
	void SetTextures						( const ModelPart& model );
	void SetIndexBuffer						( BufferObject* buffer );
	bool SetVertexBuffer					( BufferObject* buffer );
	void CopyMaterial						( ConstantPerMesh* shader_data_per_mesh, const ModelPart* model );
	void DepthBufferEnable					( bool state );
};


/**@deprecated
@brief Funkcja interpoluje warto�� po�o�enia obiektu.

@param[in] tima_lag Czas jaki up�yn�� od ostatniego przeliczenia pozycji obiekt�w.
@param[in] object Objekt, dla kt�rego liczymy macierz przekszta�cenia.
@param[out] transform_matrix Zmienna, w kt�rej zostanie umieszczona interpolowana warto��.
*/
inline void interpolate_position( float time_lag, const DynamicActor* object, DirectX::XMVECTOR& result_vector )
{
	DirectX::XMVECTOR position = object->GetPosition( );
	DirectX::XMVECTOR velocity = object->GetSpeed( );

	result_vector = DirectX::XMVectorScale( velocity, time_lag );
	result_vector = DirectX::XMVectorAdd( result_vector, position );
}

/**@deprecated
@brief Funkcja interpoluje orientacj� obiektu.

@param[in] tima_lag Czas jaki up�yn�� od ostatniego przeliczenia pozycji obiekt�w.
@param[in] object Objekt, dla kt�rego liczymy macierz przekszta�cenia.
@param[out] transform_matrix Zmienna, w kt�rej zostanie umieszczona interpolowana warto��.
*/
inline void interpolate_orientation( float time_lag, const DynamicActor* object, DirectX::XMVECTOR& result_vector )
{
	DirectX::XMVECTOR orientation = object->GetOrientation( );
	DirectX::XMVECTOR rotation_velocity = object->GetRotationSpeed( );

#ifdef _QUATERNION_SPEED
	//najpierw liczymy nowy kwaternion dla obrotu w czasie sekundy
	rotation_velocity = XMQuaternionMultiply( orientation, rotation_velocity );
	//teraz interpolujemy poprzedni� orientacj� i orientacj� po sekundzie
	//ze wsp�czynnikiem r�wnym czasowi jaki up�yn�� faktycznie
	result_vector = XMQuaternionSlerp( orientation, rotation_velocity, time_lag );

	/*Du�o oblicze�, mo�e da si� to jako� za�atwi� bez interpolacji...*/
#else


	if ( !DirectX::XMVector3Equal( rotation_velocity, DirectX::XMVectorZero() ) )
	{
		float rot_angle = DirectX::XMVectorGetW( rotation_velocity ) * time_lag;					// liczymy k�t obrotu
		rotation_velocity = DirectX::XMQuaternionRotationAxis( rotation_velocity, rot_angle );	// przerabiamy na kwaternion
		result_vector = DirectX::XMQuaternionMultiply( orientation, rotation_velocity );			// liczymy nowy kwaternion orientacji
	}
	else
		result_vector = orientation;
#endif
}

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

