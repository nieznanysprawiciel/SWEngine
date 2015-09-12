#pragma once
/**@file AbstractTextInput.h
@brief Plik zawiera deklaracj� klasy AbstractTextInput.*/

#include "AbstractText.h"

namespace XGUI
{
	/**@brief Klasa bazowa dla p�l tekstowych.

	Umo�liwia wpisywanie tekstu. Obs�uguje po�o�enie karetki oraz zaznaczanie tekstu.
	Tekst jest zaznaczony od po�o�enia karetki do indeksu wskazywanego przez selection.
	Nale�y pami�ta�, �e oba indeksy wskazuj� na miejsce przed znakiem.
	Je�eli zmienne s� r�wne, oznacza to, �e nic nie jest zaznaczone.*/
	class AbstractTextInput	:	public Control
	{
	private:
		AbstractText*	text;		///<Kontrolka do wy�wietlania tekstu.

		uint16			caret;		///<Po�o�enie karetki w tek�cie (indeks znaku). Karetka znajduje si� przed wskazywanym znakiem.
		uint16			selection;	///<Indeks ostatniego zaznaczonego elementu. Pierwszym zaznaczonym elementem jest znak wskazywany przez karetk�. caret == selection oznacza brak zaznaczenia.
		uint16			max_length;	///<Maksymalna d�ugo�� tekstu, jak� mo�e wpisa� uzytkownik.
	protected:
		int8			enable_onTextChanged : 1;
		int8			enable_onCaretChanged : 1;

	public:
		AbstractTextInput( Control* parent );
		~AbstractTextInput( );

		XGUIEventDelegate			eventTextChanged;
		XGUIEventDelegate			eventCaretChanged;

		virtual void onTextChanged();
		virtual void onCaretChanged();

		inline uint16 getMaxLength() const { return max_length; }
		std::wstring getSelectedTextString() const;
		inline std::wstring getTextString() const  { return text->getTextString(); }
		//const wchar_t* getText() const { return text->getTextStringRef().c_str(); }		///<Nie tworzy nowego stringa.

		void setMaxLength( uint16 new_length );
	};

}