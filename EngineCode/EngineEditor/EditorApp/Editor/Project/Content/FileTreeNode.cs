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


	public class FileTreeNode : INotifyPropertyChanged
	{
		ObservableCollection< FileTreeNode >				m_folderContent;
		string                                              m_name;

		public event PropertyChangedEventHandler			PropertyChanged;




		public FileTreeNode( string name )
		{
			Name = name;
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
					var dirNode = new FileTreeNode( Path.GetFileName( dir ) );
					dirNode.BuildTreeFromDir( Path.Combine( directory, dir ) );
					DirectoryContent.Add( dirNode );
				}

				foreach( var file in files )
				{
					DirectoryContent.Add( new FileTreeNode( Path.GetFileName( file ) ) );
				}
			}
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
