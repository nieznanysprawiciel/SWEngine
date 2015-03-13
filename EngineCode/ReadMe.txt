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


1. Uruchamianie projektu 
#
W najnowszej wersji poniższe kroki nie są już potrzebne, ponieważ
udało mi się znaleźć zmienną środowiskową opisującą ścieżkę do folderu z
DirectXem.
Polecam jednak przeczytać notkę na końcu odnośnie modyfikowania opcji projektu!


Konfigurowanie projektu Visual Studio:
Większość ścieżek jest ustalona względnie i nie należy nic zmieniać.
Jedynie DirectX SDK trzeba ustawić ręcznie. (Jeżeli ktoś zna sposób, żeby zdefiniować makrodefinicję
na taką ścieżkę do folderu, a potem ją zastosować w opcjach projektu, to byłoby to bardzo przydatne.)

1.	Ustawienie ścieżek do katalogów include
	-	Z menu wybierz PROJECT -> PROPERTIES.
	-	W oknie wybierz Configuration Properties -> C/C++ -> General.
	-	WAŻNE !!!! Upewnij się, że w polu wyboru Configuration na górze okna,
		jest wybrana wartość All Configurations
	-	Rozwiń pole Additional Include Directories.
	-	Zmien ścieżkę Microsoft DirectX SDK %June 2010%29 na swoją własną.
2.	Ustawienie ścieżek dla linkera (w konfiguracji Release)
	-	Z menu wybierz PROJECT -> PROPERTIES.
	-	W oknie wybierz Configuration Properties -> Linker -> General.
	-	WAŻNE !!!! Upewnij się, że w polu wyboru Configuration na górze okna,
		jest wybrana wartość Release.
	-	Rozwiń pole Additional Library Directories.
	-	Zmien ścieżkę Microsoft DirectX SDK %June 2010%29 na swoją własną.
3.	Ustawienie ścieżek dla linkera (w konfiguracji Debug)
	-	Zrób to samo co powyżej, mając w polu Configuration wybraną opcję Debug.
#
	
	
	
!!! Przy wprowadzaniu jakichkolwiek zmian w konfiguracji projektu
trzeba zawsze patrzeć, czy ma się wybraną opcję All Configurations czy Release albo Debug.
Tak naprawdę jedynie przy linkowaniu biliotek potrzebujemy innych wartości tych konfiguracjach,
bo linkujemy biblioteki debugowe albo zwykłe.
We wszystkich innych przypadkach powinniśmy używać All Configurations. Jeżeli tego nie zrobimy,
to wprowadzanie zmian i kompiowanie opcji jest potem katorgą, a nie da się tego zrobić automatycznie.

Wprowadzenie zmian w ścieżkach jednej z konfiguracji, w momencie gdy wpisano coś w All Configurations
powoduje skasowanie danych z All Configurations.


2. Kompilowanie wersji 64-bitowej

Wszystkie ustawienia projektu dla wersji 64-bitowej są poprawnie ustawione.
Jedyne co należy zrobić, to przestawić active solution platform z Win32 na x64.
Można to zrobić następująco:
-	Rozwijamy listę Solution Cofigurations po prawej od zielonego przycisku do włączania projektu i kompilacji.
-	Wybieramy Configuration Manager
-	Po prawej stronie na górze zmieniamy Active Solution Platform na x64
-	Zamykamy okno
Od tej pory silnik będzie kompilowany do wersji 64-bitowej.

Katalogiem wynikowym pliku .exe jest GameRelease\binaries_x64.
Wersja 64-bitowa uruchamia się w Visual Studio, ale poza nim nie, ponieważ
jest w złym katalogu. Instrukcja uruchamiania znajduje się w pliku 
GameRelease\binaries_x64\read_before_use.txt.

3. Kompilowanie do biblioteki statycznej

Za kompilowanie do wersji biblioteki statycznej odpowiadają konfiguracje static_lib_release
i static_lib_debug. Istnieją też odpowiedniki dla wersji 64-bitowej.

W katalogu GameCode\EntryPoint powstał projekt będący szablonem dla aplikacji korzystającej
z silnika i linkującej biliotekę odpowiednią do konfiguracji. Tworząc grę należy używać tego szablonu.
Nie należy modyfikować pliku zawierającego funkcję WinMain.

Aby stworzyć grę należy zaimplementować własne klasy dziedzicząc po GamePlay. Szablon pierwszej klasy
o nazwie EntryPoint jest zaimplementowany w projekcie i podawany przy starcie silnika.
Należy go zmodyfikować do swoich potrzeb.

4. Generowanie dokumentacji

Należy ściągnąć program doxywizard ze strony: http://www.stack.nl/~dimitri/doxygen/download.html

Plik konfiguracyjny silnika znajduje się w katalogu:
SW_engine\EngineCode\documentation i nazywa się Doxyfile

Trzeba go wczytać doxywizardem, przejść do zakładki Run i kliknąć Run doxygen.

Dokumentacja zostanie wygenerowana w katalogu:
SW_engine\Documentation\SWEngine_documentation\html

Aby obejrzeć ją w przeglądarce offline można:
-	kliknąć na przycisk Show HTML output w programie doxywizard
-	poszukać pliu index.html w katalogu z dokumentacją

Dokumentacja jest generowana na podstawie kodu źródłowego.
Jak ktoś będzie kiedyś implementował jakieś funkcje, to musi na bieżąco komentować
swoje osiągnięcia.




AppWizard has created this SW_engine application for you.

This file contains a summary of what you will find in each of the files that
make up your StarWars_engine application.


SW_engine.vcxproj
    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

SW_engine.vcxproj.filters
    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the association between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).

SW_engine.cpp
    This is the main application source file.

/////////////////////////////////////////////////////////////////////////////
AppWizard has created the following resources:

SW_engine.rc
    This is a listing of all of the Microsoft Windows resources that the
    program uses.  It includes the icons, bitmaps, and cursors that are stored
    in the RES subdirectory.  This file can be directly edited in Microsoft
    Visual C++.

Resource.h
    This is the standard header file, which defines new resource IDs.
    Microsoft Visual C++ reads and updates this file.

SW_engine.ico
    This is an icon file, which is used as the application's icon (32x32).
    This icon is included by the main resource file StarWars_engine.rc.

small.ico
    This is an icon file, which contains a smaller version (16x16)
    of the application's icon. This icon is included by the main resource
    file StarWars_engine.rc.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named SW_engine.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////
