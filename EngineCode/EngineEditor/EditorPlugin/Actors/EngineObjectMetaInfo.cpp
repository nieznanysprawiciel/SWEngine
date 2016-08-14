#include "EngineObjectMetaInfo.h"



namespace EditorPlugin
{

EngineObjectMetaInfo::EngineObjectMetaInfo()
{}

/**@brief */
void		EngineObjectMetaInfo::ResetActor		( EngineObjectWrapper^ objectPtr )
{
	BuildHierarchy( objectPtr );
}

/**@brief */
void		EngineObjectMetaInfo::BuildHierarchy	( EngineObjectWrapper^ objectPtr )
{
	throw gcnew System::NotImplementedException();
}



}	// EditorPlugin
