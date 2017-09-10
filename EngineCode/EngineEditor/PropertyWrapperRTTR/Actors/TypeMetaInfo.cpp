/**
@file TypeMetaInfo.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "EngineEditor/PropertyWrapperRTTR/stdafx.h"
#include "TypeMetaInfo.h"



namespace sw {
namespace EditorPlugin
{



// ================================ //
//
TypeMetaInfo::TypeMetaInfo( const TypeID& type )
{
	m_typeId = type.get_id();
	m_typeName = gcnew System::String( type.get_name().to_string().c_str() );
}


// ================================ //
//
uint32						TypeMetaInfo::TypeSize::get()
{
	return (uint32)GetType().get_sizeof();
}


// ================================ //
//
TypeID						TypeMetaInfo::GetType		()
{
	// It could be slow. Find another way to find type by id or to store TypeID in managed class.
	auto typesRange = TypeID::get_types();

	for( auto type : typesRange )
	{
		if( type.get_id() == m_typeId )
			return type;
	}

	return TypeID::get_by_name( "" );;
}


// ================================ //
//
List< System::String^ >^	TypeMetaInfo::BaseClasses::get()
{
	List< System::String^ >^ classesList = gcnew List< System::String^ >();

	auto baseClassesVec = GetType().get_base_classes();
	for each( auto baseClass in baseClassesVec )
	{
		System::String^ className = gcnew System::String( baseClass.get_name().to_string().c_str() );
		classesList->Add( className );
	}

	return classesList;
}


}	// EditorPlugin
}	// sw
