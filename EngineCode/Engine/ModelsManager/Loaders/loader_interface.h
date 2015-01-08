#pragma once
#include "..\..\..\stdafx.h"
#include "..\meshes_textures_materials.h"
#include "..\..\ModelsManager\ModelsManager.h"


typedef enum
{
	MESH_LOADING_OK,
	MESH_LOADING_WRONG,
	MESH_LOADING_OUT_OF_MEMORY,
	MESH_LOADING_WRONG_FILE_FORMAT
} MESH_LOADING;

class Loader
{
protected:
	ModelsManager*		models_manager;
public:
	Loader(ModelsManager* models_manager) : models_manager(models_manager){};
	virtual ~Loader(){};

	virtual bool can_load(const std::wstring& name) = 0;
	virtual int load_mesh(Model3DFromFile* new_file_mesh, const std::wstring& name) = 0;
	//virtual void load_animation(const std::string& name) = 0;
};