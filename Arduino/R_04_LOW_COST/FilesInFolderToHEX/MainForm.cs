using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace FileToByteArrayConverter
{
    public partial class MainForm : Form
    {
        string startPositionOpenFileDialog = "d:\\";
        string startPositionSaveFileDialog = "d:\\";
        public MainForm()
        {
            InitializeComponent();

            using(StreamReader reader = new StreamReader("property"))
            {
                startPositionOpenFileDialog = reader.ReadLine();
                startPositionSaveFileDialog = reader.ReadLine();
            }
        }

        private void writeFilesInFolder(Stream saveFileStream, string folderPath)
        {
            string[] allFiles = Directory.GetFiles(folderPath);

            byte[] bytesToWrite;

            if (allFiles.Length == 0) return;
            /*bytesToWrite = Encoding.UTF8.GetBytes("#ifndef __FSDATA_CUSTOM_H\r\n");
            saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);
            bytesToWrite = Encoding.UTF8.GetBytes("#define __FSDATA_CUSTOM_H\r\n");
            saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);*/

            /*bytesToWrite = Encoding.UTF8.GetBytes("#include \"fsdata.h\"\r\n\r\n");
            saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);*/
           /* bytesToWrite = Encoding.UTF8.GetBytes("#include \"fs.h\"\r\n");
            saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);
            bytesToWrite = Encoding.UTF8.GetBytes("#include \"string.h\"\r\n");
            saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);
            bytesToWrite = Encoding.UTF8.GetBytes("#include \"fsdata.h\"\r\n\r\n");
            saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);

            bytesToWrite = Encoding.UTF8.GetBytes("#define file_NULL (struct fsdata_file *) NULL\r\n\r\n");
            saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);

            string previousFsDataFile = "file_NULL";*/

            int i = 0;

            for (i = 0; i < allFiles.Length; i++)
            {
                string currentFileName = Path.GetFileName(allFiles[i]);
                //string pathRelativeMainFolder = allFiles[i].Remove(0, (folderPath + "\\").Length).Replace("\\\\","\\");

                /*bytesToWrite = Encoding.UTF8.GetBytes("static const unsigned char name_" + currentFileName.Replace('.', '_') + "[]={\r\n");
                saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);
                */
                byte[] array; /*= Encoding.UTF8.GetBytes(currentFileName + '\0');

                for (int j = 0; j < array.Length; j++)
                {
                    bytesToWrite = Encoding.UTF8.GetBytes(string.Format("0x{0:X2},", array[j]));
                    saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);
                }

                bytesToWrite = Encoding.UTF8.GetBytes("};\r\n\r\n");
                saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);*/

                using (StreamReader reader = new StreamReader(allFiles[i]))
                {
                    string fileContent = reader.ReadToEnd();
                    array = Encoding.UTF8.GetBytes(fileContent);
                }

                bytesToWrite = Encoding.UTF8.GetBytes("static const unsigned char " + currentFileName.Replace('.', '_') + "[]={\r\n");
                saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);

                for (int j = 0; j < array.Length; j++)
                {
                    if (j % 20 == 19) bytesToWrite = Encoding.UTF8.GetBytes(string.Format("0x{0:X2},\r\n", array[j]));
                    else bytesToWrite = Encoding.UTF8.GetBytes(string.Format("0x{0:X2},", array[j]));
                    saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);
                }

                bytesToWrite = Encoding.UTF8.GetBytes("};\r\n\r\n");
                saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);

                bytesToWrite = Encoding.UTF8.GetBytes("static const int "+ currentFileName.Replace('.', '_')+"_size = sizeof(" + currentFileName.Replace('.', '_') + ");\r\n\r\n");
                saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);

                /*bytesToWrite = Encoding.UTF8.GetBytes("const struct fsdata_file file_" + currentFileName.Replace('.', '_') + "[]={{\r\n");
                saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);

                bytesToWrite = Encoding.UTF8.GetBytes(previousFsDataFile + ",\r\n");
                saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);

                previousFsDataFile = "file_" + currentFileName.Replace('.', '_');

                bytesToWrite = Encoding.UTF8.GetBytes("name_" + currentFileName.Replace('.', '_') + ",\r\n");
                saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);
                bytesToWrite = Encoding.UTF8.GetBytes("data_" + currentFileName.Replace('.', '_') + ",\r\n");
                saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);
                bytesToWrite = Encoding.UTF8.GetBytes("sizeof(data_" + currentFileName.Replace('.', '_') + "),\r\n");
                saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);
                bytesToWrite = Encoding.UTF8.GetBytes("1,\r\n");
                saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);

                bytesToWrite = Encoding.UTF8.GetBytes("}};\r\n\r\n");
                saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);*/
            }

            /*bytesToWrite = Encoding.UTF8.GetBytes("#define FS_ROOT " + previousFsDataFile + "\r\n");
            saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);

            bytesToWrite = Encoding.UTF8.GetBytes("#define FS_NUMFILES " + i.ToString() + "\r\n\r\n");
            saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);

            bytesToWrite = Encoding.UTF8.GetBytes("err_t fs_open(struct fs_file *file, const char *name){\r\n");
            saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);
            bytesToWrite = Encoding.UTF8.GetBytes("	const struct fsdata_file *f;\r\n\r\n");
            saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);
            bytesToWrite = Encoding.UTF8.GetBytes("	for(f = FS_ROOT; f != NULL; f = f->next){\r\n");
            saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);
            bytesToWrite = Encoding.UTF8.GetBytes("		if(memcmp(name, (const char *)f->name,strlen((const char *)f->name))==0){\r\n");
            saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);
            bytesToWrite = Encoding.UTF8.GetBytes("			file->data = (const char *)f->data;\r\n");
            saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);
            bytesToWrite = Encoding.UTF8.GetBytes("         file->len = f->len;\r\n");
            saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);
            bytesToWrite = Encoding.UTF8.GetBytes("			file->index = f->len;\r\n");
            saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);
            bytesToWrite = Encoding.UTF8.GetBytes("			file->pextension = NULL;\r\n");
            saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);
            bytesToWrite = Encoding.UTF8.GetBytes("			file->flags = f->flags;\r\n");
            saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);
            bytesToWrite = Encoding.UTF8.GetBytes("			return ERR_OK;\r\n");
            saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);
            bytesToWrite = Encoding.UTF8.GetBytes("		}\r\n");
            saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);
            bytesToWrite = Encoding.UTF8.GetBytes("	}\r\n");
            saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);
            bytesToWrite = Encoding.UTF8.GetBytes("	return ERR_VAL;\r\n");
            saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);
            bytesToWrite = Encoding.UTF8.GetBytes("}\r\n\r\n\r\n");
            saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);*/

            /*bytesToWrite = Encoding.UTF8.GetBytes("#endif\r\n\r\n");
            saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);*/

        }

        private void ButtonSelectFile_Click(object sender, EventArgs e)
        {
            using (SaveFileDialog saveFileDialog = new SaveFileDialog())
            {
                saveFileDialog.Filter = "header files (*.h)|*.h|All files (*.*)|*.*";
                saveFileDialog.FilterIndex = 1;
                saveFileDialog.RestoreDirectory = true;
                saveFileDialog.InitialDirectory = startPositionSaveFileDialog;
                saveFileDialog.FileName = "pages.h";

                if (saveFileDialog.ShowDialog() == DialogResult.OK)
                {
                    startPositionSaveFileDialog = Path.GetDirectoryName(saveFileDialog.FileName);

                    Stream saveFileStream;
                    if ((saveFileStream = saveFileDialog.OpenFile()) != null)
                    {
                        using (FolderBrowserDialog folderBrowserDialog = new FolderBrowserDialog())
                        {
                            folderBrowserDialog.SelectedPath = startPositionOpenFileDialog;

                            if (folderBrowserDialog.ShowDialog() == DialogResult.OK)
                            {
                                startPositionOpenFileDialog = folderBrowserDialog.SelectedPath;

                                writeFilesInFolder(saveFileStream, folderBrowserDialog.SelectedPath);
                            }
                        }

                        saveFileStream.Close();

                        MessageBox.Show("Запись выполнена");
                    }
                }
            }
        }

        /*private void ButtonSelectFile_Click(object sender, EventArgs e)
        {
            using (SaveFileDialog saveFileDialog = new SaveFileDialog())
            {
                saveFileDialog.Filter = "header files (*.h)|*.h|All files (*.*)|*.*";
                saveFileDialog.FilterIndex = 1;
                saveFileDialog.RestoreDirectory = true;
                saveFileDialog.InitialDirectory = startPositionSaveFileDialog;
                saveFileDialog.FileName = "fsdata_custom";

                if (saveFileDialog.ShowDialog() == DialogResult.OK)
                {
                    startPositionSaveFileDialog = Path.GetDirectoryName(saveFileDialog.FileName);
                    
                    Stream saveFileStream;
                    if ((saveFileStream = saveFileDialog.OpenFile()) != null)
                    {
                        using (FolderBrowserDialog folderBrowserDialog = new FolderBrowserDialog())
                        {
                            folderBrowserDialog.SelectedPath = startPositionOpenFileDialog;

                            if (folderBrowserDialog.ShowDialog() == DialogResult.OK)
                            {
                                startPositionOpenFileDialog = folderBrowserDialog.SelectedPath;

                                string[] allFiles = Directory.GetFiles(folderBrowserDialog.SelectedPath);

                                byte[] bytesToWrite;

                                if (allFiles.Length > 0)
                                {
                                    bytesToWrite = Encoding.UTF8.GetBytes("#ifndef __FSDATA_CUSTOM_H\r\n");
                                    saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);
                                    bytesToWrite = Encoding.UTF8.GetBytes("#define __FSDATA_CUSTOM_H\r\n");
                                    saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);

                                    bytesToWrite = Encoding.UTF8.GetBytes("#include \"fsdata.h\"\r\n\r\n");
                                    saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);

                                    bytesToWrite = Encoding.UTF8.GetBytes("#define file_NULL (struct fsdata_file *) NULL\r\n\r\n");
                                    saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);

                                    string previousFsDataFile = "file_NULL";

                                    int i = 0;

                                    for (i = 0; i < allFiles.Length; i++)
                                    {
                                        string currentFileName = Path.GetFileName(allFiles[i]);

                                        bytesToWrite = Encoding.UTF8.GetBytes("static const unsigned char name_" + currentFileName.Replace('.', '_') + "[]={\r\n");
                                        saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);

                                        byte[] array = Encoding.UTF8.GetBytes(currentFileName + '\0');

                                        for (int j = 0; j < array.Length; j++)
                                        {
                                            bytesToWrite = Encoding.UTF8.GetBytes(string.Format("0x{0:X2},", array[j]));
                                            saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);
                                        }

                                        bytesToWrite = Encoding.UTF8.GetBytes("};\r\n\r\n");
                                        saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);

                                        using (StreamReader reader = new StreamReader(allFiles[i]))
                                        {
                                            string fileContent = reader.ReadToEnd();
                                            array = Encoding.UTF8.GetBytes(fileContent);
                                        }

                                        bytesToWrite = Encoding.UTF8.GetBytes("static const unsigned char data_" + currentFileName.Replace('.', '_') + "[]={\r\n");
                                        saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);

                                        for (int j = 0; j < array.Length; j++)
                                        {
                                            if (j % 20 == 19) bytesToWrite = Encoding.UTF8.GetBytes(string.Format("0x{0:X2},\r\n", array[j]));
                                            else bytesToWrite = Encoding.UTF8.GetBytes(string.Format("0x{0:X2},", array[j]));
                                            saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);
                                        }

                                        bytesToWrite = Encoding.UTF8.GetBytes("};\r\n\r\n");
                                        saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);

                                        bytesToWrite = Encoding.UTF8.GetBytes("const struct fsdata_file file_" + currentFileName.Replace('.', '_') + "[]={{\r\n");
                                        saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);

                                        bytesToWrite = Encoding.UTF8.GetBytes(previousFsDataFile + ",\r\n");
                                        saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);

                                        previousFsDataFile = "file_" + currentFileName.Replace('.', '_');

                                        bytesToWrite = Encoding.UTF8.GetBytes("name_" + currentFileName.Replace('.', '_') + ",\r\n");
                                        saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);
                                        bytesToWrite = Encoding.UTF8.GetBytes("data_" + currentFileName.Replace('.', '_') + ",\r\n");
                                        saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);
                                        bytesToWrite = Encoding.UTF8.GetBytes("sizeof(data_" + currentFileName.Replace('.', '_') + "),\r\n");
                                        saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);
                                        bytesToWrite = Encoding.UTF8.GetBytes("1,\r\n");
                                        saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);

                                        bytesToWrite = Encoding.UTF8.GetBytes("}};\r\n\r\n");
                                        saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);
                                    }

                                    bytesToWrite = Encoding.UTF8.GetBytes("#define FS_ROOT "+ previousFsDataFile + "\r\n");
                                    saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);

                                    bytesToWrite = Encoding.UTF8.GetBytes("#define FS_NUMFILES " + i.ToString() + "\r\n\r\n");
                                    saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);

                                    bytesToWrite = Encoding.UTF8.GetBytes("#endif\r\n\r\n");
                                    saveFileStream.Write(bytesToWrite, 0, bytesToWrite.Length);
                                }
                            }
                        }

                        saveFileStream.Close();

                        MessageBox.Show("Запись выполнена");
                    }
                }
            }
        }*/

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            using(StreamWriter writer = new StreamWriter("property"))
            {
                writer.WriteLine(startPositionOpenFileDialog);
                writer.WriteLine(startPositionSaveFileDialog);
            }
        }
    }
}
