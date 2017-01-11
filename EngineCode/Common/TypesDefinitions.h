#pragma once

/**@file TypesDefinitions.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Plik zawiera definicje podstawowych typ�w zmiennych.

Zmienne s� maj� sta�y rozmiar, powinny by� mniej wi�cej niezale�ne od platformy bo
opieraj� si� na bibliotece standardowej. Mimo wszystko moze si� zdarzy�, �e na jakich�
platformach typy nie istniej� i wtedy jest problem.

Definicje s� stworzone w celu �atwiejszego u�ytku. Biblioteka standardowa definiuje np.
typ int8_t, kt�ry jest zamieniany na int8.
*/

/**@defgroup CommonFiles Common
@ingroup ModulesStructure
@brief Cz�sto u�ywane deklaracje i inne funkcjonalno�ci.*/

#include <cstdint>
#include <memory>



typedef int8_t int8;
typedef uint8_t uint8;
typedef int16_t int16;
typedef uint16_t uint16;
typedef int32_t int32;
typedef uint32_t uint32;
typedef int64_t int64;
typedef uint64_t uint64;

typedef size_t Size;

typedef ptrdiff_t PtrOffset;

typedef double TimeType;


namespace rttr
{
	class type;
}


typedef rttr::type ActorType;

/// @todo Use std::unique_ptr instead. Unique_ptr didn't want to compile in current visual compiler.
template< typename PtrType >
using UPtr = std::shared_ptr< PtrType >;

template< typename PtrType >
using Ptr = std::shared_ptr< PtrType >;


template< typename PtrType, typename... Args >
Ptr< PtrType >		MakePtr		( Args&&... args )
{
	return std::make_shared< PtrType >( args... );
}

template< typename PtrType, typename... Args >
UPtr< PtrType >		MakeUPtr		( Args... args )
{
	return std::make_unique< PtrType >( args... );
}


#define DEFINE_PTR_TYPE( type )			typedef Ptr< type > type ## Ptr;
#define DEFINE_UPTR_TYPE( type )		typedef UPtr< type > type ## UPtr;
#define DEFINE_WPTR_TYPE( type )		typedef std::weak_ptr< type > type ## WPtr;

