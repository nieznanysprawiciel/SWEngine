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
				// Istniejące lokalnie repozytoria mają poprawną ścieżkę.
				if( version.Remote )
					version.Path = defaultPath;
			}

			InitializeComponent();

			listBox.DataContext = m_install;
		}

		
		private void ChooseDirectoryClick( object sender, RoutedEventArgs e )
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

		private void InstallButton_Click( object sender, RoutedEventArgs e )
		{
			foreach( var version in m_install )
			{
				if( version.Remote )
				{
					var installed = m_versionManager.FindVersionByVersionName( version.Version );
					if( installed != null )
					{
						ErrorLabel.Content = "Wersja silnika o nazwie [" + version.Version + "] jest już zainstalowana.";
						return;
					}
				}
				else
				{
					var installed = m_versionManager.FindVersionByPath( version.Path );
					if( installed != null )
					{
						ErrorLabel.Content = "W katalogu: [" + version.Path + "] jest zainstalowana wersja [" + installed.Version + "].";
						return;
					}
				}
			}

			// Czyszczenie komunikatu o błędach.
			ErrorLabel.Content = "";

			// Installing
			foreach( var version in m_install )
			{
				bool result;
				if( version.Remote )
					result = InstallRemote( version );
				else
					result = InstallLocal( version );
				version.Installed = result;
			}

			listBox.DataContext = null;
			listBox.DataContext = m_install;

			InstallButton.IsEnabled = false;
		}

		private bool InstallRemote( EngineVersionData versionData )
		{

			return false;
		}

		private bool InstallLocal( EngineVersionData versionData )
		{
			return m_versionManager.RegisterEngineVersion( versionData );
		}
	}
}
