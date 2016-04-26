using System;
using System.Globalization;
using System.Windows.Data;
using System.Windows.Media.Imaging;
using System.Windows;
using EditorApp.Editor.Project.Content;

namespace EditorApp.GUI
{

	class FileIconConverter : IValueConverter
	{
		public object Convert( object value, Type targetType, object parameter, CultureInfo culture )
		{
			Nullable< FileTreeNodeType > fileType = value as FileTreeNodeType ?;

			if( fileType.Value == FileTreeNodeType.Directory )
				return Application.Current.FindResource( "FolderIcon" ) as BitmapImage;
			else if( fileType.Value == FileTreeNodeType.Model3D )
				return Application.Current.FindResource( "Model3dIcon" ) as BitmapImage;
			else if( fileType.Value == FileTreeNodeType.Shader )
				return Application.Current.FindResource( "ShaderIcon" ) as BitmapImage;

			return null;
		}

		public object ConvertBack( object value, Type targetType,
			object parameter, CultureInfo culture )
		{
			return null;
		}
	}
}
