#include "EnablePropertyBase.h"


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
IMetaProperty*		EnablePropertyBase::GetMetaProperty	( const char* propertyName )
{
	std::string propName = propertyName;

	auto iter = m_propertiesMetaData.find( propName );
	if( iter == m_propertiesMetaData.end() )
		return nullptr;

	return iter->second;
}