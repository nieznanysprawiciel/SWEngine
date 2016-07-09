using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EditorApp.Project
{
	public class ProjectView : EditorApp.Editor.UpdatableViewBase
	{
		private ProjectSettings     m_projectSettings;
		private ProjectManager      m_projManagerRef;


		public ProjectView( ProjectManager manager )
		{
			DisplayName = "Project";
			m_projManagerRef = manager;
			ProjectSettings = manager.ProjectSettings;
		}



		public void UpdateProjectSettings( ProjectSettings projSettings )
		{
			ProjectSettings = projSettings;
			OnPropertyChanged( "ProjectSettings" );
		}

		#region Properties

		public ProjectSettings ProjectSettings
		{
			get
			{
				return m_projectSettings;
			}

			set
			{
				m_projectSettings = value;
			}
		}

		#endregion
	}
}
