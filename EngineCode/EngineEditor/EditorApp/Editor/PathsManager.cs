using System.IO;

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
		}

		public void			UpdateProjectPaths( string projectPath )
		{
			ProjectDir = Path.GetDirectoryName( projectPath );
			ProjectFileName = Path.GetFileName( projectPath );
			m_assetsDir = Path.Combine( ProjectDir, "GameRelease" );
			m_sourceCodeDir = Path.Combine( ProjectDir, "ProjectCode" );
			m_visualProjectsDir = Path.Combine( ProjectDir, "VisualProjects" );
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

		#endregion
	}
}
