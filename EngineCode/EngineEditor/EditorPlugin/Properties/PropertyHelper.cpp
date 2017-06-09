/**
@file PropertyHelper.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "PropertyHelper.h"


#include "AccessPrivateIdiom.h"


/**@brief Access private property_wrapper_base* from rttr.
CLR wrappers can use only basic types and pointers as class members. Thats why we need to extract 
property_wrapper_base* from rttr::property class. Remember to never use property_wrapper_base directly.
We must always recreate rttr::property to make it work.*/
struct AccessPrivate
{
	typedef const rttr::detail::property_wrapper_base*	rttr::property::*type;
	friend type		Get( AccessPrivate );
};

template struct AccessPrivateIdiom< AccessPrivate, &rttr::property::m_wrapper >;



namespace rttr {
namespace detail {
namespace
{

//// ================================ //
////
//template<>
//rttr::property		rttr::detail::create_item					( const rttr::detail::property_wrapper_base* wrapper )
//{
//	return property( wrapper );
//}

//// ================================ //
////
//template<>
//rttr::property		rttr::detail::create_invalid_item			()
//{
//	static const detail::property_wrapper_base invalid_wrapper( rttr::string_view(), rttr::detail::get_invalid_type() );
//	return property( &invalid_wrapper );
//}

}	// anonymous
}	// detail
}	// rttr


namespace sw {
namespace EditorPlugin
{




//// ================================ //
////
//rttr::property											RTTRPropertyRapist::MakeProperty		( const rttr::detail::property_wrapper_base * wrapper )
//{
//	return rttr::detail::create_item( wrapper );
//}
// ================================ //
//
const rttr::detail::property_wrapper_base*				RTTRPropertyRapist::GetWrapperBase		( rttr::property prop )
{
	return prop.*Get( AccessPrivate() );
}

}	// EditorPlugin
}	// sw
