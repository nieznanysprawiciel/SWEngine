#include "EngineCore/stdafx.h"

#include "Common/RTTR.h"

#include "ShadingModelData.h"
#include "PhongMaterialData.h"

#include "Common/MemoryLeaks.h"


RTTR_REGISTRATION
{
	rttr::registration::class_< ShadingModelBase >( "ShadingModelBase" )
		.property_readonly( "TypeName", &ShadingModelBase::GetShadingModelTypeName );

	rttr::registration::class_< PhongMaterial >( "PhongMaterial" )
		.property( "Diffuse", &PhongMaterial::Diffuse ) BIND_AS_PTR
		.property( "Specular", &PhongMaterial::Specular ) BIND_AS_PTR
		.property( "Emissive", &PhongMaterial::Emissive ) BIND_AS_PTR
		.property( "Ambient", &PhongMaterial::Ambient ) BIND_AS_PTR
		.property( "Power", &PhongMaterial::Power );

	rttr::registration::class_< ShadingModelData< PhongMaterial > >( "ShadingModelData< PhongMaterial >" )
		.property( "Data", &ShadingModelData< PhongMaterial >::Data ) BIND_AS_PTR;
}


// Note: There should be at least one function (or constructor) in this file. Otherwise compiler
//doesn't want to compile RTTR type registrations.

ShadingModelBase::ShadingModelBase()
{}

// ================================ //
//
std::string			ShadingModelBase::GetShadingModelTypeName ()
{
	return GetShadingModelType().get_name();
}

