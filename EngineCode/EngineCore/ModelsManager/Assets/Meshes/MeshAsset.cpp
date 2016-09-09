/**
@file MeshAsset.cpp
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.
*/

#include "EngineCore/stdafx.h"
#include "MeshAsset.h"
#include "Common/Converters.h"
#include "Common/Serialization/SW/Serialization.h"

#include "Common/MemoryLeaks.h"


RTTR_REGISTRATION
{
	rttr::registration::class_< MeshAsset >( "MeshAsset" )
	.property( "FileName", &MeshAsset::m_filePath );
}



/**@brief */
MeshAsset::MeshAsset( const std::wstring& filePath, MeshAssetInitWithExistingData&& initData )
	:	ResourceObject( WRONG_ID )
	,	m_filePath( filePath )
	,	m_vertexBuffer( std::move( initData.VertexBuffer ) )
	,	m_indexBuffer( std::move( initData.IndexBuffer ) )
	,	m_layout( std::move( initData.VertexLayout ) )
	,	m_segments( std::move( initData.MeshSegments ) )
{}

/**@brief */
MeshAsset::~MeshAsset()
{
	m_segments.clear( );
}



/**@brief zwraca wska�nik na ModelPart spod podanego indeksu.
@param[in] Indeks.
@return Wska�nik na ModelPart lub nullptr, je�eli indeks by� nieprawid�owy.*/
const MeshPart*	MeshAsset::GetSegment( Size index ) const
{
	if ( index < m_segments.size( ) )
		return &m_segments[index];
	return nullptr;
}

/**@brief Zwraca liczb� obiekt�w ModelPart w tablicy.*/
Size		MeshAsset::GetSegmentsCount() const
{
	return m_segments.size();
}

/**@brief Zwraca nazw� mesha (nazw� pliku)*/
std::string MeshAsset::GetResourceName() const
{
	return Converters::ToString( m_filePath );
}

//====================================================================================//
//			Serializacja	
//====================================================================================//

/**@brief Domy�lna serializacja.

@todo W zasadzie domy�lny tryb serializacji powinna implementowa� klasa EngineObject.*/
void		MeshAsset::Serialize( ISerializer* ser ) const
{
	Serialization::DefaultSerialize( ser, this );
}

void		MeshAsset::Deserialize( IDeserializer* deser )
{ }
