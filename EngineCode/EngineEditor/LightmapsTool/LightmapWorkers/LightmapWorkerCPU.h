#pragma once

#include "EngineEditor/LightmapsTool/LightmapWorker.h"
#include <DirectXMath.h>

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


class LightmapWorkerCPU	:	public LightmapWorker
{
private:
	float			m_threshold;
protected:
public:
	LightmapWorkerCPU( SceneData* sceneData );
	~LightmapWorkerCPU() = default;

	/**@brief G³ówna funkcja generuj¹ca lightmapy. Zaimplementuj w klasie pochodnej.*/
	virtual void	Generate() override;
private:
	void			Prepare		( std::vector<MemoryChunk>& emissionLight, std::vector<MemoryChunk>& reachedLight, std::vector<MemoryChunk>& verticies );
	void			Radiosity	( std::vector<MemoryChunk>& emissionLight, std::vector<MemoryChunk>& reachedLight, std::vector<MemoryChunk>& verticies );
	void			BuildResult	( std::vector<MemoryChunk>& reachedLight  );

	void			DepthPass	( std::tuple<unsigned int, unsigned int, float>& emissionMax,
								  std::vector<MemoryChunk>& verticies,
								  MemoryChunk& depthBuffer,
								  MemoryChunk& indexBuffer );
	void			TransferPass( std::tuple<unsigned int, unsigned int, float>& emissionMax,
								  std::vector<MemoryChunk>& verticies,
								  std::vector<MemoryChunk>& emissionLight,
								  std::vector<MemoryChunk>& reachedLight,
								  MemoryChunk& indexBuffer );

	DirectX::XMFLOAT3		HemisphereCast			( Triangle4& emiter, Triangle4& receiver, Triangle4& emiterCoordSystem );
	Triangle4				EmiterCoordinatesSystem	( Triangle4& emiter );
	void					RasterizeTriangle		( const Triangle4& triangle,
													  DirectX::XMFLOAT3* depths,
													  unsigned int chunkIdx,
													  unsigned int triangleIdx,
													  MemoryChunk& depthBuffer,
													  MemoryChunk& indexBuffer );

	DirectX::XMVECTOR		ProjectPointToPlane		( DirectX::XMVECTOR point, const Triangle4& coordsSystem );
	DirectX::XMVECTOR		AverageNormal			( const VertexFormat* triangle );
	//DirectX::XMFLOAT3		ComputeDepths			( const Triangle4& receiver, const Triangle4& emiter );
	float					BarycentricCoords		( DirectX::XMFLOAT2& vertex1, DirectX::XMFLOAT2& vertex2, DirectX::XMINT2& point );
	void					HemisphereViewport		( Triangle4& receiver );

	std::tuple<unsigned int, unsigned int, float>		FindMaxEmision		( std::vector<MemoryChunk>& emisionLight );
};

