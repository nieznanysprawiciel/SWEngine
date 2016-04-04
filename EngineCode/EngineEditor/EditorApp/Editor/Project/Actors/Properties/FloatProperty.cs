using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EditorApp.Editor.Project.Actors.Properties
{
	class FloatProperty : IProperty
	{
		EditorPlugin.FloatPropertyWrapper   m_propertyWrapper;
		float           m_minValue;
		float           m_maxValue;
		IntPtr			m_actorPtr;



		public float Value
		{
			get
			{
				return m_propertyWrapper.GetValue( m_actorPtr );
			}

			set
			{
				m_propertyWrapper.SetValue( m_actorPtr, value );
			}
		}

		public float MinValue
		{
			get
			{
				return m_minValue;
			}
		}

		public float MaxValue
		{
			get
			{
				return m_maxValue;
			}
		}
	}
}
