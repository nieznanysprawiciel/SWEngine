using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using Installer.Version;
using System.Collections.ObjectModel;

namespace Installer
{
	/// <summary>
	/// Interaction logic for InstallOptions.xaml
	/// </summary>
	public partial class InstallOptions : Window
	{
		List<EngineVersionData>     m_install;
		VersionManager              m_versionManager;

		string						defaultPath = "D:\\ProgramyVS\\SWEngine\\";

		public InstallOptions( List<EngineVersionData> toInstall, VersionManager manager )
		{
			m_install = toInstall;
			m_versionManager = manager;

			foreach( var version in toInstall )
			{
				version.Path = defaultPath;
			}

			InitializeComponent();

			listBox.DataContext = m_install;
		}

		private void Button_Click( object sender, RoutedEventArgs e )
		{
			var dialog = new System.Windows.Forms.FolderBrowserDialog();
			dialog.SelectedPath = defaultPath;
			if( dialog.ShowDialog() == System.Windows.Forms.DialogResult.OK )
			{
				Button button = sender as Button;
				var engineVersion = button.DataContext as EngineVersionData;
				engineVersion.Path = dialog.SelectedPath;

				listBox.DataContext = null;
				listBox.DataContext = m_install;
			}
		}
	}
}
