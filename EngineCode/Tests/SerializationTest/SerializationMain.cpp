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
#include "Common/Serialization/Deserializer.h"


#ifdef TEST_JSON
	std::string writeFileName = "tylko_do_testow/serialization/serialWrite.json";
	std::string readFileName = "tylko_do_testow/serialization/serialRead.json";
	std::ofstream outputFile( "tylko_do_testow/serialization/testResultJSON.txt" );
#elif TEST_XML
	std::string writeFileName = "tylko_do_testow/serialization/serialWrite.xml";
	std::string readFileName = "tylko_do_testow/serialization/serialRead.xml";
	std::ofstream outputFile( "tylko_do_testow/serialization/testResultXML.txt" );
#endif



int main()
{
	ISerializer serializer;

	serializer.SetAttribute( "FreeValue", "value" );	// Nie pojawi siê w XMLu.
	serializer.EnterObject( "FirstObject" );
		serializer.SetAttribute( "Map", "LightmapGen1" );
		serializer.SetAttribute( "Path", "/LightmapGen1.map" );
		serializer.SetAttribute( "Load", true );
		serializer.EnterObject( "Data" );
			serializer.SetAttribute( "NumberUnits", 1266643 );
			serializer.SetAttribute( "PositionOffset", 0.4124667623 );
			serializer.SetAttribute( "Visible", true );
			serializer.SetAttribute( "Key", 218588284382834538 );
		serializer.Exit();
		serializer.EnterArray( "Actors" );
			for( unsigned int i = 0; i < 12; ++i )
			{
				serializer.EnterObject( "Actor" );
					serializer.SetAttribute( "Name", std::string( "TIE Fighter " ) + std::to_string( i ) );
					serializer.EnterObject( "Position" );
						serializer.SetAttribute( "X", 12 );
						serializer.SetAttribute( "Y", 12 );
						serializer.SetAttribute( "Z", 12 );
					serializer.Exit();
				serializer.Exit();
			}
		serializer.Exit();	// Actors
		serializer.EnterArray( "Assets" );
			serializer.SetAttribute( "Asset", "TIE Fighter model" );
			serializer.SetAttribute( "Asset", "Nebulon B" );
			serializer.SetAttribute( "Asset", "Death Star" );
			serializer.SetAttribute( "Asset", "Calamarian Cruiser" );
			serializer.SetAttribute( "Asset", "Star Destroyer" );
		serializer.Exit();	// Assets

		// Nie wiem czy takie coœ ma sens
		serializer.EnterArray( "Passes" );
			serializer.EnterArray( "Lights" );
				serializer.SetAttribute( "light", "directional" );
				serializer.SetAttribute( "light", "point" );
			serializer.Exit();	//	Lights
		serializer.Exit();	// Passes
		//serializer.EnterObject( "Data" );
		//	serializer.SetAttribute( "NumberUnits", 23 );
		//serializer.Exit();
	serializer.Exit();

	//serializer.EnterObject( "FirstObject" );
	//	serializer.EnterObject( "Data" );
	//		serializer.SetAttribute( "Map", "LightmapGen1" );
	//		serializer.SetAttribute( "Path", "/LightmapGen1.map" );
	//		serializer.SetAttribute( "Load", true );
	//	serializer.Exit();
	//	serializer.EnterObject( "Data2" );
	//		serializer.SetAttribute( "Asset1", "TIE Fighter model" );
	//		serializer.SetAttribute( "Asset2", "Nebulon B" );
	//		serializer.SetAttribute( "Asset3", "Death Star" );
	//		serializer.SetAttribute( "Asset4", "Calamarian Cruiser" );
	//		serializer.SetAttribute( "Asset5", "Star Destroyer" );
	//	serializer.Exit();
	//	serializer.EnterObject( "Data3" );
	//		serializer.SetAttribute( "NumberUnits", std::numeric_limits<uint32>::max() );
	//		serializer.SetAttribute( "UnitsModifier", std::numeric_limits<int32>::min() );
	//		serializer.SetAttribute( "PositionOffset", 0.412466762375758958574674674764488949497747648 );
	//		serializer.SetAttribute( "Visible", true );
	//		serializer.SetAttribute( "Key", std::numeric_limits<int64>::min() );
	//		serializer.SetAttribute( "KeyHash", std::numeric_limits<uint64>::max() );
	//	serializer.Exit();
	//serializer.Exit();


	if( !serializer.SaveFile( writeFileName, WritingMode::Readable ) )
#ifdef TEST_JSON
		OutputDebugString( L"Error: Saving \"tylko_do_testow/serialization/serialWrite.json\" failed!\n" );
#elif TEST_XML
		OutputDebugString( L"Error: Saving \"tylko_do_testow/serialization/serialWrite.xml\" failed!\n" );
#endif
	
	IDeserializer deser;


	if( deser.LoadFromFile( readFileName, ParsingMode::ParseInsitu ) )
	{
		outputFile << deser.GetAttribute( "FreeValue", "GetAttribute error" ) << std::endl;
		deser.EnterObject( "FirstObject" );
			outputFile << deser.GetAttribute( "Map", "GetAttribute error" ) << std::endl;
			outputFile << deser.GetAttribute( "Path", "GetAttribute error" ) << std::endl;
			outputFile << deser.GetAttribute( "Load", true ) << std::endl;
			deser.EnterObject( "Data" );
				outputFile << deser.GetAttribute( "NumberUnits", 1266643 ) << std::endl;
				outputFile << deser.GetAttribute( "PositionOffset", 0.4124667623 ) << std::endl;
				outputFile << deser.GetAttribute( "Visible", true ) << std::endl;
				outputFile << deser.GetAttribute( "Key", 218588284382834538 ) << std::endl;
			deser.Exit();	// Data
		if( deser.EnterArray( "Actors" ) )
		{
			if( deser.FirstElement() )
			{
				do
				{
					outputFile << deser.GetAttribute( "Name", "Wrong string" ) << std::endl;
					
					deser.EnterObject( "Position" );
						outputFile << "\t" << deser.GetAttribute( "X", 0 ) << std::endl;
						outputFile << "\t" << deser.GetAttribute( "Y", 0 ) << std::endl;
						outputFile << "\t" << deser.GetAttribute( "Z", 0 ) << std::endl;
					deser.Exit();

				} while( deser.NextElement() );
				deser.Exit();		// Mo¿e to jest do wyrzucenia. Serializator móg³by sam wycyhodziæ.
			}

			deser.Exit();	// Actors
		}

		deser.Exit();	// FirstObject

	}
	else
	{
		std::string errorMessage = deser.GetError();
		OutputDebugString( std::wstring( errorMessage.begin(), errorMessage.end() ).c_str() );
		OutputDebugString( L"\n" );
	}

	return 0;
}