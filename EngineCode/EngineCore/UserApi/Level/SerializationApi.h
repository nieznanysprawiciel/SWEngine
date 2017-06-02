#pragma once
/**@file SerializationApi.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Deklaracja klasy SerializationApi.*/

#include "EngineCore/Actors/ActorInfo.h"
#include "EngineCore/UserApi/ApiGroup.h"
#include "swCommonLib/Common/EngineObject.h"

class IDeserializer;



namespace Api { namespace Level
{

/**@defgroup SerializationApi Serializacja levelu
@ingroup UserAPI
@brief Funkcje pomocnicze do serializacji.*/

	/**@brief Zawiera funkcje pomocnicze do serializacji.
	U¿ytkownik raczej nie powinien korzystaæ z tych funkcji, poniewa¿ zapisywanie i wczytywanie
	dzieje siê automatycznie.

	@ingroup UserAPI
	@ingroup SerializationApi*/
	class SerializationApi : public ApiGroup
	{
	public:
		static const char*	ACTOR_INFO_STRING;

		static const char*	ACTOR_INFO_ENABLE_DISPLAY;
		static const char*	ACTOR_INFO_ENABLE_MOVEMENT;
		static const char*	ACTOR_INFO_ENABLE_PRE_CONTROLLERS;
		static const char*	ACTOR_INFO_ENABLE_POST_CONTROLLERS;
		static const char*	ACTOR_INFO_ENABLE_PHYSIC;
		static const char*	ACTOR_INFO_ENABLE_SHADOW;
		static const char*	ACTOR_INFO_ENABLE_COLLISION;
		static const char*	ACTOR_INFO_IS_LIGHT;
		static const char*	ACTOR_INFO_IS_CAMERA;
		static const char*	ACTOR_INFO_ENABLE_SAVING_TO_FILE;

	private:
	protected:
	public:
		SerializationApi() = default;
		~SerializationApi() = default;
	public:
	
		void		DefaultDeserialize			( IDeserializer* deser, EngineObject* object );
		bool		DeserializeSingleGeneric	( IDeserializer* deser, rttr::property prop, const EngineObject* object );
		bool		DeserializeResource			( IDeserializer* deser, rttr::property prop, const EngineObject* object );
		

		void		SerializeActorInfo			( ISerializer* ser, ActorInfo info );
		ActorInfo	DeserializeActorInfo		( IDeserializer* deser );
	};



}	// Level
}	// Api
