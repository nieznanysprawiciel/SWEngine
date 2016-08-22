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
		private ResourceMetaInfo     m_asset;


		public AssetView( ResourceMetaInfo asset )
		{
			DisplayName = asset.Resource.ResourceName;
			IsRemoveable = true;

			ObjectData = asset;
		}


		public ResourceMetaInfo ObjectData
		{
			get
			{
				return m_asset;
			}

			internal set
			{
				m_asset = value as ResourceMetaInfo;
			}
		}

	}
}
