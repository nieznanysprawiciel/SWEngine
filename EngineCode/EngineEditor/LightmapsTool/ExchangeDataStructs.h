#pragma once

#include <DirectXMath.h>
#include <vector>
#include "Common/SWEngineInclude.h"

/**Stan generowania lightmap.*/
enum class LightmapState
{
	Generating,
	GenerationEnded,
	RenderingLightmaps,
	ReadyToGenerate
};

/**Dane poszczeg�lnych cz�ci obiekt�w sceny.*/
struct SceneObjectData
{
	DynamicMeshActor*		object;				///< Obiekt, do kt�rego nale�y dana cz�� mesha. Potrzebujemy, �eby potem stworzy� tekstury i ustawi� dla odpowiednich obiekt�w.
	Size					chunkIdx;			///< Wskazuje na obiekt z danymi wierzcho�k�w w strukturze SceneData.
	DirectX::XMFLOAT4X4		transform;			///< Macierz przekszta�cenia dla danej cz�ci mesha.
	unsigned int			bufferOffset;		///< Offset wzgl�dem pocz�tku bufora albo wierzcho�k�w.
	unsigned int			verticesCount;		///< Liczba wierzcho�k�w w danym kawa�ku mesha.
	DirectX::XMFLOAT4		diffuse;			///< Materia� - kana� diffuse, czyli jedyny, kt�ry bierzemy pod uwag�.
	DirectX::XMFLOAT4		emissive;			///< Je�eli wierzcho�ki opisuj� �r�d�o �wiat�a, to ten kana� powinien by� niezerowy.
};

/**Dane sceny.*/
struct SceneData
{
	std::vector<MemoryChunk>		verticies;	///< Bufory wierzcho�k�w. MemoryChunki s� unikatowe. Wiele obiekt�w mo�e si� odwo�ywa� do jednego chunka.
	std::vector<BufferObject*>		buffers;	///< Silnikowe obiekty bufor�w. Odpowiadaj� MemoryChunkom.
	std::vector<SceneObjectData>	objectParts;///< Obiekty s� przechowywane w cz�ciach, podzielonych po materia�ach.
};

