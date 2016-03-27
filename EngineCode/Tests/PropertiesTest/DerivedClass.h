#pragma once

#include "BaseClass.h"
#include "InnerStruct.h"

class DerivedClass	:	public BaseClass
{
private:

	InnerStruct			PhysicalProperty;
	InnerStruct*		PhysicalPropertyPrev;

public:
	DerivedClass();
	virtual ~DerivedClass();
};

