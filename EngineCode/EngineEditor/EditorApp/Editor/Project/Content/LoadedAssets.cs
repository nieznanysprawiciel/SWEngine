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

		private ResourcePropertyType                        m_selectedResourceType;

		private ObservableCollection< ResourceWrapper >     m_textures;
		private ObservableCollection< ResourceWrapper >     m_buffers;
		private ObservableCollection< ResourceWrapper >     m_meshes;
		private ObservableCollection< ResourceWrapper >     m_materials;
		private ObservableCollection< ResourceWrapper >     m_shaders;
		private ObservableCollection< ResourceWrapper >     m_renderTargets;

		private ObservableCollection< ResourceWrapper >		m_selectedResource;


		public LoadedAssets( Logic editorLogic )
		{
			DisplayName = "Assets Browser";

			m_editorLogic = editorLogic;
			m_textures		= new ObservableCollection<ResourceWrapper>();
			m_buffers		= new ObservableCollection<ResourceWrapper>();
			m_meshes		= new ObservableCollection<ResourceWrapper>();
			m_materials		= new ObservableCollection<ResourceWrapper>();
			m_shaders		= new ObservableCollection<ResourceWrapper>();
			m_renderTargets = new ObservableCollection<ResourceWrapper>();

			SelectedResourceCollection = m_textures;
			SelectedResourceType = ResourcePropertyType.Texture;

			RefreshAssetsCommand = new Commands.RelayCommand( RefreshAssets );
			ViewAssetCommand = new Commands.RelayCommand( ViewAsset );
		}

		public void RefreshAssets	( object parameter )
		{
			var engine = m_editorLogic.Displayer.EngineWrapper;

			m_textures= engine.CreateTexturesList();
			m_buffers = engine.CreateBuffersList();
			m_meshes = engine.CreateMeshesList();
			m_materials = engine.CreateMaterialsList();
			m_shaders = engine.CreateShadersList();
			m_renderTargets = engine.CreateRenderTargetsList();

			SelectedResourceType = SelectedResourceType;    // Update SelectedResourceCollection this way.
		}

		public void ViewAsset		( object parameter )
		{
			ResourceWrapper asset = parameter as ResourceWrapper;

			if( asset != null )
			{
				ResourceMetaInfo assetMeta = new ResourceMetaInfo( asset );

				AssetView assetView = new AssetView( assetMeta );
				m_editorLogic.MainPanelView.Add( assetView );
			}
		}



		#region Properties

		public ObservableCollection< ResourceWrapper > SelectedResourceCollection
		{
			get
			{
				return m_selectedResource;
			}

			internal set
			{
				m_selectedResource = value;
				OnPropertyChanged( "SelectedResourceCollection" );
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

		public ResourcePropertyType SelectedResourceType
		{
			get
			{
				return m_selectedResourceType;
			}

			set
			{
				m_selectedResourceType = value;
				OnPropertyChanged( "SelectedResourceType" );

				if( m_selectedResourceType == ResourcePropertyType.Buffer )
					SelectedResourceCollection = m_buffers;
				else if( m_selectedResourceType == ResourcePropertyType.Texture )
					SelectedResourceCollection = m_textures;
				else if( m_selectedResourceType == ResourcePropertyType.Material )
					SelectedResourceCollection = m_materials;
				else if( m_selectedResourceType == ResourcePropertyType.Mesh )
					SelectedResourceCollection = m_meshes;
				else if( m_selectedResourceType == ResourcePropertyType.RenderTarget )
					SelectedResourceCollection = m_renderTargets;
				else if( m_selectedResourceType == ResourcePropertyType.Shader )
					SelectedResourceCollection = m_shaders;
				else
					SelectedResourceCollection = null;

			}
		}

		#endregion
	}
}
