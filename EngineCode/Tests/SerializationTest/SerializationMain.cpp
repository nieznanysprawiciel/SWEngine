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
	serializer.Exit();

	std::string fileName = "tylko_do_testow/serialization/serialWrite.json";
	if( !serializer.SaveFile( fileName, WritingMode::Readable ) )
		OutputDebugString( L"Error: Saving \"tylko_do_testow/serialization/serialWrite.json\" failed!\n" );

	return 0;
}