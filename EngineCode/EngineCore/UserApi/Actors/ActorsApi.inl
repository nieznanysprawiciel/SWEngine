#pragma once

#include "ActorsApi.h"
#include "EngineCore/Actors/ActorFactory.h"

namespace Api
{


	/**@brief Rejestruje klasê o odanym typie.*/
	template< typename Type >
	inline ActorType ActorsApi::RegisterClass( const std::string& name, CreateActorFunction function )
	{
		auto& actorsFactory = Context->actorsManager->GetActorFactory();
		return actorsFactory.RegisterClass< Type >( name, function );
	}

}	// Api

