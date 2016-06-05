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
	actorFlags = actorInfoFlag;
}

/**@brief Ustawia wszystkie wartoœci na false.*/
inline void ActorInfo::InitZero()
{
	actorFlags = 0;
}

/**@brief Wylicza sumê dla ka¿dej pary flag.*/
inline void ActorInfo::operator|=( ActorInfo second )
{
	actorFlags = actorFlags | second.actorFlags;
}
