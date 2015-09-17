#pragma once
/**
@file AbstractWindow.h
@brief Plik zawiera deklaracj� klasy AbstractWindow.
*/

#include "XGUI.h"
#include "AbstractMenu.h"
#include "AbstractButton.h"


namespace XGUI
{
	/**@brief Klasa bazowa dla okien.
	
	Klasa mo�e stanowi� podstaw� dla innych typ�w kontrolek, mniej przypominaj�cych
	okna, np. kontener�w dla innych kontrolek oraz kontrolek u�ytkownika realizuj�cych
	funkcjonalno�ci zupe�nie niepodobne do innych klas bazowych.*/
	class AbstractWindow : public Control
	{
	private:
		int8		resizeable : 1;			///<Czy okno mo�e zmienia� rozmiar.
		int8		moveable : 1;			///<Czy okno mo�e by� przesuwane przez u�ytkownika.
		int8		close_button : 1;		///<Czy okno ma przycisk zamykaj�cy.
		int8		minimize_button : 1;	///<Czy okno ma przycisk minimalizuj�cy. @todo Wymy�le� ca�y mechanizm.
		int8		maximize_button : 1;	///<Czy okno ma przycisk maksymalizuj�cy. @todo Wymy�le� ca�y mechanizm.
		int8		window_menu : 1;		///<Czy okno ma w�asne menu.

		int8		is_moving : 1;			///<Zmienna ustawiana, je�eli okno jest w trakcie przesuwania. @note Musi by� ustawiona zmienna moveable.
		int8		is_resizing : 1;		///<Zmienna ustawiana, je�eli okno jest w trakcie zmieniania rozmiaru. @note Musi by� ustawiona zmienna resizeable.
	protected:
		int8		enable_onResizeWindow : 1;		///<W��cza wywo�ywanie funkcji w momencie powstania eventu.
		int8		enable_onMinimizeWindow : 1;	///<W��cza wywo�ywanie funkcji w momencie powstania eventu.
		int8		enable_onMaximizeWindow : 1;	///<W��cza wywo�ywanie funkcji w momencie powstania eventu.
		int8		enable_onCloseWindow : 1;		///<W��cza wywo�ywanie funkcji w momencie powstania eventu.

		AbstractMenu*		menu;						///<Wska�nik na obiekt menu. @note Musi by� ustawiona zmienna window_menu.
		AbstractButton*		close_window_button;		///<Wska�nik na przycisk zamykaj�cy okno. @note Musi by� ustawiona zmienna close_button.
		AbstractButton*		minimize_window_buttn;		///<Wska�nik na przycisk minimalizuj�cy okno. @note Musi by� ustawiona zmienna minimize_button.
		AbstractButton*		maximize_window_button;		///<Wska�nik na przycisk maksymalizuj�cy okno. @note Musi by� ustawiona zmienna maximize_button.

		virtual void onResizeWindow();
		virtual void onMinimizeWindow();
		virtual void onMaximizeWindow();
		virtual void onCloseWindow();
	public:
		AbstractWindow( Control* parent );
		~AbstractWindow();

		XGUIEventDelegate		eventResizeWindow;		///<Delegat, kt�ry zostanie wywo�any w momencie powstania eventu.
		XGUIEventDelegate		eventMinimizeWindow;	///<Delegat, kt�ry zostanie wywo�any w momencie powstania eventu.
		XGUIEventDelegate		eventMaximizeWindow;	///<Delegat, kt�ry zostanie wywo�any w momencie powstania eventu.
		XGUIEventDelegate		eventCloseWindow;		///<Delegat, kt�ry zostanie wywo�any w momencie powstania eventu.

		inline bool isResizeable() const { return resizeable && 1; }
		inline bool isResizing() const { return is_resizing && 1; }
		inline bool isMoveable() const { return moveable && 1; }
		inline bool isMoving() const { return is_moving && 1; }
		inline bool hasCloseButton() const { return close_button && 1; }
		inline bool hasMenu() const { return window_menu && 1; }
		inline bool hasMinimizeButton() const { return minimize_button && 1; }
		inline bool hasMaximizeButton() const { return maximize_button && 1; }

		inline AbstractMenu* getMenu() const { return menu; }
	};

}
