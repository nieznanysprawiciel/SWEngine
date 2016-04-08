using System;
using System.Windows.Media;
using System.Windows.Interop;
using System.Runtime.InteropServices;
using EditorPlugin;
using System.Windows;

namespace EditorApp.Engine
{
	public class Displayer
	{
		private D3DImageEx                      m_viewportSurface;
		private EngineWrapper                   m_engineWrapper;
		private bool                            m_editorReady = false;



		public Displayer()
		{
			m_viewportSurface = null;
			m_engineWrapper = null;
		}
		
		public bool		InitRenderer()
		{
			m_viewportSurface = new D3DImageEx();

			m_engineWrapper = new EngineWrapper();
			IntPtr handle = Marshal.GetHINSTANCE( m_engineWrapper.GetType().Module );

			if( m_engineWrapper.InitializeEngine( handle ) )
				m_editorReady = true;
			else
				return false;
			
			//EditorPlugin.ObjectPropertyWrapper prop;// = new EditorPlugin.ObjectPropertyWrapper( sd );
			//prop.

			m_engineWrapper.BasicScene();

			return true;
		}

		public void		ReleaseRenderer()
		{
			m_editorReady = false;
			m_engineWrapper.ReleaseEngine();
		}

		public void		RenderFrame()
		{
			if( m_editorReady )
			{
				m_engineWrapper.UpdateScene();
				m_engineWrapper.RenderScene();

				InvalidateD3DImage();
			}
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

		public EngineWrapper EngineWrapper
		{
			get
			{
				return m_engineWrapper;
			}
		}


		#endregion
	}
}
