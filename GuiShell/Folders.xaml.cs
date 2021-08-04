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

namespace GuiShell
{
    /// <summary>
    /// Interaction logic for Folders.xaml
    /// </summary>
    public partial class Folders : Page
    {
        public Folders(ConfigData.FoldersData data)
        {
            InitializeComponent();
            checkGetSizesByEverything.IsChecked = data.FolderBehaviour.GetSizesByEverything;
        }

        public ConfigData.FoldersData Save()
        {
            var data = new ConfigData.FoldersData();
            data.FolderBehaviour.GetSizesByEverything = (bool)checkGetSizesByEverything.IsChecked;
            return data;
        }
    }
}