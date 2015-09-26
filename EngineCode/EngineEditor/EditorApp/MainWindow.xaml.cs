using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Interop;
using EditorPlugin;

namespace EditorApp
{
    /**@brief Główne okno edytora.*/
    public partial class MainWindow : Window
    {
        private D3DImageEx                      m_ViewportSurface;
        private EditorPlugin.EngineWrapper      m_engine;

        public MainWindow()
        {
            InitializeComponent();
            Loaded += MainWindowLoaded;
        }

        void MainWindowLoaded( object sender, RoutedEventArgs e )
        {
            /* Create a new D3DImageEx class */
            m_ViewportSurface = new D3DImageEx();

            /* Set our image's source to our D3DImage9Ex */
            EngineViewport.Source = m_ViewportSurface;

            /* Create a new D3DTest scene */
            m_engine = new EditorPlugin.EngineWrapper();

            /* Set the backbuffer, which is a ID3D10Texture2D pointer */
            m_ViewportSurface.SetBackBufferEx( D3DResourceTypeEx.ID3D11Texture2D, m_engine.GetRenderTarget() );

            //CompositionTarget.Rendering += CompositionTarget_Rendering;
        }
    }
}
