#pragma once

#include "IMetaProperty.h"
#include <cassert>

class IEnableProperty;



class IProperty
{
protected:

	const IMetaProperty*		m_metaInfo;
	IEnableProperty*			m_ownerObj;

public:

	IProperty( const IMetaProperty* metaInfo, IEnableProperty* object )
		:	m_metaInfo( metaInfo )
		,	m_ownerObj( object )
	{}

	const char*		GetPropertyName()		{ return m_metaInfo->GetPropertyName(); }
	TypeInfo		GetPropertyType()		{ return m_metaInfo->GetPropertyType(); }

	bool			IsValid()				{ return m_metaInfo && m_ownerObj; }
};



template< typename PropertyType >
class Property	:	public IProperty
{
private:
public:

	Property( const IMetaProperty* metaInfo, IEnableProperty* object )
		:	IProperty( metaInfo, object )
	{
		//assert( IsValid() );
	}

	inline PropertyType&		operator()( void )
	{
		assert( IsValid() );

		auto typedProperty = static_cast< const MetaProperty< PropertyType >* >( m_metaInfo );
		return m_ownerObj->*typedProperty->GetPtr();
	}

	inline						operator PropertyType&()
	{
		assert( IsValid() );

		auto typedProperty = static_cast< const MetaProperty< PropertyType >* >( m_metaInfo );
		return m_ownerObj->*typedProperty->GetPtr();
	}
};
