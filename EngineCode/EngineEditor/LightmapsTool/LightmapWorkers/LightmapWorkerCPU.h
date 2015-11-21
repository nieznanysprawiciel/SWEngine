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

	DirectX::XMVECTOR		HemisphereRatio		( Triangle4& emiter, Triangle4& receiver );
	DirectX::XMVECTOR		ProjectPointToPlane	( DirectX::XMVECTOR plane, DirectX::XMVECTOR planePoint, DirectX::XMVECTOR point );
	DirectX::XMVECTOR		AverageNormal		( const VertexFormat* triangle );

	std::tuple<unsigned int, unsigned int, float>		FindMaxEmision		( std::vector<MemoryChunk>& emisionLight );
};

