// PropertiesTest.cpp : Defines the entry point for the console application.
//

#include "DerivedClass.h"

#include <iostream>

int main()
{
	BaseClass		base;
	DerivedClass	derived;


	Property< float > typedPropertyX = base.GetTypedProperty< float >( "PositionX" );
	Property< float > typedPropertyY = base.GetTypedProperty< float >( "PositionY" );
	Property< float > typedPropertyZ = base.GetTypedProperty< float >( "PositionZ" );

	Property< bool > typedPropertySeialize = base.GetTypedProperty< bool >( "SerializeObject" );
	Property< int > typedPropertyNum = base.GetTypedProperty< int >( "NumObjects" );

	std::cout << "PositionX: " << typedPropertyX() << std::endl;
	std::cout << "PositionY: " << typedPropertyY() << std::endl;
	std::cout << "PositionZ: " << typedPropertyZ() << std::endl;
	
	std::cout << "SerializeObject: " << typedPropertySeialize() << std::endl;
	std::cout << "NumObjects: " << typedPropertyNum() << std::endl;


	Property< IEnableProperty > PhysicalProperty = derived.GetTypedProperty< IEnableProperty >( "PhysicalProperty" );

	Property< float > MassProperty = PhysicalProperty().GetTypedProperty< float >( "Mass" );
	Property< float > MomentProperty = PhysicalProperty().GetTypedProperty< float >( "Moment" );
	Property< float > RadiusProperty = PhysicalProperty().GetTypedProperty< float >( "Radius" );

	std::cout << "Mass: " << MassProperty() << std::endl;
	std::cout << "Moment: " << MomentProperty() << std::endl;
	std::cout << "Radius: " << RadiusProperty() << std::endl;

	Property< bool > MassPropertyWrongType = PhysicalProperty().GetTypedProperty< bool >( "Mass" );
	Property< int > MomentPropertyWrongType = PhysicalProperty().GetTypedProperty< int >( "Moment" );
	Property< double > RadiusPropertyWrongType = PhysicalProperty().GetTypedProperty< double >( "Radius" );

	if( MassPropertyWrongType.IsValid() )
		std::cout << "Mass casted to bool shouldn't be valid" << std::endl;

	if( MomentPropertyWrongType.IsValid() )
		std::cout << "Mass casted to int shouldn't be valid" << std::endl;

	if( MomentPropertyWrongType.IsValid() )
		std::cout << "Mass casted to double shouldn't be valid" << std::endl;

	char end;
	std::cout << "Press any key to end...";
	std::cin >> end;

	return 0;
}

