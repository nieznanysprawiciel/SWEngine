#pragma once

#include "BaseClass.h"
#include "InnerStruct.h"

class DerivedClass	:	public BaseClass
{
	RTTR_ENABLE_DERIVED_FROM( BaseClass )
	RTTR_ENABLE_PRIVATE_REGISTRATION( DerivedClass )
private:

	InnerStruct			PhysicalProperty;
	InnerStruct*		PhysicalPropertyPrev;

public:
	DerivedClass();
	virtual ~DerivedClass();
};
RTTR_DECLARE_STANDARD_META_TYPE_VARIANTS( DerivedClass )
