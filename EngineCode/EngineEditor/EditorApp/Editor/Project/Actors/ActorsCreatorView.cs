using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using sw.EditorPlugin;
using System.Windows.Input;
using EditorApp.GUI;
using EditorApp.Editor.Commands;


namespace EditorApp.Editor.Project.Actors
{
	public class ActorsCreatorView : UpdatableViewBase
	{
		private ActorsLogic         m_logicRef;

		public			ActorsCreatorView	( ActorsLogic actorsLogic )
		{
			DisplayName = "Actors Creator";
			m_logicRef = actorsLogic;

			//CreateActor = new RelayCommand();
		}

		public List<ActorClassMetaInfo> ActorsMetaData
		{
			get
			{
				return m_logicRef.ActorsTypesList;
			}
		}

		//public ICommand CreateActor
		//{
		//	get;
		//	internal set;
		//}

	}
}
