#pragma once
/**
@file MeshAsset.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.
*/

#include "GraphicAPI/MeshResources.h"
#include "EngineCore/ModelsManager/Assets/Materials/MaterialAsset.h"
#include "MeshPart.h"
#include "MeshAssetInitData.h"

#include <vector>

/**@defgroup MeshAsset Meshe
@ingroup Assets*/



/**@brief Obiekt przechowuj¹cy dane mesha.

Klasa zawiera tablicê elementów @ref MeshAsset (@see MeshPart), które przechowuj¹ informacje potrzebne do wyrenderowania
poszczególnych segmentów mesha. Meshe musza byæ trzymane w czêœciach, ¿eby mo¿na by³o ka¿dej z tych czêœci
przypisaæ ró¿ne materia³y i parametry. Gdyby nie taka struktura, to wczytanie pliku nie by³oby mo¿liwe.
Meshe rzadko sk³adaj¹ siê z jednego kawa³ka.

Wszystkie wierzcho³ki przechowywane s¹ tylko w jednym buforze m_vertexBuffer. Na podstawie zaleceñ
w dokumentacji DirectXa, lepiej jest mieæ jeden du¿y bufor ni¿ wiele ma³ych.
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
	explicit						MeshAsset			( const std::wstring& filePath, MeshAssetInitWithExistingData&& initData );

	Size							GetSegmentsCount	() const;
	const MeshPart*					GetSegment			( Size index ) const;
	const std::vector< MeshPart >&	GetSegments			()			{ return m_segments; }

	ResourcePtr< BufferObject >		GetVertexBuffer		()			{ return m_vertexBuffer; }		///< Zwraca wskaŸnik na bufor wierzcho³ków.
	ResourcePtr< BufferObject >		GetIndexBuffer		()			{ return m_indexBuffer; }		///< Zwraca wskaŸnik na bufor indeksów.
	ResourcePtr< ShaderInputLayout >GetLayout			()			{ return m_layout; }			///< Zwraca wskaŸnik na layout bufora wierzcho³ków.


	virtual std::string				GetResourceName		() const override;

	virtual void					Serialize			( ISerializer* ser ) const override;
	virtual void					Deserialize			( IDeserializer* deser ) override;

	///@name @ref DisplayEngine functions
	///Functions allow DisplayEngine class aquire raw buffer pointers. Engine user should choose
	///functions returning ResourcePtrs.
	///@{
	inline BufferObject*			GetVertexBufferRawPtr	()		{ return *m_vertexBuffer; }		///< Zwraca wskaŸnik na bufor wierzcho³ków.
	inline BufferObject*			GetIndexBufferRawPtr	()		{ return *m_indexBuffer; }		///< Zwraca wskaŸnik na bufor indeksów.
	inline ShaderInputLayout*		GetLayoutRawPtr			()		{ return *m_layout; }			///< Zwraca wskaŸnik na layout bufora wierzcho³ków.
	///@}

private:
	~MeshAsset();
};
