using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Interop;
using System.Runtime.InteropServices;
using EditorApp.Editor;
using EditorApp.GUI;
using sw.EditorPlugin;



namespace EditorApp.Engine
{
	public class Displayer : UpdatableViewBase
	{
		private D3DImageEx                      m_viewportSurface;

		private int                             m_width;
		private int                             m_height;



		public Displayer( EngineWrapper engine )
		{
			DisplayName = "Preview";
			Height = 800;
			Width = 1000;

			m_viewportSurface = new D3DImageEx();
			ViewportSurface.SetBackBufferEx( D3DResourceTypeEx.ID3D11Texture2D, engine.GetMainRenderTarget() );
		}


		#region PrivateHelpers

		private void InvalidateD3DImage()
		{
			m_viewportSurface.Lock();
			m_viewportSurface.AddDirtyRect( new Int32Rect()
			{
				X = 0,
				Y = 0,
				Height = m_viewportSurface.PixelHeight,
				Width = m_viewportSurface.PixelWidth
			} );
			m_viewportSurface.Unlock();
		}

		#endregion


		#region GettersSetters

		public D3DImageEx ViewportSurface
		{
			get
			{
				return m_viewportSurface;
			}
		}


		public int Width
		{
			get
			{
				return m_width;
			}

			set
			{
				m_width = value;
			}
		}

		public int Height
		{
			get
			{
				return m_height;
			}

			set
			{
				m_height = value;
			}
		}


		#endregion
	}
}
