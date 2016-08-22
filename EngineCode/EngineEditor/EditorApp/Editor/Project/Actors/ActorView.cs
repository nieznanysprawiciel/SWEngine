using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using EditorPlugin;



namespace EditorApp.Editor.Project.Actors
{
	class ActorView : UpdatableViewBase
	{
		private ActorClassMetaInfo  m_actorPreview;


		public ActorView( ActorClassMetaInfo actor )
		{
			DisplayName = actor.Actor.ActorName;
			IsRemoveable = true;

			m_actorPreview = actor;
		}

		public ActorClassMetaInfo SelectedActorMeta
		{
			get
			{
				return m_actorPreview;
			}
		}


	}
}
