using System;
using System.Collections.Generic;
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


		public void PostInitLevel()
		{
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
