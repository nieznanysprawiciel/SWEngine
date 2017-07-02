#include "Actor.h"




RTTR_REGISTRATION
{
	rttr::registration::class_< MeshSegment >( "MeshSegment" )
		.property( "Int8Prop", &MeshSegment::FloatProp )
		.property( "Int16Prop", &MeshSegment::DoubleProp )
		.property( "Text", &MeshSegment::Uint8Prop );


	rttr::registration::class_< ActorInfo >( "ActorInfo" )
		.property( "ControlledActor", &ActorInfo::ControlledActor )
		.property( "Int8Prop", &ActorInfo::Int8Prop )
		.property( "Int16Prop", &ActorInfo::Int16Prop )
		.property( "Text", &ActorInfo::Int32Prop );
	

	rttr::registration::class_< Asset >( "Asset" );


	rttr::registration::class_< Actor >( "Actor" )
		.property( "Position", &Actor::Position )
		(
			rttr::metadata( MetaDataType::Category, "Transformation" ),
			rttr::policy::prop::bind_as_ptr
		)
		.property( "Uint16Prop", &Actor::Uint16Prop )
		(
			rttr::metadata( MetaDataType::Category, "Movement" )
		)
		.property( "Uint32Prop", &Actor::Uint32Prop )
		(
			rttr::metadata( MetaDataType::Category, "Basic" )
		);


	rttr::registration::class_< std::vector< MeshSegment > >( "Vector[MeshSegment]" );

	rttr::registration::class_< DerivedActor >( "DerivedActor" )
		.property( "Text", &DerivedActor::Text )
		(
			rttr::metadata( MetaDataType::Category, "Transformation" )
		)
		.property( "Assets", &DerivedActor::Assets )
		(
			rttr::metadata( MetaDataType::Category, "Basic" ),
			rttr::policy::prop::bind_as_ptr
		);

	rttr::registration::class_< ControllerBase >( "ControllerBase" );
	rttr::registration::class_< std::vector< ControllerBase* > >( "Vector[ControllerBase*]" );


	rttr::registration::class_< ActorController >( "ActorController" )
		.property( "Info", &ActorController::Info );

	rttr::registration::class_< MultiController >( "MultiController" )
		.property( "Controllers", &MultiController::Controllers );
}
