using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Web.Script.Serialization;
namespace CJKOther
{
    class Tool
    {
        static private JavaScriptSerializer json = new JavaScriptSerializer();
        static Tool() {}
        static public string Object2Json(object data){
            if (data == null)
                return "";
            return json.Serialize(data);
        }
        static public object Json2Object(string s)
        {
            return json.DeserializeObject(s);
        }        
        
    }
}
