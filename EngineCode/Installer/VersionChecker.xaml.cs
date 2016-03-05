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

namespace Installer
{
	/// <summary>
	/// Interaction logic for RegistryChecker.xaml
	/// </summary>
	public partial class VersionChecker : Window
	{
		VersionManager				m_versionManager;
		List<EngineVersionData>     m_dialogResult;

		public VersionChecker()
		{
			m_versionManager = new VersionManager();
			m_dialogResult = null;

			InitializeComponent();
		}

		private void WindowInitialized	( object sender, EventArgs e )
		{
			listBox.DataContext = m_versionManager.CreateVersionsList();
		}

		private void CancelButton_Click( object sender, RoutedEventArgs e )
		{
			DialogResult = false;
			Close();
		}

		private void InstallButton_Click( object sender, RoutedEventArgs e )
		{
			m_dialogResult = new List<EngineVersionData>();

			var versionList = m_versionManager.GetVersionsList();
			foreach( var version in versionList )
			{
				if( version.Install )
					m_dialogResult.Add( version );
			}

			DialogResult = true;
			Close();
		}

		public List<EngineVersionData>		GetResult()
		{
			return m_dialogResult;
		}

		public VersionManager				GetVersionManager()
		{
			return m_versionManager;
		}
	}
}
