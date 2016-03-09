using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Installer.Version
{
	public class VisualStudioData
	{
		public string		ToolsetPath { set; get; }
		public string		ToolsetName { get; set; }
		public string		ToolsetShort{ get; set; }
		public bool			Exists		{ get; set; }

		public VisualStudioData( string path, string name, bool exists)
		{
			ToolsetPath = path;
			ToolsetName = "Toolset " + name;
			ToolsetShort = name;
			Exists = exists;
		}
	}
}
