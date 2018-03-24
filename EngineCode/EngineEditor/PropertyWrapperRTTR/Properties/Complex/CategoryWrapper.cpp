/**
@file CategoryWrapper.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/
#include "EngineEditor/PropertyWrapperRTTR/stdafx.h"
#include "CategoryWrapper.h"

#include "EngineEditor/PropertyWrapperRTTR/Properties/Basic/BasicTypesWrappers.h"
#include "EngineEditor/PropertyWrapperRTTR/Properties/Basic/EnumPropertyWrapper.h"
#include "EngineEditor/PropertyWrapperRTTR/Properties/Basic/StringPropertyWrapper.h"

#include "EngineEditor/PropertyWrapperRTTR/Properties/Array/ArrayPropertyWrapper.h"
#include "EngineEditor/PropertyWrapperRTTR/Properties/Array/ArrayElementPropertyWrapper.h"

// Remove in future and create proper mechanism
#include "EngineEditor/PropertyWrapperRTTR/Properties/ResourcePropertyWrapper.h"


namespace sw {
namespace EditorPlugin
{


// ================================ //
//
void				CategoryWrapper::BuildHierarchy			( rttr::type classType, BuildContext& context )
{
	//// Note: We don't wrapp any real object here so we can set WrappedObject to parent pointer.
	//SetWrappedObject( parentPtr );

	//classType = classType.get_raw_type();
	//auto properties = classType.get_properties();
	//Dictionary< System::String^, CategoryWrapper^ >  categories;

	//for( auto& prop : properties )
	//{
	//	auto categoryNameMeta = prop.get_metadata( MetaDataType::Category );
	//	if( categoryNameMeta.is_valid() && categoryNameMeta.is_type< std::string >() )
	//	{
	//		const std::string& categoryNameStdStr = categoryNameMeta.get_value< std::string >();
	//		System::String^ categoryNameStr = gcnew System::String( categoryNameStdStr.c_str() );
	//		if( !categories.ContainsKey( categoryNameStr ) )
	//			categories[ categoryNameStr ] = gcnew CategoryWrapper( parentPtr, categoryNameStdStr.c_str() );

	//		categories[ categoryNameStr ]->Properties->Add( BuildProperty( parentPtr, prop, context ) );
	//	}
	//	else
	//	{
	//		if( !categories.ContainsKey( "Other" ) )
	//			categories[ "Other" ] = gcnew CategoryWrapper( parentPtr, "Other" );

	//		categories[ "Other" ]->Properties->Add( BuildProperty( parentPtr, prop, context ) );
	//	}
	//}

	//for each ( auto var in categories )
	//	AddPropertyChild( var.Value );
}




}
}
