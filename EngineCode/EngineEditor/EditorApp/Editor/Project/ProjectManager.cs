using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Serialization;
using System.IO;
using EditorApp.Editor.Project.Actors;
using EditorApp.Editor.Project.Content;
using EditorApp.Editor.Helpers;

namespace EditorApp.Project
{
	public class ProjectManager
	{
		private ProjectSettings				m_projectSettings;
		private UserSettings                m_userSettings;
		private ActorsLogic                 m_actorsLogic;
		private ContentManager              m_contentManager;


		private Logic                       m_editorLogic;      ///< Referencja na główny obiekt edytora.
		private ProjectView                 m_projectView;

		#region Contructor

		public ProjectManager( Logic editorLogic )
		{
			m_projectSettings = new ProjectSettings();
			m_userSettings = new UserSettings();
			m_actorsLogic = new ActorsLogic( editorLogic );
			m_contentManager = new ContentManager( editorLogic );
			m_editorLogic = editorLogic;

			ProjectView = new ProjectView();
			ProjectView.UpdateProjectSettings( m_projectSettings );
		}

		#endregion


		public bool			LoadProject( string projectFilePath )
		{
			LoadProjectSettings( projectFilePath );
			LoadUserSettings();

			//m_actorsLogic.PostInitLevel();
			m_actorsLogic.PostInitEngine();
			m_contentManager.ResetAssetsRoot( m_editorLogic.PathsManager.AssetsDir );

			return true;
		}

		public bool			SaveProject()
		{
			string projectFilePath = Path.Combine( m_editorLogic.PathsManager.ProjectDir, m_editorLogic.PathsManager.ProjectFileName );
			Serialization.Serialize( projectFilePath, m_projectSettings );
			Serialization.Serialize( m_editorLogic.PathsManager.UserConfigPath, m_userSettings );

			// TEMP
			//m_editorLogic.Displayer.EngineWrapper.SaveLevel( Path.Combine( m_editorLogic.PathsManager.LevelsDir, "TestLevel.swmap" ) );

			return true;
		}

		public bool			LoadProjectSettings	( string projectFilePath )
		{
			bool result = true;

			ProjectSettings newProjSettings = Serialization.Deserialize< ProjectSettings >( projectFilePath );
			if( newProjSettings != null )
			{
				ProjectSettings = newProjSettings;
				result = true;
			}
			else
			{
				ProjectSettings = new ProjectSettings();
				result = false;
			}

			return result;
		}

		public bool			LoadUserSettings	()
		{
			UserSettings newUserSettings = Serialization.Deserialize< UserSettings >( m_editorLogic.PathsManager.UserConfigPath );
			if( newUserSettings != null )
			{
				UserSettings = newUserSettings;
				return true;
			}
			else
			{
				UserSettings = new UserSettings();
				return false;
			}
		}

		#region properties

		public ActorsLogic ActorsLogic
		{
			get
			{
				return m_actorsLogic;
			}
		}

		public ContentManager ContentManager
		{
			get
			{
				return m_contentManager;
			}

			set
			{
				m_contentManager = value;
			}
		}

		public UserSettings UserSettings
		{
			get
			{
				return m_userSettings;
			}

			internal set
			{
				m_userSettings = value;
			}
		}

		public ProjectSettings ProjectSettings
		{
			get
			{
				return m_projectSettings;
			}

			internal set
			{
				m_projectSettings = value;
				ProjectView.UpdateProjectSettings( m_projectSettings );
			}
		}

		public ProjectView ProjectView
		{
			get
			{
				return m_projectView;
			}

			set
			{
				m_projectView = value;
			}
		}

		#endregion

	}
}
