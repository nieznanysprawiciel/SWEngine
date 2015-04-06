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
		int8			pushed : 1;				///<Identyfikuje czy przycisk jest aktualnie wci�ni�ty
	protected:
		int8			work_as_switch : 1;		///<Tryb pracy przycisku. Zmienn� trzeba ustawi� w konstruktorze klasy pochodnej.

		int8			enable_onPush : 1;		///<W��cza wywo�ywanie funkcji w momencie powstania eventu
		int8			enable_onPull : 1;		///<W��cza wywo�ywanie funkcji w momencie powstania eventu
		int8			enable_onChange : 1;	///<W��cza wywo�ywanie funkcji w momencie powstania eventu

		virtual void onPush();
		virtual void onPull();
		virtual void onChangeState();

		void pushButton();		///<Programowe wci�ni�cie przycisku. Klasa pochodna mo�e zdecydowa�, �e trzeba wcisn�� przycisk mimo, �e nie kliknieto go.
	public:
		AbstractButton( Control* parent );
		~AbstractButton();

		inline bool isUp() const { return !pushed && 1; }		///<Zwraca informacj� czy przycisk jest niewci�ni�ty
		inline bool isDown() const { return pushed && 1; }		///<Zwraca informacj� czy przycisk jest wci�ni�ty
		inline bool isChecked() const { return pushed && 1; }	///<Zwraca informacj� czy przycisk jest wci�ni�ty
		inline bool isSwitch() const { return work_as_switch && 1; }	///<Zwraca informacje czy przycisk jest switchem czy przyciskiem wciskanym


		XGUIEventDelegate		eventPush;			///<Delegat, kt�ry zostanie wywo�any w momencie powstania eventu.
		XGUIEventDelegate		eventPull;			///<Delegat, kt�ry zostanie wywo�any w momencie powstania eventu.
		XGUIEventDelegate		eventChangeState;	///<Delegat, kt�ry zostanie wywo�any w momencie powstania eventu.
	};







}