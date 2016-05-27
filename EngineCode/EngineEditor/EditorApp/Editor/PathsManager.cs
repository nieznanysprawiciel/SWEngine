using System.IO;
using System.Text;
using System;

namespace EditorApp
{
	public class PathsManager
	{
		string          m_editorDir;
		string          m_defaultAssetsDir;

		string          m_projectDir;
		string          m_assetsDir;
		string          m_projectFileName;
		string          m_sourceCodeDir;
		string          m_visualProjectsDir;

		// Relative directories
		string          m_shadersRelativeDir;
		string          m_levelsRelativeDir;
		string          m_texturesRelativeDir;
		string          m_animationsRelativeDir;
		string          m_meshesRelativeDir;

		string          m_globalConfigRelativePath;


		public void			InitPaths( string[] cmdArguments )
		{
			InitDefaultRelativesPaths();

			string appPath = cmdArguments[ 0 ];

			if( appPath.Contains( "EngineBuildDir" ) )
			{
				// Hack for Debug versions.
				// @todo Determine this in other way. EngineBuildDir can change or someone can place editor in directory
				// under this name.
				EditorDir = Path.GetDirectoryName( appPath );
				EditorDir = Path.GetFullPath( Path.Combine( EditorDir, "../../EditorRelease/" ) );
			}
			else
				EditorDir = Path.GetDirectoryName( appPath );       // First argument is application path.

			DefaultAssetsDir = Path.GetFullPath( Path.Combine( EditorDir, "../GameRelease/" ) );

			Directory.SetCurrentDirectory( DefaultAssetsDir );

			if( cmdArguments.Length >= 2 )
			{
				string projectFilePath = cmdArguments[ 1 ];
				ProjectDir = Path.GetDirectoryName( projectFilePath );
				ProjectFileName = Path.GetFileName( projectFilePath );
			}
			else
			{
				ProjectDir = "";
				ProjectFileName = "";
			}
		}

		void InitDefaultRelativesPaths()
		{
			m_shadersRelativeDir = "shaders";
			m_levelsRelativeDir = "levels";
			m_texturesRelativeDir = "textures";
			m_animationsRelativeDir = "animations";
			m_meshesRelativeDir = "meshes";

			m_globalConfigRelativePath = "Configuration/GlobalConfig.xml";
		}

		public void			UpdateProjectPaths( string projectPath )
		{
			ProjectDir = Path.GetDirectoryName( projectPath );
			ProjectFileName = Path.GetFileName( projectPath );
			m_assetsDir = Path.Combine( ProjectDir, "GameRelease" );
			m_sourceCodeDir = Path.Combine( ProjectDir, "ProjectCode" );
			m_visualProjectsDir = Path.Combine( ProjectDir, "VisualProjects" );
		}

		// http://mrpmorris.blogspot.com/2007/05/convert-absolute-path-to-relative-path.html
		// Peter Morris 
		static public string RelativePath( string absolutePath, string relativeTo )
		{
			string[] absoluteDirectories = absolutePath.Split('\\');
			string[] relativeDirectories = relativeTo.Split('\\');

			//Get the shortest of the two paths
			int length = absoluteDirectories.Length < relativeDirectories.Length ? absoluteDirectories.Length : relativeDirectories.Length;

			//Use to determine where in the loop we exited
			int lastCommonRoot = -1;
			int index;

			//Find common root
			for( index = 0; index < length; index++ )
				if( absoluteDirectories[ index ] == relativeDirectories[ index ] )
					lastCommonRoot = index;
				else
					break;

			//If we didn't find a common prefix then throw
			if( lastCommonRoot == -1 )
				throw new ArgumentException( "Paths do not have a common base" );

			//Build up the relative path
			StringBuilder relativePath = new StringBuilder();

			//Add on the ..
			for( index = lastCommonRoot + 1; index < absoluteDirectories.Length; index++ )
				if( absoluteDirectories[ index ].Length > 0 )
					relativePath.Append( "..\\" );

			//Add on the folders
			for( index = lastCommonRoot + 1; index < relativeDirectories.Length - 1; index++ )
				relativePath.Append( relativeDirectories[ index ] + "\\" );
			relativePath.Append( relativeDirectories[ relativeDirectories.Length - 1 ] );

			return relativePath.ToString();
		}


		#region GettersSetters

		public string ProjectDir
		{
			get
			{
				return m_projectDir;
			}

			set
			{
				m_projectDir = value;
			}
		}

		/// <summary>
		/// Directory with Editor binaries (Release version).
		/// </summary>
		public string EditorDir
		{
			get
			{
				return m_editorDir;
			}

			set
			{
				m_editorDir = value;
			}
		}

		/// <summary>
		/// Directory, where default editor and engine assets are stored.
		/// </summary>
		public string DefaultAssetsDir
		{
			get
			{
				return m_defaultAssetsDir;
			}

			set
			{
				m_defaultAssetsDir = value;
			}
		}

		public string ProjectFileName
		{
			get
			{
				return m_projectFileName;
			}

			set
			{
				m_projectFileName = value;
			}
		}

		/// <summary>
		/// Project directory with assets.
		/// </summary>
		public string AssetsDir
		{
			get
			{
				return m_assetsDir;
			}

			//set
			//{
			//	m_assetsDir = value;
			//}
		}

		public string GameReleaseDir
		{
			get
			{
				// It's the same directory.
				return m_assetsDir;
			}

			//set
			//{
			//	m_assetsDir = value;
			//}
		}

		public string SourceCodeDir
		{
			get
			{
				return m_sourceCodeDir;
			}

			set
			{
				m_sourceCodeDir = value;
			}
		}

		public string VisualProjectsDir
		{
			get
			{
				return m_visualProjectsDir;
			}

			set
			{
				m_visualProjectsDir = value;
			}
		}

		/// Assets in current project directory
		#region AssetsDirs

		public string ShadersDir
		{
			get
			{
				return Path.Combine( ProjectDir, m_shadersRelativeDir );
			}
		}

		public string LevelsDir
		{
			get
			{
				return Path.Combine( ProjectDir, m_levelsRelativeDir );
			}
		}

		public string TexturesDir
		{
			get
			{
				return Path.Combine( ProjectDir, m_texturesRelativeDir );
			}
		}

		public string AnimationsDir
		{
			get
			{
				return Path.Combine( ProjectDir, m_animationsRelativeDir );
			}
		}

		public string MeshesDir
		{
			get
			{
				return Path.Combine( ProjectDir, m_meshesRelativeDir );
			}
		}
		#endregion


		/// Default assets directories (engine assets)
		#region DefaultAssetsDirs

		public string DefaultShadersDir
		{
			get
			{
				return Path.Combine( DefaultAssetsDir, m_shadersRelativeDir );
			}
		}

		public string DefaultTexturesDir
		{
			get
			{
				return Path.Combine( DefaultAssetsDir, m_texturesRelativeDir );
			}
		}

		public string DefaultAnimationsDir
		{
			get
			{
				return Path.Combine( DefaultAssetsDir, m_animationsRelativeDir );
			}
		}

		public string DefaultMeshesDir
		{
			get
			{
				return Path.Combine( DefaultAssetsDir, m_meshesRelativeDir );
			}
		}
		#endregion


		/// <summary>
		/// Ścieżka do głównego pliku konfiguracyjnego.
		/// </summary>
		public string GlobalConfigPath
		{
			get
			{
				return  Path.Combine( EditorDir, m_globalConfigRelativePath );
			}
		}



		#endregion
	}
}
