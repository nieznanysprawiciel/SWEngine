#pragma once

#include "DX11_interfaces_container.h"
#include "ModelsManager\meshes_textures_materials.h"
#include "DisplayEngine\ConstantBuffersFormat.h"


typedef DX11Renderer Renderer;




/**@brief Renderer obs³uguj¹cy DirectX 11.*/
class DX11Renderer	:	public DX11_interfaces_container
{
private:


public:
	DX11Renderer();
	~DX11Renderer();

	inline bool supportsDefferedContextRendering() { return false; }		///< Informuje czy Renderer nadaje siê do renderowania w wielu w¹tkach do opóŸnionego contextu.

	void setTextures( const ModelPart& model );
	void setIndex_buffer( BufferObject* buffer );
	bool setVertex_buffer( BufferObject* buffer );
	void copyMaterial( ConstantPerMesh* shader_data_per_mesh, const ModelPart* model );
	void depthBufferEnable( bool state );
};



#include "DX11Renderer.inl"
