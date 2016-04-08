using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Serialization;
using System.IO;
using EditorApp.Editor.Project.Actors;

namespace EditorApp.Project
{
	public class Manager
	{
		private ProjectSettings				m_projectSettings;
		private UserSettings                m_userSettings;
		private ActorsLogic                 m_actorsLogic;
		private Logic                       m_editorLogic;			///< Referencja na główny obiekt edytora.

		#region Contructor

		public Manager( Logic editorLogic )
		{
			m_projectSettings = new ProjectSettings();
			m_userSettings = new UserSettings();
			m_actorsLogic = new ActorsLogic( editorLogic );
			m_editorLogic = editorLogic;
		}

		#endregion


		public bool			LoadProject( string projectFilePath )
		{
			try
			{
				XmlSerializer deser = new XmlSerializer( typeof( ProjectSettings ) );

				using( System.IO.FileStream reader = new System.IO.FileStream( projectFilePath, FileMode.Open ) )
				{
					m_projectSettings = deser.Deserialize( reader ) as ProjectSettings;
				}

			}
			catch( Exception e )
			{
				Console.WriteLine( e.ToString() );
				return false;
			}

			m_actorsLogic.InitLevel();

			return true;
		}

		public bool			SaveProject()
		{
			try
			{
				XmlSerializer ser = new XmlSerializer( typeof( ProjectSettings ) );

				string projectFilePath = Path.Combine( m_editorLogic.PathsManager.ProjectDir, m_editorLogic.PathsManager.ProjectFileName );

				using( System.IO.FileStream writer = new System.IO.FileStream( projectFilePath, FileMode.Truncate ) )
				{
					ser.Serialize( writer, m_projectSettings );
				}
			}
			catch( Exception e )
			{
				Console.WriteLine( e.ToString() );
				return false;
			}

			return true;
		}

	}
}
