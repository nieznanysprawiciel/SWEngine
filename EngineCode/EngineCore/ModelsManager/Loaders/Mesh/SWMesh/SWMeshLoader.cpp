#include "EngineCore/stdafx.h"
#include "SWMeshLoader.h"


const char* ExtensionsList[] =
{
	"swmesh"
};


SWMeshLoader::SWMeshLoader( ModelsManager* modelsManager )
	: ILoader( modelsManager )
{}


SWMeshLoader::~SWMeshLoader()
{}



bool SWMeshLoader::can_load( const std::wstring& name )
{
	return false;
}

LoaderResult SWMeshLoader::load_mesh( Model3DFromFile* new_file_mesh, const std::wstring& name )
{
	return LoaderResult::MESH_LOADING_WRONG;
}


bool								SWMeshLoader::CanLoad	( const filesystem::Path& fileName )
{
	auto fileExt = fileName.GetExtension();

	for( auto extension : ExtensionsList )
	{
		if( fileExt == extension )
			return true;
	}
	return false;
}


Nullable< MeshInitData >		SWMeshLoader::LoadMesh	( const filesystem::Path& fileName )
{

	return Nullable< MeshInitData >();
}
