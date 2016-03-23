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
		private D3DImageEx						m_viewportSurface;
		private EngineWrapper					m_engine;
		private bool							m_editorReady = false;

		private Logic							m_editorLogic;

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

			m_editorLogic = new Logic();
			m_editorLogic.Init( cmdArgs );

			m_viewportSurface = new D3DImageEx();
            EngineViewport.Source = m_viewportSurface;

            m_engine = new EngineWrapper();
			IntPtr handle = Marshal.GetHINSTANCE( m_engine.GetType().Module );

			if ( m_engine.InitializeEngine( handle ) )
                m_editorReady = true;
            else
                return;

			m_engine.BasicScene();

			m_viewportSurface.SetBackBufferEx( D3DResourceTypeEx.ID3D11Texture2D, m_engine.GetRenderTarget( (ushort)EngineViewport.Width, (ushort)EngineViewport.Height ) );

            CompositionTarget.Rendering += CompositionTargetRendering;
        }

        void MainWindowClosed( object sender, EventArgs e )
        {
			m_editorReady = false;
			m_engine.ReleaseEngine();
        }

        private void InvalidateD3DImage()
        {
            m_viewportSurface.Lock();
            m_viewportSurface.AddDirtyRect(new Int32Rect()
            {
                X = 0,
                Y = 0,
                Height = m_viewportSurface.PixelHeight,
                Width = m_viewportSurface.PixelWidth
            });
            m_viewportSurface.Unlock();
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
			m_editorLogic.ProjectManager.SaveProject();
		}
	}
}
