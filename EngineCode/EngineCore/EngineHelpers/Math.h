#pragma once
/**
@file Math.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include <DirectXMath.h>


#define FASTCALL __vectorcall


/**@brief */
class Math
{
private:
protected:
public:

	/**@brief Computes oriented (right handed) angle beteween vectors. Parameters are normalized in function.
	@return Angle in radians.*/
	static float FASTCALL			OrientedAngle		( DirectX::XMVECTOR first, DirectX::XMVECTOR second, DirectX::XMVECTOR planeNormal );



	static float					ToDegrees			( float value );
	static float					ToRadians			( float value );
};
