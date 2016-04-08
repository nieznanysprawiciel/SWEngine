#pragma once

#include "BaseClass.h"
#include "InnerStruct.h"

class DerivedClass	:	public BaseClass
{
	RTTR_ENABLE( BaseClass )
	RTTR_REGISTRATION_FRIEND

private:

	InnerStruct			PhysicalProperty;
	InnerStruct*		PhysicalPropertyPrev;

public:
	DerivedClass();
	virtual ~DerivedClass();
};

