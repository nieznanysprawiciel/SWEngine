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
		private Logic                       m_editorLogic;		///< Referencja na główny obiekt edytora.

		#region Contructor

		public ProjectManager( Logic editorLogic )
		{
			m_projectSettings = new ProjectSettings();
			m_userSettings = new UserSettings();
			m_actorsLogic = new ActorsLogic( editorLogic );
			m_contentManager = new ContentManager( editorLogic );
			m_editorLogic = editorLogic;
		}

		#endregion


		public bool			LoadProject( string projectFilePath )
		{
			m_projectSettings = Serialization.Deserialize<ProjectSettings>( projectFilePath );

			//m_actorsLogic.PostInitLevel();
			m_contentManager.ResetAssetsRoot( m_editorLogic.PathsManager.AssetsDir );

			return true;
		}

		public bool			SaveProject()
		{
			string projectFilePath = Path.Combine( m_editorLogic.PathsManager.ProjectDir, m_editorLogic.PathsManager.ProjectFileName );
			Serialization.Serialize( projectFilePath, m_projectSettings );

			// TEMP
			//m_editorLogic.Displayer.EngineWrapper.SaveLevel( Path.Combine( m_editorLogic.PathsManager.LevelsDir, "TestLevel.swmap" ) );

			return true;
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

			set
			{
				m_userSettings = value;
			}
		}

		#endregion

	}
}
