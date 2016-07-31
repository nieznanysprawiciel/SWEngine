#pragma once

#include "ClassMetaInfo/EnablePropertyBase.h"
#include <DirectXMath.h>


class BaseClass	:	public EnablePropertyBase
{
	RTTR_ENABLE( EnablePropertyBase )
	RTTR_REGISTRATION_FRIEND

private:
	
	bool		serializeObject;
	float		positionX;
	float		positionY;
	float		positionZ;
	int			numObjects;

	DirectX::XMFLOAT4		rotation;

public:
	BaseClass();
	virtual ~BaseClass();


private:

	//struct PropertyBase
	//{};

	//template< typename ClassType, int ClassThisOffset >
	//struct Property : public PropertyBase
	//{
	//
	//private:
	//	rttr::property	GetProperty( const char* name )
	//	{
	//		static rttr::property thisProp = rttr::type::get< ClassType >().get_property( name );
	//		return thisProp;
	//	}

	//	ClassType* GetClassPtr()
	//	{
	//		return reinterpret_cast< void* >( this ) - PropertyStructPtr;
	//	}
	//};

	//Property< BaseClass, offsetof( BaseClass, SerializeObject ) >		SerializeObject;
};

