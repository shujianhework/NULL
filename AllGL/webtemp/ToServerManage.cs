using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.Sockets;
using System.Web;
namespace CJKOther
{
    class ToServerManage{
        static private Dictionary<string,Tuple<string,int,string> > Sockets;
        static private int RecvBufferLen_K = 60;
        static public int BufferLen
        {
            get { return RecvBufferLen_K;}
            set { if (value > 0 && value < 4097) RecvBufferLen_K = value; }
        }
        static ToServerManage(){
            string strcon = "Data Source=47.75.78.46;Initial Catalog=ply_web;User ID=sa;Password=shu110SHUsa";
            Dictionary<int,Dictionary<string,object>> dbServerData = new Dictionary<int,Dictionary<string,object>>();
            if (DB.select(strcon, "SELECT *  FROM ServerInfo", ref dbServerData) == false)
                return;
            Sockets = new Dictionary<string, Tuple<string, int, string>>();
            foreach (var item in dbServerData)
            {
                Dictionary<string, object> temp = item.Value;
                Sockets[(string)temp["SID"]] = new Tuple<string, int, string>(temp["ServerIp"].ToString(),Int32.Parse(temp["ServerPort"].ToString()), temp["Remarks"].ToString());
            }
        }
        static public bool sendKeyNoRecv(string key, byte[] barr){
            if (Sockets == null)
                return false;
            if(Sockets.ContainsKey(key) == false)
                return false;
            bool ret = true;
            try
            {
                Socket socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                using (socket)
                {
                    socket.Connect(Sockets[key].Item1, Sockets[key].Item2);
                    socket.Send(barr);
                    socket.Close();
                }
                ret = true;
            }
            catch (Exception ex) {
                ret = false;
            }
            return ret;
        }
        static public bool sendKeyNoRecv(string key, string s)
        {
            return sendKeyNoRecv(key, System.Text.Encoding.Default.GetBytes(s));
        }
        static public bool sendKeyNoRecv(string key, object d)
        {
            if (d == null)
                return false;
            return sendKeyNoRecv(key,Tool.Object2Json(d));
        }
        static public bool sendKeyRecv(string key, ref byte[] barr) {
            if (Sockets == null)
                return false;
            if (Sockets.ContainsKey(key) == false)
                return false;
            bool ret = true;
            try
            {
                Socket socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                using (socket)
                {
                    socket.Connect(Sockets[key].Item1, Sockets[key].Item2);
                    socket.Send(barr);
                    int BufferLen = RecvBufferLen_K * 1024;
                    byte[] rarr = new byte[BufferLen];
                    List<byte[]> lb = new List<byte[]>();
                    int len = 0;
                    do
                    {
                        if (len > 0)
                        {
                            byte[] tarr = new byte[BufferLen];
                            rarr.CopyTo(tarr, 0);
                            lb.Add(tarr);
                        }
                        len = socket.Receive(rarr);
                    } while (len < BufferLen);
                    socket.Close();
                    barr = new byte[BufferLen * lb.Count + len];
                    int index = 0;
                    for (int i = 0; i < lb.Count; i++)
                    {
                        lb[i].CopyTo(barr, index);
                        index += BufferLen;
                    }
                    rarr.CopyTo(barr, index);
                }
                ret = true;
            }
            catch (Exception ex)
            {
                ret = false;
            }
            return ret;
        }
        static public bool sendKeyRecv(string key, ref string s) {
            byte[] barr = System.Text.Encoding.Default.GetBytes(s);
            bool ret = sendKeyRecv(key,ref barr);
            if (ret)
                s = System.Text.Encoding.Default.GetString(barr);
            return ret;
        }
        static public bool sendKeyRecv(string key, ref object d) {
            try
            {
                string s = Tool.Object2Json(d);
                if (s == "")
                    return false;
                bool ret = sendKeyRecv(key, ref s);
                if (ret == true)
                {
                    d = Tool.Json2Object(s);
                }
                return ret;
            }
            catch (Exception ex)
            {
                return false;
            }
        }
    }
}
