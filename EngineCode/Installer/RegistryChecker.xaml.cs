using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using Microsoft.Win32;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using Installer.Data;

namespace Installer
{
	/// <summary>
	/// Interaction logic for RegistryChecker.xaml
	/// </summary>
	public partial class RegistryChecker : Window
	{
		List<RegData>       m_engineData;

		public RegistryChecker()
		{
			m_engineData = new List<RegData>();

			InitializeComponent();
		}

		private void WindowInitialized	( object sender, EventArgs e )
		{
			RegistryKey registryKey = Registry.CurrentUser;

			RegistryKey SWEngineReg = registryKey.OpenSubKey( "Software\\SchopenhauersWeltschmerz" );
			if ( SWEngineReg == null )
				return;

			string[] engineVersions = SWEngineReg.GetSubKeyNames();
			ListEngineVersions( SWEngineReg, engineVersions );

			listBox.DataContext = m_engineData;
		}

		private void ListEngineVersions( RegistryKey engineRegDir, string[] names )
		{
			foreach( var name in names )
			{
				RegData newRegData = new RegData();

				RegistryKey enginePathReg = engineRegDir.OpenSubKey( name );
				object enginePath = enginePathReg.GetValue( "EngineRootPath" );

				newRegData.Path = enginePath.ToString();
				newRegData.Version = name;
				m_engineData.Add( newRegData );
			}

		}
	}
}
