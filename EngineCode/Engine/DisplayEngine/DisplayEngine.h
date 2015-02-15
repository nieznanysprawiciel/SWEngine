#pragma once

/**@file DisplayEngine.h
@brief Plik zawiera deklaracj� klasy DisplayEngine.
*/

#include "..\..\stdafx.h"
#include "..\..\Interfaces\basic_interfaces.h"
#include "ConstantBuffersFormat.h"
#include "SkyDome.h"



class Engine;

/**@brief Klasa odpowiedzialna za wy�wietlanie sceny, obs�ug� kamery oraz interpolacj� po�o�e� obiekt�w.

Obiekty s� rozdzielone na kilka grup:
- Meshe instancjonowane
- Meshe dynamiczne
- Meshe dynamiczne o kr�tkim czasie �ycia
- Cz�steczki
- Skyboxy, sky sphere'y, sky plane'y i inne obiekty s�u��ce za t�o
Ka�da z tych grup ma sw�j w�asny spos�b reprezentacji i wy�wietlania ze wzgl�du na odmienny charakter
i inne mo�liwo�ci optymalizacji.
*/
class DisplayEngine : public DX11_constant_buffers_container
{
private:
	Engine* engine;

	ConstantPerFrame				shader_data_per_frame;		///<Bufor sta�ych zmiennych co ramk� animacji
	Camera_object*					current_camera;				///<Akutalnie aktywna kamera
	SkyDome*						sky_dome;					///<Klasa odpowiedzialna za kopu�� nieba

	std::vector<Dynamic_mesh_object*>		meshes;					///<Modele nieanimowane
	XMFLOAT4X4*								interpolated_matrixes;	///<Tablica macierzy interpolowanych po�o�e� obiekt�w
	unsigned int							interpol_matrixes_count;///<Liczba macierzy interpolowanych

	std::vector<Camera_object*>				cameras;			///<Kontener zawieraj�cy kamery
public:
	DisplayEngine(Engine* engine);
	~DisplayEngine();

	void init_const_buffers();

	// G��wna funkcja do wy�wietlania sceny
	void display_scene(float time_interval);
	// Funkcja do interpolacji pozycji obiekt�w
	void interpolate_positions(float time_lag);
	void set_projection_matrix(float angle, float X_to_Y, float near_plane, float far_plane);

	// Zarz�dzanie meshami
	void add_dynamic_mesh_object( Dynamic_mesh_object* object );

	// �wiat�a
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

	// Wy�wietlanie (funkcje wewn�trzne)
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

