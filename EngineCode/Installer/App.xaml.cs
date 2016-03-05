using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;

namespace Installer
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
		private void ApplicationStart( object sender, StartupEventArgs e )
		{
			Current.ShutdownMode = ShutdownMode.OnExplicitShutdown;

			var versionCheckerDialog = new VersionChecker();

			if( versionCheckerDialog.ShowDialog().Value )
			{
				var result = versionCheckerDialog.GetResult();
				var installOptionsDialog = new InstallOptions( result, versionCheckerDialog.GetVersionManager() );

				installOptionsDialog.ShowDialog();
			}

			Current.Shutdown( -1 );
		}

	}
}
