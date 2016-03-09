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
using Installer.Version;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Net;
using System.IO;
using System.Diagnostics;
using Microsoft.Build.Execution;



namespace Installer
{
	/// <summary>
	/// Interaction logic for InstallOptions.xaml
	/// </summary>
	public partial class InstallOptions : Window
	{
		List<EngineVersionData>     m_install;
		List<VisualStudioData>      m_visualVersions;
		VersionManager              m_versionManager;
		bool                        m_compile;

		string						defaultPath = "D:\\ProgramyVS\\SWEngine\\";

		private BackgroundWorker    m_installWorker;

		public InstallOptions( List<EngineVersionData> toInstall, VersionManager manager )
		{
			m_install = toInstall;
			m_versionManager = manager;
			m_visualVersions = Version.VisualStudioVersion.ListVisualVersions();
			m_compile = false;

			foreach( var version in toInstall )
			{
				// Istniejące lokalnie repozytoria mają poprawną ścieżkę.
				if( version.Remote )
					version.Path = defaultPath;
			}

			InitializeComponent();

			listBox.DataContext = m_install;
			VisualStudioVersion.DataContext = m_visualVersions;
			EndButton.IsEnabled = true;

			m_installWorker = new BackgroundWorker();
			m_installWorker.DoWork += new DoWorkEventHandler( InstallDoWork );
			m_installWorker.ProgressChanged += new ProgressChangedEventHandler( InstallProgress );
			m_installWorker.RunWorkerCompleted += new RunWorkerCompletedEventHandler( InstallCompleted );
			m_installWorker.WorkerReportsProgress = true;
			m_installWorker.WorkerSupportsCancellation = false;
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
					if( installed != null && installed.Installed )
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
			InstallButton.IsEnabled = false;
			EndButton.IsEnabled = false;

			m_compile = BuildEngine.IsChecked.Value;

			m_visualVersions.Clear();
			foreach( var item in VisualStudioVersion.SelectedItems )
			{
				m_visualVersions.Add( item as VisualStudioData );
			}

			m_installWorker.RunWorkerAsync();

			Refresh();
		}



		void InstallDoWork( object sender, DoWorkEventArgs e )
		{
			BackgroundWorker worker = sender as BackgroundWorker;

			int numInstalled = 0;

			// Installing
			foreach( var version in m_install )
			{
				bool result;
				if( version.Remote )
					result = InstallRemote( version );
				else
					result = InstallLocal( version );
				version.Installed = result;

				numInstalled++;
				worker.ReportProgress( numInstalled * 100 / m_install.Count );
			}

		}

		void InstallCompleted( object sender, RunWorkerCompletedEventArgs e )
		{
			EndButton.IsEnabled = true;
		}

		private void InstallProgress( object sender, ProgressChangedEventArgs e )
		{
			Refresh();
		}

		void Refresh()
		{
			listBox.DataContext = null;
			listBox.DataContext = m_install;
		}




		private bool InstallRemote( EngineVersionData versionData )
		{
			bool result = LoadVersionFromWeb( versionData );

			if( !result )
				return false;

			result = UnzipVersion( versionData );
			if( !result )
				return false;

			versionData.Path = Path.Combine( versionData.Path + versionData.Version ) + "\\";

			if( m_compile )
			{
				result = BuildVersion( versionData );
				if( !result )
					return false;
			}

			result = m_versionManager.RegisterEngineVersion( versionData );

			if( result )
				versionData.Installed = true;

			return result;
		}

		private bool InstallLocal( EngineVersionData versionData )
		{
			return m_versionManager.RegisterEngineVersion( versionData );
		}


		private bool LoadVersionFromWeb( EngineVersionData versionData )
		{
			if( !versionData.Remote )
				return false;


			try
			{
				HttpWebRequest releasesRequest = (HttpWebRequest)HttpWebRequest.Create( versionData.RemotePath );
				releasesRequest.UserAgent = "Mozilla/5.0 (compatible; MSIE 10.0; Windows NT 6.2; WOW64; Trident/6.0)";  // Udajemy przeglądarkę.

				HttpWebResponse response = (HttpWebResponse)releasesRequest.GetResponse();

				using( Stream output = File.OpenWrite( Path.Combine( versionData.Path, versionData.Version + ".zip" ) ) )
				using( Stream receiveStream = response.GetResponseStream() )
				{
					receiveStream.CopyTo( output );
				}
			}
			catch( Exception e )
			{
				Console.Write( e.ToString() );
				return false;
			}

			return true;
		}

		private bool UnzipVersion( EngineVersionData versionData )
		{
			string zipPath = Path.Combine( versionData.Path, versionData.Version + ".zip" );
			try
			{
				string unzipDir = Path.Combine( versionData.Path, "~temp" + versionData.Version );
				System.IO.Compression.ZipFile.ExtractToDirectory( zipPath, unzipDir );

				var dirs = Directory.GetDirectories( unzipDir );
				if( dirs.Length != 1 )
					return false;

				string unzipInnerDir = Path.Combine( unzipDir, dirs[ 0 ] );

				Directory.Move( unzipInnerDir, Path.Combine( versionData.Path, versionData.Version ) );
				Directory.Delete( unzipDir );

				return true;
			}
			catch( Exception e )
			{
				Console.Write( e.ToString() );
			}
			finally
			{
				File.Delete( zipPath );
			}

			return false;
		}

		private bool BuildVersion( EngineVersionData versionData )
		{
			string[] configuration = new string[ 4 ];
			configuration[ 0 ] = "Debug";
			configuration[ 1 ] = "Release";
			configuration[ 2 ] = "Debug";
			configuration[ 3 ] = "Release";

			string[] platform = new string[ 4 ];
			platform[ 0 ] = "Win32";
			platform[ 1 ] = "Win32";
			platform[ 2 ] = "x64";
			platform[ 3 ] = "x64";

			foreach( var toolset in m_visualVersions )
			{
				for( int config = 0; config < 4; ++config )
				{
					string arguments = versionData.Path + "Projects\\SWEngine.sln /p:";
					arguments += "Configuration=" + configuration[ config ];
					arguments += ";Platform=" + platform[ config ];
					//arguments += ";PlatformToolset=" + toolset.ToolsetShort;

					Process MSBuildProcess = new Process();
					MSBuildProcess.StartInfo.FileName = toolset.ToolsetPath + "MSBuild.exe";
					MSBuildProcess.StartInfo.RedirectStandardOutput = true;
					MSBuildProcess.StartInfo.UseShellExecute = false;
					//MSBuildProcess.StartInfo.
					MSBuildProcess.StartInfo.Arguments = arguments;
					MSBuildProcess.StartInfo.WorkingDirectory = versionData.Path;
					MSBuildProcess.Start();

					Console.WriteLine( MSBuildProcess.StandardOutput.ReadToEnd() );

					MSBuildProcess.WaitForExit();
					MSBuildProcess.Close();

					//Microsoft.Build.Evaluation.ProjectCollection pc = new Microsoft.Build.Evaluation.ProjectCollection();

					//Dictionary<string, string> GlobalProperty = new Dictionary<string, string>();

					//GlobalProperty.Add( "Configuration", configuration[ config ] );
					//GlobalProperty.Add( "Platform", platform[ config ] );
					//GlobalProperty.Add( "PlatformToolset", toolset.ToolsetShort );

					//BuildRequestData BuildRequest = new BuildRequestData( versionData.Path + "Projects\\SWEngine.sln", GlobalProperty, null, new string[] { "Build" }, null );
					//BuildResult buildResult = BuildManager.DefaultBuildManager.Build( new BuildParameters( pc ), BuildRequest );
				}
			}




			return true;
		}

		private void EndButton_Click( object sender, RoutedEventArgs e )
		{
			DialogResult = false;
			Close();
		}
	}

}
