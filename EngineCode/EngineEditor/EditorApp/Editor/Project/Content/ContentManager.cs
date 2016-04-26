using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EditorApp.Editor.Project.Content
{
	public class ContentManager
	{
		FileTreeNode		m_fileTreeRoot;


		public		ContentManager()
		{ }


		public void		ResetAssetsRoot		( string directory )
		{
			FileTreeRoot = new FileTreeNode( "", FileTreeNodeType.Directory );
			FileTreeRoot.BuildTreeFromDir( directory );
		}


		#region Properties
		public FileTreeNode FileTreeRoot
		{
			get
			{
				return m_fileTreeRoot;
			}

			set
			{
				m_fileTreeRoot = value;
			}
		} 
		#endregion
	}
}
