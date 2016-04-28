using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.IO;
using System.Windows.Input;
using Microsoft.TeamFoundation.MVVM;


namespace EditorApp.Editor.Project.Content
{
	public class ContentManager
	{
		FileTreeNode		m_fileTreeRoot;
		FileTreeNode        m_selectedFile;

		private Logic		m_editorLogic;      ///< Referencja na główny obiekt edytora.
		



		public			ContentManager		( Logic editorLogic )
		{
			m_editorLogic = editorLogic;
			SelectedAssetChangedCommand = new RelayCommand( ActorSelectionChanged );
			SelectedFile = null;
		}


		public void		ResetAssetsRoot		( string directory )
		{
			FileTreeRoot = new FileTreeNode( "", FileTreeNodeType.Directory );
			FileTreeRoot.BuildTreeFromDir( directory );
		}

		/**Zwraca ścieżkę względem katalogu z assetami. To jest dokładnie ta sama ścieżka
		jaką trzeba podać, aby wczytać podany plik.*/
		public string	GetSelectedFilePath	()
		{
			// @todo Zlikwidować rozwiązanie tymczasowe, po opanowaniu ścieżek w edytorze.
			if( m_selectedFile != null )
				return m_selectedFile.FilePath;

			//if( m_selectedFile != null )
			//	return PathsManager.RelativePath( m_selectedFile.FilePath, m_editorLogic.PathsManager.AssetsDir );

			return null;
		}

		private void	ActorSelectionChanged	( object parameter )
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
