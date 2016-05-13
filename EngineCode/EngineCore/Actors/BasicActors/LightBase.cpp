#include "EngineCore/stdafx.h"
#include "LightBase.h"

#include "Common/MemoryLeaks.h"


RTTR_REGISTRATION
{
	rttr::registration::class_< LightBase >( "LightBase" )
		.property( "Color", &LightBase::m_color )
			(	
				rttr::metadata( MetaDataType::Category, "Light" ),
				rttr::policy::prop::BindAsPtr()
			)
		.property( "Intensity", &LightBase::m_intensity )
			(	rttr::metadata( MetaDataType::Category, "Light" )	);
}


/**@brief */
LightBase::LightBase( LightType type )
	:	m_type( type )
	,	m_intensity( 1.0f )
	,	m_color( 1.0f, 1.0f, 1.0f )
{}


