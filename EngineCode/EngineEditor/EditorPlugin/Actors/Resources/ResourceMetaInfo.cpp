#pragma warning(disable : 4561)	// DirectXMath converting fastcall to stdcall

#include "EngineEditor/EditorPlugin/stdafx.h"


#include "ResourceMetaInfo.h"



namespace sw {
namespace EditorPlugin
{

// ================================ //
//
ResourceMetaInfo::ResourceMetaInfo( ResourceWrapper^ resource )
	: EngineObjectMetaInfo( resource )
{}


}	// EditorPlugin
}	// sw


