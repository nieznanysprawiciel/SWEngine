#include "DerivedClass.h"


DerivedClass::DerivedClass()
{
	PhysicalProperty.Mass = 10021.0f;
	PhysicalProperty.Moment = 0.4312f;
	PhysicalProperty.Radius = 15.2f;

	AddProperty( "PhysicalProperty", static_cast< InnerStruct IEnableProperty::* >( &DerivedClass::PhysicalProperty ) );
}


DerivedClass::~DerivedClass()
{}
