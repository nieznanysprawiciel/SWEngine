using System;
using System.Windows.Media;
using System.Windows.Interop;
using System.Runtime.InteropServices;
using EditorPlugin;
using System.Windows;
using System.Windows.Input;
using EditorApp.Editor.Commands;
using EditorApp.Editor;
using EditorApp.GUI;

namespace EditorApp.Engine
{
	public class Displayer : UpdatableViewBase, IDropable
	{
		private D3DImageEx                      m_viewportSurface;
		private EngineWrapper                   m_engineWrapper;
		private bool                            m_editorReady = false;

		private int                             m_width;
		private int                             m_height;

		private Logic                           m_logicRef;	/// Wolałbym, żeby ta referencja nie była potrzebna.


		public Displayer( Logic appLogic )
		{
			DisplayName = "Preview";
			Height = 800;
			Width = 1000;

			m_viewportSurface = null;
			m_engineWrapper = null;

			m_logicRef = appLogic;
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

			m_engineWrapper.EnableInput( true );
			//m_engineWrapper.BasicScene();
			m_engineWrapper.TestScene();

			ViewportSurface.SetBackBufferEx( D3DResourceTypeEx.ID3D11Texture2D, EngineWrapper.GetRenderTarget( (ushort)Width, (ushort)Height ) );

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

		public void		GotFocus	()
		{
			m_engineWrapper.EnableInput( true );
		}

		public void		LostFocus	()
		{
			m_engineWrapper.EnableInput( false );
		}



		#region IDropable Implementation
		public Type DataType
		{
			get
			{
				return typeof( ActorClassMetaInfo );
			}
		}

		public void Drop			( object data, int index = -1 )
		{
			// Dodać informacje o pozycji myszy w momencie upuszczenia obiektu.
			ActorClassMetaInfo actorClass = data as ActorClassMetaInfo;
			m_logicRef.ProjectManager.ActorsLogic.CreateActor( actorClass );
		}
		#endregion

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
