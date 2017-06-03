/**
@file ComplexActor.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "EngineCore/stdafx.h"
#include "ComplexActor.h"

#include "swCommonLib/Common/MemoryLeaks.h"


RTTR_REGISTRATION
{
	rttr::registration::class_< sw::ComplexActor >( "ComplexActor" );
}

using namespace DirectX;



namespace sw
{

// ================================ //
//
void			ComplexActor::MoveComplex( float time_interval, const XMFLOAT3& parent_speed, const XMFLOAT4& parent_rotation )
{

}

}	// sw

