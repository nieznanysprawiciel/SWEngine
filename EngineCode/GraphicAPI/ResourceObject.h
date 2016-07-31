#pragma once
/**@file ResourceObject.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Plik zawiera deklaracjê i definicjê klasy ResourceObject s³u¿¹c¹
do zliczania odwo³añ do obiektu.*/

#include "Common/EngineObject.h"


/**@brief Klasa u³atwiaj¹ca zarz¹dzanie odwo³aniami do assetów.
@ingroup GraphicAPI

Obiekty assetów (np. MaterialObject, TextureObject, VertexShaderObject, PixelShaderObject itp.) wymagaj¹ jakiegoœ systemu zapewniaj¹cego wspó³dzielenie miêdzy innymi obiektami.

Do ka¿dego pojedynczego obiektu mog¹ istnieæ wilokrotne odwo³ania w klasie Model3DFromFile,
a tak¿e w obiektach dziedzicz¹cych po @ref DynamicMeshActor oraz w modu³ach silnika.
Z tego wzglêdu istniej¹ zmienne m_assetReferences i m_objectReferences.
Pierwsza okreœla, ile razy wystêpuj¹ odwo³ania do obiektu w klasie Model3DFromFile.
(uwaga: nie zliczamy, ile klas siê odwo³uje. Je¿eli w klasie pojawiaj¹ siê 3 odwo³ania, to licz¹ siê jako 3. Taka konwencja
u³atwia zliczanie i zwalnianie pamiêci)
Druga zmienna okreœla, ile wystepuje odwo³añ bezpoœrednich przez obiekty, które bêd¹ nastêpnie wyœwietlane. Oznacza to, ¿e przypisuj¹c
jakiemus obiektowi plik z modelem, musimy zinkrementowaæ m_objectReferences o tyle, ile by³o odwo³añ w tym pliku.
¯aden obiekt nie powinien byæ kasowany, dopóki istniej¹ do niego odwo³ania.

Zmienna m_uniqueId jest na pocz¹tku ustawiana na 0. Jej faktyczne ustawienie odbywa robi klasa ResourceContainer.
Jest to wymagane do u³atwienia obs³ugi wielow¹tkowoœci. Inaczej mog³yby siê pokrywaæ identyfikatory.

@todo Zliczanie referencji w ResourceObject nie nadaje siê do wielow¹tkowoœci. Poprawiæ w odpowiednim momencie.
*/

class ResourceObject	: public EngineObject
{
	RTTR_ENABLE( EngineObject )
private:
	unsigned int	m_assetReferences;	///< Liczba assetów, które sie odwo³uj¹.
	unsigned int	m_objectReferences;	///< Liczba obiektów aktorów, które siê odwo³uj¹.
	unsigned int	m_uniqueId;			///< Unikalny identyfikator zasobu.

protected:
	virtual ~ResourceObject() = default;		///<Nie ka¿dy mo¿e skasowaæ obiekt

public:
	/**
	Ustawia zerow¹ liczbê odwo³añ.*/
	ResourceObject( int id )
	{
		m_assetReferences = 0;
		m_objectReferences = 0;
		m_uniqueId = id;
	}


	inline void SetID( unsigned int id ) { m_uniqueId = id; }	///<Ustawia identyfikator obiektu

	///sprawdza czy mo¿na zwolniæ zmienn¹
	inline bool CanDelete( unsigned int& assetRef, unsigned int& objectRef );
	inline bool CanDelete();

	/**Funkcje s³u¿¹ce do zarz¹dzania odwo³aniami.
	Nale¿y pilnowaæ, aby wszystkie funkcje, które modyfikuj¹ jakiekolwiek przypisania obiektów
	do tekstur, materia³ów i meshy, modyfikowa³y równie¿ iloœæ odwo³añ.
	U¿ytkownik silnika powinien mieæ udostêpnion¹ wartstwê poœredniczac¹, ¿eby nie musia³
	pamiêtaæ o odwo³aniach.*/
	inline void AddAssetReference()		{ ++m_assetReferences; }	///< Dodaje odwo³anie plikowe do assetu
	inline void AddObjectReference()	{ ++m_objectReferences; }	///< Dodaje odwo³anie bezpoœrednie obiektu do assetu
	inline void DeleteAssetReference()	{ --m_assetReferences; }	///< Kasuje odwo³anie plikowe do assetu
	inline void DeleteObjectReference()	{ --m_objectReferences; }	///< Kasuje odwo³anie bezpoœrednie obiektu do assetu

	inline unsigned int GetID()			{ return m_uniqueId; }		///< Zwraca identyfikator nadany assetowi
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
inline bool ResourceObject::CanDelete( unsigned int& assetRef, unsigned int& objectRef )
{
	assetRef = m_assetReferences;
	objectRef = m_objectReferences;

	if( m_assetReferences == 0 && m_objectReferences == 0 )
		return true;
	return false;
}

/**@brief Funkcja informuje czy obiekt s¹ obiektu, które odwo³uj¹ siê do assetu.

@return Zwraca wartoœæ logiczn¹ mówi¹c¹ czy asset nadaje siê do usuniêcia.
*/
inline bool ResourceObject::CanDelete()
{
	if( m_assetReferences == 0 && m_objectReferences == 0 )
		return true;
	return false;
}
