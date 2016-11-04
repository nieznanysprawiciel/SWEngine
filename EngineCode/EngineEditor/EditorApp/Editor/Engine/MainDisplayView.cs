using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using EditorApp.Editor.Project.Actors;
using EditorApp.Editor;

namespace EditorApp.Engine
{
	public class MainDisplayView : UpdatableViewBase
	{
		MainDisplayer						m_displayer;
		ActorsLogic							m_actorsLogic;


		public MainDisplayView( MainDisplayer displayer, ActorsLogic actorsLog )
		{
			DisplayName = "Preview";

			m_actorsLogic = actorsLog;
			m_displayer = displayer;
		}



		#region Properties
		public MainDisplayer Displayer
		{
			get
			{
				return m_displayer;
			}
		}

		public ActorsLogic ActorsLogic
		{
			get
			{
				return m_actorsLogic;
			}
		}


		#endregion
	}
}
