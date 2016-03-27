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

//	static void AccessTest();
//
//
//	BaseClass& operator()()
//	{
//		return *this;
//	}
//
//	BaseClass& Property()
//	{
//		return *this;
//	}
//
//private:
//
//	struct FirstLevelNested
//	{
//		struct SecondLevelNested
//		{
//			static void AccessTest();
//		};
//	};
};

