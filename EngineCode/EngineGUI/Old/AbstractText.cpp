#include "AbstractText.h"

using namespace XGUI;

AbstractText::AbstractText(Control* parent)
: Control( parent )
{
	enable_onTextChange = false;
	enable_onAlignmentChange = false;
	enable_onFontChange = false;
	enable_onTextColorChange = false;
	enable_onTextSizeChange = false;
}


AbstractText::~AbstractText()
{
}


//========================================================================================//
//		Funkcje wywo�ywane w reakcji na zdarzenie i implementowane przez programist�
//========================================================================================//

/**@brief Funkcja wywo�ywana, gdy zmieni si� tekst do wy�wietlania.

Tekst mo�na pobra� za pomoca funkcji getText lub getTextString. W momencie wywo�ania nowy
tekst jest ju� ustawiony.

Funkcja domy�lnie jest pusta. Programista kontrolek z klas pochodnych mo�e zaimplementowa�
w tej funkcji dowoln� funkcjonalno��.

@note Aby funkcja zosta�a wywo�ana, zmienna @ref enable_onTextChange musi by� ustawiona na true.
*/
void AbstractText::onTextChange()
{}

/**@brief Funkcja wywo�ywana, gdy zmieni si� rozmiar tekst do wy�wietlania.

Rozmiar znajduje si� w zmiennych klasy, w momencie wywo�ania ustawiony jest ju� nowy.

Funkcja domy�lnie jest pusta. Programista kontrolek z klas pochodnych mo�e zaimplementowa�
w tej funkcji dowoln� funkcjonalno��.

@note Aby funkcja zosta�a wywo�ana, zmienna @ref enable_onTextSizeChange musi by� ustawiona na true.
*/
void AbstractText::onTextSizeChange()
{}

/**@brief Funkcja wywo�ywana, gdy zmieni si� kolor tekstu do wy�wietlania.

Kolor znajduje si� w zmiennych klasy, w momencie wywo�ania ustawiony jest ju� nowy.

Funkcja domy�lnie jest pusta. Programista kontrolek z klas pochodnych mo�e zaimplementowa�
w tej funkcji dowoln� funkcjonalno��.

@note Aby funkcja zosta�a wywo�ana, zmienna @ref enable_onTextColorChange musi by� ustawiona na true.
*/
void AbstractText::onTextColorChange()
{}

/**@brief Funkcja wywo�ywana, gdy zmieni si� czcionka tekstu do wy�wietlania.

Czcionka znajduje si� w zmiennych klasy, w momencie wywo�ania ustawiona jest ju� nowa.

Funkcja domy�lnie jest pusta. Programista kontrolek z klas pochodnych mo�e zaimplementowa�
w tej funkcji dowoln� funkcjonalno��.


@note Aby funkcja zosta�a wywo�ana, zmienna @ref enable_onFontChange musi by� ustawiona na true.
*/
void AbstractText::onFontChange()
{}

/**@brief Funkcja wywo�ywana, gdy zmieni si� wyr�wnanie tekstu w zgl�dem kraw�dzi kotrolki.

Funkcja domy�lnie jest pusta. Programista kontrolek z klas pochodnych mo�e zaimplementowa�
w tej funkcji dowoln� funkcjonalno��.

@note Aby funkcja zosta�a wywo�ana, zmienna @ref enable_onAlignmentChange musi by� ustawiona na true.
*/
void AbstractText::onAlignmentChange()
{}

//========================================================================================//
//					Funkcje s�u��ce do ustawiania r�nych parametr�w klasy
//========================================================================================//


void AbstractText::setFont( Font* new_font )
{
	if ( !new_font )
		return;

	font = new_font;
	if ( enable_onFontChange )
		onFontChange();
}

void AbstractText::setFontSize( unsigned char new_size )
{
	if ( new_size == 0 )
		return;
	font_size = new_size;

	if ( enable_onTextSizeChange )
		onTextSizeChange();
}

void AbstractText::setAlignment( ALIGNMENT new_align )
{
	text_alignment = new_align;
	if ( enable_onAlignmentChange )
		onAlignmentChange();
}

void AbstractText::setTextColor( DirectX::XMFLOAT4 new_color )
{
	text_color = new_color;
	if ( enable_onTextColorChange )
		onTextColorChange();
}

void AbstractText::setText( std::wstring new_text )
{
	text = new_text;
	if ( enable_onTextChange )
		onTextChange();
}