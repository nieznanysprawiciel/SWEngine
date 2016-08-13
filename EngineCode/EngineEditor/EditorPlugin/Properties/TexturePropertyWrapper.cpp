#include "TexturePropertyWrapper.h"

#include "GraphicAPI/MeshResources.h"


namespace EditorPlugin
{



TexturePropertyWrapper::TexturePropertyWrapper( rttr::property prop )
	:	ResourcePropertyWrapper( prop )
{}

/**@brief */
void		TexturePropertyWrapper::ResetActor( System::IntPtr parentObjectPtr )
{
	auto prop = RTTRPropertyRapist::MakeProperty( m_metaProperty );

	auto value = prop.get_value( *static_cast< EngineObject* >( parentObjectPtr.ToPointer() ) );
	auto object = value.get_value< EngineObject* >();

	auto descriptor = object->GetType().get_property( "Descriptor" );
	auto descPtr = descriptor.get_value( *static_cast< EngineObject* >( object ) );
	auto texInfo = descPtr.get_value< TextureInfo* >();

	for each( auto property in Properties )
		property->ResetActor( System::IntPtr( (void*)texInfo ) );
}

/**@brief */
void		TexturePropertyWrapper::BuildHierarchy( rttr::type classType )
{
	classType = classType.get_raw_type();
	auto descriptor = classType.get_property( "Descriptor" );
	auto properties = descriptor.get_type().get_properties();

	for( auto& prop : properties )
	{
		Properties->Add( BuildProperty( prop ) );
	}
}

}	// EditorPlugin
