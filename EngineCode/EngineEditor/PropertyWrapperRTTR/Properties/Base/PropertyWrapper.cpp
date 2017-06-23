#pragma warning(disable : 4561)
#include "EngineEditor/PropertyWrapperRTTR/stdafx.h"

#include "PropertyWrapper.h"
#include "swCommonLib/Common/RTTR.h"

#include <msclr/marshal_cppstd.h>




namespace sw {
namespace EditorPlugin
{


//====================================================================================//
//			EnumPropertyWrapper	
//====================================================================================//



//====================================================================================//
//			ObjectPropertyWrapper	CategoryPropertyWrapper
//====================================================================================//



//====================================================================================//
//				CategoryLessPropertyWrapper
//====================================================================================//

/**@brief Buduje hierarchiê dla typów DirectX::XMFLOAT...

Funkcja nie grupuje Property w kategorie w przeciwieñstwie do
@ref CategoryPropertyWrapper::BuildHierarchy.
*/
void CategoryLessPropertyWrapper::BuildHierarchy( void* parent, rttr::type classType )
{
	m_actorPtr = parent;

	classType = classType.get_raw_type();
	auto properties = classType.get_properties();

	for( auto& prop : properties )
	{
		Properties->Add( BuildProperty( parent, prop ) );
	}
}


void* VoidMove( void* obj ) { return obj; }

/**@brief Zbuduj hierarchiê metadanych z podanego obiektu.*/
void CategoryLessPropertyWrapper::BuildHierarchy()
{
	auto property = RTTRPropertyRapist::MakeProperty( m_metaProperty );

	rttr::variant declaringObject( VoidMove( m_actorPtr ) );
	bool success = declaringObject.unsafe_convert_void( property.get_declaring_type_ptr() );

	assert( success );

	// Trzeba pobraæ realny type w³aœciwoœci. Mo¿e byæ tak, ¿e w³aœciwoœæ jest klas¹ bazow¹,
	// a tak my chcemy zbudowaæ hierarchiê dla klasy pochodnej.
	auto realContent = property.get_value( declaringObject );

	void* realPtr = realContent.get_value< void* >();
	rttr::type realType = realContent.get_type();

	// Obs³ugujemy type owrappowane.
	if( realContent.get_type().is_wrapper() && realPtr != nullptr )
	{
		rttr::instance realInstance( realContent );
		realPtr = realInstance.get_wrapped_instance();
		realType = realContent.get_type().get_wrapped_type();
	}

	if( realPtr != nullptr )
		BuildHierarchy( realPtr, realType );
}



}
}	// sw

