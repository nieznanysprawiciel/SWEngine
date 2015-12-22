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
	//DirectX::XMFLOAT3	normal;
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

	/**@brief G��wna funkcja generuj�ca lightmapy. Zaimplementuj w klasie pochodnej.*/
	virtual void	Generate() override;
private:
	void			Prepare		( thrust::device_vector<DirectX::XMFLOAT3>& emissionLight, thrust::device_vector<DirectX::XMFLOAT3>& reachedLight, thrust::device_vector<VertexFormat>& verticies, std::vector<Size>& chunkOffsets );
	void			Radiosity	( thrust::device_vector<DirectX::XMFLOAT3>& emissionLight, thrust::device_vector<DirectX::XMFLOAT3>& reachedLight, thrust::device_vector<VertexFormat>& verticies, std::vector<Size>& chunkOffsets );
	void			BuildResult	( thrust::device_vector<DirectX::XMFLOAT3>& reachedLight  );

	void			DepthPass(	std::tuple<unsigned int, unsigned int, float>& emissionMax,
								thrust::device_vector<VertexFormat>& verticies,
								float* deviceDepthBuffer,
								BufferIndexing* deviceIndexBuffer,
								std::vector<Size>& chunkOffsets);

	void			TransferPass(	std::tuple<unsigned int, unsigned int, float>& emissionMax,
									thrust::device_vector<VertexFormat>& verticies,
									thrust::device_vector<DirectX::XMFLOAT3>& emissionLight,
									thrust::device_vector<DirectX::XMFLOAT3>& reachedLight,
									BufferIndexing* indexBuffer,
									std::vector<Size>& chunkOffsets );

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

