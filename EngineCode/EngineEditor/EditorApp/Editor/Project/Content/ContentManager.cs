using System;
using System.Collections.Generic;
using System.IO;
using System.Windows.Input;
using EditorApp.Editor.Commands;



namespace EditorApp.Editor.Project.Content
{
	public class ContentManager : UpdatableViewBase
	{
		FileTreeNode		m_fileTreeRoot;
		FileTreeNode        m_selectedFile;
		LoadedAssets        m_loadedAssets;

		private Logic		m_editorLogic;      ///< Referencja na główny obiekt edytora.
		



		public			ContentManager		( Logic editorLogic )
		{
			DisplayName = "Content Browser";

			m_editorLogic = editorLogic;
			SelectedAssetChangedCommand = new RelayCommand( SelectionChanged );
			SelectedFile = null;

			LoadedAssets = new LoadedAssets( m_editorLogic );
			m_editorLogic.LeftPanelView.Add( m_loadedAssets );
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
			if( m_selectedFile != null )
				return PathsManager.RelativePath( m_editorLogic.PathsManager.AssetsDir, m_selectedFile.FilePath );

			return null;
		}

		private void	SelectionChanged	( object parameter )
		{
			FileTreeNode contentNode = parameter as FileTreeNode;
			if( contentNode != null && contentNode.Type != FileTreeNodeType.Directory )
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
				OnPropertyChanged( "FileTreeRoot" );
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
				OnPropertyChanged( "SelectedFile" );
			}
		}

		public LoadedAssets LoadedAssets
		{
			get
			{
				return m_loadedAssets;
			}

			set
			{
				m_loadedAssets = value;
			}
		}

		#endregion
	}
}
