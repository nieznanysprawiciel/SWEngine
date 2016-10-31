#pragma once
/**
@file VisibleComponent.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "Common/RTTR.h"
#include "EngineCore/ModelsManager/Assets/Meshes/MeshAsset.h"



/**@brief Mesh data.

@ingroup Components*/
class VisibleComponent
{
	RTTR_REGISTRATION_FRIEND;
private:
	ResourcePtr< MeshAsset >		m_mesh;

protected:
public:
	explicit		VisibleComponent	() = default;
					~VisibleComponent	() = default;

	void							SetAsset			( const ResourcePtr< MeshAsset >& mesh );
	const ResourcePtr< MeshAsset >&	GetAsset			() const;
};
