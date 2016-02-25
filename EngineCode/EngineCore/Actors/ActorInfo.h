#pragma once

/**@brief Flagi do ustawiania wartoœci struktury ActorInfo.*/
enum ActorInfoFlag	: unsigned short
{
	DisableAll				= 0,			///< £¹czenie z innymi flagami powoduje, ¿e DisableAll zostanie zignorowane.
	EnableDisplay			= 1,
	EnableMovement			= 1 << 1,
	EnablePhysic			= 1 << 2,
	EnableCollisions		= 1 << 3,
	EnableShadow			= 1 << 4,
	EnablePreController		= 1 << 5,
	EnablePostController	= 1 << 6,
	AsLight					= 1 << 7,
	AsCamera				= 1 << 8
};

/**@brief Przechowuje informacje w jakich modu³ach silnika znajduje siê
aktor.*/
struct ActorInfo
{
	unsigned short			enableDisplay		: 1;	///< Obiekt znajduje siê w module DisplayEngine.
	unsigned short			enableMovement		: 1;	///< Obiekt znajduje siê w module MovementEngine.
	unsigned short			enablePhysic		: 1;	///< Obiekt znajduje siê w module PhysicEngine.
	unsigned short			enableCollisions	: 1;	///< Obiekt znajduje siê w module CollisionsEngine.
	unsigned short			enableShadow		: 1;	///< Obiekt rzuca cieñ.
	unsigned short			enablePreController	: 1;	///< Obiekt posiada controller typu pre. @ref PrePostControllers
	unsigned short			enablePostController: 1;	///< Obiekt posiada controller typu post. @ref PrePostControllers
	unsigned short			isLight				: 1;	///< Obiekt jest œwiat³em i zosta³ dodany do modu³u obs³uguj¹cego oœwietlenie.
	unsigned short			isCamera			: 1;	///< Obiekt jest kamer¹ i zostanie dodany do modu³u DisplayEngine.

					ActorInfo	( unsigned short actorInfoFlag );
	inline void		InitZero	();
};

/**@brief Ustawia stan struktury ActorInfo na podstawie sumy bitowej
flag ActorInfoFlag.*/
inline ActorInfo::ActorInfo( unsigned short actorInfoFlag )
{
	enableDisplay = actorInfoFlag & ActorInfoFlag::EnableDisplay ? true : false;
	enableMovement = actorInfoFlag & ActorInfoFlag::EnableMovement ? true : false;
	enablePhysic = actorInfoFlag & ActorInfoFlag::EnablePhysic ? true : false;
	enableCollisions = actorInfoFlag & ActorInfoFlag::EnableCollisions ? true : false;
	enableShadow = actorInfoFlag & ActorInfoFlag::EnableShadow ? true : false;
	enablePreController = actorInfoFlag & ActorInfoFlag::EnablePreController ? true : false;
	enablePostController = actorInfoFlag & ActorInfoFlag::EnablePostController ? true : false;
	isLight = actorInfoFlag & ActorInfoFlag::AsLight ? true : false;
	isCamera = actorInfoFlag & ActorInfoFlag::AsCamera ? true : false;
}

/**@brief Ustawia wszystkie wartoœci na false.*/
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
	isCamera				= false;
}