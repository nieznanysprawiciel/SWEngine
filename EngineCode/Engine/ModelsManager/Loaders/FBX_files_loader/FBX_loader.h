#pragma once

#include "..\..\..\..\stdafx.h"
#include "..\loader_interface.h"
#include "fbxsdk.h"

using namespace DirectX;

class FBX_loader	:	public Loader
{
private:
	FbxManager*			fbx_manager;
	FbxIOSettings*		fbx_IOsettings;
	Model3DFromFile*	cur_model;


	void process_node(FbxNode* node);
	void process_mesh(FbxNode* node, FbxMesh* mesh, const DirectX::XMFLOAT4X4& transformation);
	int process_tree(FbxNode* root_node);

	void read_UVs(FbxMesh* mesh, int control_point, unsigned int vertex_counter, XMFLOAT2& UV_cords);
	int read_material_index(FbxMesh* mesh, unsigned int polygon_counter);

	void copy_material( D3DMATERIAL9& directXmaterial, const FbxSurfacePhong& FBXmaterial );
	void copy_material( MaterialObject& engine_material, const FbxSurfacePhong& FBXmaterial );

public:
	FBX_loader(ModelsManager* models_manager);
	~FBX_loader();

	int load_mesh(Model3DFromFile* new_file_mesh, const std::wstring& name);
	bool can_load(const std::wstring& name);


private:
#ifndef __UNUSED
	//juz nieu¿ywane, ale zachowane na ewentualn¹ przysz³oœæ

	std::vector<std::pair<MaterialObject*, FbxSurfacePhong*>>		materials;

	bool process_existing_mesh(FbxNode* node, FbxMesh* mesh, const DirectX::XMFLOAT4X4& transformation);
	void process_materials(FbxNode* node);
	bool check_if_exists(MaterialObject** directXmaterial, FbxSurfacePhong* FBXmaterial);
	void add_pair(MaterialObject* directXmaterial, FbxSurfacePhong* FBXmaterial);
#endif
};

