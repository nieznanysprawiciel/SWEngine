#pragma once

/**
@file AbstractButton.h
@brief Plik zawiera deklaracjê klasy AbstractButton.
*/

#include "XGUI.h"

namespace XGUI
{


	class AbstractButton : public Control
	{
	private:
		bool			pushed;				///<Identyfikuje czy przycisk jest aktualnie wciœniêty
		bool			disabled;			///<Umo¿liwia wy³¹czenie przycisku (mimo ¿e jest widoczny)
	protected:
		bool			work_as_switch;		///<Tryb pracy przycisku. Zmienn¹ trzeba ustawiæ w konstruktorze klasy pochodnej.

		bool			enable_onPush;		///<W³¹cza wywo³ywanie funkcji w momencie powstania eventu
		bool			enable_onPull;		///<W³¹cza wywo³ywanie funkcji w momencie powstania eventu
		bool			enable_onChange;	///<W³¹cza wywo³ywanie funkcji w momencie powstania eventu
		bool			enable_onDisable;	///<W³¹cza wywo³ywanie funkcji w momencie powstania eventu

		virtual void onPush();
		virtual void onPull();
		virtual void onChangeState();
		virtual void onDisable( bool disable );
	public:
		AbstractButton( Control* parent );
		~AbstractButton();

		inline bool isUp() const { return !pushed; }		///<Zwraca informacjê czy przycisk jest niewciœniêty
		inline bool isDown() const { return pushed; }		///<Zwraca informacjê czy przycisk jest wciœniêty
		inline bool isChecked() const { return pushed; }	///<Zwraca informacjê czy przycisk jest wciœniêty
		inline bool isSwitch() const { return work_as_switch; }	///<Zwraca informacje czy przycisk jest switchem czy przyciskiem wciskanym


		XGUIEventDelegate		eventPush;			///<Delegat, który zostanie wywo³any w momencie powstania eventu.
		XGUIEventDelegate		eventPull;			///<Delegat, który zostanie wywo³any w momencie powstania eventu.
		XGUIEventDelegate		eventChangeState;	///<Delegat, który zostanie wywo³any w momencie powstania eventu.
	};







}