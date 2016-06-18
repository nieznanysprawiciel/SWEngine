#include "HostWindow.h"


namespace GUI
{


/**@brief Zwraca iloœæ pamiêci zajmowan¹ przez dany obiekt i wszystkie obiekty, których jest
on w³aœcicielem.*/
Size HostWindow::GetMemorySize()
{
	Size size = sizeof( HostWindow );

	size += m_mousePath.capacity() * sizeof( IControl* );
	size += m_focusPath.capacity() * sizeof( IControl* );
	size += m_invalidated.capacity() * sizeof( IControl* );
	size += m_controlTree.capacity() * sizeof( TopLevelControl* );

	//size += m_nativeWindow->MemorySize();

	for( auto control : m_controlTree )
		size += control->MemorySize();

	return size;
}


}	// GUI
