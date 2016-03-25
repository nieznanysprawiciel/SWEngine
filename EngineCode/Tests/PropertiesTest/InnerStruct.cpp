#include "InnerStruct.h"

RTTR_DEFINE_STANDARD_META_TYPE_VARIANTS( InnerStruct )


InnerStruct::InnerStruct()
{
	AddProperty( "Mass", static_cast< float IEnableProperty::* >( &InnerStruct::Mass ) );
	AddProperty( "Moment", static_cast< float IEnableProperty::* >( &InnerStruct::Moment ) );
	AddProperty( "Radius", static_cast< float IEnableProperty::* >( &InnerStruct::Radius ) );
}
