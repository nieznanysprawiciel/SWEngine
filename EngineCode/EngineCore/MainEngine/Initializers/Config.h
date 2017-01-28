#pragma once
/**
@file Config.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "Common/TypesDefinitions.h"

#include <string>


/**@brief Engine start configuration.*/
class Config
{
private:

	uint16			m_screenWidth;
	uint16			m_screenHeight;
	bool			m_fullscreen;

	std::string		m_startLevelPath;

protected:
public:
	explicit		Config	( const std::string filePath );
					~Config	() = default;


	uint16			ScreenWidth		() const;
	uint16			ScreenHeight	() const;
	bool			Fullscreen		() const;

	const std::string		StartLevelPath	() const;

	bool			SaveConfig		( const std::string filePath ) const;
};






