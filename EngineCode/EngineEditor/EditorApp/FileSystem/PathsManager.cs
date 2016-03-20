using System.IO;

namespace EditorApp.FileSystem
{
	public class PathsManager
	{
		string          m_editorDir;
		string          m_projectDir;
		string          m_defaultAssetsDir;



		public void			InitPaths( string[] cmdArguments )
		{
			EditorDir = Directory.GetCurrentDirectory();
			DefaultAssetsDir = Path.GetFullPath( Path.Combine( EditorDir, "../GameRelease/" ) );

			Directory.SetCurrentDirectory( DefaultAssetsDir );

			ProjectDir = "";
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

		#endregion
	}
}
