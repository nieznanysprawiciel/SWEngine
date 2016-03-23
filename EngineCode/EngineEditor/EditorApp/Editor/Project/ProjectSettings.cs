using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Collections;


namespace EditorApp.Project
{
	public class ProjectSettings
	{
		private string				m_projectName;
		private List<LevelInfo>		m_levels;



		public ProjectSettings()
		{
			m_levels = new List<LevelInfo>();

			LevelInfo levelInfo1 = new LevelInfo();
			LevelInfo levelInfo2 = new LevelInfo();
			LevelInfo levelInfo3 = new LevelInfo();

			levelInfo1.LevelName = "test1";
			levelInfo2.LevelName = "test2";
			levelInfo3.LevelName = "test3";

			m_levels.Add( levelInfo1 );
			m_levels.Add( levelInfo2 );
			m_levels.Add( levelInfo3 );
		}


		#region GettersSetters

		public string ProjectName
		{
			get
			{
				return m_projectName;
			}

			set
			{
				m_projectName = value;
			}
		}

		public List<LevelInfo> Levels
		{
			get
			{
				return m_levels;
			}

			set
			{
				m_levels = value;
			}
		}

		#endregion
	}
}
