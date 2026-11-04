using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using Microsoft.Win32.SafeHandles;

namespace MouseTest.HID
{
    public sealed partial class Hid
    {
        //  API declarations for HID communications.

        //  from hidpi.h
        //  Typedef enum defines a set of integer constants for HidP_Report_Type

        public const Int16 HidP_Input = 0;
        public const Int16 HidP_Output = 1;
        public const Int16 HidP_Feature = 2;

        [StructLayout(LayoutKind.Sequential)]
        public struct HIDD_ATTRIBUTES
        {
            public Int32 Size;
            public UInt16 VendorID;
            public UInt16 ProductID;
            public UInt16 VersionNumber;
        }

        public struct HIDP_CAPS
        {
            public Int16 Usage;
            public Int16 UsagePage;
            public Int16 InputReportByteLength;
            public Int16 OutputReportByteLength;
            public Int16 FeatureReportByteLength;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 17)]
            public Int16[] Reserved;
            public Int16 NumberLinkCollectionNodes;
            public Int16 NumberInputButtonCaps;
            public Int16 NumberInputValueCaps;
            public Int16 NumberInputDataIndices;
            public Int16 NumberOutputButtonCaps;
            public Int16 NumberOutputValueCaps;
            public Int16 NumberOutputDataIndices;
            public Int16 NumberFeatureButtonCaps;
            public Int16 NumberFeatureValueCaps;
            public Int16 NumberFeatureDataIndices;
        }

        //  If IsRange is false, UsageMin is the Usage and UsageMax is unused.
        //  If IsStringRange is false, StringMin is the String index and StringMax is unused.
        //  If IsDesignatorRange is false, DesignatorMin is the designator index and DesignatorMax is unused.

        public struct HidP_Value_Caps
        {
            public Int16 UsagePage;
            public Byte ReportID;
            public Int32 IsAlias;
            public Int16 BitField;
            public Int16 LinkCollection;
            public Int16 LinkUsage;
            public Int16 LinkUsagePage;
            public Int32 IsRange;
            public Int32 IsStringRange;
            public Int32 IsDesignatorRange;
            public Int32 IsAbsolute;
            public Int32 HasNull;
            public Byte Reserved;
            public Int16 BitSize;
            public Int16 ReportCount;
            public Int16 Reserved2;
            public Int16 Reserved3;
            public Int16 Reserved4;
            public Int16 Reserved5;
            public Int16 Reserved6;
            public Int32 LogicalMin;
            public Int32 LogicalMax;
            public Int32 PhysicalMin;
            public Int32 PhysicalMax;
            public Int16 UsageMin;
            public Int16 UsageMax;
            public Int16 StringMin;
            public Int16 StringMax;
            public Int16 DesignatorMin;
            public Int16 DesignatorMax;
            public Int16 DataIndexMin;
            public Int16 DataIndexMax;
        }

        //清空输入缓冲区
        [DllImport("hid.dll", SetLastError = true)]
        public static extern Boolean HidD_FlushQueue(SafeFileHandle HidDeviceObject);

        //释放设备
        [DllImport("hid.dll", SetLastError = true)]
        public static extern Boolean HidD_FreePreparsedData(IntPtr PreparsedData);

        /// <summary>
        ///  获取厂商与产品，版本号
        /// </summary>
        /// <param name="HidDeviceObject"></param>
        /// <param name="Attributes"></param>
        /// <returns></returns>
        [DllImport("hid.dll", SetLastError = true)]
        public static extern Boolean HidD_GetAttributes(SafeFileHandle HidDeviceObject, ref HIDD_ATTRIBUTES Attributes);

        //从设备获取特征报文(数据)
        [DllImport("hid.dll", SetLastError = true)]
        public static extern Boolean HidD_GetFeature(SafeFileHandle HidDeviceObject, Byte[] lpReportBuffer, Int32 ReportBufferLength);

        //用于设备到主机方向的数据通信
        [DllImport("hid.dll", SetLastError = true)]
        public static extern Boolean HidD_GetInputReport(SafeFileHandle HidDeviceObject, Byte[] lpReportBuffer, Int32 ReportBufferLength);

        //获得GUID
        [DllImport("hid.dll", SetLastError = true)]
        public static extern void HidD_GetHidGuid(ref System.Guid HidGuid);

        //获取 HID设备驱动中的 buffer 大小
        [DllImport("hid.dll", SetLastError = true)]
        public static extern Boolean HidD_GetNumInputBuffers(SafeFileHandle HidDeviceObject, ref Int32 NumberBuffers);

        //获取保存设备能力信息的缓冲器的句柄
        [DllImport("hid.dll", SetLastError = true)]
        public static extern Boolean HidD_GetPreparsedData(SafeFileHandle HidDeviceObject, ref IntPtr PreparsedData);

        //发送一个特征报文给设备
        [DllImport("hid.dll", SetLastError = true)]
        public static extern Boolean HidD_SetFeature(SafeFileHandle HidDeviceObject, Byte[] lpReportBuffer, Int32 ReportBufferLength);
        //通过 HidD_SetNumInputBuffers 来改变HID设备驱动中的 buffer 大小
        [DllImport("hid.dll", SetLastError = true)]
        public static extern Boolean HidD_SetNumInputBuffers(SafeFileHandle HidDeviceObject, Int32 NumberBuffers);

        //用于主机到设备方向的数据通信
        [DllImport("hid.dll", SetLastError = true)]
        public static extern Boolean HidD_SetOutputReport(SafeFileHandle HidDeviceObject, Byte[] lpReportBuffer, Int32 ReportBufferLength);

        //获取设备的容量
        [DllImport("hid.dll", SetLastError = true)]
        public static extern Int32 HidP_GetCaps(IntPtr PreparsedData, ref HIDP_CAPS Capabilities);

        //获取描述设备能力(容量)的结构
        [DllImport("hid.dll", SetLastError = true)]
        public static extern Int32 HidP_GetValueCaps(Int32 ReportType, Byte[] ValueCaps, ref Int32 ValueCapsLength, IntPtr PreparsedData);

        //通过HidD_GetIndexedString来获得字符串描述符
        [DllImport("hid.dll", SetLastError = true)]
        public static extern Boolean HidD_GetIndexedString(SafeFileHandle HidDeviceObject, Int32 NumberBuffers, Byte[] lpReportBuffer, Int32 ReportBufferLength);

        //返回上一个API执行的错误
        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern int GetLastError();

        [DllImport("kernel32.dll", CharSet = CharSet.Ansi)]
        public static extern void SetLastError(int dwErr);
    }
}
