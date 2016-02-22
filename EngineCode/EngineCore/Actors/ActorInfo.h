#pragma once


/**@brief Przechowuje informacje w jakich modu³ach silnika znajduje siê
aktor.*/
struct ActorInfo
{
	int			enableDisplay		: 1;	///< Obiekt znajduje siê w module DisplayEngine.
	int			enableMovement		: 1;	///< Obiekt znajduje siê w module MovementEngine.
	int			enablePhysic		: 1;	///< Obiekt znajduje siê w module PhysicEngine.
	int			enableCollisions	: 1;	///< Obiekt znajduje siê w module CollisionsEngine.
	int			enableShadow		: 1;	///< Obiekt rzuca cieñ.
	int			enablePreController	: 1;	///< Obiekt posiada controller typu pre. @ref PrePostControllers
	int			enablePostController: 1;	///< Obiekt posiada controller typu post. @ref PrePostControllers
	int			isLight				: 1;	///< Obiekt jest œwiat³em i zosta³ dodany do modu³u obs³uguj¹cego oœwietlenie.

	inline void		InitZero();
};

inline void ActorInfo::InitZero()
{
	enableDisplay			= false;
	enableMovement			= false;
	enableCollisions		= false;
	enablePhysic			= false;
	enableShadow			= false;
	enablePreController		= false;
	enablePostController	= false;
	isLight					= false;
}