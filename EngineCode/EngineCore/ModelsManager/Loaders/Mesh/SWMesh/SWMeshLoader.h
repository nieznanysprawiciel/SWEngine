#pragma once
/**
@file SWMeshLoader.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/



#include "EngineCore/ModelsManager/Loaders/ILoader.h"


namespace sw
{


/**Loads .swmesh files.*/
class SWMeshLoader : public ILoader
{
private:
public:
	SWMeshLoader( AssetsManager* modelsManager );
	~SWMeshLoader();

	virtual bool			can_load	( const std::wstring& name );
	virtual LoaderResult	load_mesh	( Model3DFromFile* new_file_mesh, const std::wstring& name );

	Nullable< MeshInitData >		LoadMesh	( const filesystem::Path& fileName );
	bool								CanLoad		( const filesystem::Path& fileName );
};


}	// sw

