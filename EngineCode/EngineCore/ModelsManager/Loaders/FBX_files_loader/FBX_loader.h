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
#include "FbxHelperStructs.h"

#include "Common/System/Path.h"


struct FbxMeshCollection;


/**@brief Klasa s³u¿y do wczytywania plików w formacie Autodesk FBX.


@ingroup MakingLoaders*/
class FBX_loader	:	public ILoader
{
private:
	FbxManager*			fbx_manager;
	FbxIOSettings*		fbx_IOsettings;
	Model3DFromFile*	cur_model;		///<Na czas wczytywania zapisujemy sobie obiekt, do którego wczytujemy dane

	filesystem::Path	m_filePath;

public:
	FBX_loader( AssetsManager* models_manager );
	~FBX_loader();

	LoaderResult load_mesh( Model3DFromFile* new_file_mesh, const std::wstring& name ) override;
	bool can_load( const std::wstring& name ) override;


	Nullable< MeshInitData >		LoadMesh	( const filesystem::Path& fileName ) override;
	bool							CanLoad		( const filesystem::Path& fileName ) override;

private:
	void		process_node( FbxNode* node );
	void		process_mesh( FbxNode* node, FbxMesh* mesh, const DirectX::XMFLOAT4X4& transformation );
	int			process_tree( FbxNode* root_node );

	void		read_UVs( FbxMesh* mesh, int control_point, unsigned int vertex_counter, DirectX::XMFLOAT2& UV_cords );
	int			read_material_index( FbxMesh* mesh, unsigned int polygon_counter );

	void		CopyMaterial		( MaterialObject& engineMaterial, const FbxSurfaceLambert& FBXmaterial );
	void		CopyMaterial		( MaterialObject& engineMaterial, const FbxSurfacePhong& FBXmaterial );
	void		ProcessMaterial		( FbxSurfaceMaterial* FBXmaterial );

private:

	Nullable< FbxMeshCollection >	ProcessNode		( FbxNode* node, Nullable< FbxMeshCollection >& meshes );
	Nullable< MeshInitData >		ProcessMesh		( FbxNodeMesh& nodeData, FbxAssetsCollection& assets, Nullable< MeshInitData >& mesh );
};

