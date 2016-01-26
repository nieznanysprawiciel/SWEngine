#pragma once

#include "EngineEditor/LightmapsTool/LightmapWorker.h"
//#include "cudaDirectXMath.h"
#include "glm/glm.hpp"

#include <thrust/functional.h>
#include <thrust/random.h>
#include <thrust/device_vector.h>
#include <thrust/transform_reduce.h>
#include <thrust/iterator/counting_iterator.h>
#include <thrust/iterator/zip_iterator.h>

struct VertexFormat
{
	glm::vec3	position;
	//float3	normal;

};

struct Triangle4
{
	glm::vec3	vertex1;
	glm::vec3	vertex2;
	glm::vec3	vertex3;

	Triangle4() = default;
	inline Triangle4( VertexFormat* vertexPtr )
	{
		vertex1 = (vertexPtr++)->position;
		vertex2 = (vertexPtr++)->position;
		vertex3 = vertexPtr->position;
	}
};

struct Triangle3
{
	glm::vec3	vertex1;
	glm::vec3	vertex2;
	glm::vec3	vertex3;
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
	void			Prepare		( thrust::device_vector<glm::vec3>& emissionLight, thrust::device_vector<glm::vec3>& reachedLight, thrust::device_vector<VertexFormat>& verticies, std::vector<Size>& chunkOffsets );
	void			Radiosity	( thrust::device_vector<glm::vec3>& emissionLight, thrust::device_vector<glm::vec3>& reachedLight, thrust::device_vector<VertexFormat>& verticies, std::vector<Size>& chunkOffsets );
	void			BuildResult	( thrust::device_vector<glm::vec3>& reachedLight  );

	void			DepthPass(	std::tuple<unsigned int, unsigned int, float>& emissionMax,
								thrust::device_vector<VertexFormat>& verticies,
								float* deviceDepthBuffer,
								BufferIndexing* deviceIndexBuffer,
								std::vector<Size>& chunkOffsets);

	void			TransferPass(	std::tuple<unsigned int, unsigned int, float>& emissionMax,
									thrust::device_vector<VertexFormat>& verticies,
									thrust::device_vector<glm::vec3>& emissionLight,
									thrust::device_vector<glm::vec3>& reachedLight,
									BufferIndexing* indexBuffer,
									std::vector<Size>& chunkOffsets );

	glm::vec3				HemisphereCast			( Triangle4& emiter, Triangle4& receiver, DirectX::XMMATRIX& emiterViewMatrix );
	Triangle4				EmiterCoordinatesSystem	( Triangle4& emiter );
	glm::mat4				EmiterViewMatrix		( Triangle4& emiter );
	void					RasterizeTriangle		( const Triangle4& triangle,
													  glm::vec3* depths,
													  unsigned int chunkIdx,
													  unsigned int triangleIdx,
													  MemoryChunk& depthBuffer,
													  MemoryChunk& indexBuffer );

	glm::vec4				ProjectPointToPlane		( glm::vec4 point, const Triangle4& coordsSystem );

	std::tuple<unsigned int, unsigned int, float>		FindMaxEmision		( std::vector<MemoryChunk>& emisionLight );
};

