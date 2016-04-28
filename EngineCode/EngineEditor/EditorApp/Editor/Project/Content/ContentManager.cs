using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Windows.Input;
using Microsoft.TeamFoundation.MVVM;


namespace EditorApp.Editor.Project.Content
{
	public class ContentManager
	{
		FileTreeNode		m_fileTreeRoot;

		FileTreeNode        m_selectedFile;



		public		ContentManager()
		{
			SelectedAssetChangedCommand = new RelayCommand( ActorSelectionChanged );
			SelectedFile = null;
		}


		public void		ResetAssetsRoot		( string directory )
		{
			FileTreeRoot = new FileTreeNode( "", FileTreeNodeType.Directory );
			FileTreeRoot.BuildTreeFromDir( directory );
		}

		public string	GetSelectedFileName	()
		{
			return "";
		}

		private void ActorSelectionChanged( object parameter )
		{
			FileTreeNode contentNode = parameter as FileTreeNode;
			if( contentNode.Type != FileTreeNodeType.Directory )
				SelectedFile = contentNode;
			else
				SelectedFile = null;
		}

		#region Properties
		public FileTreeNode		FileTreeRoot
		{
			get
			{
				return m_fileTreeRoot;
			}

			set
			{
				m_fileTreeRoot = value;
			}
		}

		public ICommand			SelectedAssetChangedCommand
		{
			get;
			internal set;
		}

		public FileTreeNode		SelectedFile
		{
			get
			{
				return m_selectedFile;
			}

			set
			{
				m_selectedFile = value;
			}
		}

		#endregion
	}
}
