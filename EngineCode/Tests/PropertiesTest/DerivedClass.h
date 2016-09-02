#pragma once

#include "BaseClass.h"
#include "InnerStruct.h"
#include "GraphicAPI/MeshResources.h"

class DerivedClass	:	public BaseClass
{
	RTTR_ENABLE( BaseClass )
	RTTR_REGISTRATION_FRIEND

private:

	InnerStruct			PhysicalProperty;
	InnerStruct*		PhysicalPropertyPrev;
	
	ResourcePtr< TextureObject >	Texture;

public:
	DerivedClass();
	virtual ~DerivedClass();
};

