#include "DerivedClass.h"



RTTR_REGISTRATION
{
	rttr::registration::class_< DerivedClass >( "DerivedClass" )
		.property( "PhysicalProperty", &DerivedClass::PhysicalProperty )
		.property( "PhysicalPropertyPrev", &DerivedClass::PhysicalPropertyPrev )
		.property( "Texture", &DerivedClass::Texture );
}



DerivedClass::DerivedClass()
{
	PhysicalProperty.Mass = 10021.0f;
	PhysicalProperty.Moment = 0.4312f;
	PhysicalProperty.Radius = 15.2f;

	PhysicalPropertyPrev = new InnerStruct;
	PhysicalPropertyPrev->Mass = 233.42f;
	PhysicalPropertyPrev->Moment = 16.3f;
	PhysicalPropertyPrev->Radius = 56;
}


DerivedClass::~DerivedClass()
{}
