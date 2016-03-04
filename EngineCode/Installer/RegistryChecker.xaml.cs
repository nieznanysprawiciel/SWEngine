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
	public partial class RegistryChecker : Window
	{
		VersionManager      m_versionManager;

		public RegistryChecker()
		{
			m_versionManager = new VersionManager();

			InitializeComponent();
		}

		private void WindowInitialized	( object sender, EventArgs e )
		{
			listBox.DataContext = m_versionManager.CreateVersionsList();
		}


	}
}
