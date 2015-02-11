#pragma once

/**@file DisplayEngine.h
@brief Plik zawiera deklaracjê klasy DisplayEngine.
*/

#include "..\..\stdafx.h"
#include "..\..\Interfaces\basic_interfaces.h"
#include "ConstantBuffersFormat.h"



class Engine;

/**@brief Klasa odpowiedzialna za wyœwietlanie sceny.
*/
class DisplayEngine : public DX11_constant_buffers_container
{
private:
	Engine* engine;

	ConstantPerFrame				shader_data_per_frame;		///<Bufor sta³ych zmiennych co ramkê animacji
	Camera_object*					current_camera;				///<Akutalnie aktywna kamera

	std::vector<Dynamic_mesh_object*>		meshes;					///<Modele nieanimowane
	XMFLOAT4X4*								interpolated_matrixes;	///<Tablica macierzy interpolowanych po³o¿eñ obiektów
	unsigned int							interpol_matrixes_count;	///<Liczba macierzy interpolowanych

	std::vector<Camera_object*>				cameras;			///<Kontener zawieraj¹cy kamery
public:
	DisplayEngine(Engine* engine);
	~DisplayEngine();

	void init_const_buffers();

	void display_scene(float time_interval);
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
private:
	void set_view_matrix();

	void realocate_interpolation_memory(unsigned int min = 1);
};

