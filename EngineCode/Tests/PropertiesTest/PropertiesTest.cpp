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

	char end;
	std::cout << "Press any key to end.";
	std::cin >> end;

	return 0;
}

