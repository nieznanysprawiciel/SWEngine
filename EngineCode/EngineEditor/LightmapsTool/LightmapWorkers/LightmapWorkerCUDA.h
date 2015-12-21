#pragma once

#include "EngineEditor/LightmapsTool/LightmapWorker.h"
#include <DirectXMath.h>

#include <thrust/functional.h>
#include <thrust/random.h>
#include <thrust/device_vector.h>
#include <thrust/transform_reduce.h>
#include <thrust/iterator/counting_iterator.h>
#include <thrust/iterator/zip_iterator.h>

struct VertexFormat
{
	DirectX::XMFLOAT3	position;
	DirectX::XMFLOAT3	normal;
};

struct Triangle4
{
	DirectX::XMVECTOR	vertex1;
	DirectX::XMVECTOR	vertex2;
	DirectX::XMVECTOR	vertex3;

	Triangle4() = default;
	inline Triangle4( VertexFormat* vertexPtr )
	{
		vertex1 = XMLoadFloat3( &(vertexPtr++)->position );
		vertex2 = XMLoadFloat3( &(vertexPtr++)->position );
		vertex3 = XMLoadFloat3( &vertexPtr->position );
	}
};

struct Triangle3
{
	DirectX::XMFLOAT3	vertex1;
	DirectX::XMFLOAT3	vertex2;
	DirectX::XMFLOAT3	vertex3;
};

struct BufferIndexing
{
	uint16	first;
	uint16	second;
};

static const uint16 INVALID_INDEX = 0xFFFF;

typedef thrust::device_vector<MemoryChunk> MemoryChunkVector;


class LightmapWorkerCUDA	:	public LightmapWorker
{
private:
	float			m_threshold;
protected:
public:
	LightmapWorkerCUDA( SceneData* sceneData );
	~LightmapWorkerCUDA() = default;

	/**@brief G³ówna funkcja generuj¹ca lightmapy. Zaimplementuj w klasie pochodnej.*/
	virtual void	Generate() override;
private:
	void			Prepare		( std::vector<MemoryChunk>& emissionLight, std::vector<MemoryChunk>& reachedLight, std::vector<MemoryChunk>& verticies );
	void			Radiosity	( std::vector<MemoryChunk>& emissionLight, std::vector<MemoryChunk>& reachedLight, std::vector<MemoryChunk>& verticies );
	void			BuildResult	( std::vector<MemoryChunk>& reachedLight  );

	void			DepthPass(	std::tuple<unsigned int, unsigned int, float>& emissionMax,
								BufferObject *dev_verticies, unsigned verticiesSize, BufferObject *dev_depthBuffer, BufferObject *dev_indexBuffer);

	void			TransferPass( std::tuple<unsigned int, unsigned int, float>& emissionMax,
								  BufferObject *verticies, unsigned verticiesSize,
								  BufferObject *emissionLight,
								  BufferObject *reachedLight,
								  BufferIndexing *indexBuffer);

	DirectX::XMFLOAT3		HemisphereCast			( Triangle4& emiter, Triangle4& receiver, DirectX::XMMATRIX& emiterViewMatrix );
	Triangle4				EmiterCoordinatesSystem	( Triangle4& emiter );
	DirectX::XMMATRIX		EmiterViewMatrix		( Triangle4& emiter );
	void					RasterizeTriangle		( const Triangle4& triangle,
													  DirectX::XMFLOAT3* depths,
													  unsigned int chunkIdx,
													  unsigned int triangleIdx,
													  MemoryChunk& depthBuffer,
													  MemoryChunk& indexBuffer );

	DirectX::XMVECTOR		ProjectPointToPlane		( DirectX::XMVECTOR point, const Triangle4& coordsSystem );

	std::tuple<unsigned int, unsigned int, float>		FindMaxEmision		( std::vector<MemoryChunk>& emisionLight );
};

