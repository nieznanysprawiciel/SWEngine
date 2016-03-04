using System;
using System.Globalization;
using System.Windows.Data;
using System.Windows.Media.Imaging;
using System.Windows;


namespace Installer.ViewHelpers
{
	class InstalledToImageConverter : IValueConverter
	{
		public object Convert		( object value, Type targetType, object parameter, CultureInfo culture )
		{
			Nullable< bool > installed = value as Nullable< bool >;

			if ( installed.Value )
				return Application.Current.FindResource( "OkIcon" ) as BitmapImage;
			else
				return Application.Current.FindResource( "FalseIcon" ) as BitmapImage;
		}

		public object ConvertBack	( object value, Type targetType,
			object parameter, CultureInfo culture )
		{
			return null;
		}
	}
}
