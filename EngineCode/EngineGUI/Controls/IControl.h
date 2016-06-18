#pragma once

#include "Common/EngineObject.h"
#include "Common/Delegate.h"


/**@defgroup EngineGUI
@ingroup ModulesStructure
@brief Modu³ s³u¿¹cy do renderowania interfejsu graficznego u¿ytkownika.*/

namespace GUI
{

struct IEventArgs;
struct InputEventArgs;
struct MouseEventArgs;
struct MouseButtonEventArgs;
struct MouseWheelEventArgs;

class HostWindow;
class TopLevelControl;



/**@brief Klasa bazowa dla kontrolek.*/
class IControl : public EngineObject
{
	RTTR_ENABLE( EngineObject )
	RTTR_REGISTRATION_FRIEND
private:
protected:

	IControl*			m_parent;

public:
							IControl		( IControl* parent );
	virtual					~IControl		();

	IControl*				GetParent		()			{ return m_parent; }
	TopLevelControl*		GetRootControl	();
	HostWindow*				GetHost			();


	virtual bool			HitTest			( float X, float Y );

public:
	// Event handlers

	virtual void			Loaded						( IControl* sender, IEventArgs* e )					{}
	virtual void			Unloaded					( IControl* sender, IEventArgs* e )					{}

	virtual void			GotFocus					( IControl* sender, IEventArgs* e )					{}
	virtual void			LostFocus					( IControl* sender, IEventArgs* e )					{}

	// Mouse
	virtual void			MouseEnter					( IControl* sender, MouseEventArgs* e )				{}
	virtual void			MouseLeave					( IControl* sender, MouseEventArgs* e )				{}
	virtual void			MouseMove					( IControl* sender, MouseEventArgs* e )				{}
	virtual void			MouseWheel					( IControl* sender, MouseWheelEventArgs* e )		{}

	virtual void			MouseDown					( IControl* sender, MouseButtonEventArgs* e )		{}
	virtual void			MouseUp						( IControl* sender, MouseButtonEventArgs* e )		{}

	virtual void			MouseRightButtonDown		( IControl* sender, MouseButtonEventArgs* e )		{}
	virtual void			MouseRightButtonUp			( IControl* sender, MouseButtonEventArgs* e )		{}

	virtual void			MouseLeftButtonDown			( IControl* sender, MouseButtonEventArgs* e )		{}
	virtual void			MouseLeftButtonUp			( IControl* sender, MouseButtonEventArgs* e )		{}


	virtual void			PreviewMouseEnter				( IControl* sender, MouseEventArgs* e )				{}
	virtual void			PreviewMouseLeave				( IControl* sender, MouseEventArgs* e )				{}
	virtual void			PreviewMouseMove				( IControl* sender, MouseEventArgs* e )				{}
	virtual void			PreviewMouseWheel				( IControl* sender, MouseWheelEventArgs* e )		{}

	virtual void			PreviewMouseDown				( IControl* sender, MouseButtonEventArgs* e )		{}
	virtual void			PreviewMouseUp					( IControl* sender, MouseButtonEventArgs* e )		{}

	virtual void			PreviewMouseRightButtonDown		( IControl* sender, MouseButtonEventArgs* e )		{}
	virtual void			PreviewMouseRightButtonUp		( IControl* sender, MouseButtonEventArgs* e )		{}

	virtual void			PreviewMouseLeftButtonDown		( IControl* sender, MouseButtonEventArgs* e )		{}
	virtual void			PreviewMouseLeftButtonUp		( IControl* sender, MouseButtonEventArgs* e )		{}

	//Keyboard


};

}	// GUI
