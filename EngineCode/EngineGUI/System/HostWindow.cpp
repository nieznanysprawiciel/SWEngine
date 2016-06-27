#include "HostWindow.h"


#include "CommonTypes/CommonTypes.h"

namespace GUI
{

// One pointer at least, but I don't know how much it needs in reality
#define STD_MAP_OVERHEAD_PER_ELEMENT 8



HostWindow::HostWindow( INativeWindow* nativeWindow, IInput* input )
	:	m_input( input )
	,	m_nativeWindow( nativeWindow )
{}



/**@brief Returns memory consumed by this object and all object owned by this.*/
Size		HostWindow::GetMemorySize()
{
	Size size = sizeof( HostWindow );

	size += m_mousePath.capacity() * sizeof( IControl* );
	size += m_focusPath.capacity() * sizeof( IControl* );
	size += m_invalidated.capacity() * sizeof( IControl* );
	size += m_controlTree.capacity() * sizeof( TopLevelControl* );

	size += m_controlsNames.size() * ( sizeof( std::pair< IControl*, std::string > ) + STD_MAP_OVERHEAD_PER_ELEMENT );

	//size += m_nativeWindow->MemorySize();

	for( auto control : m_controlTree )
		size += control->MemorySize();

	return size;
}

/**@brief Removes control from GUI system.*/
void		HostWindow::RemoveControl( IControl* control )
{

}

/**@brief Allows control to register it's name.*/
void		HostWindow::RegisterControlName( IControl* control, const std::string& name )
{
	assert( m_controlsNames.find( control ) == m_controlsNames.end() );
	m_controlsNames[ control ] = name;
}

/**@brief Gets name of registered control.*/
const std::string& HostWindow::GetControlName( IControl* control )
{
	auto iter = m_controlsNames.find( control );
	if( iter != m_controlsNames.end() )
		return iter->second;
	else 
		return EMPTY_STRING;
}


}	// GUI
