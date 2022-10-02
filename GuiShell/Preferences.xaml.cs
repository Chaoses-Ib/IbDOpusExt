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
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;
using System.Threading;

namespace GuiShell
{
    public class PreferenceShow
    {
        Preferences preferences;

        public bool Show()
        {
            if (preferences != null)
            {
                preferences.Dispatcher.BeginInvoke(DispatcherPriority.Render, (ThreadStart)delegate ()
                {
                    preferences.Activate();
                });
                return false;
            }

            Thread t = new Thread(() =>
            {
                preferences = new Preferences();
                preferences.ShowDialog();
                preferences = null;
            });
            t.SetApartmentState(ApartmentState.STA);
            t.Start();
            return true;
        }
    }

    /// <summary>
    /// Interaction logic for Preferences.xaml
    /// </summary>
    public partial class Preferences : Window
    {
        ConfigData config;
        FileOperations fileOperations;
        Folders folders;
        About about;

        public Preferences()
        {
            InitializeComponent();

            config = Config.Read();
            fileOperations = new FileOperations(config.FileOperations);
            folders = new Folders(config.Folders);
            about = new About();

            Frame.Navigate(fileOperations);
        }

        public Preferences(IntPtr parentWindow) : this()
        {
            var interop = new System.Windows.Interop.WindowInteropHelper(this);
            interop.EnsureHandle();
            interop.Owner = parentWindow;
        }

        private void FileOperations_Selected(object sender, RoutedEventArgs e)
        {
            Frame.Navigate(fileOperations);
        }

        private void Folders_Selected(object sender, RoutedEventArgs e)
        {
            Frame.Navigate(folders);
        }

        private void About_Selected(object sender, RoutedEventArgs e)
        {
            Frame.Navigate(about);
        }

        void Apply()
        {
            config.FileOperations = fileOperations.Save();
            config.Folders = folders.Save();
            Config.Write(config);
            Config.Apply(config);
        }

        private void btnApply_Click(object sender, RoutedEventArgs e)
        {
            Apply();
        }

        private void btnOk_Click(object sender, RoutedEventArgs e)
        {
            Apply();
            Close();
        }

        private void btnCancel_Click(object sender, RoutedEventArgs e)
        {
            Close();
        }
    }
}