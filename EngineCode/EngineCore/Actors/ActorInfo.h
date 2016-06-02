#pragma once

#include <utility>

class ActorBase;



/**@brief Flagi do ustawiania wartoœci struktury ActorInfo.*/
enum ActorInfoFlag	: unsigned short
{
	DisableAll				= 0,			///< £¹czenie z innymi flagami powoduje, ¿e DisableAll zostanie zignorowane.
	EnableDisplay			= 1,			///< Obiekt znajduje siê w module DisplayEngine.
	EnableMovement			= 1 << 1,		///< Obiekt znajduje siê w module MovementEngine.
	EnablePhysic			= 1 << 2,		///< Obiekt znajduje siê w module PhysicEngine.
	EnableCollisions		= 1 << 3,		///< Obiekt znajduje siê w module CollisionsEngine.
	EnableShadow			= 1 << 4,		///< Obiekt rzuca cieñ.
	EnablePreController		= 1 << 5,		///< Obiekt posiada controller typu pre. @ref PrePostControllers
	EnablePostController	= 1 << 6,		///< Obiekt posiada controller typu post. @ref PrePostControllers
	AsLight					= 1 << 7,		///< Obiekt jest œwiat³em i zosta³ dodany do modu³u obs³uguj¹cego oœwietlenie.
	AsCamera				= 1 << 8		///< Obiekt jest kamer¹ i zostanie dodany do modu³u DisplayEngine.
};

/**@brief Przechowuje informacje w jakich modu³ach silnika znajduje siê
aktor.*/
struct ActorInfo
{
	//bool			enableDisplay		: 1;	///< Obiekt znajduje siê w module DisplayEngine.
	//bool			enableMovement		: 1;	///< Obiekt znajduje siê w module MovementEngine.
	//bool			enablePhysic		: 1;	///< Obiekt znajduje siê w module PhysicEngine.
	//bool			enableCollisions	: 1;	///< Obiekt znajduje siê w module CollisionsEngine.
	//bool			enableShadow		: 1;	///< Obiekt rzuca cieñ.
	//bool			enablePreController	: 1;	///< Obiekt posiada controller typu pre. @ref PrePostControllers
	//bool			enablePostController: 1;	///< Obiekt posiada controller typu post. @ref PrePostControllers
	//bool			isLight				: 1;	///< Obiekt jest œwiat³em i zosta³ dodany do modu³u obs³uguj¹cego oœwietlenie.
	//bool			isCamera			: 1;	///< Obiekt jest kamer¹ i zostanie dodany do modu³u DisplayEngine.

	unsigned short	actorFlags;

					ActorInfo	();
					ActorInfo	( unsigned short actorInfoFlag );
	inline void		InitZero	();
	void			operator|=	( ActorInfo second );

	bool EnableDisplay			() const		{ return ( actorFlags & ActorInfoFlag::EnableDisplay ) != 0; }
	bool EnableMovement			() const		{ return ( actorFlags & ActorInfoFlag::EnableMovement ) != 0; }
	bool EnablePhysic			() const		{ return ( actorFlags & ActorInfoFlag::EnablePhysic ) != 0; }
	bool EnableCollisions		() const		{ return ( actorFlags & ActorInfoFlag::EnableCollisions ) != 0; }
	bool EnableShadow			() const		{ return ( actorFlags & ActorInfoFlag::EnableShadow ) != 0; }
	bool EnablePreController	() const		{ return ( actorFlags & ActorInfoFlag::EnablePreController ) != 0; }
	bool EnablePostController	() const		{ return ( actorFlags & ActorInfoFlag::EnablePostController ) != 0; }
	bool IsLight				() const		{ return ( actorFlags & ActorInfoFlag::AsLight ) != 0; }
	bool IsCamera				() const		{ return ( actorFlags & ActorInfoFlag::AsCamera ) != 0; }
};

typedef std::pair< ActorBase*, ActorInfo > ActorData;

/**@brief Ustawia zerowy stan struktury ActorInfo.*/
inline ActorInfo::ActorInfo	()
{
	InitZero();
}

/**@brief Ustawia stan struktury ActorInfo na podstawie sumy bitowej
flag ActorInfoFlag.*/
inline ActorInfo::ActorInfo( unsigned short actorInfoFlag )
{
	//enableDisplay = actorInfoFlag & ActorInfoFlag::EnableDisplay ? true : false;
	//enableMovement = actorInfoFlag & ActorInfoFlag::EnableMovement ? true : false;
	//enablePhysic = actorInfoFlag & ActorInfoFlag::EnablePhysic ? true : false;
	//enableCollisions = actorInfoFlag & ActorInfoFlag::EnableCollisions ? true : false;
	//enableShadow = actorInfoFlag & ActorInfoFlag::EnableShadow ? true : false;
	//enablePreController = actorInfoFlag & ActorInfoFlag::EnablePreController ? true : false;
	//enablePostController = actorInfoFlag & ActorInfoFlag::EnablePostController ? true : false;
	//isLight = actorInfoFlag & ActorInfoFlag::AsLight ? true : false;
	//isCamera = actorInfoFlag & ActorInfoFlag::AsCamera ? true : false;

	actorFlags = actorInfoFlag;
}

/**@brief Ustawia wszystkie wartoœci na false.*/
inline void ActorInfo::InitZero()
{
	actorFlags = 0;

	//enableDisplay			= false;
	//enableMovement			= false;
	//enableCollisions		= false;
	//enablePhysic			= false;
	//enableShadow			= false;
	//enablePreController		= false;
	//enablePostController	= false;
	//isLight					= false;
	//isCamera				= false;
}

/**@brief Wylicza sumê dla ka¿dej pary flag.*/
inline void ActorInfo::operator|=( ActorInfo second )
{
	//enableDisplay			= enableDisplay | second.enableDisplay;
	//enableMovement			= enableMovement | second.enableMovement;
	//enableCollisions		= enableCollisions | second.enableCollisions;
	//enablePhysic			= enablePhysic | second.enablePhysic;
	//enableShadow			= enableShadow | second.enableShadow;
	//enablePreController		= enablePreController | second.enablePreController;
	//enablePostController	= enablePostController | second.enablePostController;
	//isLight					= isLight | second.isLight;
	//isCamera				= isCamera | second.isCamera;

	actorFlags = actorFlags | second.actorFlags;
}
