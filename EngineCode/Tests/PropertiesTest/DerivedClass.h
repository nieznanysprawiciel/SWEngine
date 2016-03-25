#pragma once

#include "BaseClass.h"
#include "InnerStruct.h"

class DerivedClass	:	public BaseClass
{
private:

	InnerStruct			PhysicalProperty;

public:
	DerivedClass();
	virtual ~DerivedClass();
};

