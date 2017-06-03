#pragma once
/**
@file MeshPart.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "swGraphicAPI/Resources/MeshResources.h"
#include "EngineCore/ModelsManager/Assets/Materials/MaterialAsset.h"

namespace sw
{


/**@brief Flagi pola Flags w strukturze @ref MeshPart.
@ingroup MeshAsset*/
enum MeshPartFlags : int
{
	UseAdditionalBuffer = 0x01,		///< Materia³ u¿ywa dodatkowego bufora, który ma zostaæ wype³niony w klasie @ref StaticActor.
	Use4BytesIndex		= 0x02,		///< Dane w buforze indeksów bêd¹ traktowane jako 4 bajty zamiast domyœlnych dwóch.
};

/**@brief Przechowuje informacje o segmencie mesha.

@ingroup MeshAsset*/
struct MeshPart
{
	ResourcePtr< MaterialAsset >	Material;			///< Materia³ dla danej czêœci mesha.
	uint32							BufferOffset;		///< Offset wzglêdem pocz¹tku bufora indeksów albo wierzcho³ków. @see MeshAsset.
	uint32							NumVertices;		///< Liczba wierzcho³ków do wyœwietlenia.
	int32							BaseVertex;			///< Wartoœæ dodawana do ka¿dego indeksu przed przeczytaniem wierzcho³ka z bufora. (Tylko wersja indeksowana).
	int16							Flags;				///< Dodatkowe flagi @see MeshPartFlags
	PrimitiveTopology				Topology;			///< Topologia mesha.

// ================================ //
//
	bool	GetUseAdditionalBuffer	();
	void	SetUseAdditionalBuffer	( bool value );

	bool	GetUseExtendedIndex		();
	void	SetUseExtendedIndex		( bool value );

};


//====================================================================================//
//			Implementation	
//====================================================================================//

// ================================ //
//
inline bool		MeshPart::GetUseAdditionalBuffer()
{
	return Flags & UseAdditionalBuffer ? true : false;
}

// ================================ //
//
inline void		MeshPart::SetUseAdditionalBuffer( bool value )
{
	assert( false );
	//if( value )
	//	Flags = Flags | UseAdditionalBuffer;
	//else
	//	Flags = Flags & ( UseAdditionalBuffer ^  )
}

// ================================ //
//
inline bool		MeshPart::GetUseExtendedIndex()
{
	return Flags & Use4BytesIndex ? true : false;
}

// ================================ //
//
inline void		MeshPart::SetUseExtendedIndex( bool value )
{
	assert( false );
}


}	// sw