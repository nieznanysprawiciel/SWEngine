#include "EnablePropertyBase.h"



RTTR_REGISTRATION
{
	rttr::registration::class_<EnablePropertyBase>( "EnablePropertyBase" );
}



/**@brief */
EnablePropertyBase::EnablePropertyBase()
{ }

/**@brief */
EnablePropertyBase::~EnablePropertyBase()
{
}

///**@brief */
//const IMetaProperty*		EnablePropertyBase::GetMetaProperty	( const char* propertyName )
//{
//	auto typeInfo = getTypeInfo();
//
//	return typeInfo.GetProperty( propertyName );
//}