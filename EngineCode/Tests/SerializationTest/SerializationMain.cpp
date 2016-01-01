/**@file SerializationMain.cpp
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Testy serializatora.*/

#include <windows.h>
#include <fstream>
#include <numeric>
#undef max
#undef min

#include "Common/Serialization/Serializer.h"


int main()
{
	ISerializer serializer;

	//serializer.SetValue( "FreeValue", "value" );
	//serializer.EnterObject( "FirstObject" );
	//	serializer.SetValue( "Map", "LightmapGen1" );
	//	serializer.SetValue( "Path", "/LightmapGen1.map" );
	//	serializer.SetValue( "Load", true );
	//	serializer.EnterObject( "Data" );
	//		serializer.SetValue( "NumberUnits", 1266643 );
	//		serializer.SetValue( "PositionOffset", 0.4124667623 );
	//		serializer.SetValue( "Visible", true );
	//		serializer.SetValue( "Key", 218588284382834538 );
	//	serializer.Exit();
	//	serializer.EnterArray( "Actors" );
	//		for( unsigned int i = 0; i < 12; ++i )
	//		{
	//			serializer.EnterObject( "" );
	//				serializer.SetValue( "Name", std::string( "TIE Fighter " ) + std::to_string( i ) );
	//				serializer.EnterObject( "Position" );
	//					serializer.SetValue( "X", 12 );
	//					serializer.SetValue( "Y", 12 );
	//					serializer.SetValue( "Z", 12 );
	//				serializer.Exit();
	//			serializer.Exit();
	//		}
	//	serializer.Exit();
	//	serializer.EnterArray( "Assets" );
	//		serializer.SetValue( "Asset", "TIE Fighter model" );
	//		serializer.SetValue( "Asset", "Nebulon B" );
	//		serializer.SetValue( "Asset", "Death Star" );
	//		serializer.SetValue( "Asset", "Calamarian Cruiser" );
	//		serializer.SetValue( "Asset", "Star Destroyer" );
	//	serializer.Exit();

	//	// Nie wiem czy takie coœ ma sens
	//	serializer.EnterArray( "Passes" );
	//		serializer.EnterArray( "Lights" );
	//			serializer.SetValue( "light", "directional" );
	//			serializer.SetValue( "light", "point" );
	//		serializer.Exit();
	//	serializer.Exit();
	//	//serializer.EnterObject( "Data" );
	//	//	serializer.SetValue( "NumberUnits", 23 );
	//	//serializer.Exit();
	//serializer.Exit();

	serializer.EnterObject( "FirstObject" );
		serializer.EnterObject( "Data" );
			serializer.SetValue( "Map", "LightmapGen1" );
			serializer.SetValue( "Path", "/LightmapGen1.map" );
			serializer.SetValue( "Load", true );
		serializer.Exit();
		serializer.EnterObject( "Data2" );
			serializer.SetValue( "Asset1", "TIE Fighter model" );
			serializer.SetValue( "Asset2", "Nebulon B" );
			serializer.SetValue( "Asset3", "Death Star" );
			serializer.SetValue( "Asset4", "Calamarian Cruiser" );
			serializer.SetValue( "Asset5", "Star Destroyer" );
		serializer.Exit();
		serializer.EnterObject( "Data3" );
			serializer.SetValue( "NumberUnits", std::numeric_limits<uint32>::max() );
			serializer.SetValue( "UnitsModifier", std::numeric_limits<int32>::min() );
			serializer.SetValue( "PositionOffset", 0.412466762375758958574674674764488949497747648 );
			serializer.SetValue( "Visible", true );
			serializer.SetValue( "Key", std::numeric_limits<int64>::min() );
			serializer.SetValue( "KeyHash", std::numeric_limits<uint64>::max() );
		serializer.Exit();
	serializer.Exit();

	std::string fileName = "tylko_do_testow/serialization/serialWrite.xml";
	if( !serializer.SaveFile( fileName, WritingMode::Readable ) )
		OutputDebugString( L"Error: Saving \"tylko_do_testow/serialization/serialWrite.xml\" failed!\n" );

	return 0;
}