using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;
using System.IO;
using System.Collections.ObjectModel;


namespace EditorApp.Editor.Project.Content
{

	public enum FileTreeNodeType
	{
		Directory,
		Model3D,
		Shader,
		UnknownFileType
	}


	public class FileTreeNode : INotifyPropertyChanged
	{
		ObservableCollection< FileTreeNode >				m_folderContent;
		string												m_name;
		FileTreeNodeType									m_type;

		public event PropertyChangedEventHandler			PropertyChanged;




		public FileTreeNode( string name, FileTreeNodeType type )
		{
			Name = name;
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
					var dirNode = new FileTreeNode( Path.GetFileName( dir ), FileTreeNodeType.Directory );
					dirNode.BuildTreeFromDir( Path.Combine( directory, dir ) );
					DirectoryContent.Add( dirNode );
				}

				foreach( var file in files )
				{
					DirectoryContent.Add( new FileTreeNode( Path.GetFileName( file ), GetFileTypeByExtension( file ) ) );
				}
			}
		}

		FileTreeNodeType		GetFileTypeByExtension	( string fileName )
		{
			string extension = Path.GetExtension( fileName ).ToLower();
			if( extension == ".fbx" )
				return FileTreeNodeType.Model3D;
			else if( extension == ".hlsl"
				|| extension == ".glsl"
				|| extension == ".fx" )
				return FileTreeNodeType.Shader;
			else
				return FileTreeNodeType.UnknownFileType;
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
				return m_name;
			}

			set
			{
				m_name = value;
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
