#pragma once

#include <map>
#include <string>

#include "Common/Properties/IEnableProperty.h"


class EnablePropertyBase	: public IEnableProperty
{
	RTTR_ENABLE( IEnableProperty )
private:
public:

	EnablePropertyBase();
	virtual ~EnablePropertyBase();

protected:

	//template< typename PropertyType >
	//void AddProperty		( const char* propertyName, PropertyType IEnableProperty::* memberPtr );
	//
private:

	//virtual const IMetaProperty*		GetMetaProperty	( const char* propertyName ) override;

};

//RTTR_DECLARE_STANDARD_META_TYPE_VARIANTS( EnablePropertyBase )

