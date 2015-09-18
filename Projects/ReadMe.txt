========================================================================
    WIN32 APPLICATION : SW_engine Project Overview
========================================================================


Aby skompilować projekt, trzeba mieć ściągnięte DirectX SDK
(http://www.microsoft.com/en-us/download/details.aspx?id=6812)

Przy instalacji najczęściej są problemy.
Jeżeli wystąpi błąd S1023 należy odinstalować Visual C++ 2010 Redistributable (zarówno x86 jak i x64).
Błąd jest podobno związany z Service Packiem 1, dokładniej można przeczytać w internecie co i jak.

Po dezinstalacji Visual C++ 2010 Redistributable, DirectX SDK daje się zainstalować poprawnie.
Potem należy ponownie zainstalować Visual C++ 2010 Redistributable, co nie powoduje już problemów,
a przynajmniej nic nie wiem.
(link: http://www.microsoft.com/en-us/download/details.aspx?displaylang=en&id=26999)


1. Struktura katalogowa

Pod makrem $(RootDir) znajduje się ścieżka do katalogu SWEngine (chyba, że ktoś inaczej nazwał katalog z projektem).

-	Projects
Katalog zawiera plik Solution oraz w podfolderach wszystkie pliki projektów oraz konfiguracji (.props).

-	GameRelease
Zawiera strukturę katalogową wynikowej aplikacji oraz modele testowe.

-	External
Zawiera biblioteki zewnetrzne. Nie wszystkie biblioteki tu trafiają. DirectX SDK na przykład
może znajdować się w dowolnym miejscu i ma zdefiniowaną zmienną środowiskową ze ścieżką.

-	EngineBuildDir
Tu trafiają wyniki kompilacji, czyli cały śmietnik.

-	Documentation
Zawiera dokumentację projektu generowaną przez doxygen (w folderze /html).

-	EngineLib
Tutaj trafiają skompilowane biblioteki statyczne.

-	EngineCode
W tym katalogu znajdują się wszystkie pliki z kodem źródłowym projektów.
Każdy projekt powinien dodać ścieżkę do tego katalogu w Additional Include Directories.

Zaleca się, żeby includując pliki podawać wszystki ścieżki względem tego katalogu.
Trzeba to robić szczególnie wtedy, gdy projekt kompiluje się do biblioteki statycznej.
W przeciwnym wypadku wszystkie projekty, które includują jakieś headery biblioteczne, mogą
się nie kompilować, ze względu na błędne ścieżki.

2. Konfiguracje projektu

!!! Zmian w konfiguracjach projektu raczej nie należy wprowadzać bezpośrednio dla konfiguracji
Debug i Release. Do tego służą pliki .props, które można zobaczyć otwierając Property Manager dla
projektów C++.

Ułatwia to zarządzanie ustawieniami, ponieważ pliki te odnoszą się do wszystkich konfiguracji projektu
na raz. Ponadto można je współdzielić w wielu projektach jednocześnie.

Bezpośrednio w konfiguracji można wprowadzać jedynie te zmiany, które odnoszą się konkretnie do danej konfiguracji
(np. do Debug x86).

3. Podstawowe pliki konfiguracyjne

Istnieją dwa pliki .props, których powinien używac każdy projekt:
-	DebugConfig lub ReleaseConfig

Te pliki definiują makra ConfigTypeString, które zawierają napis "Debug" lub "Release" w zależności
od konfiguracji. Następnie te makra są używane do tworzenia nazw ścieżek i plików.

-	PathsMacros
Zawiera podstawowe ścieżki silnikowe, do których odwołują się później kolejne pliki .props.

Każdy projekt ma dwa właściwe tylko dla siebie pliki konfiguracyjne
-	[NazwaProjektu]PathMacros
-	[NazwaProjektu]Configuration

Pierwszy plik zawiera definicje makr, zawierających podstawowe ścieżki i nazwy dla danego projektu.
Makra mają pewien schemat i każdy nowy projekt powinien się go mniej więcej trzymać.
Najlepszym sposobem jest przekopiowanie i edytowanie pliku .props w edytorze tekstowym zamiast w Visual Studio.

Dla projektów kompilujących się do biblioteki statycznej, plik ten zawiera również w sekcji Linker
komendy umożliwiające zlinkowanie tej biblioteki. Projekt, który chce zlinkować bibliotekę musi jedynie
dodać odpowiedni plik .props.

[NazwaProjektu]Configuration zawiera zasadnicze ustawienia projektu. W żadnym razie nie należy ustawień umieszczać
w [NazwaProjektu]PathMacros, ponieważ ustawienia są dziedziczone i mogą wpływać na inne projekty.

Większość projektów umieszcza tu w Additional Include Directories ścieżkę $(EngineSourceDir).

4. Kompilowanie wersji 64-bitowej

Wszystkie ustawienia projektu dla wersji 64-bitowej są poprawnie ustawione.
Jedyne co należy zrobić, to przestawić active solution platform z Win32 na x64.
Można to zrobić następująco:
-	Rozwijamy listę Solution Cofigurations po prawej od zielonego przycisku do włączania projektu i kompilacji.
-	Wybieramy Configuration Manager
-	Po prawej stronie na górze zmieniamy Active Solution Platform na x64
-	Zamykamy okno
Od tej pory silnik będzie kompilowany do wersji 64-bitowej.

Katalogiem wynikowym pliku .exe jest GameRelease/binaries_x64.
Wersja 64-bitowa uruchamia się w Visual Studio, ale poza nim nie, ponieważ
jest w złym katalogu. Instrukcja uruchamiania znajduje się w pliku 
GameRelease/binaries_x64/read_before_use.txt.

5. Kompilowanie do biblioteki statycznej

UWAGA!!!!! Nieaktualne. EngineCore zawsze jest kompilowany do biblioteki statycznej.
Plik .exe jest aktualnie kompilowany na podstawie projektu TestEngine.

###
Za kompilowanie do wersji biblioteki statycznej odpowiadają konfiguracje static_lib_release
i static_lib_debug. Istnieją też odpowiedniki dla wersji 64-bitowej.

W katalogu GameCode\EntryPoint powstał projekt będący szablonem dla aplikacji korzystającej
z silnika i linkującej biliotekę odpowiednią do konfiguracji. Tworząc grę należy używać tego szablonu.
Nie należy modyfikować pliku zawierającego funkcję WinMain.

Aby stworzyć grę należy zaimplementować własne klasy dziedzicząc po GamePlay. Szablon pierwszej klasy
o nazwie EntryPoint jest zaimplementowany w projekcie i podawany przy starcie silnika.
Należy go zmodyfikować do swoich potrzeb.
###


6. Dokumentacja

Dokumentacja jest dostępna online pod linkiem:
http://nieznanysprawiciel.github.io/SWEngine/

Istnieje specjalny branch gh-pages, który zawiera tylko i wyłącznie dokumentację
w postaci html-ów.

UWAGA!!!! Aby móc generować dokumentację do tego brancha, skolonowałem całe repo tak,
żeby nowy folder .git znajdował się w katalogu Documentation/html/.
Dzięki temu można będąc w branchu gh-pages generować dokumentację z plików projektowych.

Ale przez taką strukturę należy też bardzo uważać co się gdzie commituje, pushuje i pulluje.
Branch gh-pages nie ma prawa mieć nic oprócz dokumentacji, a pozstałe branche nie mają prawa
mieć żadnego wygenerowanego przez doxygen pliku.

Najlepiej jest WOGÓLE nic nie commitować, pushować ani pullować z tego brancha.


7. Generowanie dokumentacji

Dokumentacja jest generowana automatycznie na podstawie kodu źródłowego w komentarzach.
Poza tym w katalogu SWEngine/EngineCode/documentation znajdują się pliki dodatkowe z opisem tematycznym
niektórych zagadnień.
Jak ktoś będzie kiedyś implementował jakieś funkcje, to musi na bieżąco komentować
swoje osiągnięcia.

Aby wygenerować dokumentację:

Należy ściągnąć program doxywizard ze strony: http://www.stack.nl/~dimitri/doxygen/download.html

Plik konfiguracyjny silnika znajduje się w katalogu:
SWEngine/EngineCode/documentation i nazywa się Doxyfile

Trzeba go wczytać doxywizardem, przejść do zakładki Run i kliknąć Run doxygen.

Dokumentacja zostanie wygenerowana w katalogu:
SWEngine/Documentation/html

Aby obejrzeć ją w przeglądarce offline można:
-	kliknąć na przycisk Show HTML output w programie doxywizard
-	poszukać pliu index.html w katalogu z dokumentacją



