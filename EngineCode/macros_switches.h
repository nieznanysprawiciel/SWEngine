#pragma once

/*Ten plik zawiera makrodefinicje b�d�ce prze��cznikami mi�dzy r�nymi opcjami kompilacji.
Wi�kszo�� z tych makrodefinicji pozwala na wyb�r jednej z opcji tylko tymczasowo,
w docelowej wersji silnika trzeba b�dzie si� zdecydowa� na kt�r�� z nich.*/

#define _INTERPOLATE_POSITIONS		//w��cza funkcj� interpoluj�ca po�o�enia obiekt�w
//#define _QUATERNION_SPEED			//predko�� obrotowa jest wyra�ona kwaternionem
/*Je�eli to makro nie jest zdefiniowane, to pr�dko�� obrotowa jest wyra�ona jako wektor
4-elementowy, w kt�rym pierwsze 3 sk�adowe opisuj� wektor obrotu, a sk�adowa w k�t obrotu*/


//#define __TEST		//w��cza funkcje testuj�ce
#define FIXED_FRAMES_COUNT		//ustawia w funkcji Engine::render_frame() sta�� liczb� klatek na sekund�

#define _SCALEABLE_OBJECTS		//umo�liwia ustawienie skalowania dla meshy
#define ENGINE_MAX_TEXTURES		8	//maksymalna liczba tekstur w silniku
#define ENGINE_MAX_LIGHTS		2	//maksymalna liczba �wiate� w silniku