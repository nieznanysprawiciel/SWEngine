#include <windows.h>
#include <fstream>

#include "Tools/SerializerJson/Serializer.h"


int main()
{
	ISerializer serializer;

	serializer.SetValue( "FreeValue", "value" );
	serializer.EnterObject( "FirstObject" );
		serializer.SetValue( "Map", "LightmapGen1" );
		serializer.SetValue( "Path", "/LightmapGen1.map" );
		serializer.SetValue( "Load", true );
		serializer.EnterObject( "Data" );
			serializer.SetValue( "NumberUnits", 1266643 );
			serializer.SetValue( "PositionOffset", 0.4124667623 );
			serializer.SetValue( "Visible", true );
			serializer.SetValue( "Key", 218588284382834538 );
		serializer.Exit();
		serializer.EnterArray( "Actors" );
			for( unsigned int i = 0; i < 12; ++i )
			{
				serializer.EnterObject( "" );
					serializer.SetValue( "Name", std::string( "TIE Fighter " ) + std::to_string( i ) );
					serializer.EnterObject( "Position" );
						serializer.SetValue( "X", 12 );
						serializer.SetValue( "Y", 12 );
						serializer.SetValue( "Z", 12 );
					serializer.Exit();
				serializer.Exit();
			}
		serializer.Exit();
		serializer.EnterArray( "Assets" );
			serializer.SetValue( "Asset", "TIE Fighter model" );
			serializer.SetValue( "Asset", "Nebulon B" );
			serializer.SetValue( "Asset", "Death Star" );
			serializer.SetValue( "Asset", "Calamarian Cruiser" );
			serializer.SetValue( "Asset", "Star Destroyer" );
		serializer.Exit();

		// Nie wiem czy takie coœ ma sens
		serializer.EnterArray( "Passes" );
			serializer.EnterArray( "Lights" );
				serializer.SetValue( "light", "directional" );
				serializer.SetValue( "light", "point" );
			serializer.Exit();
		serializer.Exit();
		//serializer.EnterObject( "Data" );
		//	serializer.SetValue( "NumberUnits", 23 );
		//serializer.Exit();
	serializer.Exit();

	std::string fileName = "tylko_do_testow/serialization/serialWrite.json";
	if( !serializer.SaveFile( fileName, WritingMode::Readable ) )
		OutputDebugString( L"Error: Saving \"tylko_do_testow/serialization/serialWrite.json\" failed!\n" );

	return 0;
}