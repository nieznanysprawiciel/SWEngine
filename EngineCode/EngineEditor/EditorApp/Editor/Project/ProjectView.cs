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



		public ProjectView()
		{
			DisplayName = "Project";
			ProjectSettings = null;
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
