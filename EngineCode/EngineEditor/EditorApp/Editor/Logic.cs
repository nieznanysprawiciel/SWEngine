using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

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

			return true;
		}
	}
}
