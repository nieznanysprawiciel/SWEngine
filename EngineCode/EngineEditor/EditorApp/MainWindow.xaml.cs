using System;
using System.Windows;
using System.Windows.Media;
using System.Windows.Interop;



namespace EditorApp
{
    /**@brief Główne okno edytora.*/
    public partial class MainWindow : Window
    {
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

			EditorLogic = new Logic( this );
			EditorLogic.Init( cmdArgs );

			DataContext = EditorLogic;

            CompositionTarget.Rendering += EditorLogic.RenderFrame;
        }

        void MainWindowClosed( object sender, EventArgs e )
        {
			EditorLogic.CloseApplication();
        }


		#region Properties

		public Logic EditorLogic
		{
			get
			{
				return m_editorLogic;
			}

			set
			{
				m_editorLogic = value;
			}
		}

		#endregion

	}
}
