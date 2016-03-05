using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Installer.Version
{
	public class EngineVersionData
	{
		public string	Version		{ get; set; }
		public string	Path		{ get; set; }
		public string	RemotePath	{ get; set; }
		public bool		Installed	{ get; set; }
		public bool		Remote		{ get; set; }
		public bool		Install		{ get; set; }

		public EngineVersionData()
		{
			Install = false;
		}
	}
}
