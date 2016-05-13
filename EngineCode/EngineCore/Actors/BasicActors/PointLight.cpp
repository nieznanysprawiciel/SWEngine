#include "EngineCore/stdafx.h"
#include "PointLight.h"

#include "Common/MemoryLeaks.h"

RTTR_REGISTRATION
{
	rttr::registration::class_< PointLight >( "PointLight" )
		.property( "Radius", &PointLight::m_radius )
			(	rttr::metadata( MetaDataType::Category, "Light" )	);
}


/**@brief */
PointLight::PointLight()
	:	LightBase( LightType::PointLight )
	,	m_radius( 30.0f )
{}

/**@brief Konstruktor dla klas dziedzicz¹cych.*/
PointLight::PointLight	( LightType type )
	: LightBase( type )
{}

