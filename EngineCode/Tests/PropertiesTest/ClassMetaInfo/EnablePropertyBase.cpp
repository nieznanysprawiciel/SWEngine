#include "EnablePropertyBase.h"

RTTR_DEFINE_STANDARD_META_TYPE_VARIANTS( EnablePropertyBase )


/**@brief */
EnablePropertyBase::EnablePropertyBase()
{ }

/**@brief */
EnablePropertyBase::~EnablePropertyBase()
{
	for( auto& metaData : m_propertiesMetaData )
	{
		delete metaData.second;
	}
}

/**@brief */
const IMetaProperty*		EnablePropertyBase::GetMetaProperty	( const char* propertyName )
{
	auto typeInfo = getTypeInfo();

	return typeInfo.GetProperty( propertyName );
}