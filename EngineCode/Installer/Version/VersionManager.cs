using System.Collections.Generic;
using Microsoft.Win32;
using System.IO;
using System;
using System.Net;
using System.Text;
using Newtonsoft.Json.Linq;

namespace Installer.Version
{
	public class VersionManager
	{
		List< EngineVersionData >       m_versionData;


		const string		m_engineKeyName			= "Software\\SleepingWombat";
		const string        m_engineRootPathName    = "EngineRootPath";
		const string        m_solutionRelToRoot     = "Projects/SWEngine.sln";

		const string        m_repositoryURL         = "https://api.github.com/repos/nieznanysprawiciel/SWEngine/releases";

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

			RegistryKey SWEngineReg = registryKey.OpenSubKey( m_engineKeyName );
			if ( SWEngineReg == null )
				return versions;

			string[] engineVersions = SWEngineReg.GetSubKeyNames();

			foreach ( var name in engineVersions )
			{
				EngineVersionData newRegData = new EngineVersionData();

				RegistryKey enginePathReg = SWEngineReg.OpenSubKey( name );
				object enginePath = enginePathReg.GetValue( m_engineRootPathName );

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

			string solutionPath = Path.Combine( rootPath, m_solutionRelToRoot );

			if( File.Exists( solutionPath ) )
			{
				EngineVersionData localRepoData = new EngineVersionData();
				localRepoData.Path = rootPath;
				localRepoData.Installed = false;
				localRepoData.Version = "Repository";
				localRepoData.Remote = false;

				versions.Add( localRepoData );
			}

			return versions;
		}

		private List<EngineVersionData> ListRemoteVersions()
		{
			List<EngineVersionData> versions = new List<EngineVersionData>();

			HttpWebRequest releasesRequest = (HttpWebRequest)HttpWebRequest.Create( "https://api.github.com/repos/nieznanysprawiciel/SWEngine/releases" );
			releasesRequest.UserAgent = "Mozilla/5.0 (compatible; MSIE 10.0; Windows NT 6.2; WOW64; Trident/6.0)";	// Udajemy przeglądarkę.

			HttpWebResponse response = (HttpWebResponse)releasesRequest.GetResponse();

			Stream receiveStream = response.GetResponseStream();
			StreamReader readStream = new StreamReader (receiveStream, Encoding.UTF8);

			string json = readStream.ReadToEnd();
			JArray releasesJson = JArray.Parse(json);

			foreach( var release in releasesJson )
			{
				EngineVersionData remoteVersionData = new EngineVersionData();
				remoteVersionData.RemotePath = (string)release[ "zipball_url" ];
				remoteVersionData.Path = remoteVersionData.RemotePath;
				remoteVersionData.Installed = false;
				remoteVersionData.Version = (string)release[ "tag_name" ];
				remoteVersionData.Remote = true;

				versions.Add( remoteVersionData );
			}

			receiveStream.Close();
			readStream.Close();

			return versions;
		}

		private EngineVersionData FindVersionByVersionName( string versionName, List<EngineVersionData> resultList )
		{
			foreach( var resultData in resultList )
			{
				if( versionName == resultData.Version )
					return resultData;
			}
			return null;
		}

		public EngineVersionData FindVersionByVersionName( string versionName )
		{
			return FindVersionByVersionName( versionName, m_versionData );
		}

		private EngineVersionData FindVersionByPath( string path, List<EngineVersionData> resultList )
		{
			foreach( var resultData in resultList )
			{
				if( ComparePaths( path, resultData.Path ) )
					return resultData;
			}
			return null;
		}

		public EngineVersionData FindVersionByPath( string versionPath )
		{
			return FindVersionByVersionName( versionPath, m_versionData );
		}

		/**@brief Dołącza do listy resultList elementy, których tam jeszcze nie było.*/
		private void CombineVersionsByVersion( ref List<EngineVersionData> resultList, List<EngineVersionData> srcList )
		{
			foreach( EngineVersionData srcData in srcList )
			{
				if ( FindVersionByVersionName( srcData.Version, resultList ) == null )
					resultList.Add( srcData );
			}
		}

		/**@brief Dołącza do listy resultList elementy, których tam jeszcze nie było.*/
		private void CombineVersionsByPath( ref List<EngineVersionData> resultList, List<EngineVersionData> srcList )
		{
			foreach ( EngineVersionData srcData in srcList )
			{
				if ( FindVersionByPath( srcData.Path, resultList ) == null )
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

		public RegistryKey		OpenOrCreateEngineKey()
		{
			RegistryKey registryKey = Registry.CurrentUser;
			RegistryKey SWEngineReg = registryKey.OpenSubKey( m_engineKeyName );

			if( SWEngineReg == null )
				SWEngineReg = registryKey.CreateSubKey( m_engineKeyName );

			return SWEngineReg;
		}

		public bool				RegisterEngineVersion( EngineVersionData versionData )
		{
			RegistryKey engineKey = OpenOrCreateEngineKey();

			RegistryKey versionKey = engineKey.OpenSubKey( versionData.Version );
			if( versionKey != null )
			{
				// Obsługa błędów.
				return false;
			}

			versionKey = Registry.CurrentUser.CreateSubKey( m_engineKeyName + "\\" + versionData.Version );
			//versionKey = engineKey.CreateSubKey( versionData.Version );
			versionKey.SetValue( m_engineRootPathName, versionData.Path );

			return true;
		}

	}
}
