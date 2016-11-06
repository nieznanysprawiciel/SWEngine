using System;
using System.Collections.Generic;
using System.Windows.Controls;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.IO;
using System.Collections.ObjectModel;
using System.Windows.Input;
using EditorPlugin;
using EditorApp.Editor.Commands;
using EditorApp.GUI;

namespace EditorApp.Editor.Project.Actors
{
	public class ActorsLogic : UpdatableViewBase, IDeletableItem
	{
		private List< ActorClassMetaInfo >						m_actorsTypesList;
		private ObservableCollection< ActorWrapper >			m_actors;

		/// <summary>
		/// Lista aktorów, którzy pełnią funkcję pomocniczą dla edytora, ale nie wystąpią w grze. Są to np. wizualne reprezentacje 
		/// świateł, triggerów, gizmo itp.
		/// </summary>
		private List< ActorWrapper >                            m_editorActors;		


		private ActorClassMetaInfo              m_selectedActorMeta;
		private ActorWrapper                    m_selectedActor;
		private GizmoActorWrapper				m_gizmoActor;

		private Logic							m_editorLogic;      ///< Referencja na główny obiekt edytora.

		private ActorsCreatorView               m_actorsCreatorView;


		public ActorsLogic		( Logic editorLogic )
		{
			DisplayName = "Actors Viewer";

			m_editorLogic = editorLogic;
			m_actorsTypesList = null;
			m_editorActors = new List<ActorWrapper>();
			Actors = new ObservableCollection<ActorWrapper>();

			SelectedActor = null;
			SelectedActorMeta = null;
			GizmoActor = null;

			LoadAssetCommand = new RelayCommand( LoadAsset, CanLoadAsset );
			ActorSelectionChangedCommand = new RelayCommand( ActorSelectionChanged );
			DeleteActorCommand = new RelayCommand( DeleteItem );
			ViewActorInNewWindow = new RelayCommand( ViewActor );

			ActorsCreatorView = new ActorsCreatorView( this );
		}

		private void ClearState()
		{
			EngineWrapper engine = m_editorLogic.Displayer.EngineWrapper;
			engine.ClearActors();

			m_actorsTypesList = null;
			m_actors = null;

			m_editorActors = new List<ActorWrapper>();

			SelectedActor = null;
			SelectedActorMeta = null;
			GizmoActor = null;
		}

		public void				UnloadScene()
		{
			ClearState();
		}

		public void				InitDefaultActors()
		{
			// Pobieramy metadane o typach aktorów.
			EngineWrapper engine = m_editorLogic.Displayer.EngineWrapper;
			m_actorsTypesList = engine.CreateActorsMetadata();


			// Tworzymy aktorów pomocniczych.
			var translateGizmoPath = Path.Combine( m_editorLogic.PathsManager.DefaultMeshesDir, m_editorLogic.GlobalSettings.TranslateGizmoAssetFile );
			var rotateGizmoPath = Path.Combine( m_editorLogic.PathsManager.DefaultMeshesDir, m_editorLogic.GlobalSettings.RotateGizmoAssetFile );
			var scaleGizmoPath = Path.Combine( m_editorLogic.PathsManager.DefaultMeshesDir, m_editorLogic.GlobalSettings.ScaleGizmoAssetFile );


			GizmoActor = EditorActorsFactory.CreateGizmoActor( translateGizmoPath, rotateGizmoPath, scaleGizmoPath );
			m_editorActors.Add( GizmoActor );

			var camera = EditorActorsFactory.CreateDefaultCamera( true );
			m_editorActors.Add( camera );
		}

		public void				PostInitLevel( SceneLoader loader )
		{
			// Trzeba dodać aktorów silnikowych.
			InitDefaultActors();

			// Pobieramy aktorów ze sceny
			m_actors = loader.GetLoadedActors();

			// Przypisujemy nazwy aktorom.
			int actorCounter = 0;
			foreach( var actor in m_actors )
			{
				// Give names to actors without name.
				if( actor.ActorName == "" )
				{
					actor.ActorName = actor.GetTypeName() + actorCounter.ToString();
					++actorCounter;
				}
			}

			OnPropertyChanged( "Actors" );
		}

		public void				SelectActor( ActorWrapper actor )
		{
			SelectedActor = actor;
			OnPropertyChanged( "SelectedActor" );

			ActorSelectionChanged( actor );
		}

		public void				ActorSelectionChanged( object parameter )
		{
			if( parameter != null )
			{
				var actor = parameter as ActorWrapper;
				m_editorLogic.Displayer.EngineWrapper.SelectActor( GizmoActor, actor );

				SelectedActorMeta = new ActorClassMetaInfo( actor );
				OnPropertyChanged( "SelectedActorMeta" );
			}
			else
			{
				// Nie ma wybranego aktora, więc nie wyświetlamy jego danych.
				SelectedActorMeta = null;
				OnPropertyChanged( "SelectedActorMeta" );
			}

		}

		public ActorWrapper		CreateMeshActor	( string meshPath, double mouseX, double mouseY )
		{
			if( meshPath != null )
			{
				string meshActorName = m_editorLogic.ProjectManager.UserSettings.DefaultMeshActorTypeName;
				ActorWrapper newActor = CreateActor( meshActorName, mouseX, mouseY );
				if( newActor != null )
				{
					//string filePath = PathsManager.RelativePath( m_editorLogic.PathsManager.AssetsDir, meshPath );
					newActor.LoadMesh( meshPath );
				}
			}
			return null;
		}

		public ActorWrapper		CreateActor		( ActorClassMetaInfo actorInfo, double mouseX, double mouseY )
		{
			if( actorInfo != null )
				return CreateActor( actorInfo.TypeName, mouseX, mouseY );
			return null;
		}

		public void				DeleteActor		( ActorWrapper actor )
		{
			SelectActor( null );
			m_actors.Remove( actor );

			m_editorLogic.Displayer.EngineWrapper.DeleteActor( actor );
		}

		public ActorWrapper		CreateActor		( string actorTypeName, double mouseX, double mouseY )
		{
			if( actorTypeName != null )
			{
				EngineWrapper engine = m_editorLogic.Displayer.EngineWrapper;
				var newActor = engine.CreateActor( actorTypeName, mouseX, mouseY );

				newActor.ActorName = newActor.TypeName + Actors.Count.ToString();
				Actors.Add( newActor );

				SelectActor( newActor );
				return newActor;
			}
			return null;
		}

		public void				LoadAsset		( object parameter )
		{
			ResourcePropertyWrapper resource = parameter as ResourcePropertyWrapper;
			var assetPath = m_editorLogic.ProjectManager.ContentManager.GetSelectedFilePath();

			if( assetPath == null )
				throw new ArgumentException( "There's no selected asset" );

			SelectedActor.LoadMesh( assetPath );
		}

		private bool CanLoadAsset	( object parameter )
		{
			ResourcePropertyWrapper resource = parameter as ResourcePropertyWrapper;

			if( m_editorLogic.ProjectManager.ContentManager.SelectedFile == null )
				return false;

			if( m_editorLogic.ProjectManager.ContentManager.SelectedFile.Type == Content.FileTreeNodeType.Directory )
				return false;

			if( resource.ResourceType == ResourcePropertyType.Mesh &&
				m_editorLogic.ProjectManager.ContentManager.SelectedFile.Type == Content.FileTreeNodeType.Model3D )
				return true;

			if( resource.ResourceType == ResourcePropertyType.Shader &&
				m_editorLogic.ProjectManager.ContentManager.SelectedFile.Type == Content.FileTreeNodeType.Shader )
				return true;

			if( resource.ResourceType == ResourcePropertyType.Texture &&
				m_editorLogic.ProjectManager.ContentManager.SelectedFile.Type == Content.FileTreeNodeType.Texture )
				return true;

			if( resource.ResourceType == ResourcePropertyType.Buffer &&
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

		private void ViewActor		( object actor )
		{
			ActorClassMetaInfo actorMeta = new ActorClassMetaInfo( SelectedActor );
			ActorView newView = new ActorView( actorMeta );

			m_editorLogic.MainPanelView.Add( newView );
		}

		private void DeleteItem		( object item )
		{
			DeleteActor( SelectedActor );
		}

		void IDeletableItem.DeleteItem( object item )
		{
			DeleteActor( item as ActorWrapper );
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

		public ICommand DeleteActorCommand
		{
			get;
			internal set;
		}

		public ICommand ViewActorInNewWindow
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

		public GizmoActorWrapper GizmoActor
		{
			get
			{
				return m_gizmoActor;
			}

			set
			{
				m_gizmoActor = value;
				OnPropertyChanged( "GizmoActor" );
			}
		}

		#endregion

	}
}
