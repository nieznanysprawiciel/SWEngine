using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Collections.ObjectModel;
using EditorPlugin;
using System.Windows.Input;

namespace EditorApp.Editor.Project.Content
{
	public class LoadedAssets : UpdatableViewBase
	{
		private Logic       m_editorLogic;      ///< Referencja na główny obiekt edytora.

		private ObservableCollection< ResourceWrapper >     m_textures;

		
		public LoadedAssets( Logic editorLogic )
		{
			DisplayName = "Assets Browser";

			m_editorLogic = editorLogic;
			m_textures = new ObservableCollection<ResourceWrapper>();

			RefreshAssetsCommand = new Commands.RelayCommand( RefreshAssets );
			ViewAssetCommand = new Commands.RelayCommand( ViewAsset );
		}

		public void RefreshAssets	( object parameter )
		{
			Textures = m_editorLogic.Displayer.EngineWrapper.CreateTexturesList();
		}

		public void ViewAsset		( object parameter )
		{
			ResourceWrapper asset = parameter as ResourceWrapper;

			if( asset != null )
			{
				AssetView assetView = new AssetView( asset );
				m_editorLogic.MainPanelView.Add( assetView );
			}
		}



		#region Properties

		public ObservableCollection< ResourceWrapper > Textures
		{
			get
			{
				return m_textures;
			}

			internal set
			{
				m_textures = value;
				OnPropertyChanged( "Textures" );
			}
		}

		public ICommand RefreshAssetsCommand
		{
			get;
			internal set;
		}

		public ICommand ViewAssetCommand
		{
			get;
			internal set;
		}

		#endregion
	}
}
