#include "EngineCore/stdafx.h"
/**
@file Config.cpp
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "Config.h"

#include "Common/Serialization/Deserializer.h"
#include "Common/Serialization/Serializer.h"


// ================================ //
//
Config::Config	( const std::string& filePath )
	:	m_screenWidth( 1024 )
	,	m_screenHeight( 768 )
	,	m_fullscreen( false )
	,	m_startLevelPath( "" )
{
	IDeserializer deser;
	if( deser.LoadFromFile( filePath, ParsingMode::ParseInsitu ) )
	{
		if( deser.EnterObject( "Config" ) )
		{
			if( deser.EnterObject( "Screen" ) )
			{
				m_screenHeight	= deser.GetAttribute( "Height", m_screenHeight );
				m_screenWidth	= deser.GetAttribute( "Width", m_screenWidth );
				m_fullscreen	= deser.GetAttribute( "Fullscreen", m_fullscreen );

				deser.Exit();
			}

			if( deser.EnterObject( "Game" ) )
			{
				if( deser.EnterObject( "StartLevel" ) )
				{
					m_startLevelPath = deser.GetAttribute( "Path", m_startLevelPath );

					deser.Exit();
				}

				deser.Exit();
			}

			deser.Exit();
		}
	}
}

// ================================ //
//
uint16			Config::ScreenWidth() const
{
	return m_screenWidth;
}

// ================================ //
//
uint16			Config::ScreenHeight() const
{
	return m_screenHeight;
}

// ================================ //
//
bool			Config::Fullscreen() const
{
	return m_fullscreen;
}

// ================================ //
//
const std::string		Config::StartLevelPath() const
{
	return m_startLevelPath;
}

// ================================ //
//
bool			Config::SaveConfig	( const std::string& filePath ) const
{
	ISerializer ser( nullptr );
	
	ser.EnterObject( "Config" );

		ser.EnterObject( "Screen" );
			ser.SetAttribute( "Height", m_screenHeight );
			ser.SetAttribute( "Width", m_screenWidth );
			ser.SetAttribute( "Fullscreen", m_fullscreen );
		ser.Exit();

		ser.EnterObject( "Game" );
			ser.EnterObject( "StartLevel" );
				ser.SetAttribute( "Path", m_startLevelPath );
			ser.Exit();
		ser.Exit();

	ser.Exit();

	return ser.SaveFile( filePath, WritingMode::Readable );
}
