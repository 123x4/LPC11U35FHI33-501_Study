using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace MouseTest
{
    internal class CIniFileHandler
    {
        private string iniFile;
        private const int NULL_COUNT = 2;
        private const int GET_VALSTR_LENGTH = 1024;
        private const int GET_KEYSTR_LENGTH = 32768;
        private const int GET_CATSTR_LENGTH = 65536;
        [DllImport("KERNEL32.DLL", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode, EntryPoint = "GetPrivateProfileStringW", ExactSpelling = true, SetLastError = true)]
        private static extern int GetPrivateProfileString(string lpAppName, string lpKeyName, string lpDefault, string lpReturnString, int nSize, string lpFilename);
        [DllImport("KERNEL32.DLL", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode, EntryPoint = "WritePrivateProfileStringW", ExactSpelling = true, SetLastError = true)]
        private static extern int WritePrivateProfileString(string lpAppName, string lpKeyName, string lpString, string lpFilename);
        public CIniFileHandler(string filename)
        {
            this.iniFile = filename;
        }
        private string getIniFileString(string iniFile, string category, string key)
        {
            string text = new string(' ', 1024);
            CIniFileHandler.GetPrivateProfileString(category, key, "", text, 1024, iniFile);
            return text.Split(new char[1])[0];
        }
        private bool setInitFileString(string iniFile, string category, string key, string strD)
        {
            CIniFileHandler.WritePrivateProfileString(category, key, strD, iniFile);
            return true;
        }
        private List<string> getKeys(string iniFile, string category)
        {
            string text = new string(' ', 32768);
            CIniFileHandler.GetPrivateProfileString(category, null, null, text, 32768, iniFile);
            List<string> expr_2E = new List<string>(text.Split(new char[1]));
            expr_2E.RemoveRange(expr_2E.Count - 2, 2);
            return expr_2E;
        }
        private List<string> getCategories(string iniFile)
        {
            string text = new string(' ', 65536);
            CIniFileHandler.GetPrivateProfileString(null, null, null, text, 65536, iniFile);
            List<string> expr_2E = new List<string>(text.Split(new char[1]));
            expr_2E.RemoveRange(expr_2E.Count - 2, 2);
            return expr_2E;
        }
        public string getValue(string category, string key)
        {
            return this.getIniFileString(this.iniFile, category, key);
        }
        public bool setValue(string category, string key, string strData)
        {
            return this.setInitFileString(this.iniFile, category, key, strData);
        }
    }
}
