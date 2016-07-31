#pragma once
/**@file FBX_loader.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Plik zawiera deklaracj� klasy FBX_loader s�u��cej do wczytywania plik�w w formacie FBX.

Obecna wersja FBX_loadera jest jedynie lekko zmodyfikowan� wersj� tego, kt�ry
dzia�a� pod DirectX 9. Zmiany, kt�re zasz�y, to jedynie dopasowanie do nowego modelu
przechowywania danych.

Niestety nie czerpiemy korzy�ci z takich rzeczy jak bufor indeks�w oraz
multitekturing. Nie jest r�wnie� u�ywany bumpmapping. Domy�lnie tekstura
wczytana jest traktowana jakby by�a dla kana�u diffuse.

To trzeba kiedy� zmieni�, aby po pierwsze optymalniej wykorzystywa� silnik, a po
drugie, �eby wykorzystywa� wszystkie jego mo�liwo�ci.*/

#include "EngineCore/ModelsManager/Loaders/ILoader.h"
#include "fbxsdk.h"

#include <filesystem>

namespace filesystem = std::tr2::sys;

/**@brief Klasa s�u�y do wczytywania plik�w w formacie Autodesk FBX.
@ingroup MakingLoaders*/
class FBX_loader	:	public ILoader
{
private:
	FbxManager*			fbx_manager;
	FbxIOSettings*		fbx_IOsettings;
	Model3DFromFile*	cur_model;		///<Na czas wczytywania zapisujemy sobie obiekt, do kt�rego wczytujemy dane

	filesystem::path	m_filePath;

	void process_node( FbxNode* node );
	void process_mesh( FbxNode* node, FbxMesh* mesh, const DirectX::XMFLOAT4X4& transformation );
	int process_tree( FbxNode* root_node );

	void read_UVs( FbxMesh* mesh, int control_point, unsigned int vertex_counter, DirectX::XMFLOAT2& UV_cords );
	int read_material_index( FbxMesh* mesh, unsigned int polygon_counter );

	//void copy_material( D3DMATERIAL9& directXmaterial, const FbxSurfacePhong& FBXmaterial );
	void CopyMaterial( MaterialObject& engine_material, const FbxSurfacePhong& FBXmaterial );

public:
	FBX_loader( ModelsManager* models_manager );
	~FBX_loader();

	LoaderResult load_mesh( Model3DFromFile* new_file_mesh, const std::wstring& name ) override;
	bool can_load( const std::wstring& name ) override;


private:
#ifndef __UNUSED
	//juz nieu�ywane, ale zachowane na ewentualn� przysz�o��

	std::vector<std::pair<MaterialObject*, FbxSurfacePhong*>>		Materials;

	bool process_existing_mesh(FbxNode* node, FbxMesh* mesh, const DirectX::XMFLOAT4X4& transformation);
	void process_materials(FbxNode* node);
	bool check_if_exists(MaterialObject** directXmaterial, FbxSurfacePhong* FBXmaterial);
	void add_pair(MaterialObject* directXmaterial, FbxSurfacePhong* FBXmaterial);
#endif
};

