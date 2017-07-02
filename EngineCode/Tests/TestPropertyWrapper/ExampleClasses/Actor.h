#pragma once

#include "EngineEditor/PropertyWrapperRTTR/Build/IncludeRTTR.h"
#include "swCommonLib/Common/EngineObject.h"

#include <DirectXMath.h>
#include <string>
#include <vector>


class Actor;
class DerivedActor;
class ActorController;


// ================================ //
//
struct Asset
{


};

// ================================ //
//
struct MeshSegment
{
	float					FloatProp;
	double					DoubleProp;
	uint8					Uint8Prop;
};

// ================================ //
//
struct ActorInfo
{
	Actor*					ControlledActor;
	int8					Int8Prop;
	int16					Int16Prop;
	int32					Int32Prop;
};

// ================================ //
//
class Actor : public EngineObject
{
	RTTR_ENABLE( EngineObject )
public:

	DirectX::XMFLOAT3		Position;

	uint16					Uint16Prop;
	uint32					Uint32Prop;

		
protected:
public:
	explicit		Actor		() = default;
	~Actor	() = default;

};

// ================================ //
//
class DerivedActor : public Actor
{
	RTTR_ENABLE( Actor )
public:

	std::string					Text;
	std::vector< MeshSegment >	Assets;

public:


};

// ================================ //
//
class ControllerBase : public EngineObject
{
	RTTR_ENABLE( EngineObject )
public:

	virtual ~ControllerBase() {}
};

// ================================ //
//
class ActorController : public ControllerBase
{
	RTTR_ENABLE( ControllerBase )
public:

	ActorInfo			Info;

public:

};


// ================================ //
//
class MultiController : public ControllerBase
{
	RTTR_ENABLE( ControllerBase )
public:

	std::vector< ControllerBase* >		Controllers;

public:

};

