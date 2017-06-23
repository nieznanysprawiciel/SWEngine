#pragma once
/**
@file AccessPrivateIdiom.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/



/**@brief This class allows to access private member field without modyfing class
declaration. Usufull when you want to rape someones else library...*/
template< typename Tag, typename Tag::type MemberPointer >
struct AccessPrivateIdiom
{
	friend typename Tag::type		Get( Tag )
	{		return MemberPointer;		}
};




