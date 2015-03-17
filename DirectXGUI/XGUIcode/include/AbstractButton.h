#pragma once

/**
@file AbstractButton.h
@brief Plik zawiera deklaracj� klasy AbstractButton.
*/

#include "XGUI.h"

namespace XGUI
{


	class AbstractButton : public Control
	{
	private:
		bool			pushed;				///<Identyfikuje czy przycisk jest aktualnie wci�ni�ty
		bool			disabled;			///<Umo�liwia wy��czenie przycisku (mimo �e jest widoczny)
	protected:
		bool			work_as_switch;		///<Tryb pracy przycisku. Zmienn� trzeba ustawi� w konstruktorze klasy pochodnej.

		bool			enable_onPush;		///<W��cza wywo�ywanie funkcji w momencie powstania eventu
		bool			enable_onPull;		///<W��cza wywo�ywanie funkcji w momencie powstania eventu
		bool			enable_onChange;	///<W��cza wywo�ywanie funkcji w momencie powstania eventu
		bool			enable_onDisable;	///<W��cza wywo�ywanie funkcji w momencie powstania eventu

		virtual void onPush();
		virtual void onPull();
		virtual void onChangeState();
		virtual void onDisable( bool disable );
	public:
		AbstractButton( Control* parent );
		~AbstractButton();

		inline bool isUp() const { return !pushed; }		///<Zwraca informacj� czy przycisk jest niewci�ni�ty
		inline bool isDown() const { return pushed; }		///<Zwraca informacj� czy przycisk jest wci�ni�ty
		inline bool isChecked() const { return pushed; }	///<Zwraca informacj� czy przycisk jest wci�ni�ty
		inline bool isSwitch() const { return work_as_switch; }	///<Zwraca informacje czy przycisk jest switchem czy przyciskiem wciskanym


		XGUIEventDelegate		eventPush;			///<Delegat, kt�ry zostanie wywo�any w momencie powstania eventu.
		XGUIEventDelegate		eventPull;			///<Delegat, kt�ry zostanie wywo�any w momencie powstania eventu.
		XGUIEventDelegate		eventChangeState;	///<Delegat, kt�ry zostanie wywo�any w momencie powstania eventu.
	};







}