using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Installer.Version
{
	class EngineVersionData
	{
		public string	Version		{ get; set; }
		public string	Path		{ get; set; }
		public bool		Installed	{ get; set; }
		public bool		Remote		{ get; set; }
	}
}
