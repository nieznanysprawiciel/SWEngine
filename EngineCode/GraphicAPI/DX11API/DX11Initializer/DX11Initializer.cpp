#include "DX11Initializer.h"
#include "../DX11Renderer/DX11Renderer.h"


/**@brief Tworzy renderer zgodny z u¿ywanym API graficznym.

@param[in] usage Specyfikuje czy u¿yæ opóŸnionego kontekstu renderowania czy natychmiastowego.
@note Wszystkie renderery stworzone w aplikacji powinny byæ tego samego typu.*/
IRenderer* DX11Initializer::CreateRenderer( RendererUsage usage )
{
	return new DX11Renderer( usage );
}

/**@brief Inicjalizuje API graficzne.

@param[in] initData Dane u¿ywane do inicjalizacji.
@return Zwraca true, je¿eli inicjalizacja siê uda.*/
bool DX11Initializer::InitAPI( GraphicAPIInitData& initData )
{
	DX11_INIT_RESULT result = init_DX11( initData.windowWidth, initData.windowHeight, (HWND)initData.windowHandle, initData.fullScreen, initData.singleThreaded );
	if( result != DX11_INIT_RESULT::DX11_INIT_OK )
		return false;
	return true;
}

/**@brief Zwalnia stworzone obiekty DirectX 11.*/
void DX11Initializer::ReleaseAPI()
{
	release_DirectX();
}
