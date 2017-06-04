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

#include "EngineCore/ModelsManager/Assets/Materials/PhongMaterialData.h"

#include "swCommonLib/System/Path.h"


namespace sw
{


struct FbxMeshCollection;
class Model3DFromFile;		///< @deprecated

/**@brief Klasa s³u¿y do wczytywania plików w formacie Autodesk FBX.


@ingroup MakingLoaders*/
class FBX_loader : public ILoader
{
private:
	FbxManager*			fbx_manager;
	FbxIOSettings*		fbx_IOsettings;

	filesystem::Path	m_filePath;

public:
	FBX_loader( AssetsManager* models_manager );
	~FBX_loader();

	Nullable< MeshInitData >		LoadMesh	( const filesystem::Path& fileName ) override;
	bool							CanLoad		( const filesystem::Path& fileName ) override;

private:

	DirectX::XMFLOAT2	ReadUVs				( FbxMesh* mesh, int control_point, unsigned int vertex_counter );
	int					ReadMaterialIndex	( FbxMesh* mesh, unsigned int polygon_counter );

private:

	void		TransformVerticies	( std::vector< VertexNormalTexCoord >& verticies, uint32 offset, const DirectX::XMFLOAT4X4& matrix );
	Index32		FindUniqueVertex	( VertexNormalTexCoord& vertex, std::vector< VertexNormalTexCoord >& verticies, Index32 startIndex );

	Nullable< FbxMeshCollection >	ProcessNode		( FbxNode* node, Nullable< FbxMeshCollection >& meshes );
	Nullable< TemporaryMeshInit >	ProcessMesh		( FbxNodeMesh& nodeData, FbxAssetsCollection& assets, Nullable< TemporaryMeshInit >& mesh );
	ResourcePtr< MaterialAsset >	ProcessMaterial	( FbxSurfaceMaterial* FBXmaterial, FbxAssetsCollection& assets );
	ResourcePtr< MaterialAsset >	CreateMaterial	( FbxSurfaceMaterial* FBXmaterial, FbxAssetsCollection& assets );
	ResourcePtr< TextureObject >	ProcessTexture	( FbxFileTexture* FBXTexture, FbxAssetsCollection& assets );

	PhongMaterial		CopyMaterial		( const FbxSurfaceLambert& FBXmaterial );
	PhongMaterial		CopyMaterial		( const FbxSurfacePhong& FBXmaterial );

	template< typename VertexType >
	void				CopyVertexBuffer	( const std::vector< VertexType >& verticies, MemoryChunk& destination );

	template< typename IndexType >
	void				CopyIndexBuffer		( const std::vector< std::vector< Index32 > >& indicies, MemoryChunk& destination );
};


}	// sw

