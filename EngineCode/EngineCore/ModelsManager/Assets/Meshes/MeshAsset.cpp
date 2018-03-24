/**
@file MeshAsset.cpp
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.
*/

#include "EngineCore/stdafx.h"
#include "MeshAsset.h"
#include "swCommonLib/Common/Converters.h"
#include "swCommonLib/Serialization/PropertySerialization/Serialization.h"

#include "swCommonLib/Common/MemoryLeaks.h"


RTTR_REGISTRATION
{
	rttr::registration::class_< sw::MeshPart >( "MeshPart" )
		.property_readonly( "Topology", &sw::MeshPart::Topology )
		.property_readonly( "NumVerticies", &sw::MeshPart::NumVertices )
		.property_readonly( "BufferOffset", &sw::MeshPart::BufferOffset )
		.property_readonly( "BaseVertex", &sw::MeshPart::BaseVertex )
		.property_readonly( "UseAdditionalBuffers", &sw::MeshPart::GetUseAdditionalBuffer )
		.property_readonly( "UseExtendedIndex", &sw::MeshPart::GetUseExtendedIndex )
		.property( "Material", &sw::MeshPart::Material );

	rttr::registration::class_< std::vector< sw::MeshPart > >( "Vector< MeshPart >" );

	rttr::registration::class_< sw::MeshAsset >( "MeshAsset" )
		.property_readonly( "FileName", &sw::MeshAsset::m_filePath )
		.property( "Segments", &sw::MeshAsset::m_segments ) BIND_AS_PTR
		.property( "IndexBuffer", &sw::MeshAsset::m_indexBuffer )
		.property( "VertexBuffer", &sw::MeshAsset::m_vertexBuffer )
		.property( "Layout", &sw::MeshAsset::m_layout );
}


namespace sw
{

/**@brief */
MeshAsset::MeshAsset( const std::wstring& filePath, MeshCreateData&& initData )
	: ResourceObject( WRONG_ID )
	, m_filePath( filePath )
	, m_vertexBuffer( std::move( initData.VertexBuffer ) )
	, m_indexBuffer( std::move( initData.IndexBuffer ) )
	, m_layout( std::move( initData.VertexLayout ) )
	, m_segments( std::move( initData.MeshSegments ) )
{}

/**@brief */
MeshAsset::~MeshAsset()
{
	m_segments.clear();
}



/**@brief zwraca wskaŸnik na ModelPart spod podanego indeksu.
@param[in] Indeks.
@return WskaŸnik na ModelPart lub nullptr, je¿eli indeks by³ nieprawid³owy.*/
const MeshPart*	MeshAsset::GetSegment( Size index ) const
{
	if( index < m_segments.size() )
		return &m_segments[ index ];
	return nullptr;
}

/**@brief Zwraca liczbê obiektów ModelPart w tablicy.*/
Size		MeshAsset::GetSegmentsCount() const
{
	return m_segments.size();
}

/**@brief Zwraca nazwê mesha (nazwê pliku)*/
std::string MeshAsset::GetResourceName() const
{
	return Convert::ToString( m_filePath );
}

//====================================================================================//
//			Serializacja	
//====================================================================================//

/**@brief Domyœlna serializacja.

@todo W zasadzie domyœlny tryb serializacji powinna implementowaæ klasa EngineObject.*/
void		MeshAsset::Serialize( ISerializer* ser ) const
{
	//Serialization::DefaultSerialize( ser, this );

	ser->EnterObject( GetTypeName() );

	Serialization::SerializeStringTypes( ser, this, GetType().get_property( "FileName" ) );

	ser->Exit();
}

// ================================ //
//
void		MeshAsset::Deserialize( IDeserializer* deser )
{}

}	// sw

