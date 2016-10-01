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
#include "FbxHelperStructs.h"

#include "Common/System/Path.h"


struct FbxMeshCollection;


/**@brief Klasa s�u�y do wczytywania plik�w w formacie Autodesk FBX.


@ingroup MakingLoaders*/
class FBX_loader	:	public ILoader
{
private:
	FbxManager*			fbx_manager;
	FbxIOSettings*		fbx_IOsettings;
	Model3DFromFile*	cur_model;		///<Na czas wczytywania zapisujemy sobie obiekt, do kt�rego wczytujemy dane

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

