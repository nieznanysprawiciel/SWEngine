#pragma once

enum RENDERER_USAGE
{
	USE_AS_DEFERRED,
	USE_AS_IMMEDIATE
};


/**Klasa interfejs dla renderera.

Aby u�y� nale�y odziedziczy� po tej klasie podaj�c w parametrze szablonu klas� dziedzicz�c�.*/
template<typename DerivedRenderer>
class IRenderer
{
private:

public:
	IRenderer( RENDERER_USAGE usage = USE_AS_IMMEDIATE );
	~IRenderer() = default;


	inline bool SupportsDefferedContextRendering() { return true; }		///< Informuje czy Renderer nadaje si� do renderowania w wielu w�tkach do op�nionego contextu.
	inline bool IsValid() { return m_valid; }


	void SetTextures( const ModelPart& model );
	void SetIndexBuffer( BufferObject* buffer );
	bool SetVertexBuffer( BufferObject* buffer, unsigned int offset = 0 );
	void DepthBufferEnable( bool state );
	void SetDefaultVertexLayout();
	void SetDefaultSampler();

	void InitBuffers( unsigned int sizePerFrame, unsigned int sizePerMesh );
	void InitDepthStates();

	void IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY topology );
	void IASetInputLayout( ID3D11InputLayout *pInputLayout );


	void VSSetConstantBuffers( UINT startSlot, UINT numBuffers, ID3D11Buffer *const *ppConstantBuffers );
	void VSSetConstantBuffers( UINT startSlot, UINT numBuffers, BufferObject* buffer );

	void PSSetConstantBuffers( UINT startSlot, UINT numBuffers, ID3D11Buffer *const *ppConstantBuffers );
	void PSSetConstantBuffers( UINT startSlot, UINT numBuffers, BufferObject* buffer );

	void UpdateSubresource( ID3D11Resource *pDstResource, const void *pSrcData );

	void SetShaders( ModelPart& model );

	// Rendering
	void Draw( unsigned int vertexCount, unsigned int startVertexLocation );
	void DrawIndexed( unsigned int indexCount, unsigned int startIndexLocation, int BaseVertexLocation );
};




/**@brief Klasa b�dzie renderowa� w trybie immediate albo deferred.

Je�eli ustawiono flag� D3D11_CREATE_DEVICE_SINGLETHREADED, to nie wolno poda� w parametrze
RENDERER_USAGE::USE_AS_DEFERRED.

Mo�na sprawdzi� czy klasa zosta�a zainicjowana poprawnie wywo�uj�c funkcj� IsValid.*/
template<typename DerivedRenderer>
inline IRenderer<DerivedRenderer>::IRenderer( RENDERER_USAGE usage )
{
}

//template<typename DerivedRenderer>
//inline IRenderer<DerivedRenderer>::~IRenderer()
//{
//}


/**@brief Funkcja ustawia tesktury z danego ModelParta w DirectXie.

@param[in] model ModelPart z kt�rego pochodz� tekstury do ustawienia.
@todo SetShaderResource mo�na u�y� do ustawienia od razu ca�ej tablicy. Trzeba umo�liwi� ustawianie
do VS i innych.*/
template<typename DerivedRenderer>
inline void IRenderer<DerivedRenderer>::SetTextures( const ModelPart& model )
{
	static_cast<DerivedRenderer*>(this)->setTexturesImpl( model );
}

/**@brief Ustawia w kontek�cie urz�dzenia bufor indeks�w.

@param[in] buffer Bufor do ustawienia.*/
template<typename DerivedRenderer>
inline void IRenderer<DerivedRenderer>::SetIndexBuffer( BufferObject* buffer )
{
	static_cast<DerivedRenderer*>(this)->setIndexBufferImpl( buffer );
}


/**@brief Ustawia w kontek�cie urz�dzenia bufor wierzcho�k�w.

@param[in] buffer Bufor do ustawienia.
@return Je�eli bufor nie istnieje to zwraca warto�� true. Inaczej false.
Wywo�anie if( set_vertex_buffer() ) ma zwr�ci� tak� warto��, �eby w ifie mo�na by�o
wywo�a� return lub continue, w przypadku braku bufora.*/
template<typename DerivedRenderer>
inline bool IRenderer<DerivedRenderer>::SetVertexBuffer( BufferObject* buffer, unsigned int offset )
{
	static_cast<DerivedRenderer*>(this)->setVertexBufferImpl( buffer, offset );
}


/**@brief Funkcja w��cza lub wy��cza z-bufor.

@param[in] state True je�eli z-bufor ma by� w��czony, false je�eli wy��czony.*/
template<typename DerivedRenderer>
inline void IRenderer<DerivedRenderer>::DepthBufferEnable( bool state )
{
	static_cast<DerivedRenderer*>(this)->depthBufferEnableImpl( state );
}

/**@brief Ustawia domy�lny layout z DX11_interfaces_container.

@todo Wymy�le� lepsz� implementacj� i spos�b przechowywania layout�w.*/
template<typename DerivedRenderer>
inline void IRenderer<DerivedRenderer>::SetDefaultVertexLayout()
{
	static_cast<DerivedRenderer*>(this)->setDefaultVertexLayoutImpl();
}

/**@brief Ustawia domy�lny sampler z DX11_interfaces_container.

@todo Wymy�le� lepsz� implementacj� i spos�b przechowywania sampler�w. (W zasadzie mo�na je definiowa� w shaderach.)*/
template<typename DerivedRenderer>
inline void IRenderer<DerivedRenderer>::SetDefaultSampler()
{
	static_cast<DerivedRenderer*>(this)->setDefaultSamplerImpl();
}

/**@brief Inicjuje bufory sta�ych dla shader�w.

@todo: Ta funkcja powinna znikn��. Bufory powinny by� inicjowane w DisplayEngine, ale jako BufferObject,
a nie bufory DirectXowe.*/
template<typename DerivedRenderer>
inline void IRenderer<DerivedRenderer>::InitBuffers( unsigned int sizePerFrame, unsigned int sizePerMesh )
{
	static_cast<DerivedRenderer*>(this)->initBuffersImpl( sizePerFrame, sizePerMesh );
}

/**@brief Inicjuje pomocnicze stany bufora g��boko�ci (do w��czania i wy��czania depth testu)

@todo To trzeba za�atwia� w jaki� bardziej elegancki spos�b.*/
template<typename DerivedRenderer>
inline void IRenderer<DerivedRenderer>::InitDepthStates()
{
	static_cast<DerivedRenderer*>(this)->initDepthStatesImpl();
}

//----------------------------------------------------------------------------------------------//
//								Funkcje analogiczne do DirectX11								//
//----------------------------------------------------------------------------------------------//

/**@brief Funkcja robi dok�adnie to samo, co tak samo nazywaj�ca si� funkcja DirectXa.
Po wi�cej informacji wysy�am do MSDNu.

@todo Funkcja nie powinna przyjmowa� w parametrze sta�ej definiowanej w DirectX.
Nale�y zrobi� w�asny zestaw sta�ych, �eby uniezale�ni� interfejs renderera od implementacji.*/
template<typename DerivedRenderer>
inline void IRenderer<DerivedRenderer>::IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY topology )
{
	static_cast<DerivedRenderer*>(this)->IASetPrimitiveTopologyImpl( topology );
}

/**@brief Funkcja robi dok�adnie to samo, co tak samo nazywaj�ca si� funkcja DirectXa.
Po wi�cej informacji wysy�am do MSDNu.

@todo Funkcja nie powinna przyjmowa� w parametrze obiektu DirectXa.
Trzeba co� z tym zrobi�. Nie wiem co*/
template<typename DerivedRenderer>
inline void IRenderer<DerivedRenderer>::IASetInputLayout( ID3D11InputLayout *pInputLayout )
{
	static_cast<DerivedRenderer*>(this)->IASetInputLayoutImpl( pInputLayout );
}

/**@brief Funkcja robi dok�adnie to samo, co tak samo nazywaj�ca si� funkcja DirectXa.
Po wi�cej informacji wysy�am do MSDNu.

@deprecated Powinni�my u�y� bufora silnikowego, a nie DirectXowego, �eby ukry� implementacj�.
Funkcja zniknie w p�niejszej wersji.*/
template<typename DerivedRenderer>
inline void IRenderer<DerivedRenderer>::VSSetConstantBuffers( UINT startSlot, UINT numBuffers, ID3D11Buffer *const *ppConstantBuffers )
{
	static_cast<DerivedRenderer*>(this)->VSSetConstantBuffersImpl( startSlot, numBuffers, ppConstantBuffers );
}

/**@brief Funkcja robi dok�adnie to samo, co tak samo nazywaj�ca si� funkcja DirectXa.
Po wi�cej informacji wysy�am do MSDNu.

@deprecated Powinni�my u�y� bufora silnikowego, a nie DirectXowego, �eby ukry� implementacj�.
Funkcja zniknie w p�niejszej wersji.*/
template<typename DerivedRenderer>
inline void IRenderer<DerivedRenderer>::PSSetConstantBuffers( UINT startSlot, UINT numBuffers, ID3D11Buffer *const *ppConstantBuffers )
{
	static_cast<DerivedRenderer*>(this)->PSSetConstantBuffersImpl( startSlot, numBuffers, ppConstantBuffers );
}

/**@brief Funkcja robi dok�adnie to samo, co tak samo nazywaj�ca si� funkcja DirectXa.
Po wi�cej informacji wysy�am do MSDNu.*/
template<typename DerivedRenderer>
inline void IRenderer<DerivedRenderer>::VSSetConstantBuffers( UINT startSlot, UINT numBuffers, BufferObject* buffer )
{
	static_cast<DerivedRenderer*>(this)->VSSetConstantBuffersImpl( startSlots, numBuffers, buffer );
}

/**@brief Funkcja robi dok�adnie to samo, co tak samo nazywaj�ca si� funkcja DirectXa.
Po wi�cej informacji wysy�am do MSDNu.*/
template<typename DerivedRenderer>
inline void IRenderer<DerivedRenderer>::PSSetConstantBuffers( UINT startSlot, UINT numBuffers, BufferObject* buffer )
{
	static_cast<DerivedRenderer*>(this)->PSSetConstantBuffersImpl( startSlots, numBuffers, buffer );
}

/**@brief Funkcja robi dok�adnie to samo, co tak samo nazywaj�ca si� funkcja DirectXa.
Po wi�cej informacji wysy�am do MSDNu.

@deprecated: Depracated jeszcze przed napisaniem funkcji do ko�ca. My�l�, �e bufor powinien
sam si� updatowa�.*/
template<typename DerivedRenderer>
inline void IRenderer<DerivedRenderer>::UpdateSubresource( ID3D11Resource *pDstResource, const void *pSrcData )
{
	static_cast<DerivedRenderer*>(this)->UpdateSubresourceImpl( pDstResource, pSrcData );
}

template<typename DerivedRenderer>
inline void IRenderer<DerivedRenderer>::SetShaders( ModelPart& model )
{
	static_cast<DerivedRenderer*>(this)->setShadersImpl( model );
}

//----------------------------------------------------------------------------------------------//
//								Rendering														//
//----------------------------------------------------------------------------------------------//

/**@brief Funkcja robi dok�adnie to samo, co tak samo nazywaj�ca si� funkcja DirectXa.
Po wi�cej informacji wysy�am do MSDNu.*/
template<typename DerivedRenderer>
inline void IRenderer<DerivedRenderer>::Draw( unsigned int vertexCount, unsigned int startVertexLocation )
{
	static_cast<DerivedRenderer*>(this)->DrawImpl( vertexCount, startVertexLocation );
}

/**@brief Funkcja robi dok�adnie to samo, co tak samo nazywaj�ca si� funkcja DirectXa.
Po wi�cej informacji wysy�am do MSDNu.*/
template<typename DerivedRenderer>
inline void IRenderer<DerivedRenderer>::DrawIndexed( unsigned int indexCount, unsigned int startIndexLocation, int baseVertexLocation )
{
	static_cast<DerivedRenderer*>(this)->DrawIndexedImpl( indexCount, startIndexLocation, baseVertexLocation );
}


