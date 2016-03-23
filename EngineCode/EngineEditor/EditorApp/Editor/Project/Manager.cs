using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Serialization;
using System.IO;

namespace EditorApp.Project
{
	public class Manager
	{
		private ProjectSettings				m_projectSettings;
		private UserSettings                m_userSettings;
		private Logic                       m_editorLogic;			///< Referencja na główny obiekt edytora.

		#region Contructor

		public Manager( Logic editorLogic )
		{
			m_projectSettings = new ProjectSettings();
			m_userSettings = new UserSettings();
			m_editorLogic = editorLogic;
		}

		#endregion


		public bool			LoadProject()
		{

			return false;
		}

		public bool			SaveProject()
		{
			m_projectSettings.ProjectName = "MainTest";

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
			}

			return false;
		}

	}
}
