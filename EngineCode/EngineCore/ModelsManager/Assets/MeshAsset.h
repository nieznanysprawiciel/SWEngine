#pragma once
/**
@file MeshAsset.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.
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
	UseAdditionalBuffer = 0x01,		///< Materia³ u¿ywa dodatkowego bufora, który ma zostaæ wype³niony w klasie @ref StaticActor.

};

/**@brief Przechowuje informacje o segmencie mesha.

@ingroup MeshAsset*/
struct MeshPart
{
	DirectX::XMFLOAT4X4				TransformMatrix;	///< Macierz przekszta³cenia wzglêdem œrodka modelu. @deprecated Macierz zachowana tylko w ramach zgodnoœci wstecznej. W przysz³oœci wierzcho³ki powinny zostaæ przetransformowane przed wstawieniem do bufora.
	unsigned int					BufferOffset;		///< Offset wzglêdem pocz¹tku bufora indeksów albo wierzcho³ków. @see MeshAsset.
	unsigned int					NumVertices;		///< Liczba wierzcho³ków do wyœwietlenia.
	int								BaseVertex;			///< Wartoœæ dodawana do ka¿dego indeksu przed przeczytaniem wierzcho³ka z bufora. (Tylko wersja indeksowana).
	int								Flags;				///< Dodatkowe flagi @see MeshPartFlags
	ResourcePtr< MaterialAsset >	Material;			///< Materia³ dla danej czêœci mesha.
};





/**@brief Obiekt przechowuj¹cy dane mesha.

Klasa zawiera tablicê elementów @ref MeshAsset (@see MeshPart), które przechowuj¹ informacje potrzebne do wyrenderowania
poszczególnych segmentów mesha. Meshe musza byæ trzymane w czêœciach, ¿eby mo¿na by³o ka¿dej z tych czêœci
przypisaæ ró¿ne materia³y i parametry. Gdyby nie taka struktura, to wczytanie pliku nie by³oby mo¿liwe.
Meshe rzadko sk³adaj¹ siê z jednego kawa³ka.

Wszystkie wierzcho³ki przechowywane s¹ tylko w jednym buforze m_vertexBuffer. Na podstawie zaleceñ
w dokumentacji DrectXa, lepiej jest mieæ jeden du¿y bufor ni¿ wiele ma³ych.
Poszczególne czêœci mesha zawieraj¹ albo bufor indeksów, który decyduje, które wierzcho³ki
s¹ wyœwietlane, albo odpowiedni offset od pocz¹tku bufora i iloœæ trójk¹tów.
Mo¿liwe jest równie¿, ¿e mesh bêdzie wyœwietlany w trybie mieszanym, u¿ywaj¹c
dla ka¿dej czêœci raz bufora indeksów, a raz offsetów.

@ingroup MeshAsset
@ingroup Assets
*/
class MeshAsset : public ResourceObject
{
	RTTR_ENABLE( ResourceObject );
	RTTR_REGISTRATION_FRIEND
private:

	ResourcePtr< BufferObject >			m_vertexBuffer;		///< Bufor wierzcho³ków.
	ResourcePtr< BufferObject >			m_indexBuffer;		///< Bufor indeksów.
	ResourcePtr< ShaderInputLayout >	m_layout;			///< Layout bufora wierzcho³ków.
	std::vector< MeshPart >				m_segments;			///< Opis segmentów mesha.

	///@name Descriptor
	///@{
	std::wstring				m_filePath;			///< Plik, z którego powsta³ obiekt.

	///@}
	
public:
	explicit					MeshAsset			( const std::wstring& filePath );

	Size						GetSegmentsCount	() const;
	const MeshPart*				GetSegment			( Size index ) const;

	inline BufferObject*		GetVertexBuffer		()			{ return *m_vertexBuffer; }		///< Zwraca wskaŸnik na bufor wierzcho³ków.
	inline BufferObject*		GetIndexBuffer		()			{ return *m_indexBuffer; }		///< Zwraca wskaŸnik na bufor indeksów.
	inline ShaderInputLayout*	GetLayout			()			{ return *m_layout; }			///< Zwraca wskaŸnik na layout bufora wierzcho³ków.

	virtual std::string			GetResourceName		() const override;

	virtual void				Serialize			( ISerializer* ser ) const override;
	virtual void				Deserialize			( IDeserializer* deser ) override;

private:
	~MeshAsset();
};
