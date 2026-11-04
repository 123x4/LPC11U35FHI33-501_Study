using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace MouseTest.HID
{
    public sealed partial class DeviceManagement
    {
        ///<summary >
        // API declarations relating to device management (SetupDixxx and 
        // RegisterDeviceNotification functions).   
        /// </summary>

        // from dbt.h
        public const Int32 DBT_DEVNODES_POQWE_UP = 0x00012;
        public const Int32 DBT_DEVNODES_CHANGED = 0x0007;
        public const Int32 DBT_DEVICEARRIVAL = 0X8000;
        public const Int32 DBT_DEVICEREMOVECOMPLETE = 0X8004;
        public const Int32 DBT_DEVTYP_DEVICEINTERFACE = 5;
        public const Int32 DBT_DEVTYP_HANDLE = 6;
        public const Int32 DEVICE_NOTIFY_ALL_INTERFACE_CLASSES = 4;
        public const Int32 DEVICE_NOTIFY_SERVICE_HANDLE = 1;
        public const Int32 DEVICE_NOTIFY_WINDOW_HANDLE = 0;
        public const Int32 WM_DEVICECHANGE = 0X219;
        public const Int32 WM_POWERBROADCAST = 0x0218;     // r_winuser pbt
                                                           // from setupapi.h

        public const Int32 DIGCF_PRESENT = 2;
        public const Int32 DIGCF_DEVICEINTERFACE = 0X10;

        // Two declarations for the DEV_BROADCAST_DEVICEINTERFACE structure.

        // Use this one in the call to RegisterDeviceNotification() and
        // in checking dbch_devicetype in a DEV_BROADCAST_HDR structure:

        [StructLayout(LayoutKind.Sequential)]
        public class DEV_BROADCAST_DEVICEINTERFACE
        {
            public Int32 dbcc_size;
            public Int32 dbcc_devicetype;
            public Int32 dbcc_reserved;
            public Guid dbcc_classguid;
            public Int16 dbcc_name;
        }

        // Use this to read the dbcc_name String and classguid:

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Auto)]
        public class DEV_BROADCAST_DEVICEINTERFACE_1
        {
            public Int32 dbcc_size;
            public Int32 dbcc_devicetype;
            public Int32 dbcc_reserved;
            [MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.U1, SizeConst = 16)]
            public Byte[] dbcc_classguid;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 255)]
            public Char[] dbcc_name;
        }

        [StructLayout(LayoutKind.Sequential)]
        public class DEV_BROADCAST_HDR
        {
            public Int32 dbch_size;
            public Int32 dbch_devicetype;
            public Int32 dbch_reserved;
        }

        public struct SP_DEVICE_INTERFACE_DATA
        {
            public Int32 cbSize;
            public System.Guid InterfaceClassGuid;
            public Int32 Flags;
            public IntPtr Reserved;
        }


        [StructLayout(LayoutKind.Sequential, Pack = 2)]
        public struct SP_DEVICE_INTERFACE_DETAIL_DATA
        {
            public Int32 cbSize;
            public String DevicePath;
        }

        public struct SP_DEVINFO_DATA
        {
            public Int32 cbSize;
            public System.Guid ClassGuid;
            public Int32 DevInst;
            public Int32 Reserved;
        }

        /// <summary>
        /// 注册设备或者设备类型，在指定的窗口返回相关的信息
        /// </summary>
        /// <param name="hRecipient"></param>
        /// <param name="NotificationFilter"></param>
        /// <param name="Flags"></param>
        /// <returns></returns>
		[DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        public static extern IntPtr RegisterDeviceNotification(IntPtr hRecipient, IntPtr NotificationFilter, Int32 Flags);

        /// <summary>
        /// 通过名柄，关闭指定设备的信息。(主要应用于清理非托管资源，并与RegisterDeviceNotification配对使用)
        /// </summary>
        /// <param name="Handle"></param>
        /// <returns></returns>
        [DllImport("user32.dll", SetLastError = true)]
        public static extern Boolean UnregisterDeviceNotification(IntPtr Handle);


        /// <summary>
        /// 创建设备信息块列表
        /// </summary>
        /// <param name="ClassGuid"></param>
        /// <param name="hwndParent"></param>
        /// <returns></returns>
		[DllImport("setupapi.dll", SetLastError = true)]
        public static extern Int32 SetupDiCreateDeviceInfoList(ref System.Guid ClassGuid, Int32 hwndParent);

        /// <summary>
        /// 释放设备相关信息，销毁一个设备信息集合，并且释放所有关联的内存
        /// </summary>
        /// <param name="DeviceInfoSet"></param>
        /// <returns></returns>
        [DllImport("setupapi.dll", CharSet = CharSet.Auto, SetLastError = true)]
        public static extern Int32 SetupDiDestroyDeviceInfoList(IntPtr DeviceInfoSet);

        /// <summary>
        /// 从已经获取的设备接口列表信息中获取信息并使用结构保存,每调用一次会依次返回一个
        /// 获取设备，true获取到
        /// </summary>
        /// <param name="DeviceInfoSet"></param>
        /// <param name="DeviceInfoData"></param>
        /// <param name="InterfaceClassGuid"></param>
        /// <param name="MemberIndex"></param>
        /// <param name="DeviceInterfaceData"></param>
        /// <returns></returns>
        [DllImport("setupapi.dll", CharSet = CharSet.Auto, SetLastError = true)]
        public static extern Boolean SetupDiEnumDeviceInterfaces(IntPtr DeviceInfoSet, IntPtr DeviceInfoData, ref System.Guid InterfaceClassGuid, Int32 MemberIndex, ref SP_DEVICE_INTERFACE_DATA DeviceInterfaceData);

        /// <summary>
        /// 获取一个指定类别或全部类别的所有已安装设备的信息。
        /// 过滤设备，获取需要的设备
        /// </summary>
        /// <param name="ClassGuid"></param>
        /// <param name="Enumerator"></param>
        /// <param name="hwndParent"></param>
        /// <param name="Flags"></param>
        /// <returns></returns>
		[DllImport("setupapi.dll", SetLastError = true, CharSet = CharSet.Auto)]
        public static extern IntPtr SetupDiGetClassDevs(ref System.Guid ClassGuid, IntPtr Enumerator, IntPtr hwndParent, Int32 Flags);


        /// <summary>
        ///获取接口的详细信息 必须调用两次 第1次返回长度 第2次获取数据 
        /// </summary>
        /// <param name="DeviceInfoSet"></param>
        /// <param name="DeviceInterfaceData"></param>
        /// <param name="DeviceInterfaceDetailData"></param>
        /// <param name="DeviceInterfaceDetailDataSize"></param>
        /// <param name="RequiredSize"></param>
        /// <param name="DeviceInfoData"></param>
        /// <returns></returns>
        [DllImport("setupapi.dll", SetLastError = true, CharSet = CharSet.Auto)]
        public static extern Boolean SetupDiGetDeviceInterfaceDetail(IntPtr DeviceInfoSet, ref SP_DEVICE_INTERFACE_DATA DeviceInterfaceData, IntPtr DeviceInterfaceDetailData, Int32 DeviceInterfaceDetailDataSize, ref Int32 RequiredSize, IntPtr DeviceInfoData);


    }
}
