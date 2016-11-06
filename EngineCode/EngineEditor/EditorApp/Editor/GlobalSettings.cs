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
		[XmlIgnore]
		public string		LevelMapFileExt { get; set; }

		// Default assets file names
		public string		TranslateGizmoAssetFile { get; set; }
		public string		RotateGizmoAssetFile { get; set; }
		public string		ScaleGizmoAssetFile { get; set; }


		public GlobalSettings()
        {
			LevelMapFileExt = ".swmap";
			ProjectFileExt = ".swproj";
			TranslateGizmoAssetFile = "TranslateGizmo.FBX";
			RotateGizmoAssetFile = "RotateGizmo.FBX";
			ScaleGizmoAssetFile = "ScaleGizmo.FBX";
		}


	}
}
