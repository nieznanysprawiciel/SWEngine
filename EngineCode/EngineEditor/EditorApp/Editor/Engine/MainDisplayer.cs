using System;
using System.Windows.Media;
using System.Windows.Interop;
using System.Runtime.InteropServices;
using EditorPlugin;
using System.Windows;
using System.Collections.Generic;
using System.Windows.Input;
using EditorApp.Editor.Commands;
using EditorApp.Editor;
using EditorApp.GUI;
using EditorApp.Editor.Project.Content;

namespace EditorApp.Engine
{
	public class MainDisplayer : UpdatableViewBase, IDropable
	{
		private D3DImageEx                      m_viewportSurface;
		private EngineWrapper                   m_engineWrapper;
		private bool                            m_editorReady = false;

		private int                             m_width;
		private int                             m_height;

		List<Type>                              m_dropableTypes;

		private Logic                           m_logicRef; /// Wolałbym, żeby ta referencja nie była potrzebna.
		private GizmoActorWrapper               m_gizmo;



		public MainDisplayer( Logic appLogic )
		{
			DisplayName = "Preview";
			Height = 800;
			Width = 1000;

			m_viewportSurface = null;
			m_engineWrapper = null;

			m_logicRef = appLogic;
			//Gizmo = m_logicRef.ProjectManager.ActorsLogic.
			Gizmo = null;

			m_dropableTypes = new List<Type>();
			m_dropableTypes.Add( typeof( ActorClassMetaInfo ) );
			m_dropableTypes.Add( typeof( FileTreeNode ) );
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
			m_engineWrapper.BasicScene();
			//m_engineWrapper.TestScene();

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

		List<Type> IDropable.DataTypes
		{
			get
			{
				return m_dropableTypes;
			}
		}

		void IDropable.Drop( object data, double mouseX, double mouseY, int index )
		{
			if( data.GetType() == typeof( ActorClassMetaInfo ) )
			{
				ActorClassMetaInfo actorClass = data as ActorClassMetaInfo;
				m_logicRef.ProjectManager.ActorsLogic.CreateActor( actorClass, mouseX, mouseY );
			}
			else if( data.GetType() == typeof( FileTreeNode) )
			{
				FileTreeNode file = data as FileTreeNode;
				if( file.Type == FileTreeNodeType.Model3D )
				{
					string filePath = PathsManager.RelativePath( m_logicRef.PathsManager.AssetsDir, file.FilePath );
					m_logicRef.ProjectManager.ActorsLogic.CreateMeshActor( filePath, mouseX, mouseY );
				}
			}
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

		public GizmoActorWrapper Gizmo
		{
			get
			{
				return m_gizmo;
			}

			internal set
			{
				m_gizmo = value;
			}
		}


		#endregion
	}
}
