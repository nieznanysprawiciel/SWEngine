#pragma once

/**@file DisplayEngine.h
@brief Plik zawiera deklaracjê klasy DisplayEngine.
*/

#include "..\..\stdafx.h"
#include "..\..\Interfaces\basic_interfaces.h"
#include "ConstantBuffersFormat.h"
#include "SkyDome.h"



class Engine;

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
class DisplayEngine : public DX11_constant_buffers_container
{
private:
	Engine* engine;

	ConstantPerFrame				shader_data_per_frame;		///<Bufor sta³ych zmiennych co ramkê animacji
	Camera_object*					current_camera;				///<Akutalnie aktywna kamera
	SkyDome*						sky_dome;					///<Klasa odpowiedzialna za kopu³ê nieba

	std::vector<Dynamic_mesh_object*>		meshes;					///<Modele nieanimowane
	XMFLOAT4X4*								interpolated_matrixes;	///<Tablica macierzy interpolowanych po³o¿eñ obiektów
	unsigned int							interpol_matrixes_count;///<Liczba macierzy interpolowanych

	std::vector<Camera_object*>				cameras;			///<Kontener zawieraj¹cy kamery
public:
	DisplayEngine(Engine* engine);
	~DisplayEngine();

	void init_const_buffers();

	// G³ówna funkcja do wyœwietlania sceny
	void display_scene(float time_interval);
	// Funkcja do interpolacji pozycji obiektów
	void interpolate_positions(float time_lag);
	void set_projection_matrix(float angle, float X_to_Y, float near_plane, float far_plane);

	// Zarz¹dzanie meshami
	void add_dynamic_mesh_object( Dynamic_mesh_object* object );

	// Œwiat³a
	int set_directional_light( const DirectX::XMFLOAT4& direction, const DirectX::XMFLOAT4& color, unsigned int index );
	void set_ambient_light( const DirectX::XMFLOAT4& color );

	// camera functions
	int add_camera( Camera_object* camera );
	int set_current_camera( Camera_object* camera );

	// SkyDome
	SkyDome* set_skydome( SkyDome* dome );
private:
	void set_view_matrix();

	void realocate_interpolation_memory(unsigned int min = 1);

	// Wyœwietlanie (funkcje wewnêtrzne)
	void display_instanced_meshes( float time_interval );
	void display_dynamic_objects( float time_interval );
	void display_particles( float time_interval );
	void display_short_live_objects( float time_interval );
	void display_sky_box( float time_interval );
	void display_skeletons( float time_interval );

	// Funkcje pomocnicze do renderingu
	void set_textures( const ModelPart& model );
	void set_index_buffer( BufferObject* buffer );
	bool set_vertex_buffer( BufferObject* buffer );
	void copy_material( ConstantPerMesh* shader_data_per_mesh, const ModelPart* model );
	void depth_buffer_enable( bool state );
};

