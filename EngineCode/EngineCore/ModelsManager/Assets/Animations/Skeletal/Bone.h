#pragma once
/**
@file Bone.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "swCommonLib/Common/TypesDefinitions.h"

#include <string>
#include <vector>

#include <DirectXMath.h>

struct Bone;


//typedef uint16 BoneRef;
typedef Bone* BoneRef;

/**@brief Bone descriptor.
@ingroup AnimationAsset*/
struct BoneInfo
{
	std::vector< BoneRef >	Children;
	std::string				BoneName;
};


/**@brief Bone structure.
@ingroup AnimationAsset*/
struct Bone
{
	DirectX::XMFLOAT3		Offset;
	BoneRef					Parent;
	UPtr< BoneInfo >		Info;
};

