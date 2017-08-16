#include "CreateTestActor.h"




// ================================ //
//
ControllerBase*		CreateControllerBase()
{
	return new ControllerBase;
}

// ================================ //
//
Actor*				CreateSimpleNullActor			()
{
	Actor* actor = new Actor();

	actor->Uint16Prop = 0;
	actor->Uint32Prop = 0;
	actor->Position = DirectX::XMFLOAT3( 0.0, 0.0, 0.0 );

	return actor;
}

// ================================ //
//
DerivedActor*		CreateDerivedNullActor			()
{
	DerivedActor* actor = new DerivedActor();

	actor->Uint16Prop = 0;
	actor->Uint32Prop = 0;
	actor->Position = DirectX::XMFLOAT3( 0.0, 0.0, 0.0 );
	actor->Text = "TestText";

	return actor;
}


#pragma unmanaged



RTTR_REGISTRATION
{
	rttr::registration::class_< MeshSegment >( "MeshSegment" )
		.property( "FloatProp", &MeshSegment::FloatProp )
		.property( "DoubleProp", &MeshSegment::DoubleProp )
		.property( "Uint8Prop", &MeshSegment::Uint8Prop );


	rttr::registration::class_< ActorInfo >( "ActorInfo" )
		.property( "ControlledActor", &ActorInfo::ControlledActor )
		.property( "Int8Prop", &ActorInfo::Int8Prop )
		.property( "Int16Prop", &ActorInfo::Int16Prop )
		.property( "Int32Prop", &ActorInfo::Int32Prop );
	

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


	//rttr::registration::class_< ActorController >( "ActorController" )
	//	.property( "Info", &ActorController::Info );

	//rttr::registration::class_< MultiController >( "MultiController" )
	//	.property( "Controllers", &MultiController::Controllers );
}
