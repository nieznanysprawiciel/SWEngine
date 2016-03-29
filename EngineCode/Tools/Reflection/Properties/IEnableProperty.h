#pragma once

#include "Property.h"


class IEnableProperty
{
	RTTR_ENABLE()
private:

	virtual const IMetaProperty*		GetMetaProperty	( const char* propertyName ) = 0;

public:
	
	inline IProperty			GetProperty		( const char* propertyName )
	{
		const IMetaProperty* metaProperty = GetMetaProperty( propertyName );
		return IProperty( metaProperty, this );
	}

	template< typename PropertyType >
	inline Property< PropertyType >		GetTypedProperty		( const char* propertyName )
	{
		const IMetaProperty* metaProperty = GetMetaProperty( propertyName );

		if( metaProperty && metaProperty->GetPropertyType().isTypeDerivedFrom< PropertyType >() )
			return Property< PropertyType >( metaProperty, this );

		// Types are inconsistent. Return invalid property.
		return Property< PropertyType >( nullptr, this );
	}
};
RTTR_DECLARE_STANDARD_META_TYPE_VARIANTS( IEnableProperty )
