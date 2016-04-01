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

/**Dane poszczególnych czêœci obiektów sceny.*/
struct SceneObjectData
{
	DynamicMeshActor*		object;				///< Obiekt, do którego nale¿y dana czêœæ mesha. Potrzebujemy, ¿eby potem stworzyæ tekstury i ustawiæ dla odpowiednich obiektów.
	Size					chunkIdx;			///< Wskazuje na obiekt z danymi wierzcho³ków w strukturze SceneData.
	DirectX::XMFLOAT4X4		transform;			///< Macierz przekszta³cenia dla danej czêœci mesha.
	unsigned int			bufferOffset;		///< Offset wzglêdem pocz¹tku bufora albo wierzcho³ków.
	unsigned int			verticesCount;		///< Liczba wierzcho³ków w danym kawa³ku mesha.
	DirectX::XMFLOAT4		diffuse;			///< Materia³ - kana³ diffuse, czyli jedyny, który bierzemy pod uwagê.
	DirectX::XMFLOAT4		emissive;			///< Je¿eli wierzcho³ki opisuj¹ Ÿród³o œwiat³a, to ten kana³ powinien byæ niezerowy.
};

/**Dane sceny.*/
struct SceneData
{
	std::vector<MemoryChunk>		verticies;	///< Bufory wierzcho³ków. MemoryChunki s¹ unikatowe. Wiele obiektów mo¿e siê odwo³ywaæ do jednego chunka.
	std::vector<BufferObject*>		buffers;	///< Silnikowe obiekty buforów. Odpowiadaj¹ MemoryChunkom.
	std::vector<SceneObjectData>	objectParts;///< Obiekty s¹ przechowywane w czêœciach, podzielonych po materia³ach.
};

