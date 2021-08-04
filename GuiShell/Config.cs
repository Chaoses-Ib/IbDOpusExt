using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Serialization;
using System.IO;

namespace GuiShell
{
    public class ConfigData
    {
        public class FileOperationsData
        {
            public class LoggingData
            {
                [DefaultValue(10)]
                public byte MaxUndoNum = 10;
            }
            public LoggingData Logging = new LoggingData();
        }
        public FileOperationsData FileOperations = new FileOperationsData();

        public class FoldersData
        {
            public class FolderBehaviourData
            {
                [DefaultValue(false)]
                public bool GetSizesByEverything = false;
            }
            public FolderBehaviourData FolderBehaviour = new FolderBehaviourData();
        }
        public FoldersData Folders = new FoldersData();
    }

    public class Config
    {
        public static string Filename { set; get; }
        public static Action<ConfigData> ApplyAction { set; get; }

        public static void SetUnhandledExceptionHandler()
        {
            AppDomain.CurrentDomain.UnhandledException += (object sender, UnhandledExceptionEventArgs args) =>
            {
                Exception e = (Exception)args.ExceptionObject;
                System.Windows.MessageBox.Show(e.ToString(), "Unhandled exception");
            };
        }

        public static ConfigData Read()
        {
            XmlSerializer serializer = new XmlSerializer(typeof(ConfigData));
            serializer.UnknownNode += (object sender, System.Xml.Serialization.XmlNodeEventArgs e) =>
            {
                System.Windows.MessageBox.Show($"UnknownNode: {e.NodeType} {e.Name} at line {e.LineNumber}", "IbDOpusExt");
            };

            ConfigData config;
            if (File.Exists(Filename))
            {
                try
                {
                    using (FileStream fs = new FileStream(Filename, FileMode.Open, FileAccess.Read))
                    {
                        config = (ConfigData)serializer.Deserialize(fs) ?? new ConfigData();
                    }
                }
                catch (System.IO.IOException e)
                {
                    System.Windows.MessageBox.Show(e.ToString(), "Exception");
                    config = new ConfigData();
                }
            }
            else
            {
                config = new ConfigData();
            }
            return config;
        }

        public static void Apply(ConfigData config)
        {
            ApplyAction(config);
        }

        public static void Write(ConfigData config)
        {
            XmlSerializer serializer = new XmlSerializer(typeof(ConfigData));
            Directory.GetParent(Filename).Create();
            try
            {
                using(FileStream fs = new FileStream(Filename, FileMode.Create, FileAccess.Write))
                {
                    serializer.Serialize(fs, config);
                }
            }
            catch(System.IO.IOException e)
            {
                System.Windows.MessageBox.Show(e.ToString(), "Exception");
            }
        }
    }
}