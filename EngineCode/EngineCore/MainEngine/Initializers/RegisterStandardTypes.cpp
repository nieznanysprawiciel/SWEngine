#include "EngineCore/stdafx.h"

RTTR_REGISTRATION
{
    using namespace rttr;

	RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS( DirectX::XMFLOAT2 )
	RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS( DirectX::XMFLOAT3 )
	RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS( DirectX::XMFLOAT4 )

	registration::class_< DirectX::XMFLOAT4 >( "DirectX::XMFLOAT4" )
		.property( "X", &DirectX::XMFLOAT4::x )
		.property( "Y", &DirectX::XMFLOAT4::y )
		.property( "Z", &DirectX::XMFLOAT4::z )
		.property( "W", &DirectX::XMFLOAT4::w );

	registration::class_< DirectX::XMFLOAT3 >( "DirectX::XMFLOAT3" )
		.property( "X", &DirectX::XMFLOAT4::x )
		.property( "Y", &DirectX::XMFLOAT4::y )
		.property( "Z", &DirectX::XMFLOAT4::z );

	registration::class_< DirectX::XMFLOAT2 >( "DirectX::XMFLOAT2" )
		.property( "X", &DirectX::XMFLOAT4::x )
		.property( "Y", &DirectX::XMFLOAT4::y );
}