#pragma once
/**@file FBX_loader.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Plik zawiera deklaracjê klasy FBX_loader s³u¿¹cej do wczytywania plików w formacie FBX.

Obecna wersja FBX_loadera jest jedynie lekko zmodyfikowan¹ wersj¹ tego, który
dzia³a³ pod DirectX 9. Zmiany, które zasz³y, to jedynie dopasowanie do nowego modelu
przechowywania danych.

Niestety nie czerpiemy korzyœci z takich rzeczy jak bufor indeksów oraz
multitekturing. Nie jest równie¿ u¿ywany bumpmapping. Domyœlnie tekstura
wczytana jest traktowana jakby by³a dla kana³u diffuse.

To trzeba kiedyœ zmieniæ, aby po pierwsze optymalniej wykorzystywaæ silnik, a po
drugie, ¿eby wykorzystywaæ wszystkie jego mo¿liwoœci.*/

#include "EngineCore/ModelsManager/Loaders/ILoader.h"
#include "fbxsdk.h"

#include <filesystem>

namespace filesystem = std::tr2::sys;

/**@brief Klasa s³u¿y do wczytywania plików w formacie Autodesk FBX.
@ingroup MakingLoaders*/
class FBX_loader	:	public ILoader
{
private:
	FbxManager*			fbx_manager;
	FbxIOSettings*		fbx_IOsettings;
	Model3DFromFile*	cur_model;		///<Na czas wczytywania zapisujemy sobie obiekt, do którego wczytujemy dane

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
	//juz nieu¿ywane, ale zachowane na ewentualn¹ przysz³oœæ

	std::vector<std::pair<MaterialObject*, FbxSurfacePhong*>>		Materials;

	bool process_existing_mesh(FbxNode* node, FbxMesh* mesh, const DirectX::XMFLOAT4X4& transformation);
	void process_materials(FbxNode* node);
	bool check_if_exists(MaterialObject** directXmaterial, FbxSurfacePhong* FBXmaterial);
	void add_pair(MaterialObject* directXmaterial, FbxSurfacePhong* FBXmaterial);
#endif
};

