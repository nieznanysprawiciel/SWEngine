#include "CreateTestActor.h"




// ================================ //
//
Actor*		CreateSimpleNullActor			()
{
	Actor* actor = new Actor();

	actor->Uint16Prop = 0;
	actor->Uint32Prop = 0;
	actor->Position = DirectX::XMFLOAT3( 0.0, 0.0, 0.0 );

	return actor;
}
