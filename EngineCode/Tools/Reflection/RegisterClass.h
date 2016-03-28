#pragma once

#include <vector>
#include <memory>

#include "TypeInfo.h"
#include "Properties/IMetaProperty.h"


namespace RTTR
{


struct ClassMetaInfoContainer
{
	std::vector< std::unique_ptr< IMetaProperty > >		properties;

	ClassMetaInfoContainer() {}
	ClassMetaInfoContainer( ClassMetaInfoContainer& ) = delete;
	ClassMetaInfoContainer( ClassMetaInfoContainer&& other )
	{
		properties = std::move( other.properties );
	}
};




template< typename ClassType >
class RegisterClass
{
private:

	ClassMetaInfoContainer			classMetaInfo;

public:

	RegisterClass()		{}
	~RegisterClass()	{}


	template< typename PropertyType >
	RegisterClass< ClassType >&		Property		( const char* propertyName, PropertyType ClassType::* memberPtr )
	{
		auto typeInfo = TypeInfo::get< PropertyType >();
		auto baseMemberPtr = static_cast< PropertyType IEnableProperty::* >( memberPtr );

		std::unique_ptr< IMetaProperty > newPropertyMeta( new MetaProperty< PropertyType >( propertyName, typeInfo, baseMemberPtr ) );

		classMetaInfo.properties.push_back( std::move( newPropertyMeta ) );

		return *this;
	}


	ClassMetaInfoContainer	Return()
	{
		ClassMetaInfoContainer container = std::move( classMetaInfo );
		return std::move( container );
	}
};


}	// RTTR
