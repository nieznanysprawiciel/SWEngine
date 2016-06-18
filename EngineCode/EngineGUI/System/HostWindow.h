#pragma once

#include "EngineGUI/System/Windows/INativeWindow.h"
#include "EngineGUI/Controls/IControl.h"
#include "EngineGUI/Controls/TopLevelControl.h"

#include <vector>


namespace GUI
{

/**@brief Klasa b�d�ca rootem hierarchi kontrolek dla danego natywnego okna.

@ingroup EngineGUI

Klasa zarz�dza przesy�aniem event�w w danym oknie.

Bezpo�rednimi dzie�my tej klasy moga by� tylko kontrolki dziedzicz�ce po TopLevelControl czyli okna (Window)
oraz r�ne rodzaje menu ContextMenu, PopupMenu itp.*/
class HostWindow
{
private:

	INativeWindow*				m_nativeWindow;
	//IInput*					m_input;

	std::vector< IControl* >	m_mousePath;		///< Hierarchia kontrolek posiadaj�cych mysz w danym momencie.
	std::vector< IControl* >	m_focusPath;		///< Hierarchia kontrolek posiadaj�cych focusa.
	std::vector< IControl* >	m_invalidated;		///< Kontrolki wymagaj�ce od�wie�enia (odrysowania).

	std::vector< TopLevelControl* >		m_controlTree;	///< Hierarchia kontrolek + elementy nie nale��ce do hierarchi jak wyskakuj�ce menu itp.

protected:
public:
	explicit		HostWindow	() = default;
					~HostWindow	() = default;


	Size			GetMemorySize		();
};


}	// GUI
