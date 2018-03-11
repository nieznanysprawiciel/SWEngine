#pragma once
/**@file Interop.D3DImageEx.h
Plik pochodzi ze strony http://jmorrill.hjtcentral.com/Home/tabid/428/EntryId/437/Direct3D-10-11-Direct2D-in-WPF.aspx 
@author Jeremiah Morrill*/

#include <windows.h>

#include <stdlib.h>
#include <stdio.h>

#pragma warning ( push )
#pragma warning ( disable : 4005 )

#include "d3d9.h"
//#include "d3dx9.h"
#include "D3D10_1.h"
#include <D3D11.h>
//#include "d3dx10.h"

#pragma warning ( pop )

using namespace System;
using namespace System::Windows::Interop;
using namespace System::Windows;

namespace System { namespace Windows { namespace Interop
{
	public enum class D3DResourceTypeEx : int
	{
		ID3D10Texture2D = 0,
		ID3D11Texture2D = 1
	};

	public ref class D3DImageEx : D3DImage
	{
		static D3DImageEx()
		{
			InitD3D9(GetDesktopWindow());
		}
	public:
		void SetBackBufferEx(D3DResourceTypeEx resourceType, IntPtr pResource);

	private:
		static IDirect3D9Ex*               m_D3D9;
		static IDirect3DDevice9Ex*         m_D3D9Device;
		
	private:
		static D3DFORMAT ConvertDXGIToD3D9Format(DXGI_FORMAT format);

		static HRESULT GetSharedSurface(HANDLE hSharedHandle, void** ppUnknown, UINT width, UINT height, DXGI_FORMAT format);

		static HRESULT GetSharedHandle(IUnknown *pUnknown, HANDLE * pHandle);

		static HRESULT InitD3D9(HWND hWnd);
	};
}}}
