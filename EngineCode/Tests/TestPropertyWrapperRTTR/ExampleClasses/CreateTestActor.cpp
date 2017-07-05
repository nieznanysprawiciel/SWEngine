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
