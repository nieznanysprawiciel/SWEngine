using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Runtime.InteropServices;
using System.Windows.Interop;
using System.Windows;
using System.Windows.Input;
using Microsoft.Win32;
using EditorApp.Editor;
using EditorApp.Editor.Commands;
using EditorPlugin;


namespace EditorApp
{
	enum EditorState
	{
		Uninitialized
		
	}


	public class Logic
	{
        public PathsManager             PathsManager { get; internal set; }
        public GlobalSettings           GlobalSettings { get; internal set; }
        public Project.ProjectManager   ProjectManager { get; internal set; }
        public Engine.Displayer         Displayer { get; internal set; }

		private ObservableCollection< UpdatableViewBase >        m_leftPanelView;
		private ObservableCollection< UpdatableViewBase >        m_rightPanelView;
		private ObservableCollection< UpdatableViewBase >        m_mainPanelView;

		/// Dobry design aplikacji nie powinien zawierać żadnego obiektu GUI.
		/// Trzeba to traktować jak rozwiązanie tymczasowe.
		public MainWindow MainWindowRef { get; internal set; }


		public				Logic		( MainWindow windowRef )
		{
			LeftPanelView = new ObservableCollection< UpdatableViewBase >();
			RightPanelView = new ObservableCollection< UpdatableViewBase >();
			MainPanelView = new ObservableCollection< UpdatableViewBase >();

			MainWindowRef = windowRef;

			PathsManager = new PathsManager();
			GlobalSettings = new GlobalSettings();
			ProjectManager = new Project.ProjectManager( this );
			Displayer = new Engine.Displayer( this );

			LeftPanelView.Add( ProjectManager.ContentManager );
			LeftPanelView.Add( ProjectManager.ActorsLogic.ActorsCreatorView );
			RightPanelView.Add( ProjectManager.ActorsLogic );
			RightPanelView.Add( ProjectManager.ProjectView );
			MainPanelView.Add( Displayer );

			LoadCommand = new RelayCommand( LoadClick );
			SaveCommand = new RelayCommand( SaveClick );
			NewLevelCommand = new RelayCommand( NewLevelClick );
			SaveLevelCommand = new RelayCommand( SaveLevelClick );
			LoadLevelCommand = new RelayCommand( LoadLevelClick );
			SaveGlobalConfig = new RelayCommand( SaveGlobalConfigImpl );
			CloseTabCommand = new RelayCommand( CloseTab );
		}

		public bool			Init		( string[] cmdArgs )
		{
			PathsManager.InitPaths( cmdArgs );
			GlobalSettings = Editor.Helpers.Serialization.Deserialize< GlobalSettings >( PathsManager.GlobalConfigPath );

			if( !Displayer.InitRenderer() )
				return false;

			string projectPath = ParseCmdLineProject( cmdArgs );
			if( projectPath != null )
				return LoadProject( projectPath );

			return true;
		}

		public bool			LoadProject		( string projectFile )
		{
			PathsManager.UpdateProjectPaths( projectFile );
			bool result = ProjectManager.LoadProject( projectFile );

			return result;
		}

		public void			SaveClick		( object parameter )
		{
			ProjectManager.SaveProject();
		}

		public void			LoadClick		( object parameter )
		{
			OpenFileDialog fileDialog = new OpenFileDialog();
			fileDialog.CheckFileExists = true;
			fileDialog.CheckPathExists = true;

			fileDialog.DefaultExt = ".swproj";
			fileDialog.Filter = "SW Engine Projects (*.swproj)|*.swproj;";

			Nullable<bool> result = fileDialog.ShowDialog();

			if( result.HasValue && result.Value )
			{
				//var directory = Path.GetDirectoryName( fileDialog.FileName );
				LoadProject( fileDialog.FileName );
			}
		}

		public void			SaveLevelClick	( object parameter )
		{
			SceneSaver saver = new SceneSaver();
			saver.ActorsToSave( ProjectManager.ActorsLogic.Actors );

			saver.SaveScene( Path.Combine( PathsManager.LevelsDir, "Test2Level.swmap" ) );
		}

		public void			LoadLevelClick	( object parameter )
		{
			ProjectManager.ActorsLogic.UnloadScene();

			SceneLoader loader = new SceneLoader();
			bool result = loader.LoadScene( Path.Combine( PathsManager.LevelsDir, "Test2Level.swmap" ) );

			if( result )
				ProjectManager.ActorsLogic.PostInitLevel( loader );
		}

		public void			NewLevelClick	( object parameter )
		{

		}

		public void			CloseTab		( object parameter )
		{
			UpdatableViewBase view = parameter as UpdatableViewBase;

			if( view != null && view.IsRemoveable )
			{
				LeftPanelView.Remove( view );
				MainPanelView.Remove( view );
				RightPanelView.Remove( view );
			}
		}

		public void			RenderFrame		( object sender, EventArgs e )
		{
			Displayer.RenderFrame();
		}

		public void			CloseApplication()
		{
			Displayer.ReleaseRenderer();
		}

		#region PrivateFunctions

		private string		ParseCmdLineProject( string[] cmdArgs )
		{
			if( cmdArgs.Length == 1 )
				return null;

			string projectPath = cmdArgs[ 1 ];
			string fileExtension = Path.GetExtension( projectPath );
			if( fileExtension == GlobalSettings.ProjectFileExt )
				return projectPath;

			return null;
		}


		#endregion

		#region Properties

		public ObservableCollection<UpdatableViewBase> LeftPanelView
		{
			get
			{
				return m_leftPanelView;
			}

			internal set
			{
				m_leftPanelView = value;
			}
		}

		public ObservableCollection<UpdatableViewBase> RightPanelView
		{
			get
			{
				return m_rightPanelView;
			}

			internal set
			{
				m_rightPanelView = value;
			}
		}

		public ObservableCollection<UpdatableViewBase> MainPanelView
		{
			get
			{
				return m_mainPanelView;
			}

			internal set
			{
				m_mainPanelView = value;
			}
		}

		public ICommand SaveCommand
		{
			get;
			internal set;
		}

		public ICommand LoadCommand
		{
			get;
			internal set;
		}

		public ICommand SaveLevelCommand
		{
			get;
			internal set;
		}

		public ICommand LoadLevelCommand
		{
			get;
			internal set;
		}

		public ICommand NewLevelCommand
		{
			get;
			internal set;
		}

		public ICommand CloseTabCommand
		{
			get;
			internal set;
		}

		/// <summary>
		/// Komenda tymczasowa. Zapisuje GlobalSettings do pliku.
		/// </summary>
		public ICommand SaveGlobalConfig
		{
			get;
			internal set;
		}

		//public ICommand ExitCommand
		//{
		//	get;
		//	internal set;
		//}

		#endregion

		#region TemporaryFunctions

		private void SaveGlobalConfigImpl( object parameter )
		{
			Editor.Helpers.Serialization.Serialize( PathsManager.GlobalConfigPath, GlobalSettings );
		}

		#endregion
	}
}
