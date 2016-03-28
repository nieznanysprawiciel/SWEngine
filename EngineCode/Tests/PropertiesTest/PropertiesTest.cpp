// PropertiesTest.cpp : Defines the entry point for the console application.
//

#include "DerivedClass.h"

#include <iostream>
#include <memory>


#define SEPARATOR std::cout << std::endl;



int main()
{
	BaseClass		base;
	DerivedClass	derived;

	// O ja ciê nie wierzê, ¿e tak siê da.
	//DerivedClass( derived ).GetTypedProperty< IEnableProperty >( "PhysicalProperty" );
	//BaseClass().Property().Property();

	Property< float > typedPropertyX = base.GetTypedProperty< float >( "PositionX" );
	Property< float > typedPropertyY = base.GetTypedProperty< float >( "PositionY" );
	Property< float > typedPropertyZ = base.GetTypedProperty< float >( "PositionZ" );

	Property< bool > typedPropertySeialize = base.GetTypedProperty< bool >( "SerializeObject" );
	Property< int > typedPropertyNum = base.GetTypedProperty< int >( "NumObjects" );

	std::cout << "PositionX: " << typedPropertyX << std::endl;
	std::cout << "PositionY: " << typedPropertyY << std::endl;
	std::cout << "PositionZ: " << typedPropertyZ << std::endl;
	
	std::cout << "SerializeObject: " << typedPropertySeialize() << std::endl;
	std::cout << "NumObjects: " << typedPropertyNum() << std::endl;

	SEPARATOR

	// Nested properties and property pointer
	Property< IEnableProperty > PhysicalProperty = derived.GetTypedProperty< IEnableProperty >( "PhysicalProperty" );
	Property< IEnableProperty* > PhysicalPropertyPrev = derived.GetTypedProperty< IEnableProperty* >( "PhysicalPropertyPrev" );


	std::cout << "Nested property type id: " << PhysicalProperty.GetPropertyType().getId() << std::endl;
	std::cout << "Nested property type raw id: " << PhysicalProperty.GetPropertyType().getRawType().getId() << std::endl;

	std::cout << "Property ptr type id: " << PhysicalPropertyPrev.GetPropertyType().getId() << std::endl;
	std::cout << "Property ptr type raw id: " << PhysicalPropertyPrev.GetPropertyType().getRawType().getId() << std::endl;

	SEPARATOR

	Property< float > MassProperty = PhysicalProperty().GetTypedProperty< float >( "Mass" );
	Property< float > MomentProperty = PhysicalProperty().GetTypedProperty< float >( "Moment" );
	Property< float > RadiusProperty = PhysicalProperty().GetTypedProperty< float >( "Radius" );

	Property< float > MassPropertyPrev = PhysicalPropertyPrev()->GetTypedProperty< float >( "Mass" );
	Property< float > MomentPropertyPrev = PhysicalPropertyPrev()->GetTypedProperty< float >( "Moment" );
	Property< float > RadiusPropertyPrev = PhysicalPropertyPrev()->GetTypedProperty< float >( "Radius" );

	std::cout << "Mass: " << MassProperty << std::endl;
	std::cout << "Moment: " << MomentProperty << std::endl;
	std::cout << "Radius: " << RadiusProperty << std::endl;

	SEPARATOR

	std::cout << "MassPrev: " << MassPropertyPrev << std::endl;
	std::cout << "MomentPrev: " << MomentPropertyPrev << std::endl;
	std::cout << "RadiusPrev: " << RadiusPropertyPrev << std::endl;

	SEPARATOR

	Property< bool > MassPropertyWrongType = PhysicalProperty().GetTypedProperty< bool >( "Mass" );
	Property< int > MomentPropertyWrongType = PhysicalProperty().GetTypedProperty< int >( "Moment" );
	Property< double > RadiusPropertyWrongType = PhysicalProperty().GetTypedProperty< double >( "Radius" );

	if( MassPropertyWrongType.IsValid() )
		std::cout << "Mass casted to bool shouldn't be valid" << std::endl;

	if( MomentPropertyWrongType.IsValid() )
		std::cout << "Mass casted to int shouldn't be valid" << std::endl;

	if( MomentPropertyWrongType.IsValid() )
		std::cout << "Mass casted to double shouldn't be valid" << std::endl;

	SEPARATOR

	//std::cout << "Sizeof( vector< int > ): " << sizeof( std::vector< int > ) << std::endl;
	//std::cout << "Sizeof( vector< int* > ): " << sizeof( std::vector< int* > ) << std::endl;
	//std::cout << "Sizeof( std::unique_ptr< int > ): " << sizeof( std::unique_ptr< int > ) << std::endl;
	//std::cout << "Sizeof( std::unique_ptr< IEnableProperty > ): " << sizeof( std::unique_ptr< IEnableProperty > ) << std::endl;
	//std::cout << "Sizeof( IEnableProperty* ): " << sizeof( IEnableProperty* ) << std::endl;

	SEPARATOR

	char end;
	std::cout << "Press any key to end...";
	std::cin >> end;

	return 0;
}

