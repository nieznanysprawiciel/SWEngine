#include "stdafx.h"
/**
@file DefaultAssets.cpp
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.
*/

#include "DefaultAssets.h"


InputLayoutDescriptor DefaultAssets::LAYOUT_POSITION_NORMAL_COORD		( L"::PositionNormalCoord" );
InputLayoutDescriptor DefaultAssets::LAYOUT_POSITION_COORD				( L"::PositionCoord" );

void DefaultAssets::Init()
{
	// 
	LAYOUT_POSITION_NORMAL_COORD.AddRow( SEMANTIC_POSITION, ResourceFormat::RESOURCE_FORMAT_R32G32B32_FLOAT, 0, 0, false, 0 );
	LAYOUT_POSITION_NORMAL_COORD.AddRow( SEMANTIC_NORMAL, ResourceFormat::RESOURCE_FORMAT_R32G32B32_FLOAT, 0, 12, false, 0 );
	LAYOUT_POSITION_NORMAL_COORD.AddRow( SEMANTIC_TEXCOORD, ResourceFormat::RESOURCE_FORMAT_R32G32_FLOAT, 0, 24, false, 0 );

	//
	LAYOUT_POSITION_COORD.AddRow( SEMANTIC_POSITION, ResourceFormat::RESOURCE_FORMAT_R32G32B32_FLOAT, 0, 0, false, 0 );
	LAYOUT_POSITION_COORD.AddRow( SEMANTIC_TEXCOORD, ResourceFormat::RESOURCE_FORMAT_R32G32_FLOAT, 0, 12, false, 0 );
}