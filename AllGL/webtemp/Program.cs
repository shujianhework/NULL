using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Reflection;
using System.Data;
namespace CJKOther
{
    class Program
    {
        static void Main(string[] args)
        {
            //Log.write("中国式的免费收看的房间里的身份门口");
            //string localDB = "Data Source=127.0.0.1;Initial Catalog=ply_web;User ID=sa;Password=shu110";
            //string SelfDB = "Data Source=47.75.78.46;Initial Catalog=ply_web;User ID=sa;Password=shu110SHUsa";
            //Dictionary<int, Dictionary<string, object>> data = new Dictionary<int,Dictionary<string,object>>();
            //if (DB.select(localDB, "select * from Area ",ref data) == true)
            //{
            //    foreach (var item in data)
            //    {
            //        if (DB.insert(SelfDB, "Area", item.Value) == false)
            //        {
            //            break;
            //        }
            //    }
            //}
            ToServerManage.BufferLen = 15;
            int a = Console.Read();
        }
    }
}
