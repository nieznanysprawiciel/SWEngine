using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;


namespace EditorApp
{
	public class Logic
	{
		public PathsManager			PathsManager { get; }
		public GlobalSettings		GlobalSettings { get; }
		public Project.Manager		ProjectManager { get; }




		public Logic()
		{
			PathsManager = new PathsManager();
			GlobalSettings = new GlobalSettings();
			ProjectManager = new Project.Manager( this );
		}

		public bool			Init		( string[] cmdArgs )
		{
			PathsManager.InitPaths( cmdArgs );

			string projectPath = ParseCmdLineProject( cmdArgs );
			if( projectPath != null )
				return LoadProject( projectPath );

			return true;
		}

		public bool			LoadProject	( string projectPath )
		{
			PathsManager.UpdateProjectPaths( projectPath );
			return ProjectManager.LoadProject( projectPath );
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
	}
}
