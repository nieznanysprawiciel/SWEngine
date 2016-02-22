#pragma once


/**@brief Przechowuje informacje w jakich modu�ach silnika znajduje si�
aktor.*/
struct ActorInfo
{
	int			enableDisplay		: 1;	///< Obiekt znajduje si� w module DisplayEngine.
	int			enableMovement		: 1;	///< Obiekt znajduje si� w module MovementEngine.
	int			enablePhysic		: 1;	///< Obiekt znajduje si� w module PhysicEngine.
	int			enableCollisions	: 1;	///< Obiekt znajduje si� w module CollisionsEngine.
	int			enableShadow		: 1;	///< Obiekt rzuca cie�.
	int			enablePreController	: 1;	///< Obiekt posiada controller typu pre. @ref PrePostControllers
	int			enablePostController: 1;	///< Obiekt posiada controller typu post. @ref PrePostControllers
	int			isLight				: 1;	///< Obiekt jest �wiat�em i zosta� dodany do modu�u obs�uguj�cego o�wietlenie.

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