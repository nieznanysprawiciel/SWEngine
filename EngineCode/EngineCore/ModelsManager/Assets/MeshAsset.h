#pragma once
/**
@file MeshAsset.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.
*/

#include "GraphicAPI/MeshResources.h"
#include "MaterialAsset.h"

#include <vector>

/**@defgroup MeshAsset Meshe
@ingroup Assets*/



/**@brief Flagi pola Flags w strukturze @ref MeshPart.
@ingroup MeshAsset*/
enum MeshPartFlags : int
{
	UseAdditionalBuffer = 0x01,		///< Materia� u�ywa dodatkowego bufora, kt�ry ma zosta� wype�niony w klasie @ref StaticActor.

};

/**@brief Przechowuje informacje o segmencie mesha.

@ingroup MeshAsset*/
struct MeshPart
{
	DirectX::XMFLOAT4X4				TransformMatrix;	///< Macierz przekszta�cenia wzgl�dem �rodka modelu. @deprecated Macierz zachowana tylko w ramach zgodno�ci wstecznej. W przysz�o�ci wierzcho�ki powinny zosta� przetransformowane przed wstawieniem do bufora.
	unsigned int					BufferOffset;		///< Offset wzgl�dem pocz�tku bufora indeks�w albo wierzcho�k�w. @see MeshAsset.
	unsigned int					NumVertices;		///< Liczba wierzcho�k�w do wy�wietlenia.
	int								BaseVertex;			///< Warto�� dodawana do ka�dego indeksu przed przeczytaniem wierzcho�ka z bufora. (Tylko wersja indeksowana).
	int								Flags;				///< Dodatkowe flagi @see MeshPartFlags
	ResourcePtr< MaterialAsset >	Material;			///< Materia� dla danej cz�ci mesha.
};





/**@brief Obiekt przechowuj�cy dane mesha.

Klasa zawiera tablic� element�w @ref MeshAsset (@see MeshPart), kt�re przechowuj� informacje potrzebne do wyrenderowania
poszczeg�lnych segment�w mesha. Meshe musza by� trzymane w cz�ciach, �eby mo�na by�o ka�dej z tych cz�ci
przypisa� r�ne materia�y i parametry. Gdyby nie taka struktura, to wczytanie pliku nie by�oby mo�liwe.
Meshe rzadko sk�adaj� si� z jednego kawa�ka.

Wszystkie wierzcho�ki przechowywane s� tylko w jednym buforze m_vertexBuffer. Na podstawie zalece�
w dokumentacji DrectXa, lepiej jest mie� jeden du�y bufor ni� wiele ma�ych.
Poszczeg�lne cz�ci mesha zawieraj� albo bufor indeks�w, kt�ry decyduje, kt�re wierzcho�ki
s� wy�wietlane, albo odpowiedni offset od pocz�tku bufora i ilo�� tr�jk�t�w.
Mo�liwe jest r�wnie�, �e mesh b�dzie wy�wietlany w trybie mieszanym, u�ywaj�c
dla ka�dej cz�ci raz bufora indeks�w, a raz offset�w.

@ingroup MeshAsset
@ingroup Assets
*/
class MeshAsset : public ResourceObject
{
	RTTR_ENABLE( ResourceObject );
	RTTR_REGISTRATION_FRIEND
private:

	ResourcePtr< BufferObject >			m_vertexBuffer;		///< Bufor wierzcho�k�w.
	ResourcePtr< BufferObject >			m_indexBuffer;		///< Bufor indeks�w.
	ResourcePtr< ShaderInputLayout >	m_layout;			///< Layout bufora wierzcho�k�w.
	std::vector< MeshPart >				m_segments;			///< Opis segment�w mesha.

	///@name Descriptor
	///@{
	std::wstring				m_filePath;			///< Plik, z kt�rego powsta� obiekt.

	///@}
	
public:
	explicit					MeshAsset			( const std::wstring& filePath );

	Size						GetSegmentsCount	() const;
	const MeshPart*				GetSegment			( Size index ) const;

	inline BufferObject*		GetVertexBuffer		()			{ return *m_vertexBuffer; }		///< Zwraca wska�nik na bufor wierzcho�k�w.
	inline BufferObject*		GetIndexBuffer		()			{ return *m_indexBuffer; }		///< Zwraca wska�nik na bufor indeks�w.
	inline ShaderInputLayout*	GetLayout			()			{ return *m_layout; }			///< Zwraca wska�nik na layout bufora wierzcho�k�w.

	virtual std::string			GetResourceName		() const override;

	virtual void				Serialize			( ISerializer* ser ) const override;
	virtual void				Deserialize			( IDeserializer* deser ) override;

private:
	~MeshAsset();
};
