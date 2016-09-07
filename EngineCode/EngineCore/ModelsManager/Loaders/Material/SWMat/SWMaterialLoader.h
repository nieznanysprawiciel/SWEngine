#pragma once

#include "Common/Nullable.h"
#include "Common/System/Path.h"

#include "EngineCore/ModelsManager/Assets/Materials/MaterialAssetInitData.h"
#include "EngineCore/ModelsManager/Assets/Materials/MaterialAsset.h"


/**@brief Loads .swmat files.*/
class SWMaterialLoader
{
private:

	struct STRINGS_1_0
	{
		static const std::string		FILE_HEADER_STRING;
		static const std::string		VERSION_MAJOR_STRING;
		static const std::string		VERSION_MINOR_STRING;

		static const std::string		FILE_PATH_STRING;
		static const std::string		SHADER_ENTRY_STRING;

		static const std::string		VERTEX_SHADER_STRING;
		static const std::string		PIXEL_SHADER_STRING;
		static const std::string		GEOMETRY_SHADER_STRING;
		static const std::string		CONTROL_SHADER_STRING;
		static const std::string		EVALUATION_SHADER_STRING;

		static const std::string		TEXTURES_ARRAY_STRING;
		static const std::string		TEXTURE_STRING;
	};

	uint32		m_versionMajor;
	uint32		m_versionMinor;

public:
	SWMaterialLoader();
	~SWMaterialLoader();

	Nullable< MaterialAssetInitData >	LoadMaterial	( const filesystem::Path& fileName );
	bool								CanLoad			( const filesystem::Path& fileName );
	void								SaveMaterial	( const filesystem::Path& fileName, MaterialAsset* mat );

	uint32		GetVersionMajor()		{ return m_versionMajor; }
	uint32		GetVersionMinor()		{ return m_versionMinor; }
};

