#pragma once
/**@file DisplayEngine.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Plik zawiera deklaracjê klasy DisplayEngine i funkcje pomocnicze.
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

/**@brief Klasa odpowiedzialna za wyœwietlanie sceny, obs³ugê kamery oraz interpolacjê po³o¿eñ obiektów.
@ingroup EngineCore


Obiekty s¹ rozdzielone na kilka grup:
- Meshe instancjonowane
- Meshe dynamiczne
- Meshe dynamiczne o krótkim czasie ¿ycia
- Cz¹steczki
- Skyboxy, sky sphere'y, sky plane'y i inne obiekty s³u¿¹ce za t³o
Ka¿da z tych grup ma swój w³asny sposób reprezentacji i wyœwietlania ze wzglêdu na odmienny charakter
i inne mo¿liwoœci optymalizacji.
*/
class DisplayEngine
{
private:
	Engine*					engine;
	ModelsManager*			modelsManager;

	std::vector<IRenderer*>					m_renderers;					///< Zawiera wszystkie renderery. Ka¿dy odpowiada za jeden w¹tek renderuj¹cy.

	ConstantPerFrame						shader_data_per_frame;		///<Bufor sta³ych zmiennych co ramkê animacji.
	BufferObject*							m_constantsPerFrame;		///<Bufor sta³ych zmiennych co ramkê animacji.
	BufferObject*							m_constantsPerMesh;			///<Bufor sta³ych zmiennych dla ka¿dego fragmentu mesha.

	CameraActor*							current_camera;				///<Akutalnie aktywna kamera
	SkyDome*								sky_dome;					///<Klasa odpowiedzialna za kopu³ê nieba

	std::vector<DynamicMeshActor*>			meshes;						///<Modele nieanimowane
	DirectX::XMFLOAT4X4*					interpolated_matrixes;		///<Tablica macierzy interpolowanych po³o¿eñ obiektów
	unsigned int							interpol_matrixes_count;	///<Liczba macierzy interpolowanych

	std::vector<CameraActor*>				cameras;					///<Kontener zawieraj¹cy kamery

	ShaderInputLayoutObject*				defaultLayout;				///<@todo Hack. Zlikwidowaæ. Silnik powinien obs³ugiwaæ dowolne layouty, a przynajmniej jakiœ ustalony zbiór.
	RenderTargetObject*						m_mainRenderTarget;			///<Render target okna aplikacji. @todo W ostatecznej wersji powinien byæ render target ustawiany dla ka¿dego przebiegu.

	std::queue<RenderPass*>					m_renderOnceQueue;			///<Kolejka przebiegów, które maj¹ zostaæ wyrenderowane tylko raz.
	unsigned int							m_maxQueuedPassesPerFrame;	///<Maksymalna liczba przebiegów jaka zostanie wziêta z kolejki w ka¿dej ramce.
public:
	DisplayEngine( Engine* engine );
	~DisplayEngine();

	void InitRenderer( IRenderer* renderer );
	void InitDisplayer( ModelsManager* assetsManager );
	void BeginScene();
	void EndScene();

	void SetLayout							( ShaderInputLayoutObject* layout ) { defaultLayout = layout; }	///<@todo Hack. Zlikwidowaæ i zrobiæ docelowy sposób obs³ugi layotów.
	void SetMainRenderTarget				( RenderTargetObject* renderTarget );					///<@todo Hack. Zrobiæ docelowy sposób ustawiania render targetów.

	// G³ówna funkcja do wyœwietlania sceny
	void			DisplayScene					( float time_interval, float time_lag );
	// Funkcja do interpolacji pozycji obiektów
	void			InterpolatePositions			( float time_lag);
	void			SetProjectionMatrix				( float angle, float X_to_Y, float near_plane, float far_plane);

	// Zarz¹dzanie meshami
	void			AddDynamicMeshObject			( DynamicMeshActor* object );
	void			DeleteAllMeshes					();
	
	/// @todo Pobieranie meshy z DisplayEngine jest tymczasowe. Trzeba wymyœleæ docelowy mechanizm.
	std::vector<DynamicMeshActor*>		GetSceneObjects() { return meshes; }

	// Œwiat³a
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

	// Wyœwietlanie (funkcje wewnêtrzne)
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
@brief Funkcja interpoluje wartoœæ po³o¿enia obiektu.

@param[in] tima_lag Czas jaki up³yn¹³ od ostatniego przeliczenia pozycji obiektów.
@param[in] object Objekt, dla którego liczymy macierz przekszta³cenia.
@param[out] transform_matrix Zmienna, w której zostanie umieszczona interpolowana wartoœæ.
*/
inline void interpolate_position( float time_lag, const DynamicActor* object, DirectX::XMVECTOR& result_vector )
{
	DirectX::XMVECTOR position = object->GetPosition( );
	DirectX::XMVECTOR velocity = object->GetSpeed( );

	result_vector = DirectX::XMVectorScale( velocity, time_lag );
	result_vector = DirectX::XMVectorAdd( result_vector, position );
}

/**@deprecated
@brief Funkcja interpoluje orientacjê obiektu.

@param[in] tima_lag Czas jaki up³yn¹³ od ostatniego przeliczenia pozycji obiektów.
@param[in] object Objekt, dla którego liczymy macierz przekszta³cenia.
@param[out] transform_matrix Zmienna, w której zostanie umieszczona interpolowana wartoœæ.
*/
inline void interpolate_orientation( float time_lag, const DynamicActor* object, DirectX::XMVECTOR& result_vector )
{
	DirectX::XMVECTOR orientation = object->GetOrientation( );
	DirectX::XMVECTOR rotation_velocity = object->GetRotationSpeed( );

#ifdef _QUATERNION_SPEED
	//najpierw liczymy nowy kwaternion dla obrotu w czasie sekundy
	rotation_velocity = XMQuaternionMultiply( orientation, rotation_velocity );
	//teraz interpolujemy poprzedni¹ orientacjê i orientacjê po sekundzie
	//ze wspóczynnikiem równym czasowi jaki up³yn¹³ faktycznie
	result_vector = XMQuaternionSlerp( orientation, rotation_velocity, time_lag );

	/*Du¿o obliczeñ, mo¿e da siê to jakoœ za³atwiæ bez interpolacji...*/
#else


	if ( !DirectX::XMVector3Equal( rotation_velocity, DirectX::XMVectorZero() ) )
	{
		float rot_angle = DirectX::XMVectorGetW( rotation_velocity ) * time_lag;					// liczymy k¹t obrotu
		rotation_velocity = DirectX::XMQuaternionRotationAxis( rotation_velocity, rot_angle );	// przerabiamy na kwaternion
		result_vector = DirectX::XMQuaternionMultiply( orientation, rotation_velocity );			// liczymy nowy kwaternion orientacji
	}
	else
		result_vector = orientation;
#endif
}

/**@brief Funkcja odwraca wektor pozycji, aby by³ zwrócony w przeciwnym kierunku.
Potrzebne w momencie tworzenia macierzy widoku na podstawie po³o¿enia kamery.

@param[inout] result_vector Wektor pozycji. Nale¿y podaæ tutaj wektor do odwrócenia. Wynik zostanie zwrócony w tej zmiennej.
*/
inline void inverse_camera_position( DirectX::XMVECTOR& result_vector )
{
	result_vector = DirectX::XMVectorNegate( result_vector );
	//result_vector = XMVectorSetW( result_vector, 0.0 );		// sk³adowa W powinna byæ 0, ale funkcja XMMatrixTranslationFromVector na ni¹ wogóle nie patrzy
}

/**@brief Funkcja odwraca kwaternion orientacji, aby by³ zwrócony w przeciwnym kierunku.
Potrzebne w momencie tworzenia macierzy widoku na podstawie po³o¿enia kamery.

@param[inout] result_vector Kwaternion orientacji. Nale¿y podaæ tutaj wektor do odwrócenia. Wynik zostanie zwrócony w tej zmiennej.
*/
inline void inverse_camera_orientation( DirectX::XMVECTOR& result_vector )
{
	result_vector = DirectX::XMVectorNegate( result_vector );
	result_vector = DirectX::XMVectorSetW( result_vector, -DirectX::XMVectorGetW( result_vector ) );
}

