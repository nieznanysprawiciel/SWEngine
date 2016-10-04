#pragma once


#include "GraphicAPI/MeshResources.h"
#include "EngineCore/ModelsManager/Assets/Materials/MaterialAsset.h"



/**@brief Flagi pola Flags w strukturze @ref MeshPart.
@ingroup MeshAsset*/
enum MeshPartFlags : int
{
	UseAdditionalBuffer = 0x01,		///< Materia� u�ywa dodatkowego bufora, kt�ry ma zosta� wype�niony w klasie @ref StaticActor.
	Use4BytesIndex		= 0x02,		///< Dane w buforze indeks�w b�d� traktowane jako 4 bajty zamiast domy�lnych dw�ch.
};

/**@brief Przechowuje informacje o segmencie mesha.

@ingroup MeshAsset*/
struct MeshPart
{
	ResourcePtr< MaterialAsset >	Material;			///< Materia� dla danej cz�ci mesha.
	unsigned int					BufferOffset;		///< Offset wzgl�dem pocz�tku bufora indeks�w albo wierzcho�k�w. @see MeshAsset.
	unsigned int					NumVertices;		///< Liczba wierzcho�k�w do wy�wietlenia.
	int								BaseVertex;			///< Warto�� dodawana do ka�dego indeksu przed przeczytaniem wierzcho�ka z bufora. (Tylko wersja indeksowana).
	int16							Flags;				///< Dodatkowe flagi @see MeshPartFlags
	PrimitiveTopology				Topology;			///< Topologia mesha.
};



