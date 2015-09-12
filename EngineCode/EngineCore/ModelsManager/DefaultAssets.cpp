#include "EngineCore/stdafx.h"
/**
@file DefaultAssets.cpp
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.
*/

#include "DefaultAssets.h"
#include "GraphicAPI/ResourcesFactory.h"


InputLayoutDescriptor* DefaultAssets::LAYOUT_POSITION_NORMAL_COORD = nullptr;
InputLayoutDescriptor* DefaultAssets::LAYOUT_POSITION_COORD = nullptr;
InputLayoutDescriptor* DefaultAssets::LAYOUT_POSITION_COLOR = nullptr;

void DefaultAssets::Init()
{
	// 
	LAYOUT_POSITION_NORMAL_COORD = ResourcesFactory::CreateInputLayoutDescritor( L"::PositionNormalCoord" );
	LAYOUT_POSITION_NORMAL_COORD->AddRow( SEMANTIC_POSITION, ResourceFormat::RESOURCE_FORMAT_R32G32B32_FLOAT, 0, 0, false, 0 );
	LAYOUT_POSITION_NORMAL_COORD->AddRow( SEMANTIC_NORMAL, ResourceFormat::RESOURCE_FORMAT_R32G32B32_FLOAT, 0, 12, false, 0 );
	LAYOUT_POSITION_NORMAL_COORD->AddRow( SEMANTIC_TEXCOORD, ResourceFormat::RESOURCE_FORMAT_R32G32_FLOAT, 0, 24, false, 0 );

	//
	LAYOUT_POSITION_COORD = ResourcesFactory::CreateInputLayoutDescritor( L"::PositionCoord" );
	LAYOUT_POSITION_COORD->AddRow( SEMANTIC_POSITION, ResourceFormat::RESOURCE_FORMAT_R32G32B32_FLOAT, 0, 0, false, 0 );
	LAYOUT_POSITION_COORD->AddRow( SEMANTIC_TEXCOORD, ResourceFormat::RESOURCE_FORMAT_R32G32_FLOAT, 0, 12, false, 0 );

	//
	LAYOUT_POSITION_COLOR = ResourcesFactory::CreateInputLayoutDescritor( L"::PositionColor" );
	LAYOUT_POSITION_COLOR->AddRow( SEMANTIC_POSITION, ResourceFormat::RESOURCE_FORMAT_R32G32B32_FLOAT, 0, 0, false, 0 );
	LAYOUT_POSITION_COLOR->AddRow( SEMANTIC_COLOR, ResourceFormat::RESOURCE_FORMAT_R32G32B32_FLOAT, 0, 12, false, 0 );
}