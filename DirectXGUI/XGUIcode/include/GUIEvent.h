#pragma once
/**
@file GUIEvent.h
@brief Plik zawiera Deklaracje struktur event�w oraz enumeracje s�u��c� do identyfikowania ich typ�w.
*/


namespace XGUI
{

	/**@brief Enumeracja identyfikuj�ca mo�liwe do wygenerowania eventy.*/
	enum EventType
	{

	};


	/**@brief Struktura przekazywana w funkcjach obs�ugi zdarze�.

	Wszystkie struktury opisuj�ce eventy dziedzicz� po tej strukturze.
	*/
	struct GUIEvent
	{
		unsigned int type;		///< Jedna z warto�ci enuma @ref EventType


		virtual ~GUIEvent();
	};

}