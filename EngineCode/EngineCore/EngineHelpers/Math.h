#pragma once
/**
@file Math.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include <DirectXMath.h>

/**@brief */
class Math
{
private:
protected:
public:

	/**@brief Computes oriented (right handed) angle beteween vectors. Parameters are normalized in function.
	@return Angle in radians.*/
	static float			OrientedAngle		( DirectX::XMVECTOR first, DirectX::XMVECTOR second );

};
