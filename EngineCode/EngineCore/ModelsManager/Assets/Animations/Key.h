#pragma once
/**
@file Key.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

namespace sw
{

/**@defgroup Animation Animation
@ingroup EngineCore*/


/**@brief Key parameters.

Consist of key parameter value for and other key parameters like interpolator.
@ingroup Animation*/
template< typename ValueType >
struct KeyParam
{
	ValueType		Value;
	//Interpolator
};

typedef float KeyTime;


/**@brief Animation key.
@ingroup Animation*/
template< typename ValueType >
struct Key
{
	KeyParam< ValueType >	Value;
	KeyTime					Time;
};

}	// sw

