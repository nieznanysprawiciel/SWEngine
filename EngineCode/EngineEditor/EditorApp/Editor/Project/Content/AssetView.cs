using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using EditorPlugin;

namespace EditorApp.Editor.Project.Content
{
	public class AssetView : UpdatableViewBase
	{
		private ResourceWrapper     m_asset;


		public AssetView( ResourceWrapper asset )
		{
			DisplayName = asset.ResourceName;

			ObjectData = asset;
		}


		public EngineObjectWrapper ObjectData
		{
			get
			{
				return m_asset;
			}

			internal set
			{
				m_asset = value as ResourceWrapper;
			}
		}
	}
}
