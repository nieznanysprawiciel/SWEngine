/**
@file SpotLight.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "EngineCore/stdafx.h"
#include "SpotLight.h"

#include "swCommonLib/Common/MemoryLeaks.h"

RTTR_REGISTRATION
{
	rttr::registration::class_< SpotLight >( "SpotLight" )
		.property( "Angle", &SpotLight::m_angle )
			(	rttr::metadata( MetaDataType::Category, "Light" )	);
}


/**@brief */
SpotLight::SpotLight()
	:	PointLight( LightType::SpotLight )
	,	m_angle( 30.0f )
{}

