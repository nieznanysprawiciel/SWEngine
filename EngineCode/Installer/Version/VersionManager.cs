using System.Collections.Generic;
using Microsoft.Win32;
using System.IO;
using System;
using System.Net;
using System.Text;
using Newtonsoft.Json.Linq;

namespace Installer.Version
{
	class VersionManager
	{
		List< EngineVersionData >       m_versionData;

		public VersionManager()
		{
			m_versionData = null;
		}

		public List<EngineVersionData>	GetVersionsList()
		{
			return m_versionData;
		}

		public List<EngineVersionData>	CreateVersionsList()
		{
			m_versionData = ListInstalledVersions();

			string localRepoPath = Path.GetFullPath( Path.Combine( Environment.CurrentDirectory, "../" ) );
			var localVersion = ListLocalVersion( localRepoPath );
			CombineVersionsByPath( ref m_versionData, localVersion );

			var remoteVersions = ListRemoteVersions();
			CombineVersionsByVersion( ref m_versionData, remoteVersions );

			return m_versionData;
		}

		private List<EngineVersionData> ListInstalledVersions()
		{
			List<EngineVersionData> versions = new List<EngineVersionData>();

			RegistryKey registryKey = Registry.CurrentUser;

			RegistryKey SWEngineReg = registryKey.OpenSubKey( "Software\\SchopenhauersWeltschmerz" );
			if ( SWEngineReg == null )
				return versions;

			string[] engineVersions = SWEngineReg.GetSubKeyNames();

			foreach ( var name in engineVersions )
			{
				EngineVersionData newRegData = new EngineVersionData();

				RegistryKey enginePathReg = SWEngineReg.OpenSubKey( name );
				object enginePath = enginePathReg.GetValue( "EngineRootPath" );

				newRegData.Path = enginePath.ToString();   // Normalize path to avoid wrong comparisions
				newRegData.Version = name;
				newRegData.Installed = true;
				newRegData.Remote = false;
				versions.Add( newRegData );
			}

			return versions;
		}

		private List<EngineVersionData> ListLocalVersion( string rootPath )
		{
			List<EngineVersionData> versions = new List<EngineVersionData>();

			string solutionPath = Path.Combine( rootPath, "Projects/SWEngine.sln" );

			if( File.Exists( solutionPath ) )
			{
				EngineVersionData localRepoData = new EngineVersionData();
				localRepoData.Path = rootPath;
				localRepoData.Installed = false;
				localRepoData.Version = "local";
				localRepoData.Remote = false;

				versions.Add( localRepoData );
			}

			return versions;
		}

		private List<EngineVersionData> ListRemoteVersions()
		{
			List<EngineVersionData> versions = new List<EngineVersionData>();

			HttpWebRequest releasesRequest = (HttpWebRequest)HttpWebRequest.Create( "https://api.github.com/repos/nieznanysprawiciel/SWEngine/releases" );
			releasesRequest.UserAgent = "Mozilla/5.0 (compatible; MSIE 10.0; Windows NT 6.2; WOW64; Trident/6.0)";

			HttpWebResponse response = (HttpWebResponse)releasesRequest.GetResponse();

			Stream receiveStream = response.GetResponseStream();
			StreamReader readStream = new StreamReader (receiveStream, Encoding.UTF8);

			string json = readStream.ReadToEnd();
			JArray releasesJson = JArray.Parse(json);

			foreach( var release in releasesJson )
			{
				EngineVersionData remoteVersionData = new EngineVersionData();
				remoteVersionData.Path = (string)release[ "zipball_url" ];
				remoteVersionData.Installed = false;
				remoteVersionData.Version = (string)release[ "tag_name" ];
				remoteVersionData.Remote = true;

				versions.Add( remoteVersionData );
			}

			receiveStream.Close();
			readStream.Close();

			return versions;
		}

		/**@brief Dołącza do listy resultList elementy, których tam jeszcze nie było.*/
		private void CombineVersionsByVersion( ref List<EngineVersionData> resultList, List<EngineVersionData> srcList )
		{
			foreach( EngineVersionData srcData in srcList )
			{
				bool found = false;
				foreach( var resultData in resultList )
				{
					if ( srcData.Version == resultData.Version )
						found = true;
				}

				if ( !found )
					resultList.Add( srcData );
			}
		}

		/**@brief Dołącza do listy resultList elementy, których tam jeszcze nie było.*/
		private void CombineVersionsByPath( ref List<EngineVersionData> resultList, List<EngineVersionData> srcList )
		{
			foreach ( EngineVersionData srcData in srcList )
			{
				bool found = false;
				foreach ( var resultData in resultList )
				{
					if ( ComparePaths( srcData.Path, resultData.Path ) )
						found = true;
				}

				if ( !found )
					resultList.Add( srcData );
			}
		}

		private bool ComparePaths( string path1, string path2 )
		{
			string path1Normalized = Path.GetFullPath( path1 ).TrimEnd( Path.DirectorySeparatorChar, Path.AltDirectorySeparatorChar );
			string path2Normalized = Path.GetFullPath( path2 ).TrimEnd( Path.DirectorySeparatorChar, Path.AltDirectorySeparatorChar );

			var result = String.Compare( path1Normalized, path2Normalized, StringComparison.InvariantCultureIgnoreCase );

			if ( result == 0 )
				return true;
			return false;
		}

	}
}
