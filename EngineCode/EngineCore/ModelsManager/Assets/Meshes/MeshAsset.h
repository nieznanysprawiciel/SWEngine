#pragma once
/**
@file MeshAsset.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.
*/

#include "GraphicAPI/MeshResources.h"
#include "EngineCore/ModelsManager/Assets/Materials/MaterialAsset.h"
#include "MeshPart.h"
#include "MeshAssetInitData.h"

#include <vector>

/**@defgroup MeshAsset Meshe
@ingroup Assets*/



/**@brief Obiekt przechowuj�cy dane mesha.

Klasa zawiera tablic� element�w @ref MeshAsset (@see MeshPart), kt�re przechowuj� informacje potrzebne do wyrenderowania
poszczeg�lnych segment�w mesha. Meshe musza by� trzymane w cz�ciach, �eby mo�na by�o ka�dej z tych cz�ci
przypisa� r�ne materia�y i parametry. Gdyby nie taka struktura, to wczytanie pliku nie by�oby mo�liwe.
Meshe rzadko sk�adaj� si� z jednego kawa�ka.

Wszystkie wierzcho�ki przechowywane s� tylko w jednym buforze m_vertexBuffer. Na podstawie zalece�
w dokumentacji DirectXa, lepiej jest mie� jeden du�y bufor ni� wiele ma�ych.
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
	explicit						MeshAsset			( const std::wstring& filePath, MeshAssetInitWithExistingData&& initData );

	Size							GetSegmentsCount	() const;
	const MeshPart*					GetSegment			( Size index ) const;
	const std::vector< MeshPart >&	GetSegments			()			{ return m_segments; }

	ResourcePtr< BufferObject >		GetVertexBuffer		()			{ return m_vertexBuffer; }		///< Zwraca wska�nik na bufor wierzcho�k�w.
	ResourcePtr< BufferObject >		GetIndexBuffer		()			{ return m_indexBuffer; }		///< Zwraca wska�nik na bufor indeks�w.
	ResourcePtr< ShaderInputLayout >GetLayout			()			{ return m_layout; }			///< Zwraca wska�nik na layout bufora wierzcho�k�w.


	virtual std::string				GetResourceName		() const override;

	virtual void					Serialize			( ISerializer* ser ) const override;
	virtual void					Deserialize			( IDeserializer* deser ) override;

	///@name @ref DisplayEngine functions
	///Functions allow DisplayEngine class aquire raw buffer pointers. Engine user should choose
	///functions returning ResourcePtrs.
	///@{
	inline BufferObject*			GetVertexBufferRawPtr	()		{ return *m_vertexBuffer; }		///< Zwraca wska�nik na bufor wierzcho�k�w.
	inline BufferObject*			GetIndexBufferRawPtr	()		{ return *m_indexBuffer; }		///< Zwraca wska�nik na bufor indeks�w.
	inline ShaderInputLayout*		GetLayoutRawPtr			()		{ return *m_layout; }			///< Zwraca wska�nik na layout bufora wierzcho�k�w.
	///@}

private:
	~MeshAsset();
};
