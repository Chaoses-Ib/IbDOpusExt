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
    /// Interaction logic for FileOperations.xaml
    /// </summary>
    public partial class FileOperations : Page
    {
        public FileOperations(ConfigData.FileOperationsData data)
        {
            InitializeComponent();
            txtMaxUndoNum.Text = data.Logging.MaxUndoNum.ToString();
        }

        public ConfigData.FileOperationsData Save()
        {
            var data = new ConfigData.FileOperationsData();
            byte maxUndoNum;
            if (byte.TryParse(txtMaxUndoNum.Text, out maxUndoNum))
                data.Logging.MaxUndoNum = maxUndoNum;
            return data;
        }
    }
}