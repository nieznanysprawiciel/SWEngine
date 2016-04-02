using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Collections;

namespace EditorApp.Editor.Project.Actors.Properties
{
	/**Category node or complex object.*/
	public class GroupProperty	: IProperty
	{
		List< IProperty >       m_propertiesList;


		public List<IProperty> PropertiesList
		{
			get
			{
				return m_propertiesList;
			}

			set
			{
				m_propertiesList = value;
			}
		}
	}
}
