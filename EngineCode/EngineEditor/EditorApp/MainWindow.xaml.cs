using System;
using System.Windows;
using System.Windows.Media;
using System.Windows.Interop;
using System.Runtime.InteropServices;
using EditorPlugin;


namespace EditorApp
{
    /**@brief Główne okno edytora.*/
    public partial class MainWindow : Window
    {
		private D3DImageEx						m_ViewportSurface;
		private EngineWrapper					m_engine;
		private bool							m_editorReady = false;

		private Editor.PathsManager				m_pathManager;
		private Project.Manager                 m_projectManager;

		public MainWindow()
        {
            System.Globalization.CultureInfo.DefaultThreadCurrentUICulture = System.Globalization.CultureInfo.GetCultureInfo("en-US");
            InitializeComponent();
            Loaded += MainWindowLoaded;
            Closed += MainWindowClosed;
        }

        void MainWindowLoaded( object sender, RoutedEventArgs e )
        {
			string[] cmdArgs = Environment.GetCommandLineArgs();

			m_projectManager = new Project.Manager();
			m_pathManager = new Editor.PathsManager();
			m_pathManager.InitPaths( cmdArgs );

			m_ViewportSurface = new D3DImageEx();
            EngineViewport.Source = m_ViewportSurface;

            m_engine = new EngineWrapper();
			IntPtr handle = Marshal.GetHINSTANCE( m_engine.GetType().Module );

			if ( m_engine.InitializeEngine( handle ) )
                m_editorReady = true;
            else
                return;

			m_engine.BasicScene();

			m_ViewportSurface.SetBackBufferEx( D3DResourceTypeEx.ID3D11Texture2D, m_engine.GetRenderTarget( (ushort)EngineViewport.Width, (ushort)EngineViewport.Height ) );

            CompositionTarget.Rendering += CompositionTargetRendering;
        }

        void MainWindowClosed( object sender, EventArgs e )
        {
			m_editorReady = false;
			m_engine.ReleaseEngine();
        }

        private void InvalidateD3DImage()
        {
            m_ViewportSurface.Lock();
            m_ViewportSurface.AddDirtyRect(new Int32Rect()
            {
                X = 0,
                Y = 0,
                Height = m_ViewportSurface.PixelHeight,
                Width = m_ViewportSurface.PixelWidth
            });
            m_ViewportSurface.Unlock();
        }

        private void CompositionTargetRendering(object sender, EventArgs e)
        {
            if ( m_editorReady )
            {
                m_engine.UpdateScene();
                m_engine.RenderScene();

                InvalidateD3DImage();
            }
        }

		private void SaveClick( object sender, RoutedEventArgs e )
		{
			m_projectManager.SaveProject();
		}
	}
}
