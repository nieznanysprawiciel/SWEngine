using System.IO;

namespace EditorApp.Editor
{
	public class PathsManager
	{
		string          m_editorDir;
		string          m_defaultAssetsDir;

		string          m_projectDir;
		string          m_projectFileName;


		public void			InitPaths( string[] cmdArguments )
		{
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

		#endregion
	}
}
