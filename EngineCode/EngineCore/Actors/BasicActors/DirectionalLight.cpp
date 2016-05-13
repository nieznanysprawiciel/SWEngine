#include "EngineCore/stdafx.h"
#include "DirectionalLight.h"


RTTR_REGISTRATION
{
	rttr::registration::class_< DirectionalLight >( "DirectionalLight" );
}


/**@brief */
DirectionalLight::DirectionalLight()
	:	LightBase( LightType::DirectionalLight )
{}

