#pragma once

#include "ClassMetaInfo/EnablePropertyBase.h"
#include <DirectXMath.h>


class BaseClass	:	public EnablePropertyBase
{
	RTTR_ENABLE( EnablePropertyBase )
	RTTR_REGISTRATION_FRIEND

private:
	
	bool		serializeObject;
	float		positionX;
	float		positionY;
	float		positionZ;
	int			numObjects;

	DirectX::XMFLOAT4		rotation;

public:
	BaseClass();
	virtual ~BaseClass();

};

