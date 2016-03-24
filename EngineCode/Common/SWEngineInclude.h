/**
@file SWEngineInclude.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

Ten plik s�u�y do includowania najwa�niejszych funkcjonalno�ci silnika w aplikacji ko�cowej
Ponadto zawiera dokumentacj� uruchamiania, struktury projektu i innych element�w niezale�nych od kodu.
*/

#include "EngineCore/GamePlay/IGamePlay.h"
#include "EngineCore/MainEngine/EngineInterface.h"
#include "EngineCore/Actors/ActorObjects.h"
#include "EngineCore/EventsManager/Event.h"
#include "EngineCore/GamePlay/FableEngine.h"

#include <DirectXMath.h>

#include "TypeInfo.h"
#include "RTTREnable.h"
#include "RTTRCast.h"

/**@defgroup ModulesStructure Modu�y silnika
@brief Modu�y silnika*/


/**@page LaunchInstruction Instrukcja uruchamiania

##Niezb�dne biblioteki i kompilacja

Aby skompilowa� projekt, trzeba mie� �ci�gni�te DirectX SDK
(http://www.microsoft.com/en-us/download/details.aspx?id=6812)

Przy instalacji najcz�ciej s� problemy.
Je�eli wyst�pi b��d S1023 nale�y odinstalowa� Visual C++ 2010 Redistributable (zar�wno x86 jak i x64).
B��d jest podobno zwi�zany z Service Packiem 1, dok�adniej mo�na przeczyta� w internecie co i jak.

Po dezinstalacji Visual C++ 2010 Redistributable, DirectX SDK daje si� zainstalowa� poprawnie.
Potem nale�y ponownie zainstalowa� Visual C++ 2010 Redistributable, co nie powoduje ju� problem�w,
a przynajmniej nic nie wiem.
(link: http://www.microsoft.com/en-us/download/details.aspx?displaylang=en&id=26999)

Slnik u�ywa r�wnie� FBX SDK do wczytywania modeli w tym formacie. Biblioteka znajduje si�
w folderze /External.

Kompilowanie wersji 32-bitowej poprzez Visual Studio odbywa si� normalnie BUILD->Build Solution.

##Kompilowanie wersji 64-bitowej

Wszystkie ustawienia projektu dla wersji 64-bitowej s� poprawnie ustawione.
Jedyne co nale�y zrobi�, to przestawi� active solution platform z Win32 na x64.
Mo�na to zrobi� nast�puj�co:
-	Rozwijamy list� Solution Cofigurations po prawej od zielonego przycisku do w��czania projektu i kompilacji.
-	Wybieramy Configuration Manager
-	Po prawej stronie na g�rze zmieniamy Active Solution Platform na x64
-	Zamykamy okno
Od tej pory silnik b�dzie kompilowany do wersji 64-bitowej.

Katalogiem wynikowym pliku .exe jest GameRelease (dost�pny pod makrem $(GameReleaseDir)).
Przy ka�dej kompilacji do tego katalogu kopiowana jest biblioteka libfbxsdk.dll odpowiadaj�ca
aktualnie u�ywanej wersji aplikacji. Z tego wzgl�du klikaj�c myszk� plik .exe mo�na uruchomi� jedynie
ostatnio kompilowan� wersj� aplikacji. Aby przetestowa� inn� wersj� trzeba j� uruchomi� z poziomu
Visual Studio. 

##Uruchamianie

Uruchamialne projekty:
-	TestEngine
-	EditorApp
-	SerializationJSONTest
-	SerializationXMLTest

Wszystkie projekty opr�cz EditorApp powinny mie� ustawiony katalog roboczy na GameRelease.
Properties -> Debugging -> Working Directory
Uwaga!! Trzeba zmieni� katalog dla wszystkich konfiguracji projektu (Release/Debug; Win32/x64),
�eby potem nie by� zaskoczonym, �e co� nie dzia�a.

EditorApp powinien miec ustawiony katalog roboczy na EditorRelease (makro $(EditorReleaseDir)).


##Dokumentacja

Dokumentacja jest dost�pna online pod linkiem:
http://nieznanysprawiciel.github.io/SWEngine/

Istnieje specjalny branch gh-pages, kt�ry zawiera tylko i wy��cznie dokumentacj�
w postaci html-�w.

@attention Aby m�c generowa� dokumentacj� do tego brancha, sklonowa�em ca�e repo tak,
�eby nowy folder .git znajdowa� si� w katalogu Documentation/html/.
Dzi�ki temu b�d�c w branchu gh-pages mo�na generowa� dokumentacj� z plik�w �r�d�owych projektu znajduj�cych si� kilka katalog�w wy�ej.

Przez tak� struktur� nale�y te� bardzo uwa�a� co si� gdzie commituje, pushuje i pulluje.
Branch gh-pages nie ma prawa mie� nic opr�cz dokumentacji, a pozosta�e branche nie maj� prawa
mie� �adnego wygenerowanego przez doxygen pliku.

Najlepiej jest WOG�LE nic nie commitowa�, pushowa� ani pullowa� z brancha gh-pages.


##Generowanie dokumentacji

Dokumentacja jest generowana automatycznie na podstawie kodu �r�d�owego w komentarzach.
Poza tym w katalogu SWEngine/EngineCode/documentation znajduj� si� pliki dodatkowe z opisem tematycznym
niekt�rych zagadnie�.
Jak kto� b�dzie kiedy� implementowa� jakie� funkcje, to musi na bie��co komentowa�
swoje osi�gni�cia.

Aby wygenerowa� dokumentacj�:

- Nale�y �ci�gn�� program doxywizard ze strony: http://www.stack.nl/~dimitri/doxygen/download.html
- Ustawi� working directory, �eby odpowiada�o temu, gdzie trzymasz projekt
- Wczytac plik konfiguracyjny doxywizarda
- Przej�� do zak�adki Run i klikn�� Run doxygen

Plik konfiguracyjny silnika znajduje si� w katalogu:
SWEngine/EngineCode/documentation i nazywa si� Doxyfile

Dokumentacja zostanie wygenerowana w katalogu:
SWEngine/Documentation/html

Aby obejrze� j� w przegl�darce offline mo�na:
-	klikn�� na przycisk Show HTML output w programie doxywizard
-	poszuka� pliu index.html w katalogu z dokumentacj�
*/


/**@page ProjectSettings Konfigurowanie projekt�w

##Konfiguracje projektu

@note Zmian w konfiguracjach projektu raczej nie nale�y wprowadza� bezpo�rednio dla konfiguracji
Debug i Release. Do tego s�u�� pliki .props, kt�re mo�na zobaczy� otwieraj�c Property Manager dla
projekt�w C++.

U�atwia to zarz�dzanie ustawieniami, poniewa� pliki te odnosz� si� do wszystkich konfiguracji projektu
na raz. Ponadto mo�na je wsp�dzieli� w wielu projektach jednocze�nie.

Bezpo�rednio w konfiguracji mo�na wprowadza� jedynie te zmiany, kt�re odnosz� si� konkretnie do danej konfiguracji
(np. do Debug x86).

##Podstawowe pliki konfiguracyjne

Ka�dy projekt powinien u�ywa� pliku:
-	PathsMacros

Zawiera podstawowe �cie�ki silnikowe, do kt�rych odwo�uj� si� p�niej kolejne pliki .props.

Ka�dy projekt ma dwa w�a�ciwe tylko dla siebie pliki konfiguracyjne
-	[NazwaProjektu]PathMacros
-	[NazwaProjektu]Configuration

Pierwszy plik zawiera definicje makr, zawieraj�cych podstawowe �cie�ki i nazwy dla danego projektu.
Makra maj� pewien schemat i ka�dy nowy projekt powinien si� go mniej wi�cej trzyma�.
Najlepszym sposobem jest przekopiowanie i edytowanie pliku .props w edytorze tekstowym zamiast w Visual Studio.

Dla projekt�w kompiluj�cych si� do biblioteki statycznej, plik ten zawiera r�wnie� w sekcji Linker
komendy umo�liwiaj�ce zlinkowanie tej biblioteki. Projekt, kt�ry chce zlinkowa� bibliotek� musi jedynie
doda� odpowiedni plik .props.

[NazwaProjektu]Configuration zawiera zasadnicze ustawienia projektu. W �adnym razie nie nale�y ustawie� umieszcza�
w [NazwaProjektu]PathMacros, poniewa� ustawienia s� dziedziczone i mog� wp�ywa� na inne projekty.

Wi�kszo�� projekt�w umieszcza tu w Additional Include Directories �cie�k� $( EngineSourceDir ).

##Makra definiowane przez silnik:

W pliku PathsMacros.props zdefiniowane s� makra ze �cie�kami wewn�trz katalog�w silnika:
-	EngineShortcut
-	EngineVersion
-	EngineVersionString
-	RootDir
-	ExternalDir
-	GameReleaseDir
-	EngineLibDir
-	EngineBuildDir
-	EngineSourceDir
-	EngineLibName
-	CommonDir
-	BuildFolder
-	EditorReleaseDir
-	InstallerReleaseDir
-	ConfigTypeString
-	ConfigTypeSmallString
-	DebugSuffix

*/

/**@page FolderStructure Struktura katalogowa projektu

Pod makrem $( RootDir ) znajduje si� �cie�ka do katalogu SWEngine (chyba, �e kto� inaczej nazwa� katalog z projektem).

-	/Projects
Katalog zawiera plik Solution oraz w podfolderach wszystkie pliki projekt�w oraz konfiguracji (.props).

-	/GameRelease
Zawiera struktur� katalogow� wynikowej aplikacji oraz modele testowe.

-	/External
Zawiera biblioteki zewnetrzne. Nie wszystkie biblioteki tu trafiaj�. DirectX SDK na przyk�ad
mo�e znajdowa� si� w dowolnym miejscu i ma zdefiniowan� zmienn� �rodowiskow� ze �cie�k�.

-	/EngineBuildDir
Tu trafiaj� wyniki kompilacji, czyli ca�y �mietnik.

-	/Documentation
Zawiera dokumentacj� projektu generowan� przez doxygen (w folderze /html).

-	/EngineLib
Tutaj trafiaj� skompilowane biblioteki statyczne.

-	/EngineCode
W tym katalogu znajduj� si� wszystkie pliki z kodem �r�d�owym projekt�w.
Ka�dy projekt powinien doda� �cie�k� do tego katalogu w Additional Include Directories.

-	/EditorRelease
W tym katalogu znajduj� si� pliki edytora.

Zaleca si�, �eby includuj�c pliki podawa� wszystki �cie�ki wzgl�dem katalogu EngineCode.
Trzeba to robi� szczeg�lnie wtedy, gdy projekt kompiluje si� do biblioteki statycznej.
W przeciwnym wypadku wszystkie projekty, kt�re includuj� jakie� headery biblioteczne, mog�
si� nie kompilowa�, ze wzgl�du na b��dne �cie�ki.

*/
