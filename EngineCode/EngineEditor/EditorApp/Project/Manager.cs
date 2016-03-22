using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Serialization;

namespace EditorApp.Project
{
	public class Manager
	{
		private ProjectSettings				m_projectSettings;
		private UserSettings                m_userSettings;


		#region Contructor

		public Manager()
		{
			m_projectSettings = new ProjectSettings();
			m_userSettings = new UserSettings();
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

				System.IO.StringWriter writer = new System.IO.StringWriter();

				ser.Serialize( writer, m_projectSettings );
				Console.WriteLine( writer.ToString() );
				//Console.ReadLine();
			}
			catch( Exception e )
			{

				Console.WriteLine( e.ToString() );
			}

			return false;
		}

	}
}
