using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Data.SqlClient;
/*
使用方式
Dictionary<string, object> data = new Dictionary<string, object>();
            string strcon = "Data Source=127.0.0.1;Initial Catalog=ply_web;User ID=sa;Password=shu110";
            Action<string,object> back = delegate(string s,object o){
                Console.WriteLine("Key = "+s+"\t\t Type = "+o.GetType().ToString() + "\t\t Value = "+o.ToString());
            };
            //DB.select(strcon,"select * from Area",back);
            //bool ret = DB.delete(strcon, "delete Area where areaid=12354");
            //Console.WriteLine("修改数据执行结果：" + (ret ? "成功" : "失败"));
            Dictionary<string, object> idt = new Dictionary<string, object>();
            Dictionary<string, object> odt = new Dictionary<string, object>();
            idt["@prvev"] = 100;
            idt["@nextv"] = 200;
            odt["@Add"] = 0;
            odt["@Sub"] = 0;
            odt["@Mul"] = 0;
            var dt = DB.ExcuteProc(strcon, "MathTest",idt,odt);
            foreach (var item in dt)
            {
                Console.WriteLine("Key = " + item.Key + "\t\t Type = " + item.Value.GetType().ToString() + "\t\t Value = " + item.Value.ToString());
            }
            int a = Console.Read();
*/
namespace CJKOther
{
    class DB
    {
        static private string strDefaultConnection;
        static DB()
        {
            strDefaultConnection = "";
        }
        static private bool insert(string strconnect, string strcmd)
        {
            bool result = true;
            try
            {
                SqlConnection sConn = new SqlConnection(strconnect);
                SqlCommand cmd = new SqlCommand(strcmd, sConn);
                using (sConn){
                    sConn.Open();
                    result = cmd.ExecuteNonQuery() > 0;
                    sConn.Close();
                }
            }
            catch
            {
                result = false;
            }
            return result;
        }
        static public bool insert(string strconnect, string strtable, Dictionary<string, object> data)
        {
            string cmd = "insert into "+strtable+"(";
            string keys = "";
            string values = "";
            Type t;
            foreach (var item in data)
            {
                if (item.Value != null)
                {
                    keys = keys + item.Key + ",";
                    t = item.Value.GetType();
                    switch (t.Name)
                    {
                        case "Int32":
                            values = values + item.Value.ToString() + ",";
                            break;
                        case "Char":
                            values = values+"'" + item.Value.ToString()+"',";
                            break;
                        case "Boolean":
                            values = values + (((bool)item.Value) ? "1" : "false") + ",";
                            break;
                        case "DateTime":
                            values = values + item.Value.ToString() + ",";
                            break;
                        case "String":
                            values = values + "'" + item.Value.ToString() + "',";
                            break;
                        case "Int64":
                            values = values + item.Value.ToString() + ",";
                            break;
                        case "UInt64":
                            values = values + item.Value.ToString() + ",";
                            break;
                    }
                }
            }
            if(values == "")
                return false;
            values = values.Substring(0, values.Length - 1);
            keys = keys.Substring(0, keys.Length - 1);
            cmd = cmd + keys + ") values (" + values + ")";
            System.Console.WriteLine(cmd);
            return insert(strconnect,cmd);
        }
        static public bool insert(string strtable, Dictionary<string, object> data)
        {
            return insert(strDefaultConnection, strtable, data);
        }
        static private bool select_p(string strconnect, string strcmd, Action<string, object> action){
            SqlConnection sConn = new SqlConnection(strconnect);
            SqlDataReader sdr = null;
            SqlCommand sCmd = null;
            using (sConn)
            {
                sConn.Open();
                sCmd = sConn.CreateCommand();
                sCmd.CommandText = strcmd;
                sdr = sCmd.ExecuteReader();
                using (sdr)
                {
                    if (sdr.HasRows)
                    {
                        while (sdr.Read())
                        {
                            for (int i = 0; i < sdr.FieldCount; i++)
                            {
                                action(sdr.GetName(i), sdr.GetValue(i));
                            }
                        }
                        bool nResult = sdr.NextResult();
                        System.Console.WriteLine("nResult = " + nResult);
                    }
                }
            }
            return true;
        }
        static public bool select(string strconnect, string strcmd, ref Dictionary<int,Dictionary<string, object> > data)
        {
            if (strcmd.Length < 6)
                return false;
            //检查语句 查看里面是否包含 select 以及其他关键字防止实际update 或 delete 
            strcmd = strcmd.ToLower();
            if (strcmd.IndexOf("select ") != 0 )
                return false;
            string[] DetectingStringArrays = { "update", "update ","delete ","delete","insert","insert " };
            foreach (var item in DetectingStringArrays)
            {
                if (strcmd.IndexOf(item) > 0)
                    return false;
            }
            Dictionary<string, object> temp = new Dictionary<string, object>();
            int i = 0;
            Dictionary<int, Dictionary<string, object>> Pdata = new Dictionary<int, Dictionary<string, object>>();
            Action<string, object> back = delegate(string s, object o)
            {
                if (temp.ContainsKey(s))
                {
                    Pdata.Add(i, temp);
                    i++;
                    temp = new Dictionary<string, object>();
                }
                temp[s] = o;
            };
            bool ret = select_p(strconnect,strcmd,back);
            if (ret && temp.Count > 0)
            {
                Pdata.Add(i, temp);
                data = Pdata;
            }
            return ret;
        }
        static public bool select(string strcmd, ref Dictionary<int, Dictionary<string, object>> data)
        {
            return select(strDefaultConnection, strcmd,ref data);
        }
        static public bool select(string strconnect, string strcmd, Action<string, object> action){
            return select_p(strconnect, strcmd, action);
        }
        static public bool select(string strcmd, Action<string, object> action)
        {
            return select(strDefaultConnection, strcmd, action);
        }
        //改和删 采用 只支持一条一条来 ，先查询会有几条，然后if == 1 改和删否则直接失败
        static public bool delete(string strconnect,string strCmd)
        {
            if (strCmd.Length < 11)
                return false;
            string selectstr = "select * from " + strCmd.Substring(6, strCmd.Length - 6);
            Dictionary<int, Dictionary<string, object>> tempdata = new Dictionary<int, Dictionary<string, object>>();
            if (select(strconnect, selectstr, ref tempdata) == false)
                return false;
            if (tempdata.Count != 1)
                return false ;
            bool result = true;
            try
            {
                SqlConnection sConn = new SqlConnection(strconnect);
                SqlCommand cmd = new SqlCommand(strCmd, sConn);
                using (sConn)
                {
                    sConn.Open();
                    result = cmd.ExecuteNonQuery() == 1;
                    sConn.Close();
                }
            }
            catch
            {
                result = false;
            }

            return result;
        }
        static public bool delete(string strCmd)
        {
            return delete(strDefaultConnection, strCmd);
        }
        static public bool update(string strconnect, string strTableName, Dictionary<string, object> DnewSet, string strWhere)
        {
            if (DnewSet.Count == 0)
                return false;
            string selectStr = "select * from " + strTableName + " where " + strWhere;
            Dictionary<int, Dictionary<string, object>> tempdata = new Dictionary<int, Dictionary<string, object>>();
            if (select(strconnect, selectStr, ref tempdata) == false)
                return false;
            if (tempdata.Count != 1)
                return false;
            string strCmd = "update " + strTableName + " set  ";
            foreach (var item in DnewSet){
                strCmd = strCmd + item.Key + " = ";
                var t = item.Value.GetType();
                switch (t.Name)
                {
                    case "Int32":
                        strCmd = strCmd + item.Value.ToString() + ",";
                        break;
                    case "Char":
                        strCmd = strCmd + "'" + item.Value.ToString() + "',";
                        break;
                    case "Boolean":
                        strCmd = strCmd + (((bool)item.Value) ? "1" : "false") + ",";
                        break;
                    case "DateTime":
                        strCmd = strCmd + item.Value.ToString() + ",";
                        break;
                    case "String":
                        strCmd = strCmd + "'" + item.Value.ToString() + "',";
                        break;
                    case "Int64":
                        strCmd = strCmd + item.Value.ToString() + ",";
                        break;
                    case "UInt64":
                        strCmd = strCmd + item.Value.ToString() + ",";
                        break;
                }
            }
            strCmd = strCmd.Substring(0, strCmd.Length - 1) + " where " + strWhere;
            return insert(strconnect, strCmd);
        }
        static public bool update(string strTableName, Dictionary<string, object> DnewSet, string strWhere)
        {            
            return update(strDefaultConnection,strTableName,DnewSet,strWhere);
        }
        public static Dictionary<string, object> ExcuteProc(SqlConnection sConn, string sProcName, Dictionary<string, object> dicInputParam, Dictionary<string, object> dicOutParam)
        {
            SqlCommand cmd = new SqlCommand();
            cmd.CommandText = sProcName;
            cmd.Connection = sConn;
            cmd.CommandType = System.Data.CommandType.StoredProcedure;
            //添加输入参数
            foreach (KeyValuePair<string, object> kvpInput in dicInputParam)
            {
                cmd.Parameters.AddWithValue(kvpInput.Key, kvpInput.Value);
            }

            //添加输出参数
            foreach (KeyValuePair<string, object> kvpOut in dicOutParam)
            {
                SqlParameter param = new SqlParameter(kvpOut.Key, kvpOut.Value);
                param.Direction = System.Data.ParameterDirection.Output;
                param.Size = 64;
                param.DbType = System.Data.DbType.String;
                cmd.Parameters.Add(param);
            }

            //执行数据库操作命令
            using (sConn)
            {
                sConn.Open();
                cmd.ExecuteNonQuery();
                sConn.Close();
                sConn.Dispose();
            }

            //返回存储过程的输出参数值
            Dictionary<string, object> dicReturnParam = new Dictionary<string, object>();
            foreach (KeyValuePair<string, object> kvpOut in dicOutParam)
            {
                dicReturnParam.Add(kvpOut.Key, cmd.Parameters[kvpOut.Key].Value);
            }
            return dicReturnParam;
        }
        public static Dictionary<string, object> ExcuteProc(string strConnect, string sProcName, Dictionary<string, object> dicInputParam, Dictionary<string, object> dicOutParam)
        {
            SqlConnection sConn = new SqlConnection(strConnect);
            var result = ExcuteProc(sConn,sProcName, dicInputParam, dicOutParam);
            return result;
        }
        public static Dictionary<string, object> ExcuteProc(string sProcName, Dictionary<string, object> dicInputParam, Dictionary<string, object> dicOutParam)
        {
            return ExcuteProc(strDefaultConnection, sProcName, dicInputParam, dicOutParam);
        }
    }
}
