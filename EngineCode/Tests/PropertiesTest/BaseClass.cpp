#include "BaseClass.h"

RTTR_REGISTRATION
{
	rttr::registration::class_< BaseClass >( "BaseClass" )
		.property( "SerializeObject", &BaseClass::serializeObject )
		.property( "PositionX", &BaseClass::positionX )
		.property( "PositionY", &BaseClass::positionY )
		.property( "PositionZ", &BaseClass::positionZ )
		.property( "NumObjects", &BaseClass::numObjects )
		.property( "Rotation", &BaseClass::rotation )
		( rttr::policy::prop::BindAsPtr() );
}




BaseClass::BaseClass()
{
	serializeObject = true;;
	positionX = 1.0f;
	positionY = 2.0f;
	positionZ = 3.0f;
	numObjects = 13;
	rotation = DirectX::XMFLOAT4( 13.0, 13.0, 13.0, 13.0 );

	//AddProperty( "SerializeObject", static_cast< bool IEnableProperty::* >( &BaseClass::serializeObject ) );
	//AddProperty( "PositionX", static_cast< float IEnableProperty::* >( &BaseClass::positionX ) );
	//AddProperty( "PositionY", static_cast< float IEnableProperty::* >( &BaseClass::positionY ) );
	//AddProperty( "PositionZ", static_cast< float IEnableProperty::* >( &BaseClass::positionZ ) );
	//AddProperty( "NumObjects", static_cast< int IEnableProperty::* >( &BaseClass::numObjects ) );

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
