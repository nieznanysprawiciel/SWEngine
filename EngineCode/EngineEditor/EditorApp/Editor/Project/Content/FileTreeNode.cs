using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;
using System.IO;
using System.Collections.ObjectModel;
using EditorApp.GUI;


namespace EditorApp.Editor.Project.Content
{

	public enum FileTreeNodeType
	{
		Directory,
		Model3D,
		Shader,
		Texture,
		Buffer,
		UnknownFileType
	}


	public class FileTreeNode : INotifyPropertyChanged, IDragable
	{
		ObservableCollection< FileTreeNode >				m_folderContent;
		string												m_nodePath;
		FileTreeNodeType									m_type;

		public event PropertyChangedEventHandler			PropertyChanged;




		public FileTreeNode( string path, FileTreeNodeType type )
		{
			m_nodePath = path;
			Type = type;
			m_folderContent = new ObservableCollection< FileTreeNode >();
		}

		public void		BuildTreeFromDir	( string directory )
		{
			if( Directory.Exists( directory ) )
			{
				var directories = Directory.EnumerateDirectories( directory );
				var files = Directory.EnumerateFiles( directory );

				foreach( var dir in directories )
				{
					var dirNode = new FileTreeNode( dir, FileTreeNodeType.Directory );
					dirNode.BuildTreeFromDir( Path.Combine( directory, dir ) );
					DirectoryContent.Add( dirNode );
				}

				foreach( var file in files )
				{
					DirectoryContent.Add( new FileTreeNode( file, GetFileTypeByExtension( file ) ) );
				}
			}
		}

		FileTreeNodeType		GetFileTypeByExtension	( string fileName )
		{
			string extension = Path.GetExtension( fileName ).ToLower();
			if( extension == ".fbx" )
				return FileTreeNodeType.Model3D;
			else if( extension == ".bmp"
				|| extension == ".jpg"
				|| extension == ".png" )
				return FileTreeNodeType.Texture;
			else if( extension == ".hlsl"
				|| extension == ".glsl"
				|| extension == ".fx" )
				return FileTreeNodeType.Shader;
			else
				return FileTreeNodeType.UnknownFileType;
		}


		Type IDragable.DataType
		{
			get
			{
				return typeof( FileTreeNode );
			}
		}

		void IDragable.Remove( object i )
		{
			throw new NotImplementedException();
		}

		#region Properties
		public ObservableCollection<FileTreeNode> DirectoryContent
		{
			get { return m_folderContent; }
			set
			{
				m_folderContent = value;
				OnPropertyChanged( "DirectoryContent" );
			}
		}

		public string Name
		{
			get
			{
				return Path.GetFileName( m_nodePath );
			}
		}

		public FileTreeNodeType Type
		{
			get
			{
				return m_type;
			}

			set
			{
				m_type = value;
			}
		}

		/// <summary>
		/// Ścieżka względem wybranego katalogu nadrzędnego.
		/// </summary>
		public string FilePath
		{
			get
			{
				return m_nodePath;
			}

			set
			{
				m_nodePath = value;
			}
		}


		// Create the OnPropertyChanged method to raise the event
		protected void OnPropertyChanged( string name )
		{
			PropertyChangedEventHandler handler = PropertyChanged;
			if( handler != null )
			{
				handler( this, new PropertyChangedEventArgs( name ) );
			}
		}


		#endregion
	}


}
