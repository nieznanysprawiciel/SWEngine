#pragma once

/**
@file AbstractText.h
@brief Plik zawiera deklaracj� klasy AbstractText.
*/

#include "XGUI.h"
#include <string>

namespace XGUI
{
	class Font;

	/**@brief Klasa bazowa do wy�wietlania tekstu.

	Klasa ma za zadanie wyswietli� go�y tekst na przezroczystym tle
	ewentualnie mo�e by� jednolite wype�nienie. Nie jest to kontrolka pozawalaj�ca na jakiekolwiek
	modyfikowanie tekstu, ani nie jest to pole tekstowe.
	*/
	class AbstractText :	public Control
	{
	private:
		std::wstring		text;			///<String zawieraj�cy tekst do wy�wietlenia.
		DirectX::XMFLOAT4	text_color;		///<Kolor tekstu. Mo�e zawiera� kana� alfa, ale nie ma gwarancji, �e zostanie on u�yty jako przezroczysto��.
		ALIGNMENT			text_alignment;	///<Wyr�wnanie tekstu wzgl�dem obszaru kontrolki.
		Font*				font;			///<Wska�nik na obiekt zawieraj�cy czcionk�.
		unsigned char		font_size;		///<Rozmiar czcionki.

	protected:
		int8	enable_onTextChange : 1;			///<W��cza wywo�ywanie funkcji w momencie powstania eventu
		int8	enable_onTextSizeChange : 1;		///<W��cza wywo�ywanie funkcji w momencie powstania eventu
		int8	enable_onTextColorChange : 1;		///<W��cza wywo�ywanie funkcji w momencie powstania eventu
		int8	enable_onFontChange : 1;			///<W��cza wywo�ywanie funkcji w momencie powstania eventu
		int8	enable_onAlignmentChange : 1;		///<W��cza wywo�ywanie funkcji w momencie powstania eventu

		virtual void onTextChange();
		virtual void onTextSizeChange();
		virtual void onTextColorChange();
		virtual void onFontChange();
		virtual void onAlignmentChange();

		//inline const std::wstring& getTextStringRef() const { return text; }	///<Zwraca referencj� na przechowywanego stringa.
	public:
		AbstractText( Control* parent );
		~AbstractText();

		inline Font* getFont() const { return font; }					///<Zwraca wska�nika na obiekt czcionki.
		inline const wchar_t* getText() const { return text.c_str(); }	///<Zwraca wska�nik na tablic� znak�w zawieraj�c� tekst.
		inline std::wstring getTextString() const { return text; }		///<Zwraca string zawieraj�cy tekst.
		inline unsigned char getFontSize() const { return font_size; }	///<Zwraca rozmiar czcionki.
		inline ALIGNMENT getTextAlignment() const { return text_alignment; }	///<Zwraca wyr�wnanie tekstu wzgl�dem swojego obszzaru renderowania.
		inline DirectX::XMFLOAT4 getTextColor() const { return text_color; }	///<Zwraca kolor tekstu.

		void setFont( Font* new_font );									///<Ustawia czcionk�.
		void setFontSize( unsigned char new_size );						///<Ustawia wielko�� czcionki.
		void setAlignment( ALIGNMENT new_align );						///<Ustawia wyr�wnanie tekstu wzgl�dem obszaru kontrolki.
		void setTextColor( DirectX::XMFLOAT4 new_color );				///<Ustawia kolor tekstu.
		void setText( std::wstring new_text );							///<Ustawia nowy tekst do wy�wietlania.
	};

}
