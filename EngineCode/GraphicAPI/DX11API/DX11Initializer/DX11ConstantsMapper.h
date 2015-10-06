#pragma once

#include "../../GraphicAPIConstants.h"
#include "DX11APIObjects.h"


extern D3D11_USAGE							DX11ResourceUsage[ 4 ];
extern DXGI_FORMAT							DX11ResourceFormat[ 133 ];
extern D3D_PRIMITIVE_TOPOLOGY				DX11PrimitiveTopology[ 9 ];
extern D3D11_BIND_FLAG						DX11ResourceBinding[ 10 ];
extern D3D11_RTV_DIMENSION					DX11TextureType[ 9 ];
extern DXGI_FORMAT							DX11DepthStencilFormat[ 4 ];


extern D3D11_DSV_DIMENSION					DX11TextureTypeToDSV[ 9 ];
extern D3D11_SRV_DIMENSION					DX11TextureTypeToSRV[ 9 ];
extern DXGI_FORMAT							DX11DepthStencilTypeless[ 4 ];
extern DXGI_FORMAT							DX11DepthOnly[ 4 ];
extern DXGI_FORMAT							DX11StencilOnly[ 4 ];

/**@brief Klasa s³u¿y do mapowania sta³ych z wartoœci silnikowych do DirectX 11.
@ingroup DX11API*/
class DX11ConstantsMapper
{
private:
public:
	DX11ConstantsMapper() = default;
	~DX11ConstantsMapper() = default;


	static inline D3D11_USAGE				Get( ResourceUsage usage )			{ return DX11ResourceUsage[ (int)usage ]; }
	static inline DXGI_FORMAT				Get( ResourceFormat format )		{ return DX11ResourceFormat[ (int)format ]; }
	static inline D3D_PRIMITIVE_TOPOLOGY	Get( PrimitiveTopology topology )	{ return DX11PrimitiveTopology[ (int)topology ]; }
	static inline D3D11_BIND_FLAG			Get( ResourceBinding binding )		{ return DX11ResourceBinding[ (int)binding ]; }
	static inline D3D11_RTV_DIMENSION		Get( TextureType texType )			{ return DX11TextureType[ (int)texType ]; }
	static inline DXGI_FORMAT				Get( DepthStencilFormat format )	{ return DX11DepthStencilFormat[ (int)format ]; }
	
	static inline D3D11_SRV_DIMENSION		ConvertSRV			( TextureType texType )				{ return DX11TextureTypeToSRV[ (int)texType ]; }
	static inline D3D11_DSV_DIMENSION		ConvertDSV			( TextureType texType )				{ return DX11TextureTypeToDSV[ (int)texType ]; }
	static inline DXGI_FORMAT				ConvertTypeless		( DepthStencilFormat format )		{ return DX11DepthStencilTypeless[ (int)format ]; }
	static inline DXGI_FORMAT				ConvertDepthOnly	( DepthStencilFormat format )		{ return DX11DepthOnly[ (int)format ]; }
	static inline DXGI_FORMAT				ConvertStencilOnly	( DepthStencilFormat format )		{ return DX11StencilOnly[ (int)format ]; }
};

