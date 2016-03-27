#include "DerivedClass.h"


DerivedClass::DerivedClass()
{
	PhysicalProperty.Mass = 10021.0f;
	PhysicalProperty.Moment = 0.4312f;
	PhysicalProperty.Radius = 15.2f;

	PhysicalPropertyPrev = new InnerStruct;
	PhysicalPropertyPrev->Mass = 233.42;
	PhysicalPropertyPrev->Moment = 16.3;
	PhysicalPropertyPrev->Radius = 56;

	AddProperty( "PhysicalProperty", static_cast< InnerStruct IEnableProperty::* >( &DerivedClass::PhysicalProperty ) );
	AddProperty( "PhysicalPropertyPrev", static_cast< InnerStruct* IEnableProperty::* >( &DerivedClass::PhysicalPropertyPrev ) );
}


DerivedClass::~DerivedClass()
{}
