#pragma once

#include "EngineGUI/System/Windows/INativeWindow.h"
#include "EngineGUI/Controls/IControl.h"
#include "EngineGUI/Controls/TopLevelControl.h"

#include <vector>


namespace GUI
{

/**@brief Klasa bêd¹ca rootem hierarchi kontrolek dla danego natywnego okna.

@ingroup EngineGUI

Klasa zarz¹dza przesy³aniem eventów w danym oknie.

Bezpoœrednimi dzieæmy tej klasy moga byæ tylko kontrolki dziedzicz¹ce po TopLevelControl czyli okna (Window)
oraz ró¿ne rodzaje menu ContextMenu, PopupMenu itp.*/
class HostWindow
{
private:

	INativeWindow*				m_nativeWindow;
	//IInput*					m_input;

	std::vector< IControl* >	m_mousePath;		///< Hierarchia kontrolek posiadaj¹cych mysz w danym momencie.
	std::vector< IControl* >	m_focusPath;		///< Hierarchia kontrolek posiadaj¹cych focusa.
	std::vector< IControl* >	m_invalidated;		///< Kontrolki wymagaj¹ce odœwie¿enia (odrysowania).

	std::vector< TopLevelControl* >		m_controlTree;	///< Hierarchia kontrolek + elementy nie nale¿¹ce do hierarchi jak wyskakuj¹ce menu itp.

protected:
public:
	explicit		HostWindow	() = default;
					~HostWindow	() = default;


	Size			GetMemorySize		();
};


}	// GUI
