#pragma once
/**
@file Animation.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "swGraphicAPI/Resources/ResourceObject.h"

#include "EngineCore/ModelsManager/Assets/Animations/KeySet.h"
#include "Bone.h"

#include <vector>
#include <DirectXMath.h>




namespace sw
{

/**@defgroup AnimationAsset Animation Asset
@ingroup Assets*/


typedef DirectX::XMFLOAT4X4 BoneTransform;

/**@brief
@ingroup AnimationAsset*/
class AnimationAsset : public ResourceObject
{
	RTTR_ENABLE( ResourceObject );
	RTTR_REGISTRATION_FRIEND

		friend ObjectDeleter< AnimationAsset >;
private:

	std::vector< BoneRef >						m_boneIdx;
	std::vector< KeySet< BoneTransform > >		m_animationKeys;

	float										m_animationTime;

protected:
public:
	explicit		AnimationAsset() = default;
	~AnimationAsset() = default;

};


}	// sw
