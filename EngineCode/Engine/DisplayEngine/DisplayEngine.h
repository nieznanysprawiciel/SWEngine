#pragma once


#include "..\..\stdafx.h"
#include "..\..\Interfaces\basic_interfaces.h"




class Engine;

class DisplayEngine	:	public DX11_interfaces_container
{
private:
	Engine* engine;

	XMFLOAT4X4						projection_matrix;
	Camera_object*					current_camera;

	std::vector<Dynamic_mesh_object*>		meshes;		//modele nieanimowane
	XMFLOAT4X4*								interpolated_matrixes;
	unsigned int							interpol_matrixes_count;

	std::vector<Camera_object*>				cameras;
public:
	DisplayEngine(Engine* engine);
	~DisplayEngine();

	void display_scene(float time_interval);
	void interpolate_positions(float time_lag);
	void set_projection_matrix(float angle, float X_to_Y, float near_plane, float far_plane);

	void add_dynamic_mesh_object( Dynamic_mesh_object* object );

	//camera functions
	int add_camera( Camera_object* camera );
	int set_current_camera( Camera_object* camera );
private:
	void set_view_matrix();

	void realocate_interpolation_memory(unsigned int min = 1);
};

