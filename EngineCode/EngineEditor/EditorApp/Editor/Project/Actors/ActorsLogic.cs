using System;
using System.Collections.Generic;
using System.Windows.Controls;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using EditorPlugin;

namespace EditorApp.Editor.Project.Actors
{
	public class ActorsLogic
	{
		private List<ActorClassMetaInfo >		m_actorsTypesList;
		private List<ActorWrapper>				m_actors;

		private Logic							m_editorLogic;      ///< Referencja na główny obiekt edytora.


		public ActorsLogic		( Logic editorLogic )
		{
			m_editorLogic = editorLogic;
			m_actorsTypesList = null;
			Actors = new List<ActorWrapper>();
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

			foreach( var actorClass in m_actorsTypesList )
			{
				if( actorClass.Type == actor.Type )
				{
					actorClass.ResetActor( actor );
					m_editorLogic.MainWindowRef.ActorPreview.DataContext = null;
					m_editorLogic.MainWindowRef.ActorPreview.DataContext = actorClass;
				}
			}
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


		#endregion

	}
}
