#include "EngineCore/stdafx.h"
#include "SWMaterialLoader.h"

#include "Common/Converters.h"
#include "Common/Serialization/SW/Serialization.h"

#include "EngineCore/ModelsManager/Assets/Materials/ShadingModelData.h"
#include "EngineCore/ModelsManager/Assets/Materials/PhongMaterialData.h"



SWMaterialLoader::SWMaterialLoader()
{
	m_versionMajor = 1;
	m_versionMinor = 0;
}

SWMaterialLoader::~SWMaterialLoader()
{}



const std::string		SWMaterialLoader::STRINGS_1_0::FILE_HEADER_STRING		= "SWMaterial";
const std::string		SWMaterialLoader::STRINGS_1_0::VERSION_MAJOR_STRING		= "VersionMajor";
const std::string		SWMaterialLoader::STRINGS_1_0::VERSION_MINOR_STRING		= "VersionMinor";

const std::string		SWMaterialLoader::STRINGS_1_0::FILE_PATH_STRING			= "FilePath";
const std::string		SWMaterialLoader::STRINGS_1_0::SHADER_ENTRY_STRING		= "ShaderEntry";

const std::string		SWMaterialLoader::STRINGS_1_0::VERTEX_SHADER_STRING		= "VertexShader";
const std::string		SWMaterialLoader::STRINGS_1_0::PIXEL_SHADER_STRING		= "PixelShader";
const std::string		SWMaterialLoader::STRINGS_1_0::GEOMETRY_SHADER_STRING	= "GeometryShader";
const std::string		SWMaterialLoader::STRINGS_1_0::CONTROL_SHADER_STRING	= "TesselationControlShader";
const std::string		SWMaterialLoader::STRINGS_1_0::EVALUATION_SHADER_STRING = "TesselationEvaluationShader";

const std::string		SWMaterialLoader::STRINGS_1_0::TEXTURES_ARRAY_STRING	= "Textures";

const std::string		SWMaterialLoader::STRINGS_1_0::ADD_BUFFERS_ARRAY_STRING = "AdditionalBuffers";
const std::string		SWMaterialLoader::STRINGS_1_0::BUFFER_SIZE_STRING		= "BufferSize";
const std::string		SWMaterialLoader::STRINGS_1_0::SHADING_DATA_STRING		= "ShadingModel";




Nullable< MaterialAssetInitData >	SWMaterialLoader::LoadMaterial	( const filesystem::Path& fileName )
{

	return Nullable< MaterialAssetInitData >();
}


bool								SWMaterialLoader::CanLoad		( const filesystem::Path& fileName )
{
	if( fileName.GetExtension() == ".swmat" )
		return true;
	return false;
}

void								SWMaterialLoader::SaveMaterial	( const filesystem::Path& fileName, MaterialAsset* mat )
{
	ISerializer ser( std::make_unique< EngineSerializationContext >() );

	ser.EnterObject( STRINGS_1_0::FILE_HEADER_STRING );

		ser.SetAttribute( STRINGS_1_0::VERSION_MAJOR_STRING, m_versionMajor );
		ser.SetAttribute( STRINGS_1_0::VERSION_MINOR_STRING, m_versionMinor );

		ser.EnterObject( rttr::type::get< MaterialAsset >().get_name() );

			// Shaders

			IShader* shader = mat->GetVertexShader().Ptr();
			ser.EnterObject( STRINGS_1_0::VERTEX_SHADER_STRING );
			if( shader )
			{
				ser.SetAttribute( STRINGS_1_0::FILE_PATH_STRING, Convert::ToString( shader->GetShaderFile() ) );
				ser.SetAttribute( STRINGS_1_0::SHADER_ENTRY_STRING, shader->GetShaderEntry() );
			}
			ser.Exit();

			shader = mat->GetPixelShader().Ptr();
			ser.EnterObject( STRINGS_1_0::PIXEL_SHADER_STRING );
			if( shader )
			{
				ser.SetAttribute( STRINGS_1_0::FILE_PATH_STRING, Convert::ToString( shader->GetShaderFile() ) );
				ser.SetAttribute( STRINGS_1_0::SHADER_ENTRY_STRING, shader->GetShaderEntry() );
			}
			ser.Exit();

			shader = mat->GetGeometryShader().Ptr();
			ser.EnterObject( STRINGS_1_0::GEOMETRY_SHADER_STRING );
			if( shader )
			{
				ser.SetAttribute( STRINGS_1_0::FILE_PATH_STRING, Convert::ToString( shader->GetShaderFile() ) );
				ser.SetAttribute( STRINGS_1_0::SHADER_ENTRY_STRING, shader->GetShaderEntry() );
			}
			ser.Exit();

			shader = mat->GetTessControlShader().Ptr();
			ser.EnterObject( STRINGS_1_0::CONTROL_SHADER_STRING );
			if( shader )
			{
				ser.SetAttribute( STRINGS_1_0::FILE_PATH_STRING, Convert::ToString( shader->GetShaderFile() ) );
				ser.SetAttribute( STRINGS_1_0::SHADER_ENTRY_STRING, shader->GetShaderEntry() );
			}
			ser.Exit();

			shader = mat->GetTessEvaluationShader().Ptr();
			ser.EnterObject( STRINGS_1_0::EVALUATION_SHADER_STRING );
			if( shader )
			{
				ser.SetAttribute( STRINGS_1_0::FILE_PATH_STRING, Convert::ToString( shader->GetShaderFile() ) );
				ser.SetAttribute( STRINGS_1_0::SHADER_ENTRY_STRING, shader->GetShaderEntry() );
			}
			ser.Exit();

			// Textures
			ser.EnterArray( STRINGS_1_0::TEXTURES_ARRAY_STRING );

			for( int i = 0; i < MAX_MATERIAL_TEXTURES; ++i )
			{
				auto tex = mat->GetTexture( i ).Ptr();

				ser.EnterObject( rttr::type::get< TextureObject >().get_name() );

				if( tex )
					ser.SetAttribute( STRINGS_1_0::FILE_PATH_STRING, tex->GetFilePath().String() );

				ser.Exit();
			}

			ser.Exit();	// Textures

			
			// Shading model data
			auto shadingData = mat->GetDescriptor().ShadingData;

			TypeID shadingModelType = shadingData->GetShadingModelType();
			TypeID shadingModelPtrType = shadingData->GetShadingModelPtrType();

			rttr::variant shadingDataPtr( (void*)shadingData->GetShadingModelData() );
			shadingDataPtr.unsafe_convert_void( shadingModelPtrType );


			ser.EnterObject( STRINGS_1_0::SHADING_DATA_STRING );

				ser.SetAttribute( STRINGS_1_0::BUFFER_SIZE_STRING, shadingData->GetShadingModelSize() );
				Serialization::DefaultSerializeImpl( &ser, shadingDataPtr, shadingModelType );

			ser.Exit();		// SHADING_DATA_STRING

			// Additional buffers
			TypeID descriptorType = TypeID::get< MaterialInfo >();
			auto additionalBuffersProperty = descriptorType.get_property( STRINGS_1_0::ADD_BUFFERS_ARRAY_STRING.c_str() );

			Serialization::SerializeArrayTypes( &ser, mat->GetDescriptor(), additionalBuffersProperty );


		ser.Exit();	// type name

	ser.Exit(); // SWMaterial

	ser.SaveFile( fileName.String(), WritingMode::Readable );
}
