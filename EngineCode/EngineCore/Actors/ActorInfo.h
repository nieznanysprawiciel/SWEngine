#pragma once

#include <utility>

class EngineObject;



/**@brief Flagi do ustawiania warto�ci struktury ActorInfo.*/
enum ActorInfoFlag	: unsigned short
{
	DisableAll				= 0,			///< ��czenie z innymi flagami powoduje, �e DisableAll zostanie zignorowane.
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

/**@brief Przechowuje informacje w jakich modu�ach silnika znajduje si�
aktor.*/
struct ActorInfo
{
	bool			enableDisplay		: 1;	///< Obiekt znajduje si� w module DisplayEngine.
	bool			enableMovement		: 1;	///< Obiekt znajduje si� w module MovementEngine.
	bool			enablePhysic		: 1;	///< Obiekt znajduje si� w module PhysicEngine.
	bool			enableCollisions	: 1;	///< Obiekt znajduje si� w module CollisionsEngine.
	bool			enableShadow		: 1;	///< Obiekt rzuca cie�.
	bool			enablePreController	: 1;	///< Obiekt posiada controller typu pre. @ref PrePostControllers
	bool			enablePostController: 1;	///< Obiekt posiada controller typu post. @ref PrePostControllers
	bool			isLight				: 1;	///< Obiekt jest �wiat�em i zosta� dodany do modu�u obs�uguj�cego o�wietlenie.
	bool			isCamera			: 1;	///< Obiekt jest kamer� i zostanie dodany do modu�u DisplayEngine.

					ActorInfo	();
					ActorInfo	( unsigned short actorInfoFlag );
	inline void		InitZero	();
	void			operator|=	( ActorInfo second );
};

typedef std::pair< EngineObject*, ActorInfo > ActorData;

/**@brief Ustawia zerowy stan struktury ActorInfo.*/
inline ActorInfo::ActorInfo	()
{
	InitZero();
}

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

/**@brief Ustawia wszystkie warto�ci na false.*/
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

/**@brief Wylicza sum� dla ka�dej pary flag.*/
inline void ActorInfo::operator|=( ActorInfo second )
{
	enableDisplay			= enableDisplay | second.enableDisplay;
	enableMovement			= enableMovement | second.enableMovement;
	enableCollisions		= enableCollisions | second.enableCollisions;
	enablePhysic			= enablePhysic | second.enablePhysic;
	enableShadow			= enableShadow | second.enableShadow;
	enablePreController		= enablePreController | second.enablePreController;
	enablePostController	= enablePostController | second.enablePostController;
	isLight					= isLight | second.isLight;
	isCamera				= isCamera | second.isCamera;
}
