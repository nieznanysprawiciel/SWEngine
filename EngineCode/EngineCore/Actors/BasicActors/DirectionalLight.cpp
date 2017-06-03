/**
@file DirectionalLight.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "EngineCore/stdafx.h"
#include "DirectionalLight.h"


RTTR_REGISTRATION
{
	rttr::registration::class_< sw::DirectionalLight >( "DirectionalLight" );
}



namespace sw
{


/**@brief */
DirectionalLight::DirectionalLight()
	: LightBase( LightType::DirectionalLight )
{}


}	// sw