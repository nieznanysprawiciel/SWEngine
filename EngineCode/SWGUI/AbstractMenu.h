#pragma once
/**
@file AbstractMenu.h
@brief Plik zawiera deklaracj� klasy AbstractMenu.
*/

#include "XGUI.h"
#include <vector>

namespace XGUI
{
	class AbstractItem;

	/**@brief Klasa bazowa dla klas wy�wietlaj�cych menu.
	
	Klasa mo�e wy�wietla� zar�wno menu takie jak jest na belce okna, jak r�wnie�
	menu kontekstowe i inne temu podobne.
	
	@todo Wszystko jeszcze do wymy�lenia.*/
	class AbstractMenu : public Control
	{
	private:
		std::vector<AbstractItem*>		child_items;
		/**@brief Rozmiar obszaru w kt�rym narysuje si� kontrolka.

		Ze wzgl�du na to, �e w menu dzieci kontrolki nie znajduj� si� wewn�trz obszaru rodzica,
		to istniej� dwa wymiary charakteryzuj�ce kontrolk�. Zmienna dimmension podaje obszar
		obcinania przy renderowaniu (cz�sto jest to ca�y ekran). Zmienna visible_dimmension
		opisuje rozmiar faktyczny, jaki kontrolka b�dzie zajmowa�.*/
		DirectX::XMFLOAT2				visible_dimmension;
	protected:
		int8		selected : 1;		///<Informacja czy podmenu (dziecko naszej klasy) zosta�o otwarte/wybrane.
	public:
		AbstractMenu( Control* parent );
		~AbstractMenu();

		inline bool isSelected() const { return selected && 1; }
		inline DirectX::XMFLOAT2 getVisibleDimmension( ) const { return visible_dimmension; }
	};

}
