#pragma once


/**@file 
@brief Plik zawiera deklaracj� klasy FBX_loader s�u��cej do wczytywania plik�w w formacie FBX.

Obecna wersja FBX_loadera jest jedynie lekko zmodyfikowan� wersj� tego, kt�ry
dzia�a� pod DirectX 9. Zmiany, kt�re zasz�y, to jedynie dopasowanie do nowego modelu
przechowywania danych.

Niestety nie czerpiemy korzy�ci z takich rzeczy jak bufor indeks�w oraz
multitekturing. Nie jest r�wnie� u�ywany bumpmapping. Domy�lnie tekstura
wczytana jest traktowana jakby by�a dla kana�u diffuse.

To trzeba kiedy� zmieni�, aby po pierwsze optymalniej wykorzystywa� silnik, a po
drugie, �eby wykorzystywa� wszystkie jego mo�liwo�ci.*/

#include "..\..\..\..\stdafx.h"
#include "..\loader_interface.h"
#include "fbxsdk.h"

using namespace DirectX;

/**@brief Klasa s�u�y do wczytywania plik�w w formacie Autodesk FBX.*/
class FBX_loader	:	public Loader
{
private:
	FbxManager*			fbx_manager;
	FbxIOSettings*		fbx_IOsettings;
	Model3DFromFile*	cur_model;		///<Na czas wczytywania zapisujemy sobie obiekt, do kt�rego wczytujemy dane


	void process_node(FbxNode* node);
	void process_mesh(FbxNode* node, FbxMesh* mesh, const DirectX::XMFLOAT4X4& transformation);
	int process_tree(FbxNode* root_node);

	void read_UVs(FbxMesh* mesh, int control_point, unsigned int vertex_counter, XMFLOAT2& UV_cords);
	int read_material_index(FbxMesh* mesh, unsigned int polygon_counter);

	//void copy_material( D3DMATERIAL9& directXmaterial, const FbxSurfacePhong& FBXmaterial );
	void copy_material( MaterialObject& engine_material, const FbxSurfacePhong& FBXmaterial );

public:
	FBX_loader(ModelsManager* models_manager);
	~FBX_loader();

	LOADER_RESULT load_mesh( Model3DFromFile* new_file_mesh, const std::wstring& name ) override;
	bool can_load(const std::wstring& name) override;


private:
#ifndef __UNUSED
	//juz nieu�ywane, ale zachowane na ewentualn� przysz�o��

	std::vector<std::pair<MaterialObject*, FbxSurfacePhong*>>		materials;

	bool process_existing_mesh(FbxNode* node, FbxMesh* mesh, const DirectX::XMFLOAT4X4& transformation);
	void process_materials(FbxNode* node);
	bool check_if_exists(MaterialObject** directXmaterial, FbxSurfacePhong* FBXmaterial);
	void add_pair(MaterialObject* directXmaterial, FbxSurfacePhong* FBXmaterial);
#endif
};

