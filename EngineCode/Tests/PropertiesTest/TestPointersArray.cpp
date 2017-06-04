#include "swCommonLib/External/Catch/catch.hpp"
#include "swCommonLib/Common/RTTR.h"

#include <vector>


class Object
{
	RTTR_ENABLE();
	RTTR_REGISTRATION_FRIEND;
private:
	int			Value;
};


class ContainerObject
{
	RTTR_ENABLE();
public:

	std::vector< Object* >		m_objects;

public:

	ContainerObject()
	{
		m_objects.push_back( new Object );
	}
};


RTTR_REGISTRATION
{


	rttr::registration::class_< Object >( "Object" )
		.property( "Value", &Object::Value );


	rttr::registration::class_< std::vector< Object* > >( "std::vector< Object* >" );
	

	rttr::registration::class_< ContainerObject >( "ContainerObject" )
		.property( "TestValue", &ContainerObject::m_objects ) BIND_AS_REF;

}



TEST_CASE( "Vector of pointers test" )
{

}
