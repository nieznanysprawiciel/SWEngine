/**
@file MeshAsset.cpp
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.
*/

#include "EngineCore/stdafx.h"
#include "MeshAsset.h"

#include "Common/MemoryLeaks.h"


RTTR_REGISTRATION
{
	rttr::registration::class_< MeshAsset >( "MeshAsset" )
	.property( "FileName", &MeshAsset::m_filePath );
}




MeshAsset::MeshAsset()
	:	ResourceObject( WRONG_ID )
{ }

MeshAsset::~MeshAsset()
{ }
