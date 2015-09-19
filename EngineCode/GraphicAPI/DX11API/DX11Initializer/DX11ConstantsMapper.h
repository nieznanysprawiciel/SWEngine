#pragma once

#include "../../GraphicAPIConstants.h"
#include "DX11APIObjects.h"


extern D3D11_USAGE							DX11ResourceUsage[ 4 ];
extern DXGI_FORMAT							DX11ResourceFormat[ 133 ];
extern D3D_PRIMITIVE_TOPOLOGY				DX11PrimitiveTopology[ 9 ];
extern D3D11_BIND_FLAG						DX11REsourceBinding[ 10 ];


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
	static inline D3D11_BIND_FLAG			Get( ResourceBinding binding )		{ return DX11REsourceBinding[ (int)binding ]; }
};

