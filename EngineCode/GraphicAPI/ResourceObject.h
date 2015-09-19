#pragma once

/**@file ResourceObject.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Plik zawiera deklaracjê i definicjê klasy ResourceObject s³u¿¹c¹
do zliczania odwo³añ do obiektu.*/


/**@brief Klasa u³atwiaj¹ca zarz¹dzanie odwo³aniami do assetów.
@ingroup GraphicAPI
*
*Obiekty assetów (np. MaterialObject, TextureObject, VertexShaderObject, PixelShaderObject itp.) wymagaj¹ jakiegoœ systemu zapewniaj¹cego wspó³dzielenie miêdzy innymi obiektami.
*
*Do ka¿dego pojedynczego obiektu mog¹ istnieæ wilokrotne odwo³ania w klasie Model3DFromFile,
*a tak¿e w obiektach dziedzicz¹cych po @ref DynamicMeshObject.
*Z tego wzglêdu istniej¹ zmienne file_references i object_references.
*Pierwsza okreœla, ile razy wystêpuj¹ odwo³ania do obiektu w klasie Model3DFromFile.
*(uwaga: nie zliczamy, ile klas siê odwo³uje. Je¿eli w klasie pojawiaj¹ siê 3 odwo³ania, to licz¹ siê jako 3. Taka konwencja
*u³atwia zliczanie i zwalnianie pamiêci)
*Druga zmienna okreœla, ile wystepuje odwo³añ bezpoœrednich przez obiekty, które bêd¹ nastêpnie wyœwietlane. Oznacza to, ¿e przypisuj¹c
*jakiemus obiektowi plik z modelem, musimy zinkrementowaæ other_references o tyle, ile by³o odwo³añ w tym pliku.
*¯aden obiekt nie powinien byæ kasowany, dopóki istniej¹ do niego odwo³ania.

Zmienna unique_id jest na pocz¹tku ustawiana na 0. Jej faktyczne ustawienie odbywa robi klasa ResourceContainer.
Jest to wymagane do u³atwienia obs³ugi wielow¹tkowoœci. Inaczej mog³yby siê pokrywaæ identyfikatory.
**/

class ResourceObject
{
private:
	unsigned int	file_references;	///< Liczba plików, które sie odwo³uj¹
	unsigned int	object_references;	///< Liczba modeli, które siê odwo³uj¹
	unsigned int	unique_id;			///< Unikalny identyfikator zasobu

protected:
	virtual ~ResourceObject() = default;		///<Nie ka¿dy mo¿e skasowaæ obiekt

public:
	/**
	Ustawia zerow¹ liczbê odwo³añ.*/
	ResourceObject( int id )
	{
		file_references = 0;
		object_references = 0;
		unique_id = id;
	}


	inline void set_id( unsigned int id ) { unique_id = id; }	///<Ustawia identyfikator obiektu

	//sprawdza czy mo¿na zwolniæ zmienn¹
	inline bool can_delete( unsigned int& file_ref, unsigned int& other_ref );
	inline bool can_delete();

	/*Funkcje s³u¿¹ce do zarz¹dzania odwo³aniami.
	*Nale¿y pilnowaæ, aby wszystkie funkcje, które modyfikuj¹ jakiekolwiek przypisania obiektów
	*do tekstur, materia³ów i meshy, modyfikowa³y równie¿ iloœæ odwo³añ.
	*U¿ytkownik silnika powinien mieæ udostêpnion¹ wartstwê poœredniczac¹, ¿eby nie musia³
	*pamiêtaæ o odwo³aniach.*/
	inline void add_file_reference( ) { ++file_references; }		///< Dodaje odwo³anie plikowe do assetu
	inline void add_object_reference( ) { ++object_references; }	///< Dodaje odwo³anie bezpoœrednie obiektu do assetu
	inline void delete_file_reference( ) { --file_references; }		///< Kasuje odwo³anie plikowe do assetu
	inline void delete_object_reference( ) { --object_references; }	///< Kasuje odwo³anie bezpoœrednie obiektu do assetu

	inline unsigned int get_id() { return unique_id; }				///< Zwraca identyfikator nadany assetowi
};




//----------------------------------------------------------------------------------------------//
//									ResourceObject											//
//----------------------------------------------------------------------------------------------//

//==============================================================================================//



/**@brief Funkcja informuje czy obiekt s¹ obiektu, które odwo³uj¹ siê do assetu.

@param[out] file_ref W zmiennej zostanie umieszczona liczba referencji plikowych.
@param[out] other_ref W zmiennej zostanie umieszczona liczba referencji bezpoœrednich od obiektów.
@return Zwraca wartoœæ logiczn¹ mówi¹c¹ czy asset nadaje siê do usuniêcia.
*/
inline bool ResourceObject::can_delete(unsigned int& file_ref, unsigned int& other_ref)
{
	file_ref = file_references;
	other_ref = object_references;

	if (file_references == 0 && object_references == 0)
		return true;
	return false;
}

/**@brief Funkcja informuje czy obiekt s¹ obiektu, które odwo³uj¹ siê do assetu.

@return Zwraca wartoœæ logiczn¹ mówi¹c¹ czy asset nadaje siê do usuniêcia.
*/
inline bool ResourceObject::can_delete()
{
	if (file_references == 0 && object_references == 0)
		return true;
	return false;
}
