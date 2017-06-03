/**
@file PointLight.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "EngineCore/stdafx.h"
#include "PointLight.h"

#include "swCommonLib/Common/MemoryLeaks.h"




RTTR_REGISTRATION
{
	rttr::registration::class_< sw::PointLight >( "PointLight" )
		.property( "ClampRadius", &sw::PointLight::m_clampRadius )
			(	rttr::metadata( MetaDataType::Category, "Light" )	)
		.property( "ConstantAttenuation", &sw::PointLight::m_constAttenuation )
			(	rttr::metadata( MetaDataType::Category, "Light" )	)
		.property( "LinearAttenuation", &sw::PointLight::m_linearAttenuation )
			(	rttr::metadata( MetaDataType::Category, "Light" )	)
		.property( "QuadraticAttenuation", &sw::PointLight::m_quadraticAttentuation )
			(	rttr::metadata( MetaDataType::Category, "Light" )	);
}


namespace sw
{


/**@brief Sets light as default real world light (quadratic attenuation) and very big clamp radius.*/
PointLight::PointLight()
	: LightBase( LightType::PointLight )
	, m_clampRadius( 300000.0f )
	, m_constAttenuation( 0.0 )
	, m_linearAttenuation( 0.0 )
	, m_quadraticAttentuation( 1.0 )
{}

/**@brief Konstruktor dla klas dziedzicz¹cych.*/
PointLight::PointLight	( LightType type )
	: LightBase( type )
	, m_clampRadius( 300000.0f )
	, m_constAttenuation( 0.0 )
	, m_linearAttenuation( 0.0 )
	, m_quadraticAttentuation( 1.0 )
{}

}	// sw
