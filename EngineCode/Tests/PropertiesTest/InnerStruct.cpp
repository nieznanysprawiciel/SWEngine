#include "InnerStruct.h"

RTTR_REGISTRATION_VARIANTS( InnerStruct )
{
	return RTTR::RegisterClass< InnerStruct >()
		.Property( "Mass", &InnerStruct::Mass )
		.Property( "Moment", &InnerStruct::Moment )
		.Property( "Radius", &InnerStruct::Radius )
		.Return();
}


InnerStruct::InnerStruct()
{
	AddProperty( "Mass", static_cast< float IEnableProperty::* >( &InnerStruct::Mass ) );
	AddProperty( "Moment", static_cast< float IEnableProperty::* >( &InnerStruct::Moment ) );
	AddProperty( "Radius", static_cast< float IEnableProperty::* >( &InnerStruct::Radius ) );
}
