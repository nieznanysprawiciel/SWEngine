#include "BaseClass.h"

RTTR_REGISTRATION_VARIANTS( BaseClass )
{
	return RTTR::RegisterClass< BaseClass >()
		.Property( "SerializeObject", &BaseClass::serializeObject )
		.Property( "PositionX", &BaseClass::positionX )
		.Property( "PositionY", &BaseClass::positionY )
		.Property( "PositionZ", &BaseClass::positionZ )
		.Property( "NumObjects", &BaseClass::numObjects )
		.Return();
}



BaseClass::BaseClass()
{
	serializeObject = true;;
	positionX = 1.0f;
	positionY = 2.0f;
	positionZ = 3.0f;
	numObjects = 13;

	AddProperty( "SerializeObject", static_cast< bool IEnableProperty::* >( &BaseClass::serializeObject ) );
	AddProperty( "PositionX", static_cast< float IEnableProperty::* >( &BaseClass::positionX ) );
	AddProperty( "PositionY", static_cast< float IEnableProperty::* >( &BaseClass::positionY ) );
	AddProperty( "PositionZ", static_cast< float IEnableProperty::* >( &BaseClass::positionZ ) );
	AddProperty( "NumObjects", static_cast< int IEnableProperty::* >( &BaseClass::numObjects ) );

}


BaseClass::~BaseClass()
{}


//void BaseClass::AccessTest()
//{
//	int BaseClass::*NumObjPtr = &BaseClass::numObjects;
//}
//
//
//
//void BaseClass::FirstLevelNested::SecondLevelNested::AccessTest()
//{
//	int BaseClass::*NumObjPtr = &BaseClass::numObjects;
//}
//
