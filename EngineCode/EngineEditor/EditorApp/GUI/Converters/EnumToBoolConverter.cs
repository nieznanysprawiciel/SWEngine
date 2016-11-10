using System;
using System.Globalization;
using System.Windows.Data;
using System.Windows.Media.Imaging;
using System.Windows;
using EditorApp.Editor.Project.Content;

namespace EditorApp.GUI
{
	/// <summary>
	/// http://www.wpftutorial.net/RadioButton.html
	/// </summary>
	class EnumToBoolConverter : IValueConverter
	{
		public object Convert( object value, Type targetType,
							  object parameter, CultureInfo culture )
		{
			if( value == null || parameter == null )
				return false;

			string checkValue = value.ToString();
			string targetValue = parameter.ToString();
			return checkValue.Equals( targetValue,
					 StringComparison.InvariantCultureIgnoreCase );
		}

		public object ConvertBack( object value, Type targetType,
								  object parameter, CultureInfo culture )
		{
			if( value == null || parameter == null )
				return null;

			bool useValue = (bool)value;
			string targetValue = parameter.ToString();
			if( useValue )
				return Enum.Parse( targetType, targetValue );

			return null;
		}
	}
}
