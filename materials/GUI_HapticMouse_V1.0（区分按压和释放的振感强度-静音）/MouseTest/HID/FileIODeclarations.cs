using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using Microsoft.Win32.SafeHandles;

namespace MouseTest.HID
{
    public sealed class FileIO
    {
        public const int LEFTHIGH = 0;
        public const int LEFTLEFT = 183;
        public const int RIGHTHIGH = 0;
        public const int RIGHTLEFT = 472;
        public const string GuidWriteFull = "\\\\?\\hid#vid_1a81&pid_1004&mi_01&col06#7&b59f931&0&0005#{4d1e55b2-f16f-11cf-88cb-001111000030}";
        public const string GuidWrite = "mi_03";
        public const string GuidRead = "mi_03";
        public const string GuidWriteMouse = "mi_03";
        public const string GuidReadMouse = "mi_03";
        public const string GuidReadFull = "\\\\?\\hid#vid_1a81&pid_1004&mi_01&col02#7&b59f931&0&0001#{4d1e55b2-f16f-11cf-88cb-001111000030}";

        public const int USER = 1024;
        //as mfc Define WM_TEST WM_USER + 101 
        public const int WM_MYCOSTOM = 1125;
        public const int WM_INPUT = 0x00FF;//报告率
        public const string DriverName = "G100 Mouse";


        public const Int32 FILE_FLAG_OVERLAPPED = 0X40000000;
        public const Int32 FILE_SHARE_READ = 1;
        public const Int32 FILE_SHARE_WRITE = 2;
        public const UInt32 GENERIC_READ = 0X80000000;
        public const UInt32 GENERIC_WRITE = 0X40000000;
        public const Int32 INVALID_HANDLE_VALUE = -1;
        public const Int32 OPEN_EXISTING = 3;
        public const Int32 WAIT_TIMEOUT = 0X102;
        public const Int32 WAIT_OBJECT_0 = 0;
        public const int HWND_BROADCAST = 0xffff;
        public const Int32 SPIF_UPDATEINIFILE = 0x01;
        public const Int32 SPIF_SENDWININICHANGE = 0x02;
        /**/
        /// <summary>
        /// 鼠标动作枚举
        /// </summary>
        /// 

        public const UInt32 move = 0x0001;
        public const UInt32 leftdown = 0x0002;
        public const UInt32 leftup = 0x0004;
        public const UInt32 rightdown = 0x0008;
        public const UInt32 rightup = 0x0010;
        public const UInt32 middledown = 0x0020;
        public const UInt32 middleup = 0x0040;
        public const UInt32 xdown = 0x0080;
        public const UInt32 xup = 0x0100;
        public const UInt32 wheel = 0x0800;
        public const UInt32 virtualdesk = 0x4000;
        public const UInt32 absolute = 0x8000;
        public const UInt32 MOUSEEVENTF_HWHEEL = 0x1000;

        public const int SB_LINELEFT = 0x0;
        public const int SB_LINERIGHT = 0x1;
        public const int SB_ENDSCROLL = 0x8;
        public const int WM_HSCROLL = 0x114;
        public const int WM_VSCROLL = 0x115;
        public const int WM_MOUSEWHEEL = 0x020A;
        public const int WM_KEYDOWN = 0x100;
        public const int WM_KEYUP = 0x101;


        public const int SB_THUMBPOSITION = 4;


        public const int GW_HWNDFIRST = 0;
        public const int GW_HWNDLAST = 1;
        public const int GW_HWNDNEXT = 2;
        public const int GW_HWNDPREV = 3;
        public const int GW_OWNER = 4;
        public const int GW_CHILD = 5;

        public const int GWL_STYLE = -16;

        public const Int32 AW_HOR_POSITIVE = 0x00000001; //自左向右显示窗口
        public const Int32 AW_HOR_NEGATIVE = 0x00000002; //自右向左显示窗口
        public const Int32 AW_VER_POSITIVE = 0x00000004; //自顶向下显示窗口
        public const Int32 AW_VER_NEGATIVE = 0x00000008; //自下向上显示窗口
        public const Int32 AW_CENTER = 0x00000010; //与 AW_HIDE 效果配合使用则效果为窗口几内重叠, 单独使用窗口向外扩展.
        public const Int32 AW_HIDE = 0x00010000; //隐藏窗口
        public const Int32 AW_ACTIVATE = 0x00020000; //激活窗口, 在使用了 AW_HIDE 效果时不可使用此效果
        public const Int32 AW_SLIDE = 0x00040000; //使用滑动类型, 默认为该类型. 当使用 AW_CENTER 效果时, 此效果被忽略
        public const Int32 AW_BLEND = 0x00080000; //使用淡入效果

        [StructLayout(LayoutKind.Sequential)]
        public class SECURITY_ATTRIBUTES
        {
            public Int32 nLength;
            public Int32 lpSecurityDescriptor;
            public Int32 bInheritHandle;
        }

        //CancelIo函数来取消读取动作
        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern Int32 CancelIo(SafeFileHandle hFile);

        //创建或打开一个命名的或无名的事件对象
        [DllImport("kernel32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        public static extern IntPtr CreateEvent(IntPtr SecurityAttributes, Boolean bManualReset, Boolean bInitialState, String lpName);

        /// <summary>
        ///连接设备文件、获取设备文件
        /// </summary>
        /// <param name="lpFileName"></param>
        /// <param name="dwDesiredAccess"></param>
        /// <param name="dwShareMode"></param>
        /// <param name="lpSecurityAttributes"></param>
        /// <param name="dwCreationDisposition"></param>
        /// <param name="dwFlagsAndAttributes"></param>
        /// <param name="hTemplateFile"></param>
        /// <returns></returns>
        [DllImport("kernel32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        public static extern SafeFileHandle CreateFile(String lpFileName, UInt32 dwDesiredAccess, Int32 dwShareMode, IntPtr lpSecurityAttributes, Int32 dwCreationDisposition, Int32 dwFlagsAndAttributes, Int32 hTemplateFile);

        //判断一个重叠操作当前的状态
        //该函数返回重叠操作的结果，用来判断异步操作是否完成，它是通过判断OVERLAPPED结构中的hEvent是否被置位来

        [DllImport("kernel32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        public static extern Boolean GetOverlappedResult(SafeFileHandle hFile, IntPtr lpOverlapped, ref Int32 lpNumberOfBytesTransferred, Boolean bWait);

        //读取设备文件
        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern Boolean ReadFile(SafeFileHandle hFile, IntPtr lpBuffer, Int32 nNumberOfBytesToRead, ref Int32 lpNumberOfBytesRead, IntPtr lpOverlapped);

        //WaitForSingleObject函数用来检测hHandle事件的信号状态
        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern Int32 WaitForSingleObject(IntPtr hHandle, Int32 dwMilliseconds);

        //将数据写入一个文件。该函数比fwrite函数要灵活的多。也可将这个函数应用于对通信设备、管道、套接字以及邮槽的处理
        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern Boolean WriteFile(SafeFileHandle hFile, Byte[] lpBuffer, Int32 nNumberOfBytesToWrite, ref Int32 lpNumberOfBytesWritten, IntPtr lpOverlapped);
        //该函数合成一次击键事件。系统可使用这种合成的击键事件来产生WM_KEYUP或WM_KEYDOWN消息，
        [DllImport("user32.dll", SetLastError = true)]
        public static extern void keybd_event(byte bVk, byte bScan, uint dwFlags, uint dwExtraInfo);

        //该函数将一虚拟键码翻译（映射）成一扫描码或一字符值，或者将一扫描码翻译成一虚拟键码
        [DllImport("user32.dll", SetLastError = true)]
        public static extern byte MapVirtualKey(byte wCode, int wMap);
        //该函数把光标移到屏幕的指定位置
        [DllImport("user32.dll", SetLastError = true)]
        public static extern bool setcursorpos(int x, int y);

        //该函数综合鼠标击键和鼠标动作。
        [DllImport("user32.dll", SetLastError = true)]
        public static extern void mouse_event(UInt32 flags, int dx, int dy, int data, uint extrainfo);
        //该函数检取光标的位置，以屏幕坐标表示。
        [DllImport("User32.dll ", SetLastError = true)]
        public static extern Int32 GetCursorPos(ref Point lpPoint);

        //该函数获得包含指定点的窗口的句柄。
        [DllImport("user32.dll", SetLastError = true)]
        public static extern IntPtr WindowFromPoint(Point lpPoint);

        //发送异步消息
        [DllImport("user32.dll", SetLastError = true)]
        public static extern IntPtr PostMessage(IntPtr hwnd, int wMsg, int wParam, int lParam);
        //发送同步消息
        [DllImport("user32.dll", SetLastError = true)]
        public static extern IntPtr SendMessage(IntPtr hwnd, int wMsg, uint wParam, uint lParam);


        //拥有指定窗口类名或窗口名的窗口的句柄
        [DllImport("user32.dll", SetLastError = true)]
        public static extern IntPtr FindWindow(string lpClassName, string lpWindowName);
        //该函数获得一个窗口的句柄，该窗口的类名和窗口名与给定的字符串相匹配。
        //这个函数查找子窗口，从排在给定的子窗口后面的下一个子窗口开始。在查找时不区分大小写。

        [DllImport("user32.dll", SetLastError = true)]
        public static extern IntPtr FindWindowEx(IntPtr hwndParent, IntPtr hwndChildAfter, string lpszClass, string lpszWindow);

        //枚举一个父窗口的所有子窗口。
        [DllImport("user32")]
        public static extern bool EnumChildWindows(IntPtr window, EnumWindowProc callback, IntPtr i);
        public delegate bool EnumWindowProc(IntPtr hWnd, IntPtr parameter);

        //该函数获得一个指定子窗口的父窗口句柄。
        [DllImport("user32.dll", SetLastError = true)]
        public static extern IntPtr GetParent(IntPtr hwnd);
        //该函数获得有关指定窗口的信息，函数也获得在额外窗口内存中指定偏移位地址的32位度整型值。 
        [DllImport("user32.dll", SetLastError = true)]
        public static extern ulong GetWindowLong(IntPtr hwnd, int nIndex);

        //该函数返回与指定窗口有特定关系（如Z序或所有者）的窗口句柄。
        [DllImport("user32.dll", SetLastError = true)]
        public static extern IntPtr GetWindow(IntPtr hwnd, int nIndex);

        //该函数查询或设置系统级参数。该函数也可以在设置参数中更新用户配置文件。 
        [DllImport("user32.dll ", SetLastError = true)]
        public static extern int SystemParametersInfo(int uAction, int uParam, ref int lpvParam, int fuWinIni);
        [DllImport("user32.dll ", SetLastError = true)]
        public static extern int SystemParametersInfo(int uAction, int uParam, int lpvParam, int fuWinIni);
        [DllImport("user32.dll ", SetLastError = true)]
        public static extern int SystemParametersInfo(int uAction, int uParam, int[] lpvParam, int fuWinIni);

        [DllImport("user32.dll ", SetLastError = true)]
        public static extern int SystemParametersInfo(int uAction, int uParam, ref int[] lpvParam, int fuWinIni);

        [DllImportAttribute("user32.dll")]
        //IntPtr hwnd: 目标窗口的句柄对象, 一般为 this.Handle
        //int dwTime: 动画的持续时间, 数值越大动画效果的时间越长
        //int dwFlags: 动画效果类型选项
        public static extern bool AnimateWindow(IntPtr hwnd, int dwTime, int dwFlags);

        //该函数取得鼠标的当前双击时间
        [DllImport("user32.dll ", SetLastError = true)]
        public static extern int GetDoubleClickTime();
        //该函数设置鼠标的当前双击时间
        [DllImport("user32.dll ", SetLastError = true)]
        public static extern int SetDoubleClickTime(int wCount);

        //运行一个外部程序（或者是打开一个已注册的文件、打开一个目录、打印一个文件等等），并对外部程序有一定的控制。
        [DllImport("shell32.dll", EntryPoint = "ShellExecute")]
        public static extern int ShellExecute(
            IntPtr hwnd,
            string lpOperation,
            string lpFile,
            string lpParameters,
            string lpDirectory,
            int nShowCmd
        );

        [DllImport("shell32.dll", EntryPoint = "#61", CharSet = CharSet.Auto)]
        public static extern void RunFileDlg
            (
            IntPtr hwndOwner,
            IntPtr hIcon,
            string lpstrDirectory,
            string lpstrTitle,
            string lpstrDescription,
            int uFlags
            );
        //该函数设置由不同线程产生的窗口的显示状态。
        [DllImport("User32.dll")]
        public static extern bool ShowWindowAsync(IntPtr hWnd, int cmdShow);

        //该函数将创建指定窗口的线程设置到前台，并且激活该窗口
        [DllImport("User32.dll")]
        public static extern bool SetForegroundWindow(IntPtr hWnd);

        /// <summary>
        /// 获取前台窗体
        /// </summary>
        /// <returns></returns>
        [DllImport("user32.dll")]
        public static extern IntPtr GetForegroundWindow();

        /// <summary>
        /// 该函数返回创建指定窗口线程的标识和创建窗口的进程的标识符
        /// </summary>
        /// <param name="hwnd"></param>
        /// <param name="ProcessId">地址</param>
        /// <returns></returns>
        [DllImport("User32.dll")]
        public static extern int GetWindowThreadProcessId(IntPtr hwnd, IntPtr ProcessId);

        /// 该函数返回创建当前窗口线程id
        [DllImport("kernel32.dll")]
        public static extern int GetCurrentThreadId();

        //关联两个线程
        [DllImport("user32.dll")]
        public static extern int AttachThreadInput(int idAttach, int idAttachTo, bool fAttach);

        /// <summary>
        /// 把窗体带到Z的最前端
        /// </summary>
        /// <param name="hwnd"></param>
        /// <returns></returns>
        [DllImport("user32.dll")]
        public static extern bool BringWindowToTop(IntPtr hwnd);
    }
}
