#pragma once

enum RENDERER_USAGE
{
	USE_AS_DEFERRED,
	USE_AS_IMMEDIATE
};


/**Klasa interfejs dla renderera.

Aby u¿yæ nale¿y odziedziczyæ po tej klasie podaj¹c w parametrze szablonu klasê dziedzicz¹c¹.*/
template<typename DerivedRenderer>
class IRenderer
{
private:

public:
	IRenderer( RENDERER_USAGE usage = USE_AS_IMMEDIATE );
	~IRenderer() = default;


	inline bool SupportsDefferedContextRendering() { return true; }		///< Informuje czy Renderer nadaje siê do renderowania w wielu w¹tkach do opóŸnionego contextu.
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




/**@brief Klasa bêdzie renderowaæ w trybie immediate albo deferred.

Je¿eli ustawiono flagê D3D11_CREATE_DEVICE_SINGLETHREADED, to nie wolno podaæ w parametrze
RENDERER_USAGE::USE_AS_DEFERRED.

Mo¿na sprawdziæ czy klasa zosta³a zainicjowana poprawnie wywo³uj¹c funkcjê IsValid.*/
template<typename DerivedRenderer>
inline IRenderer<DerivedRenderer>::IRenderer( RENDERER_USAGE usage )
{
}

//template<typename DerivedRenderer>
//inline IRenderer<DerivedRenderer>::~IRenderer()
//{
//}


/**@brief Funkcja ustawia tesktury z danego ModelParta w DirectXie.

@param[in] model ModelPart z którego pochodz¹ tekstury do ustawienia.
@todo SetShaderResource mo¿na u¿yæ do ustawienia od razu ca³ej tablicy. Trzeba umo¿liwiæ ustawianie
do VS i innych.*/
template<typename DerivedRenderer>
inline void IRenderer<DerivedRenderer>::SetTextures( const ModelPart& model )
{
	static_cast<DerivedRenderer*>(this)->setTexturesImpl( model );
}

/**@brief Ustawia w kontekœcie urz¹dzenia bufor indeksów.

@param[in] buffer Bufor do ustawienia.*/
template<typename DerivedRenderer>
inline void IRenderer<DerivedRenderer>::SetIndexBuffer( BufferObject* buffer )
{
	static_cast<DerivedRenderer*>(this)->setIndexBufferImpl( buffer );
}


/**@brief Ustawia w kontekœcie urz¹dzenia bufor wierzcho³ków.

@param[in] buffer Bufor do ustawienia.
@return Je¿eli bufor nie istnieje to zwraca wartoœæ true. Inaczej false.
Wywo³anie if( set_vertex_buffer() ) ma zwróciæ tak¹ wartoœæ, ¿eby w ifie mo¿na by³o
wywo³aæ return lub continue, w przypadku braku bufora.*/
template<typename DerivedRenderer>
inline bool IRenderer<DerivedRenderer>::SetVertexBuffer( BufferObject* buffer, unsigned int offset )
{
	static_cast<DerivedRenderer*>(this)->setVertexBufferImpl( buffer, offset );
}


/**@brief Funkcja w³¹cza lub wy³¹cza z-bufor.

@param[in] state True je¿eli z-bufor ma byæ w³¹czony, false je¿eli wy³¹czony.*/
template<typename DerivedRenderer>
inline void IRenderer<DerivedRenderer>::DepthBufferEnable( bool state )
{
	static_cast<DerivedRenderer*>(this)->depthBufferEnableImpl( state );
}

/**@brief Ustawia domyœlny layout z DX11_interfaces_container.

@todo Wymyœleæ lepsz¹ implementacjê i sposób przechowywania layoutów.*/
template<typename DerivedRenderer>
inline void IRenderer<DerivedRenderer>::SetDefaultVertexLayout()
{
	static_cast<DerivedRenderer*>(this)->setDefaultVertexLayoutImpl();
}

/**@brief Ustawia domyœlny sampler z DX11_interfaces_container.

@todo Wymyœleæ lepsz¹ implementacjê i sposób przechowywania samplerów. (W zasadzie mo¿na je definiowaæ w shaderach.)*/
template<typename DerivedRenderer>
inline void IRenderer<DerivedRenderer>::SetDefaultSampler()
{
	static_cast<DerivedRenderer*>(this)->setDefaultSamplerImpl();
}

/**@brief Inicjuje bufory sta³ych dla shaderów.

@todo: Ta funkcja powinna znikn¹æ. Bufory powinny byæ inicjowane w DisplayEngine, ale jako BufferObject,
a nie bufory DirectXowe.*/
template<typename DerivedRenderer>
inline void IRenderer<DerivedRenderer>::InitBuffers( unsigned int sizePerFrame, unsigned int sizePerMesh )
{
	static_cast<DerivedRenderer*>(this)->initBuffersImpl( sizePerFrame, sizePerMesh );
}

/**@brief Inicjuje pomocnicze stany bufora g³êbokoœci (do w³¹czania i wy³¹czania depth testu)

@todo To trzeba za³atwiaæ w jakiœ bardziej elegancki sposób.*/
template<typename DerivedRenderer>
inline void IRenderer<DerivedRenderer>::InitDepthStates()
{
	static_cast<DerivedRenderer*>(this)->initDepthStatesImpl();
}

//----------------------------------------------------------------------------------------------//
//								Funkcje analogiczne do DirectX11								//
//----------------------------------------------------------------------------------------------//

/**@brief Funkcja robi dok³adnie to samo, co tak samo nazywaj¹ca siê funkcja DirectXa.
Po wiêcej informacji wysy³am do MSDNu.

@todo Funkcja nie powinna przyjmowaæ w parametrze sta³ej definiowanej w DirectX.
Nale¿y zrobiæ w³asny zestaw sta³ych, ¿eby uniezale¿niæ interfejs renderera od implementacji.*/
template<typename DerivedRenderer>
inline void IRenderer<DerivedRenderer>::IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY topology )
{
	static_cast<DerivedRenderer*>(this)->IASetPrimitiveTopologyImpl( topology );
}

/**@brief Funkcja robi dok³adnie to samo, co tak samo nazywaj¹ca siê funkcja DirectXa.
Po wiêcej informacji wysy³am do MSDNu.

@todo Funkcja nie powinna przyjmowaæ w parametrze obiektu DirectXa.
Trzeba coœ z tym zrobiæ. Nie wiem co*/
template<typename DerivedRenderer>
inline void IRenderer<DerivedRenderer>::IASetInputLayout( ID3D11InputLayout *pInputLayout )
{
	static_cast<DerivedRenderer*>(this)->IASetInputLayoutImpl( pInputLayout );
}

/**@brief Funkcja robi dok³adnie to samo, co tak samo nazywaj¹ca siê funkcja DirectXa.
Po wiêcej informacji wysy³am do MSDNu.

@deprecated Powinniœmy u¿yæ bufora silnikowego, a nie DirectXowego, ¿eby ukryæ implementacjê.
Funkcja zniknie w póŸniejszej wersji.*/
template<typename DerivedRenderer>
inline void IRenderer<DerivedRenderer>::VSSetConstantBuffers( UINT startSlot, UINT numBuffers, ID3D11Buffer *const *ppConstantBuffers )
{
	static_cast<DerivedRenderer*>(this)->VSSetConstantBuffersImpl( startSlot, numBuffers, ppConstantBuffers );
}

/**@brief Funkcja robi dok³adnie to samo, co tak samo nazywaj¹ca siê funkcja DirectXa.
Po wiêcej informacji wysy³am do MSDNu.

@deprecated Powinniœmy u¿yæ bufora silnikowego, a nie DirectXowego, ¿eby ukryæ implementacjê.
Funkcja zniknie w póŸniejszej wersji.*/
template<typename DerivedRenderer>
inline void IRenderer<DerivedRenderer>::PSSetConstantBuffers( UINT startSlot, UINT numBuffers, ID3D11Buffer *const *ppConstantBuffers )
{
	static_cast<DerivedRenderer*>(this)->PSSetConstantBuffersImpl( startSlot, numBuffers, ppConstantBuffers );
}

/**@brief Funkcja robi dok³adnie to samo, co tak samo nazywaj¹ca siê funkcja DirectXa.
Po wiêcej informacji wysy³am do MSDNu.*/
template<typename DerivedRenderer>
inline void IRenderer<DerivedRenderer>::VSSetConstantBuffers( UINT startSlot, UINT numBuffers, BufferObject* buffer )
{
	static_cast<DerivedRenderer*>(this)->VSSetConstantBuffersImpl( startSlots, numBuffers, buffer );
}

/**@brief Funkcja robi dok³adnie to samo, co tak samo nazywaj¹ca siê funkcja DirectXa.
Po wiêcej informacji wysy³am do MSDNu.*/
template<typename DerivedRenderer>
inline void IRenderer<DerivedRenderer>::PSSetConstantBuffers( UINT startSlot, UINT numBuffers, BufferObject* buffer )
{
	static_cast<DerivedRenderer*>(this)->PSSetConstantBuffersImpl( startSlots, numBuffers, buffer );
}

/**@brief Funkcja robi dok³adnie to samo, co tak samo nazywaj¹ca siê funkcja DirectXa.
Po wiêcej informacji wysy³am do MSDNu.

@deprecated: Depracated jeszcze przed napisaniem funkcji do koñca. Myœlê, ¿e bufor powinien
sam siê updatowaæ.*/
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

/**@brief Funkcja robi dok³adnie to samo, co tak samo nazywaj¹ca siê funkcja DirectXa.
Po wiêcej informacji wysy³am do MSDNu.*/
template<typename DerivedRenderer>
inline void IRenderer<DerivedRenderer>::Draw( unsigned int vertexCount, unsigned int startVertexLocation )
{
	static_cast<DerivedRenderer*>(this)->DrawImpl( vertexCount, startVertexLocation );
}

/**@brief Funkcja robi dok³adnie to samo, co tak samo nazywaj¹ca siê funkcja DirectXa.
Po wiêcej informacji wysy³am do MSDNu.*/
template<typename DerivedRenderer>
inline void IRenderer<DerivedRenderer>::DrawIndexed( unsigned int indexCount, unsigned int startIndexLocation, int baseVertexLocation )
{
	static_cast<DerivedRenderer*>(this)->DrawIndexedImpl( indexCount, startIndexLocation, baseVertexLocation );
}


