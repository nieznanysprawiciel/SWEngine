using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EditorApp.Project
{
	public class UserSettings
	{
		public string CurrentLevel				{ get; set; }
		public string DefaultMeshActorTypeName	{ get; set; }


		public UserSettings()
		{
			DefaultMeshActorTypeName = "DynamicMeshActor";
		}
	}
}
