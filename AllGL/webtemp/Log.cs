using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
namespace CJKOther
{
    class Log
    {
        static private string Date;
        static private string RootPath;
        static private string path;
        static Log(){
            Date = "";
            path = "";
            RootPath = System.Environment.CurrentDirectory + "\\..\\..\\";
            if (Directory.Exists(RootPath + "WebLog") == false)
                Directory.CreateDirectory(RootPath + "WebLog");
        }
        static private string GetHead()
        {
            DateTime dt = DateTime.Now;
            if (dt.ToString("D") != Date)
            {
                Date = dt.ToString("D");
                path = RootPath + "WebLog\\" + Date + ".log";
                if (File.Exists(path) == false)
                {
                    var f = File.Create(path);
                    f.Close();
                }
            }
            return "[" + dt.ToString("yyyy-MM-dd HH:mm:ss.fff") + "]";
        }
        static public void write(string s)
        {
            s = s + GetHead();
            var mysw = File.AppendText(path);
            mysw.WriteLine(s);
            mysw.Close();
        }
    }
}
