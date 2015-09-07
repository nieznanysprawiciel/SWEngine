#pragma once

#include "../../GraphicAPIConstants.h"
#include "DX11APIObjects.h"


extern D3D11_USAGE DX11ResourceUsage[ 4 ];
extern DXGI_FORMAT DX11ResourceFormat[ 133 ];

class DX11ConstantsMapper
{
private:
public:
	DX11ConstantsMapper() = default;
	~DX11ConstantsMapper() = default;


	static inline D3D11_USAGE		Get( ResourceUsage usage )			{ return DX11ResourceUsage[ (int)usage ]; }
	static inline DXGI_FORMAT		Get( ResourceFormat format )		{ return DX11ResourceFormat[ (int)format ]; }
};
