#pragma once


#include "GraphicAPI/MeshResources.h"
#include "EngineCore/ModelsManager/Assets/Materials/MaterialAsset.h"



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
	unsigned int					BufferOffset;		///< Offset wzglêdem pocz¹tku bufora indeksów albo wierzcho³ków. @see MeshAsset.
	unsigned int					NumVertices;		///< Liczba wierzcho³ków do wyœwietlenia.
	int								BaseVertex;			///< Wartoœæ dodawana do ka¿dego indeksu przed przeczytaniem wierzcho³ka z bufora. (Tylko wersja indeksowana).
	int16							Flags;				///< Dodatkowe flagi @see MeshPartFlags
	PrimitiveTopology				Topology;			///< Topologia mesha.
};



