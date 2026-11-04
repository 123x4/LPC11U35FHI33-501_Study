using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace MouseTest.HID
{
    public sealed partial class Debugging
    {
        public const Int16 FORMAT_MESSAGE_FROM_SYSTEM = 0X1000;
        /// <summary>
        /// FormatMessage功能就是将GetLastError得到的错误信息（这个错误信息是数字代号）转化成字符串信息的函数
        /// </summary>
        /// <param name="dwFlags"></param>
        /// <param name="lpSource"></param>
        /// <param name="dwMessageId"></param>
        /// <param name="dwLanguageZId"></param>
        /// <param name="lpBuffer"></param>
        /// <param name="nSize"></param>
        /// <param name="Arguments"></param>
        /// <returns></returns>
        [DllImport("kernel32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        public static extern Int32 FormatMessage(Int32 dwFlags, ref Int64 lpSource, Int32 dwMessageId, Int32 dwLanguageZId, String lpBuffer, Int32 nSize, Int32 Arguments);
    }
}
