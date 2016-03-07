using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Installer.Version
{
	public class VisualStudioVersion
	{

		VisualStudioVersion()
		{}

		public static List<VisualStudioData> ListVisualVersions()
		{
			List< VisualStudioData > versionData = new List<VisualStudioData>(); ;

			CheckAndAddVSVersion( "vs140", versionData );
			CheckAndAddVSVersion( "vs120", versionData );
			CheckAndAddVSVersion( "vs110", versionData );

			return versionData;
		}

		private static void CheckAndAddVSVersion( string toolsetVersion, List<VisualStudioData> versionData )
		{
			string toolsetVersionUpper = toolsetVersion.ToUpper();
			string varName = toolsetVersionUpper + "COMNTOOLS";

			var vs110 = Environment.GetEnvironmentVariable( varName );
			if( vs110 != null )
				versionData.Add( new VisualStudioData( vs110, "Toolset " + toolsetVersion, true ) );
			else
				versionData.Add( new VisualStudioData( "", "Toolset " + toolsetVersion, false ) );
		}
	}
}
