using System;
using System.Collections.Generic;
using System.Windows.Controls;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using Microsoft.TeamFoundation.MVVM;
using EditorPlugin;

namespace EditorApp.Editor.Project.Actors
{
	public class ActorsLogic
	{
		private List<ActorClassMetaInfo >		m_actorsTypesList;
		private List<ActorWrapper>				m_actors;

		private ActorClassMetaInfo              m_selectedActorMeta;
		private ActorWrapper                    m_selectedActor;

		private Logic							m_editorLogic;      ///< Referencja na główny obiekt edytora.


		public ActorsLogic		( Logic editorLogic )
		{
			m_editorLogic = editorLogic;
			m_actorsTypesList = null;
			Actors = new List<ActorWrapper>();
			SelectedActor = null;
			SelectedActorMeta = null;

			LoadAssetCommand = new RelayCommand( LoadAsset, CanLoadAsset );
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
		}

		public void ActorSelectionChanged( object sender, SelectionChangedEventArgs e )
		{
			ListView actorsListView = sender as ListView;
			var selectedIndex = actorsListView.SelectedIndex;

			var actor = m_actors.ElementAt( selectedIndex );
			SelectedActor = actor;

			foreach( var actorClass in m_actorsTypesList )
			{
				if( actorClass.Type == actor.Type )
				{
					actorClass.ResetActor( actor );
					SelectedActorMeta = actorClass;

					m_editorLogic.MainWindowRef.ActorPreview.DataContext = null;
					m_editorLogic.MainWindowRef.ActorPreview.DataContext = this;
				}
			}
		}


		public void	LoadAsset		( object parameter )
		{
			var str = parameter.ToString();
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

		public List<ActorWrapper> Actors
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

		#endregion

	}
}
