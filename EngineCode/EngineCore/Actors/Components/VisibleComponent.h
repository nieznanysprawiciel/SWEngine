#pragma once
/**
@file VisibleComponent.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "swCommonLib/Common/RTTR.h"
#include "EngineCore/ModelsManager/Assets/Meshes/MeshAsset.h"
#include "EngineCore/ModelsManager/Assets/PipelineBinding.h"


#include <vector>


namespace sw
{

typedef Binding< TextureObject > TextureBinding;
typedef Binding< BufferObject > BufferBinding;


/**@brief Mesh data.

Additional buffers can't be updated yet. @ref BufferUpdater should be placed separatly in
DynamicActor class.

@todo Optimize memory usage when no additional textures and buffers are needed.

@ingroup Components*/
class VisibleComponent
{
	RTTR_REGISTRATION_FRIEND;
private:
	ResourcePtr< MeshAsset >		m_mesh;

	std::vector< TextureBinding >	m_textures;
	std::vector< BufferBinding >	m_addititonalBuffers;

protected:
public:
	explicit		VisibleComponent	() = default;
	~VisibleComponent	() = default;

	void									SetAsset			( const ResourcePtr< MeshAsset >& mesh );
	const ResourcePtr< MeshAsset >&			GetAsset			() const;

	const std::vector< TextureBinding >&	GetActorTextures	() const;
	const std::vector< BufferBinding >&		GetAdditionalBuffers() const;
};

}	// sw

