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
		private List<ActorClassMetaInfo >       m_actorsTypesList;

		private Logic					m_editorLogic;      ///< Referencja na główny obiekt edytora.


		public ActorsLogic		( Logic editorLogic )
		{
			m_editorLogic = editorLogic;
			m_actorsTypesList = null;
		}


		public void InitLevel()
		{
			EngineWrapper engine = m_editorLogic.Displayer.EngineWrapper;
			m_actorsTypesList = engine.CreateActorsMetadata();
		}

		#region Properties

		public List<ActorClassMetaInfo> ActorsTypesList
		{
			get	{	return m_actorsTypesList;	}
		}


		#endregion

	}
}
