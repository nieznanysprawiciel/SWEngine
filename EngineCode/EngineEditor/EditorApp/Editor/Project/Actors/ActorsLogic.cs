using System;
using System.Collections.Generic;
using System.Windows.Controls;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Collections.ObjectModel;
using System.Windows.Input;
using EditorPlugin;
using EditorApp.Editor.Commands;


namespace EditorApp.Editor.Project.Actors
{
	public class ActorsLogic : UpdatableViewBase
	{
		private List< ActorClassMetaInfo >						m_actorsTypesList;
		private ObservableCollection< ActorWrapper >			m_actors;

		private ActorClassMetaInfo              m_selectedActorMeta;
		private ActorWrapper                    m_selectedActor;

		private Logic							m_editorLogic;      ///< Referencja na główny obiekt edytora.

		private ActorsCreatorView               m_actorsCreatorView;


		public ActorsLogic		( Logic editorLogic )
		{
			DisplayName = "Actors Viewer";

			m_editorLogic = editorLogic;
			m_actorsTypesList = null;
			Actors = new ObservableCollection<ActorWrapper>();
			SelectedActor = null;
			SelectedActorMeta = null;

			LoadAssetCommand = new RelayCommand( LoadAsset, CanLoadAsset );
			ActorSelectionChangedCommand = new RelayCommand( ActorSelectionChanged );

			ActorsCreatorView = new ActorsCreatorView( this );
		}

		private void ClearState()
		{
			m_actorsTypesList = null;
			m_actors = null;
		}

		public void PostInitLevel()
		{
			ClearState();

			EngineWrapper engine = m_editorLogic.Displayer.EngineWrapper;
			m_actorsTypesList = engine.CreateActorsMetadata();
			m_actors = engine.CreateActorsList();


			int actorCounter = 0;
			foreach( var actor in m_actors )
			{
				actor.ActorName = actor.GetTypeName() + actorCounter.ToString();
				++actorCounter;
			}

			OnPropertyChanged( "Actors" );
		}

		public void ActorSelectionChanged( object parameter )
		{
			var actor = parameter as ActorWrapper;
			SelectedActor = actor;

			foreach( var actorClass in m_actorsTypesList )
			{
				if( actorClass.Type == actor.Type )
				{
					actorClass.ResetActor( actor );

					// Awfull code. How to do this better ?
					SelectedActorMeta = null;
					OnPropertyChanged( "SelectedActorMeta" );
					SelectedActorMeta = actorClass;
					OnPropertyChanged( "SelectedActorMeta" );
				}
			}
		}


		public void CreateActor		( object parameter )
		{
			var eventArg = parameter as RoutedEventHandler;
			//eventArg.
		}

		public void CreateActor		( ActorClassMetaInfo actorInfo )
		{
			EngineWrapper engine = m_editorLogic.Displayer.EngineWrapper;
			var newActor = engine.CreateActor( actorInfo.TypeName, 0, 0 );

			newActor.ActorName = newActor.TypeName + Actors.Count.ToString();
			Actors.Add( newActor );
		}

		public void	LoadAsset		( object parameter )
		{
			ResourceObjectPropertyWrapper resource = parameter as ResourceObjectPropertyWrapper;
			var assetPath = m_editorLogic.ProjectManager.ContentManager.GetSelectedFilePath();

			if( assetPath == null )
				throw new ArgumentException( "There's no selected asset" );

			SelectedActor.LoadMesh( assetPath );
		}

		private bool CanLoadAsset	( object parameter )
		{
			ResourceObjectPropertyWrapper resource = parameter as ResourceObjectPropertyWrapper;

			if( m_editorLogic.ProjectManager.ContentManager.SelectedFile == null )
				return false;

			if( m_editorLogic.ProjectManager.ContentManager.SelectedFile.Type == Content.FileTreeNodeType.Directory )
				return false;

			if( resource.ResourceType == ResourcePropertyType.PropertyMesh &&
				m_editorLogic.ProjectManager.ContentManager.SelectedFile.Type == Content.FileTreeNodeType.Model3D )
				return true;

			if( resource.ResourceType == ResourcePropertyType.PropertyShader &&
				m_editorLogic.ProjectManager.ContentManager.SelectedFile.Type == Content.FileTreeNodeType.Shader )
				return true;

			if( resource.ResourceType == ResourcePropertyType.PropertyTexture &&
				m_editorLogic.ProjectManager.ContentManager.SelectedFile.Type == Content.FileTreeNodeType.Texture )
				return true;

			if( resource.ResourceType == ResourcePropertyType.PropertyBuffer &&
				m_editorLogic.ProjectManager.ContentManager.SelectedFile.Type == Content.FileTreeNodeType.Buffer )
				return true;

			return false;
		}

		public void FindAsset		( object parameter )
		{

		}

		private bool CanFindAsset	( object parameter )
		{
			return true;
		}

		#region Properties

		public List<ActorClassMetaInfo> ActorsTypesList
		{
			get	{	return m_actorsTypesList;	}
		}

		public ObservableCollection<ActorWrapper> Actors
		{
			get
			{
				return m_actors;
			}

			set
			{
				m_actors = value;
			}
		}

		public ActorWrapper SelectedActor
		{
			get
			{
				return m_selectedActor;
			}

			set
			{
				m_selectedActor = value;
			}
		}

		public ActorClassMetaInfo SelectedActorMeta
		{
			get
			{
				return m_selectedActorMeta;
			}

			set
			{
				m_selectedActorMeta = value;
			}
		}

		public ICommand LoadAssetCommand
		{
			get;
			internal set;
		}

		public ICommand ActorSelectionChangedCommand
		{
			get;
			internal set;
		}

		public ActorsCreatorView ActorsCreatorView
		{
			get
			{
				return m_actorsCreatorView;
			}

			internal set
			{
				m_actorsCreatorView = value;
			}
		}

		#endregion

	}
}
