#include <windows.h>
#include <fstream>

#include "Tools/SerializerJson/Serializer.h"


int main()
{
	ISerializer* serializer = new ISerializer;


	serializer->EnterObject( "FirstObject" );
		serializer->SetValue( "Map", "LightmapGen1" );
		serializer->SetValue( "Path", "/LightmapGen1.map" );
		serializer->SetValue( "Load", "true" );
	serializer->Exit();

	std::string fileName = "tylko_do_testow/serialization/serialWrite.json";
	if( !serializer->SaveFile( fileName ) )
		OutputDebugString( L"Error: Saving \"tylko_do_testow/serialization/serialWrite.json\" failed!\n" );

	return 0;
}