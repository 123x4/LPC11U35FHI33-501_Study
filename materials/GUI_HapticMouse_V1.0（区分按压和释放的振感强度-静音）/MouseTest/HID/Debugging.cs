using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MouseTest.HID
{
    public sealed partial class Debugging
    {
        ///  <summary>
        ///  Get text that describes the result of an API call.
        ///  将GetLastError得到的错误信息（这个错误信息是数字代号）转化成字符串信息的函数
        ///  </summary>
        ///  <param name="FunctionName"> the name of the API function. </param>
        ///  <returns>
        ///  The text.
        ///  </returns>

        public String ResultOfAPICall(String functionName)
        {
            Int32 bytes = 0;
            Int32 resultCode = 0;
            String resultString = "";
            resultString = new String(Convert.ToChar(0), 129);
            // Returns the result code for the last API call.            
            resultCode = System.Runtime.InteropServices.Marshal.GetLastWin32Error();
            // Get the result message that corresponds to the code.
            Int64 temp = 0;
            bytes = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, ref temp, resultCode, 0, resultString, 128, 0);
            // Subtract two characters from the message to strip the CR and LF.           
            if (bytes > 2)
            {
                resultString = resultString.Remove(bytes - 2, 2);
            }
            // Create the String to return.            
            resultString = "\r\n" + functionName + "\r\n" + "Result = " + resultString + "\r\n";
            return resultString;
        }
    }
}
