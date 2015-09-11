#pragma once

/**@file DisplayEngine.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Plik zawiera deklaracjê klasy DisplayEngine i funkcje pomocnicze.
*/


#include "Interfaces/basic_interfaces.h"
#include "ConstantBuffersFormat.h"
#include "SkyDome.h"
#include "GraphicAPI/IRenderer.h"

void interpolate_position			( float time_lag, const DynamicObject* object, DirectX::XMVECTOR& result_vector );
void interpolate_orientation		( float time_lag, const DynamicObject* object, DirectX::XMVECTOR& result_vector );
void inverse_camera_position		( DirectX::XMVECTOR& result_vector );
void inverse_camera_orientation		( DirectX::XMVECTOR& result_vector );

class Engine;
class ModelsManager;
class BufferObject;

/**@brief Klasa odpowiedzialna za wyœwietlanie sceny, obs³ugê kamery oraz interpolacjê po³o¿eñ obiektów.

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

	CameraObject*							current_camera;				///<Akutalnie aktywna kamera
	SkyDome*								sky_dome;					///<Klasa odpowiedzialna za kopu³ê nieba

	std::vector<DynamicMeshObject*>			meshes;						///<Modele nieanimowane
	XMFLOAT4X4*								interpolated_matrixes;		///<Tablica macierzy interpolowanych po³o¿eñ obiektów
	unsigned int							interpol_matrixes_count;	///<Liczba macierzy interpolowanych

	std::vector<CameraObject*>				cameras;					///<Kontener zawieraj¹cy kamery
public:
	DisplayEngine(Engine* engine);
	~DisplayEngine();

	void InitRenderer( IRenderer* renderer );
	void InitDisplayer( ModelsManager* assetsManager );
	void BeginScene();
	void EndScene();

	// G³ówna funkcja do wyœwietlania sceny
	void display_scene						( float time_interval, float time_lag );
	// Funkcja do interpolacji pozycji obiektów
	void interpolate_positions				(float time_lag);
	void SetProjectionMatrix				(float angle, float X_to_Y, float near_plane, float far_plane);

	// Zarz¹dzanie meshami
	void add_dynamic_mesh_object			( DynamicMeshObject* object );

	// Œwiat³a
	int set_directional_light				( const DirectX::XMFLOAT4& direction, const DirectX::XMFLOAT4& color, unsigned int index );
	void set_ambient_light					( const DirectX::XMFLOAT4& color );

	// camera functions
	int add_camera							( CameraObject* camera );
	int set_current_camera					( CameraObject* camera );

	// SkyDome
	SkyDome* set_skydome					( SkyDome* dome );
private:
	void set_view_matrix					( float time_lag );

	void realocate_interpolation_memory		(unsigned int min = 1);
	void interpolate_object					( float time_lag, const DynamicObject* object, DirectX::XMFLOAT4X4* result_matrix );
	void interpolate_object2				( float time_lag, const DynamicObject* object, DirectX::XMFLOAT4X4* result_matrix );

	// Wyœwietlanie (funkcje wewnêtrzne)
	void display_instanced_meshes			( float time_interval, float time_lag );
	void display_dynamic_objects			( float time_interval, float time_lag );
	void display_particles					( float time_interval, float time_lag );
	void display_short_live_objects			( float time_interval, float time_lag );
	void display_sky_box					( float time_interval, float time_lag );
	void display_skeletons					( float time_interval, float time_lag );
	void display_self_drawing_objects		( float time_interval, float time_lag );

	// Funkcje pomocnicze do renderingu
	void set_textures						( const ModelPart& model );
	void set_index_buffer					( BufferObject* buffer );
	bool set_vertex_buffer					( BufferObject* buffer );
	void copy_material						( ConstantPerMesh* shader_data_per_mesh, const ModelPart* model );
	void depth_buffer_enable				( bool state );
};


/**@deprecated
@brief Funkcja interpoluje wartoœæ po³o¿enia obiektu.

@param[in] tima_lag Czas jaki up³yn¹³ od ostatniego przeliczenia pozycji obiektów.
@param[in] object Objekt, dla którego liczymy macierz przekszta³cenia.
@param[out] transform_matrix Zmienna, w której zostanie umieszczona interpolowana wartoœæ.
*/
inline void interpolate_position( float time_lag, const DynamicObject* object, DirectX::XMVECTOR& result_vector )
{
	XMVECTOR position = object->get_position( );
	XMVECTOR velocity = object->get_speed( );

	result_vector = velocity * time_lag + position;
}

/**@deprecated
@brief Funkcja interpoluje orientacjê obiektu.

@param[in] tima_lag Czas jaki up³yn¹³ od ostatniego przeliczenia pozycji obiektów.
@param[in] object Objekt, dla którego liczymy macierz przekszta³cenia.
@param[out] transform_matrix Zmienna, w której zostanie umieszczona interpolowana wartoœæ.
*/
inline void interpolate_orientation( float time_lag, const DynamicObject* object, DirectX::XMVECTOR& result_vector )
{
	XMVECTOR orientation = object->get_orientation( );
	XMVECTOR rotation_velocity = object->get_rotation_speed( );

#ifdef _QUATERNION_SPEED
	//najpierw liczymy nowy kwaternion dla obrotu w czasie sekundy
	rotation_velocity = XMQuaternionMultiply( orientation, rotation_velocity );
	//teraz interpolujemy poprzedni¹ orientacjê i orientacjê po sekundzie
	//ze wspóczynnikiem równym czasowi jaki up³yn¹³ faktycznie
	result_vector = XMQuaternionSlerp( orientation, rotation_velocity, time_lag );

	/*Du¿o obliczeñ, mo¿e da siê to jakoœ za³atwiæ bez interpolacji...*/
#else


	if ( !XMVector3Equal( rotation_velocity, XMVectorZero() ) )
	{
		float rot_angle = XMVectorGetW( rotation_velocity ) * time_lag;					// liczymy k¹t obrotu
		rotation_velocity = XMQuaternionRotationAxis( rotation_velocity, rot_angle );	// przerabiamy na kwaternion
		result_vector = XMQuaternionMultiply( orientation, rotation_velocity );			// liczymy nowy kwaternion orientacji
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
	result_vector = XMVectorNegate( result_vector );
	//result_vector = XMVectorSetW( result_vector, 0.0 );		// sk³adowa W powinna byæ 0, ale funkcja XMMatrixTranslationFromVector na ni¹ wogóle nie patrzy
}

/**@brief Funkcja odwraca kwaternion orientacji, aby by³ zwrócony w przeciwnym kierunku.
Potrzebne w momencie tworzenia macierzy widoku na podstawie po³o¿enia kamery.

@param[inout] result_vector Kwaternion orientacji. Nale¿y podaæ tutaj wektor do odwrócenia. Wynik zostanie zwrócony w tej zmiennej.
*/
inline void inverse_camera_orientation( DirectX::XMVECTOR& result_vector )
{
	result_vector = XMVectorNegate( result_vector );
	result_vector = XMVectorSetW( result_vector, -XMVectorGetW( result_vector ) );
}

