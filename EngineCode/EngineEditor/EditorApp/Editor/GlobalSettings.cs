using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Serialization;

namespace EditorApp
{
	public class GlobalSettings
	{
        [XmlIgnore]
        public string		ProjectFileExt { get; set; }

		// Default assets file names
		public string		GizmoAssetFile { get; set; }



		public GlobalSettings()
        {
            ProjectFileExt = ".swproj";
			GizmoAssetFile = "Gizmo.FBX";

		}


	}
}
