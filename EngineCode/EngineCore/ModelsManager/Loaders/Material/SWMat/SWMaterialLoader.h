#pragma once

#include "swCommonLib/Common/Nullable.h"
#include "swCommonLib/System/Path.h"

#include "EngineCore/ModelsManager/Assets/Materials/MaterialAssetInitData.h"
#include "EngineCore/ModelsManager/Assets/Materials/MaterialAsset.h"


/**@brief Loads .swmat files.*/
class SWMaterialLoader
{
public:

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

		static const std::string		ADD_BUFFERS_ARRAY_STRING;
		static const std::string		BUFFER_SIZE_STRING;
		static const std::string		SHADING_DATA_STRING;
		static const std::string		SHADING_MODEL_WRAPPER_TYPE_STRING;
	};

private:
	uint32				m_versionMajor;
	uint32				m_versionMinor;

	AssetsManager*		m_modelsManager;

public:
	explicit							SWMaterialLoader( AssetsManager* modelsManager );
	~SWMaterialLoader();

	Nullable< MaterialInitData >		LoadMaterial	( const filesystem::Path& fileName );
	bool								CanLoad			( const filesystem::Path& fileName );
	void								SaveMaterial	( const filesystem::Path& fileName, MaterialAsset* mat );

	uint32		GetVersionMajor()		{ return m_versionMajor; }
	uint32		GetVersionMinor()		{ return m_versionMinor; }


private:
	typedef std::pair< const char*, const char* > ShaderLoadInfo;

	Nullable< MaterialInitData >		LoadMaterial_Version1	( IDeserializer* deser );

	Nullable< MaterialInitData >		LoadShaders				( IDeserializer* deser, Nullable< MaterialInitData >& init );
	Nullable< MaterialInitData >		LoadTextures			( IDeserializer* deser, Nullable< MaterialInitData >& init );
	Nullable< MaterialInitData >		LoadShadingData			( IDeserializer* deser, Nullable< MaterialInitData >& init );
	Nullable< MaterialInitData >		LoadAdditionalBuffers	( IDeserializer* deser, Nullable< MaterialInitData >& init );

	ShaderLoadInfo						DeserializeShader		( IDeserializer* deser, const std::string& shaderNameString );

	template< typename ShaderType >
	Nullable< ResourcePtr< ShaderType > >	LoadShader			( IDeserializer* deser, const std::string& shaderNameString );
};

