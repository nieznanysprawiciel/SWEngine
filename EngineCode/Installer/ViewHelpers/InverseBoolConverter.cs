using System;
using System.Globalization;
using System.Windows.Data;
using System.Windows.Media.Imaging;
using System.Windows;



namespace Installer.ViewHelpers
{
	public class InverseBoolConverter : IValueConverter
	{
		#region IValueConverter Members

		public object Convert( object value, Type targetType, object parameter,
			System.Globalization.CultureInfo culture )
		{
			Nullable< bool > boolVal = value as Nullable< bool >;
			return !boolVal;
		}

		public object ConvertBack( object value, Type targetType, object parameter,
			System.Globalization.CultureInfo culture )
		{
			throw new NotSupportedException();
		}

		#endregion
	}
}
