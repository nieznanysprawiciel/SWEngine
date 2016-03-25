#pragma once

#include <map>
#include <string>

#include "Common/Properties/IEnableProperty.h"


class EnablePropertyBase	: public IEnableProperty
{
	RTTR_ENABLE_DERIVED_FROM( IEnableProperty )
private:

	std::map< std::string, IMetaProperty* >		m_propertiesMetaData;

public:

	EnablePropertyBase();
	virtual ~EnablePropertyBase();

protected:

	template< typename PropertyType >
	void AddProperty		( const char* propertyName, PropertyType IEnableProperty::* memberPtr );
	
private:

	virtual IMetaProperty*		GetMetaProperty	( const char* propertyName ) override;

};

RTTR_DECLARE_STANDARD_META_TYPE_VARIANTS( EnablePropertyBase )



template<typename PropertyType>
inline void		EnablePropertyBase::AddProperty		( const char* propertyName, PropertyType IEnableProperty::* memberPtr )
{
	MetaProperty< PropertyType >* newMetaProperty = new MetaProperty< PropertyType >( propertyName, RTTR::TypeInfo::get< PropertyType >(), memberPtr );
	m_propertiesMetaData[ propertyName ] = newMetaProperty;
}
