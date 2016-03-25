#pragma once

#include "ClassMetaInfo/EnablePropertyBase.h"


class BaseClass	:	public EnablePropertyBase
{
private:
	
	bool		serializeObject;
	float		positionX;
	float		positionY;
	float		positionZ;
	int			numObjects;

public:
	BaseClass();
	virtual ~BaseClass();
};

