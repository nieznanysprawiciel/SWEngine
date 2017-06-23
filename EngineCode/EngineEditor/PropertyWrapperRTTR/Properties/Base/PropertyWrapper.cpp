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

/**@brief Buduje hierarchi� dla typ�w DirectX::XMFLOAT...

Funkcja nie grupuje Property w kategorie w przeciwie�stwie do
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

/**@brief Zbuduj hierarchi� metadanych z podanego obiektu.*/
void CategoryLessPropertyWrapper::BuildHierarchy()
{
	auto property = RTTRPropertyRapist::MakeProperty( m_metaProperty );

	rttr::variant declaringObject( VoidMove( m_actorPtr ) );
	bool success = declaringObject.unsafe_convert_void( property.get_declaring_type_ptr() );

	assert( success );

	// Trzeba pobra� realny type w�a�ciwo�ci. Mo�e by� tak, �e w�a�ciwo�� jest klas� bazow�,
	// a tak my chcemy zbudowa� hierarchi� dla klasy pochodnej.
	auto realContent = property.get_value( declaringObject );

	void* realPtr = realContent.get_value< void* >();
	rttr::type realType = realContent.get_type();

	// Obs�ugujemy type owrappowane.
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

