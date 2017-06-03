#pragma once

#include "swGraphicAPI/Resources/MeshResources.h"
#include "EngineCore/ModelsManager/Assets/Materials/MaterialAsset.h"

#include "fbxsdk.h"

#include <DirectXMath.h>
#include <vector>



enum FbxShadingModel
{
	HardwareShader,
	Lambert,
	Phong
};


struct FbxNodeMesh
{
	DirectX::XMFLOAT4X4		Transformation;
	FbxNode*				Node;
	FbxMesh*				Mesh;
};


struct FbxMeshCollection
{
	std::vector< FbxNodeMesh >		Segments;
};


struct FbxMaterialMap
{
	FbxSurfaceMaterial*				Surface;
	ResourcePtr< MaterialAsset >	EngineMaterial;

	FbxMaterialMap( FbxSurfaceMaterial* surface )
	{ Surface = surface; }
};


struct FbxTextureMap
{
	FbxTexture*						FbxTex;
	ResourcePtr< TextureObject >	EngineTex;

	FbxTextureMap( FbxTexture* texture )
	{ FbxTex = texture; }
};

typedef std::vector< FbxMaterialMap > FbxMaterialsCollection;
typedef std::vector< FbxTextureMap > FbxTexturesCollection;


struct FbxAssetsCollection
{
	FbxMaterialsCollection		Material;
	FbxTexturesCollection		Textures;
};


struct TemporaryMeshInit
{
	std::vector< VertexNormalTexCoord >		Verticies;
	std::vector< std::vector< Index32 > >	Indicies;
	std::vector< MeshPart >					Segments;
};
