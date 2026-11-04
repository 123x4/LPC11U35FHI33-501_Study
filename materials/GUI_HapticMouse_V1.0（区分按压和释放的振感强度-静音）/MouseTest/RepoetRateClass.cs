using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace MouseTest
{
    static class RepoetRateClass
    {
        [DllImport("user32.dll")]
        public static extern int RegisterRawInputDevices(RAWINPUTDEVICE[] pRawInputDevices, uint uiNumDevices, uint cbSize);

        [DllImport("user32.dll")]
        public static extern int GetRawInputData(IntPtr hRawInput, uint uiCommand, IntPtr pData, ref uint pcbSize, uint cbSizeHeader);

        [DllImport("user32.dll")]
        public static extern int GetRawInputDeviceList(IntPtr pRawInputDeviceList, ref uint numDevices, uint cbSize);

        // 定义 Raw Input 数据结构
        [StructLayout(LayoutKind.Sequential)]
        public struct RAWINPUT
        {
            public RAWINPUTHEADER header;
            public RID_DEVICE_INFO mouse;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct RAWINPUTHEADER
        {
            public uint dwType;
            public uint dwSize;
            public IntPtr hDevice;
            public IntPtr wParam;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct RID_DEVICE_INFO
        {
            public uint dwSize;
            public uint dwType;
            public mouseDeviceInfo mouse;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct mouseDeviceInfo
        {
            public uint dwId;
            public uint dwNumberOfButtons;
            public uint dwSampleRate;
            public uint dwReserved;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct RAWINPUTDEVICE
        {
            public ushort usUsagePage;
            public ushort usUsage;
            public uint dwFlags;
            public IntPtr hwndTarget;
        }
    }
}
