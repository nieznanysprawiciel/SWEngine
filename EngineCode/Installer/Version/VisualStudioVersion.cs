using System;
using System.Collections.Generic;
using Microsoft.Win32;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Installer.Version
{
	public class VisualStudioVersion
	{
		const string    MSBuildRegistryPath = "SOFTWARE\\Microsoft\\MSBuild\\ToolsVersions";
		const string    MSBuildPathKeyName = "MSBuildToolsPath";

		VisualStudioVersion()
		{}

		public static List<VisualStudioData> ListVisualVersions()
		{
			List< VisualStudioData > versionData = new List<VisualStudioData>();

			//string[] versionsVS = new string[ 2 ];
			//versionsVS[ 0 ] = "14.0";
			//versionsVS[ 1 ] = "12.0";

			//RegistryKey toolsetKey = GetNewestMSBuildKey( versionsVS );

			//CheckAndAddVSVersion( "14.0", versionData, toolsetKey );
			//CheckAndAddVSVersion( "12.0", versionData, toolsetKey );
			//CheckAndAddVSVersion( "11.0", versionData, toolsetKey );

			CheckAndAddVSVersion( "14.0", versionData );
			CheckAndAddVSVersion( "12.0", versionData );
			CheckAndAddVSVersion( "11.0", versionData );

			return versionData;
		}

		private static void CheckAndAddVSVersion( string toolsetVersion, List<VisualStudioData> versionData )
		{
			toolsetVersion = BuildToolsetVersionName( toolsetVersion );
			string toolsetVersionUpper = toolsetVersion.ToUpper();
			string varName = toolsetVersionUpper + "COMNTOOLS";

			var toolsetPath = Environment.GetEnvironmentVariable( varName );
			if( toolsetPath != null )
				versionData.Add( new VisualStudioData( toolsetPath, toolsetVersion, true ) );
			else
				versionData.Add( new VisualStudioData( "", toolsetVersion, false ) );
		}

		private static void CheckAndAddVSVersion( string toolsetVersion, List<VisualStudioData> versionData, RegistryKey toolsetReg )
		{
			string toolsetVersionName = BuildToolsetVersionName( toolsetVersion );

			if( toolsetReg != null )
			{
				// Sprawdza czy wpis istnieje. Poza tym zmienna toolset nie jest używana.
				RegistryKey toolset = toolsetReg.OpenSubKey( toolsetVersion );
				if( toolset != null )
				{
					string toolsetPath = toolsetReg.GetValue( MSBuildPathKeyName ) as string;
					versionData.Add( new VisualStudioData( toolsetPath, toolsetVersionName, true ) );
					return;
				}
			}
			
			versionData.Add( new VisualStudioData( "", toolsetVersionName, false ) );
		}

		private static RegistryKey GetNewestMSBuildKey( string[] versionsVS )
		{
			RegistryKey MSBuildPath = Registry.LocalMachine.OpenSubKey( MSBuildRegistryPath );

			RegistryKey newestToolset = null;
			foreach( var version in versionsVS )
			{
				newestToolset = MSBuildPath.OpenSubKey( version );
				if( newestToolset != null )
					break;
			}

			return newestToolset;
		}

		private static string BuildToolsetVersionName( string toolsetVersion )
		{
			return "vs" + toolsetVersion.Replace( ".", "" );
		}
	}
}
