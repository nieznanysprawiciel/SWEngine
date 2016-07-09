#pragma once



class ActorsCommonFunctions
{
private:
public:

	template< typename ActorType >
	static bool			RemoveActor		( std::vector< ActorType* >& actorsVec, ActorType* actor );
};


/**@brief Usuwa aktora z wektora.

@return Zwraca true, je�eli aktor istnia� w wektorze i zosta� usuni�ty.*/
template<typename ActorType>
static inline bool			ActorsCommonFunctions::RemoveActor( std::vector< ActorType* >& actorsVec, ActorType* actor )
{
	for( int i = (int)actorsVec.size() - 1; i > 0 ; --i )
	{
		if( actorsVec[ i ] == actor )
		{
			actorsVec.erase( actorsVec.begin() + i );
			return true;
		}
	}
	return false;
}
