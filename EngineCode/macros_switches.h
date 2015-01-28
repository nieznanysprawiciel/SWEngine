#pragma once

/*Ten plik zawiera makrodefinicje bêd¹ce prze³¹cznikami miêdzy ró¿nymi opcjami kompilacji.
Wiêkszoœæ z tych makrodefinicji pozwala na wybór jednej z opcji tylko tymczasowo,
w docelowej wersji silnika trzeba bêdzie siê zdecydowaæ na któr¹œ z nich.*/

#define _INTERPOLATE_POSITIONS		//w³¹cza funkcjê interpoluj¹ca po³o¿enia obiektów
//#define _QUATERNION_SPEED			//predkoœæ obrotowa jest wyra¿ona kwaternionem
/*Je¿eli to makro nie jest zdefiniowane, to prêdkoœæ obrotowa jest wyra¿ona jako wektor
4-elementowy, w którym pierwsze 3 sk³adowe opisuj¹ wektor obrotu, a sk³adowa w k¹t obrotu*/


//#define __TEST		//w³¹cza funkcje testuj¹ce
#define FIXED_FRAMES_COUNT		//ustawia w funkcji Engine::render_frame() sta³¹ liczbê klatek na sekundê

#define _SCALEABLE_OBJECTS		//umo¿liwia ustawienie skalowania dla meshy
#define ENGINE_MAX_TEXTURES		8	//maksymalna liczba tekstur w silniku
#define ENGINE_MAX_LIGHTS		2	//maksymalna liczba œwiate³ w silniku