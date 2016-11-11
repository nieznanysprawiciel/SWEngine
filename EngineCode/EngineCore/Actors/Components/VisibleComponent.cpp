#include "EngineCore/stdafx.h"
/**
@file VisibleComponent.cpp
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "VisibleComponent.h"


RTTR_REGISTRATION
{
	rttr::registration::class_< VisibleComponent >( "VisibleComponent" )
		.property( "Asset", &VisibleComponent::m_mesh )
		.property_readonly( "ActorTextures", &VisibleComponent::m_textures )
		.property_readonly( "AdditionalBuffers", &VisibleComponent::m_addititonalBuffers );
}


// ================================ //
//
void									VisibleComponent::SetAsset	( const ResourcePtr< MeshAsset >& mesh )
{
	m_mesh = mesh;
}

// ================================ //
//
const ResourcePtr< MeshAsset >&			VisibleComponent::GetAsset() const
{
	return m_mesh;
}

// ================================ //
//
const std::vector< TextureBinding >&	VisibleComponent::GetActorTextures() const
{
	return m_textures;
}

// ================================ //
//
const std::vector< BufferBinding >&		VisibleComponent::GetAdditionalBuffers() const
{
	return m_addititonalBuffers;
}
