#pragma once

#include "EngineCore/ModelsManager/Loaders/ILoader.h"

/**Loads .swmesh files.*/
class SWMeshLoader : public ILoader
{
private:
public:
	SWMeshLoader( ModelsManager* modelsManager );
	~SWMeshLoader();

	virtual bool			can_load	( const std::wstring& name );
	virtual LoaderResult	load_mesh	( Model3DFromFile* new_file_mesh, const std::wstring& name );

	Nullable< MeshAssetInitData >		LoadMesh	( const filesystem::Path& fileName );
	bool								CanLoad		( const filesystem::Path& fileName );
};
