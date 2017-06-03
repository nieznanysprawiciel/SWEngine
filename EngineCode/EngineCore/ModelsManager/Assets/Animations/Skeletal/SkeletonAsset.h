#pragma once
/**
@file SkeletonAsset.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/


#include "swCommonLib/System/Path.h"
#include "swGraphicAPI/Resources/ResourceObject.h"
#include "Bone.h"

#include <vector>
#include <string>



namespace sw
{

/**@brief Skeleton descriptor.
@ingroup AnimationAsset*/
struct SkeletonInfo
{
	filesystem::Path		FileName;
};


/**@brief Skeleton.
@ingroup AnimationAsset*/
class SkeletonAsset : public ResourceObject
{
	RTTR_ENABLE( ResourceObject );
	RTTR_REGISTRATION_FRIEND

		friend ObjectDeleter< SkeletonAsset >;
private:

	std::vector< Bone >		Bones;
	SkeletonInfo			Info;

protected:
public:
	explicit		SkeletonAsset() = default;
	~SkeletonAsset() = default;

};

}	// sw

