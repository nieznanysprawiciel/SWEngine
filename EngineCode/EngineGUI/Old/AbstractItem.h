#pragma once
/**
@file AbstractItem.h
@brief Plik zawiera deklaracj� klasy AbstractItem.
*/
#include "XGUI.h"
#include <vector>

namespace XGUI
{
	class AbstractMenu;

	/**@brief Klasa bazowa dla obiekt�w wybieranych w menu.

	@todo Wszystko jeszcze do wymy�lenia.
	*/
	class AbstractItem : public Control
	{
	private:
		std::vector<AbstractMenu*>		child_items;
		/**@brief Rozmiar obszaru w kt�rym narysuje si� kontrolka.
		
		Ze wzgl�du na to, �e w menu dzieci kontrolki nie znajduj� si� wewn�trz obszaru rodzica,
		to istniej� dwa wymiary charakteryzuj�ce kontrolk�. Zmienna dimmension podaje obszar
		obcinania przy renderowaniu (cz�sto jest to ca�y ekran). Zmienna visible_dimmension
		opisuje rozmiar faktyczny, jaki kontrolka b�dzie zajmowa�.*/
		DirectX::XMFLOAT2				visible_dimmension;
	protected:
		int8		selected : 1;		///<Informacja czy podmenu (dziecko naszej klasy) zosta�o otwarte/wybrane.
		int8		pushbutton : 1;		///<Przycisk mo�e wymaga� wci�ni�cia lub wystarczy najecha� mysz�.
	public:
		AbstractItem( Control* parent );
		~AbstractItem();

		inline bool isSelected( ) const { return selected && 1; }
		inline bool isPushButton() const { return pushbutton && 1; }
		inline DirectX::XMFLOAT2 getVisibleDimmension() const { return visible_dimmension; }
	};

}
