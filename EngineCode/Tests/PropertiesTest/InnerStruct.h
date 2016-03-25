#pragma once


#include "Common/RTTR.h"
#include "ClassMetaInfo/EnablePropertyBase.h"


struct InnerStruct	: public EnablePropertyBase
{
	float		Mass;
	float		Radius;
	float		Moment;

	InnerStruct();
};

RTTR_DECLARE_STANDARD_META_TYPE_VARIANTS( InnerStruct )
