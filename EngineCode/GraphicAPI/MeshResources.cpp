#include "GraphicAPI/ResourcesFactory.h"

#include "Common/MemoryLeaks.h"



RTTR_REGISTRATION
{
	RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS( std::wstring );

	rttr::registration::class_< ResourceObject >( "ResourceObject" );

	rttr::registration::class_< TextureInfo >( "TextureInfo" )
	.property_readonly( "Width", &TextureInfo::GetWidth )
	.property_readonly( "Height", &TextureInfo::GetHeight )
	.property_readonly( "ArraySize", &TextureInfo::GetArraySize )
	.property_readonly( "CPUReadable", &TextureInfo::IsCPUReadable )
	.property_readonly( "CPUWritable", &TextureInfo::IsCPUWriteable )
	.property_readonly( "SharedResource", &TextureInfo::IsSharedResource )
	.property_readonly( "CubeMap", &TextureInfo::IsCubeMapTex )
	.property_readonly( "GeneratedMipMaps", &TextureInfo::GenMipMaps )
	.property_readonly( "MipMapsLevels", &TextureInfo::GetMipLevels )
	.property_readonly( "FilePath", &TextureInfo::GetPath );

	rttr::registration::class_< TextureObject >( "TextureObject" )
	.property_readonly( "Descriptor", &TextureObject::GetDescriptor )
	(
		rttr::policy::prop::BindAsPtr()
	);
}


//----------------------------------------------------------------------------------------------//
//								PixelShader												//
//----------------------------------------------------------------------------------------------//

//----------------------------------------------------------------------------------------------//
//								VertexShader												//
//----------------------------------------------------------------------------------------------//


//----------------------------------------------------------------------------------------------//
//								TextureObject													//
//----------------------------------------------------------------------------------------------//


/**@brief Porównuje ze soba dwa obiekty tekstur.

Obiekty s¹ takie same, kiedy odwo³uj¹ siê do tego samego pliku.
*/
bool TextureObject::operator==( TextureObject& object )
{
	if( this->GetFilePath().String() == object.GetFilePath().String() )
		return true;
	return false;
}

/**@brief Porównuje ze soba dwa obiekty tekstur.

Obiekty s¹ takie same, kiedy odwo³uj¹ siê do tego samego pliku.
*/
bool TextureObject::operator==( const std::wstring& fileName )
{
	typedef std::codecvt_utf8<wchar_t> convert_type;
	std::wstring_convert<convert_type, wchar_t> converter;
	auto convertedFileName = converter.to_bytes( fileName );

	if( this->GetFilePath().String() == convertedFileName )
		return true;
	return false;
}

//----------------------------------------------------------------------------------------------//
//								RenderTargetObject												//
//----------------------------------------------------------------------------------------------//


RenderTargetObject::RenderTargetObject( TextureObject* colorBuffer, TextureObject* depthBuffer, TextureObject* stencilBuffer )
	:	m_colorBuffer( colorBuffer )
	,	m_depthBuffer( depthBuffer )
	,	m_stencilBuffer( stencilBuffer )
{
	if( m_colorBuffer )
		m_colorBuffer->AddAssetReference();
	if( m_depthBuffer )
		m_depthBuffer->AddAssetReference();
	if( m_stencilBuffer )
		m_stencilBuffer->AddAssetReference();
}

/**@brief Destruktor kasuje obiekty tekstury g³êbokoœci i bufora koloru, je¿eli nie s¹ u¿ywane.
Je¿eli istniej¹ odwo³ania do tych obiektów, to nie s¹ one kasowane, a jedynie usuwa siê odwo³ania
pochodz¹ce od RenderTargetObject.*/
RenderTargetObject::~RenderTargetObject()
{
	if( m_colorBuffer )
	{
		m_colorBuffer->DeleteAssetReference();
		//if( m_colorBuffer->CanDelete() )
		//	ObjectDeleter<TextureObject>::delete_object( m_colorBuffer, ObjectDeleterKey<TextureObject>() );
	}
	if( m_depthBuffer )
	{
		m_depthBuffer->DeleteAssetReference();
		//if( m_depthBuffer->CanDelete() )
		//	ObjectDeleter<TextureObject>::delete_object( m_depthBuffer, ObjectDeleterKey<TextureObject>() );
	}
	if( m_stencilBuffer )
	{
		m_stencilBuffer->DeleteAssetReference();
		//if( m_stencilBuffer->CanDelete() )
		//	ObjectDeleter<TextureObject>::delete_object( m_stencilBuffer, ObjectDeleterKey<TextureObject>() );
	}
}

//----------------------------------------------------------------------------------------------//
//								BufferObject													//
//----------------------------------------------------------------------------------------------//

BufferObject::BufferObject( unsigned int elementSize, unsigned int elementCount )
	:	m_elementSize( elementSize ),
		m_elementCount( elementCount )
{
}


//----------------------------------------------------------------------------------------------//
//								MaterialObject													//
//----------------------------------------------------------------------------------------------//

MaterialObject::MaterialObject( const MaterialObject* material )
: ResourceObject(WRONG_ID)
{
	memcpy( this, material, sizeof(MaterialObject) );
}

/**@brief Ustawia materia³ na wartoœci domyœlne dla silnika.

Te wartoœci s¹ najbardziej neutralne, w przypadku gdy nie ma materia³u, a jest ustawiona tekstura.
Wtedy shadery wymana¿aj¹ jasnoœæ piksela przez 1.0 i nic sie nie zmienia.*/
void MaterialObject::SetNullMaterial( )
{
	Diffuse.x = 1.0f;
	Diffuse.y = 1.0f;
	Diffuse.z = 1.0f;
	Diffuse.w = 1.0f;

	Ambient.x = 1.0f;
	Ambient.y = 1.0f;
	Ambient.z = 1.0f;
	Ambient.w = 1.0f;

	Specular.x = 1.0f;
	Specular.y = 1.0f;
	Specular.z = 1.0f;
	Specular.w = 1.0f;

	Emissive.x = 0.0f;
	Emissive.y = 0.0f;
	Emissive.z = 0.0f;
	Emissive.w = 0.0f;

	Power = 1.0f;
}


