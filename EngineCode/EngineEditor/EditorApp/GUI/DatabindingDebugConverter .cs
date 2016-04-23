﻿using System;
using System.Globalization;
using System.Windows.Data;
using System.Diagnostics;


namespace EditorApp.Debug
{
	/// <summary>
	/// This converter does nothing except breaking the
	/// debugger into the convert method
	/// </summary>
	public class DatabindingDebugConverter : IValueConverter
	{
		public object Convert( object value, Type targetType,
			object parameter, CultureInfo culture )
		{
			Debugger.Break();
			return value;
		}

		public object ConvertBack( object value, Type targetType,
			object parameter, CultureInfo culture )
		{
			Debugger.Break();
			return value;
		}
	}
}
