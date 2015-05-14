#include "XGUI.h"


using namespace XGUI;

/**@brief Funkcja rysuj�ca zewn�trzna ustawiaj�ca prostok�t obcinania i inne parametry rysowania.

Wykonywane s� nast�puj�ce czynno�ci:
- Sprawdzane jest czy kontrolka jest widoczna. Nie jest renderowana, je�eli visible jest ustawione na false.
- Obszar obcinania podany w parametrze jest ustawiany w zmiennych DirectX.
- Wywo�ywana jest funkcja onDraw, kt�ra powinna byc zaimplementowana w klasie pochodnej.

@param clipping_rect Czworok�t, wewn�trz kt�rego ma zosta� narysowana kontrolka.
*/
void Control::draw_clipped( const Rect& clipping_rect )
{


}

