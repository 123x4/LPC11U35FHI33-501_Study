using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Microsoft.Win32.SafeHandles;
using static MouseTest.HID.DeviceManagement;
using System.Windows.Forms;

namespace MouseTest.HID
{
    public class HIDCommonClass
    {

        #region 变量
        public ArrayList myMouseMacroData = new ArrayList();
        //public DriverEntity myDriverEntity = new DriverEntity();
        //public OutBuffClass Cmd = new OutBuffClass();
        public int FWver;
        public bool isMouseUpdate;
        public bool isDelMacroID;
        public bool isHaveDevice = false;
        public bool endthreadMouse = true;//线程是否结束
        public bool endthreadMouseMTK = true;//线程是否结束

        public Thread myMouseThread;//线程
        public Thread myMouseThreadMTK;//线程

        public int MouseVID; //鼠标VID
        public int MousePID; //鼠标PID
        public int VerNum;
        public static int VPIDpassVerF = 0;
        public static int VPIDpassVerFMTK = 0;

        public Boolean myDeviceDetected;//是否发现鼠标设备
        public Boolean myDeviceDetectedMTK;//是否发现鼠标设备

        public DeviceManagement MyDeviceManagement = new DeviceManagement();
        public DeviceManagement MyDeviceManagementMTK = new DeviceManagement();

        public String myDevicePathName, myDevicePathMouseName, myDevicePathNameMTK, myDevicePathMouseNameMTK;//设备路径
        private SafeFileHandle hidHandleMouse;//设备文件句柄
        private SafeFileHandle hidHandleMouseMTK;//设备文件句柄

        public Hid MyHid = new Hid();
        public ArrayList IntVidPid = new ArrayList();//文件里的vid,pid
        private String myDevicePathName64Mouse;//设备输出文件路径
        private String hidUsage;//hid 使用习惯
        private String hidUsageMTK;//hid 使用习惯

        public SafeFileHandle readHandle, writeHandleMouse, writeHandle64, readHandleMTK, writeHandleMouseMTK, writeHandle64MTK;//读写的句柄（地址）



        private String myDevicePathName6464;//设备输出文件路径
        public String myDevicePathName64;
        public String myDevicePathName64MTK;

        private IntPtr deviceNotificationHandle;//设备被发现句柄
        private IntPtr deviceNotificationHandleMTK;//设备被发现句柄


        public IntPtr FrmMainHandle { get; set; }//主窗体句柄
        public IntPtr FrmMainHandleMTK { get; set; }//主窗体句柄

        public List<string> devicePathNameList = new List<string>();
        public int VID, PID;
        public int BootloaderVID, BootloaderPID;
        public Boolean IsAppFWUpdate;
        //public DeviceState MyDeviceState;
        public int DataAccessMouse = 0;
        public int LastAccessMouse = 0;
        #endregion

        #region 委托
        //public delegate void InitUI(DeviceState type, Boolean IsAppUp);//刷新界面的委托
        //public event InitUI Init_event;
        //public delegate void Main(DriverEntity myDriverEntity);
        //public event Main Main_event;
        public delegate void DPIStateSyn(byte[] Buff);
        public event DPIStateSyn DPIStateSyn_Event;
        public delegate void MouseMacroDown(int MacroID, int Offset, HIDCommonClass MyHID);//委托
        public event MouseMacroDown Mousedown_event;
        public delegate void MouseProfileMacroDown(int MacroID, int Offset, HIDCommonClass MyHID);//委托
        public event MouseProfileMacroDown MouseProfiledown_event;
        public delegate void MouseDataList(ArrayList myMouseMacroData);//委托
        public event MouseDataList MouseDataList_event;
        public static int KeyboardVersion = 0;

        public delegate void GetMouseInputData(byte[] intputebuffer);//委托
        public event GetMouseInputData GetMouseInputData_event;
        public event GetMouseInputData GetMouseInputData_eventMTK;

        #endregion

        #region  ThreadFindDevice
        //public bool StopTread() 
        //{
        //    bool TStop = false;
        //    myMouseThread.Abort();//调用Thread.Abort方法试图强制终止thread线程
        //                          //上面调用Thread.Abort方法后线程thread不一定马上就被终止了，所以我们在这里写了个循环来做检查，看线程thread是否已经真正停止。其实也可以在这里使用Thread.Join方法来等待线程thread终止，Thread.Join方法做的事情和我们在这里写的循环效果是一样的，都是阻塞主线程直到thread线程终止为止
        //    while (myMouseThread.ThreadState != ThreadState.Aborted)
        //    {
        //        //当调用Abort方法后，如果thread线程的状态不为Aborted，主线程就一直在这里做循环，直到thread线程的状态变为Aborted为止
        //        Thread.Sleep(100);
        //    }
        //    if (myMouseThread.ThreadState == ThreadState.Aborted) 
        //    {
        //        TStop = true;
        //    }
        //    return TStop;
        //}


        public Boolean SendDevice64(Byte[] outputBuffer, int tag)
        {
            Thread.Sleep(30);
            Boolean success = false;
            try
            {
                switch (tag)
                {
                    case 0:
                        if ((writeHandle64 != null) /*&& !writeHandle64.IsInvalid*/)
                        {

                            int len = 0;
                            success = FileIO.WriteFile(writeHandle64, outputBuffer, outputBuffer.Length, ref len, IntPtr.Zero);
                        }
                        break;
                    case 1:
                        if ((writeHandle64 != null))
                        {
                            int len = 0;
                            success = FileIO.WriteFile(writeHandle64, outputBuffer, outputBuffer.Length, ref len, IntPtr.Zero);
                        }
                        break;
                    case 2:
                        if ((writeHandle64 != null) && !writeHandle64.IsInvalid)
                        {

                            success = Hid.HidD_SetFeature(writeHandle64, outputBuffer, outputBuffer.Length);
                        }
                        break;
                    case 3:
                        if ((writeHandle64 != null) && !writeHandle64.IsInvalid)
                        {

                            success = Hid.HidD_GetFeature(writeHandle64, outputBuffer, outputBuffer.Length);
                        }
                        break;

                }
                return success;
            }
            catch (Exception ex)
            {
                // MessageBox.Show("发送数据到设备异常：" + ex.ToString());
                return success;
            }
        }

        public Boolean SendDevice64MTK(Byte[] outputBuffer, int tag)
        {

            Thread.Sleep(30);
            Boolean success = false;
            try
            {
                switch (tag)
                {
                    case 0:
                        if ((writeHandle64MTK != null) /*&& !writeHandle64.IsInvalid*/)
                        {

                            int len = 0;
                            success = FileIO.WriteFile(writeHandle64MTK, outputBuffer, outputBuffer.Length, ref len, IntPtr.Zero);
                        }
                        break;
                    case 1:
                        if ((writeHandleMouseMTK != null))
                        {
                            int len = 0;
                            success = FileIO.WriteFile(writeHandleMouseMTK, outputBuffer, outputBuffer.Length, ref len, IntPtr.Zero);
                        }
                        break;
                    case 2:
                        if ((writeHandle64MTK != null) && !writeHandle64MTK.IsInvalid)
                        {

                            success = Hid.HidD_SetFeature(writeHandle64MTK, outputBuffer, outputBuffer.Length);
                        }
                        break;

                }
                return success;
            }
            catch (Exception ex)
            {
                // MessageBox.Show("发送数据到设备异常：" + ex.ToString());
                return success;
            }
        }

        public bool ExitfindMyDeviceMouseMTK()
        {

            devicePathNameList.Clear();
            try
            {

                bool isDevice = false;
                endthreadMouseMTK = true;
                //MouseVID = Convert.ToInt32(MainWindow.mainWindow.VidTBCB.Text, 16);
                //MousePID = Convert.ToInt32(MainWindow.mainWindow.PidTBCB.Text, 16);
                //VerNum = Convert.ToInt32(MainWindow.mainWindow.VerTBCB.Text, 16);
                FindTheHidMTK(MouseVID, MousePID, VerNum);
                if (IsfindMTK)
                {
                    isDevice = true;
                }
                else
                {
                    isDevice = false;
                }


                return isDevice;
            }
            catch (Exception ex)
            {
                return false;
            }
        }

        /// <summary>
        /// 找设备
        /// </summary>
        /// <returns></returns>
        public bool ExitfindMyDeviceMouse()
        {
            try
            {

                bool isDevice = false;
                endthreadMouse = true;
              
             
                    MouseVID = Convert.ToInt32(MainWindow.mainWindow.txtVid.Text, 16);
                    MousePID = Convert.ToInt32(MainWindow.mainWindow.txtPid.Text, 16);
                    VerNum = Convert.ToInt32(MainWindow.mainWindow.txtVer.Text, 16);
                    FindTheHid(MouseVID, MousePID, VerNum, MainWindow.mainWindow.setPort,MainWindow.mainWindow.getPort);
               
               

                if (Isfind)
                {
                    isDevice = true;

                }
                else
                {
                    isDevice = false;
                }


                return isDevice;
            }
            catch (Exception ex)
            {
                return false;
            }
        }
        static bool Isfind = false;//判断是否找到指定的静态变量
        static bool IsfindMTK = false;//判断是否找到指定的静态变量

        /// <summary>
        /// 搜索设备
        /// </summary>
        /// <returns></returns>
        private bool FindTheHid(Int32 myVendorID, Int32 myProductID, Int32 VerNum, string GuidWrite, string GuidRead)
        {
            Isfind = false;
            Guid hidGuid = Guid.Empty;
            int memberIndex = 0;
            //int myProductID = 0;
            //int myVendorID = 0;
            bool success = false;
            bool BoolCreateFile = true;
            this.devicePathNameList.Clear();
            int bufferSize = 0;
            IntPtr detailDataBuffer = IntPtr.Zero;
            IntPtr deviceInfoSet = (IntPtr)0;
            DeviceManagement.SP_DEVICE_INTERFACE_DATA MyDeviceInterfaceData = default(DeviceManagement.SP_DEVICE_INTERFACE_DATA);
            try
            {
                this.myDeviceDetected = false;
                Hid.HidD_GetHidGuid(ref hidGuid);
                deviceInfoSet = DeviceManagement.SetupDiGetClassDevs(ref hidGuid, IntPtr.Zero, IntPtr.Zero, 18);
                memberIndex = 0;
                MyDeviceInterfaceData.cbSize = Marshal.SizeOf(MyDeviceInterfaceData);
                bool DIDResult;
                do
                {
                    DIDResult = DeviceManagement.SetupDiEnumDeviceInterfaces(deviceInfoSet, IntPtr.Zero, ref hidGuid, memberIndex, ref MyDeviceInterfaceData);
                    bool flag = !DIDResult;
                    if (flag)
                    {
                        DIDResult = false;
                        bool flag2 = !BoolCreateFile;
                        if (flag2)
                        {
                            try
                            {
                                foreach (string tempOutPath in this.devicePathNameList)
                                {
                                    bool flag3 = tempOutPath.IndexOf(Convert.ToString(myProductID, 16)) > -1;
                                    if (flag3)
                                    {
                                        bool flag4 = tempOutPath.IndexOf(GuidWrite) > -1 && tempOutPath.IndexOf(Convert.ToString(myProductID, 16)) > -1;
                                        if (flag4)
                                        {
                                            this.myDeviceDetected = true;
                                            this.myDevicePathName64 = tempOutPath;
                                        }
                                    }
                                }
                            }
                            catch (Exception ex)
                            {
                            }
                        }
                    }
                    else
                    {
                        DIDResult = DeviceManagement.SetupDiGetDeviceInterfaceDetail(deviceInfoSet, ref MyDeviceInterfaceData, IntPtr.Zero, 0, ref bufferSize, IntPtr.Zero);
                        detailDataBuffer = Marshal.AllocHGlobal(bufferSize);
                        Marshal.WriteInt32(detailDataBuffer, (IntPtr.Size == 4) ? (4 + Marshal.SystemDefaultCharSize) : 8);
                        DIDResult = DeviceManagement.SetupDiGetDeviceInterfaceDetail(deviceInfoSet, ref MyDeviceInterfaceData, detailDataBuffer, bufferSize, ref bufferSize, IntPtr.Zero);
                        IntPtr pDevicePathName = new IntPtr(detailDataBuffer.ToInt64() + 4L);
                        string devicePathName = Marshal.PtrToStringAuto(pDevicePathName);
                        this.devicePathNameList.Add(devicePathName);
                        bool flag5 = BoolCreateFile;
                        if (flag5)
                        {
                            hidHandleMouse = FileIO.CreateFile(devicePathName, 2147483648U, 3, IntPtr.Zero, 3, 1073741824, 0);
                            bool flag6 = !hidHandleMouse.IsInvalid;
                            if (flag6)
                            {
                                this.MyHid.DeviceAttributes.Size = Marshal.SizeOf(this.MyHid.DeviceAttributes);
                                success = Hid.HidD_GetAttributes(hidHandleMouse, ref this.MyHid.DeviceAttributes);
                                bool flag7 = success;
                                if (flag7)
                                {
                                    bool flag8 = myVendorID != 0;
                                    if (flag8)
                                    {
                                        //if ((int)this.MyHid.DeviceAttributes.VendorID == myVendorID && (int)this.MyHid.DeviceAttributes.ProductID == myProductID && (int)this.MyHid.DeviceAttributes.VersionNumber != VerNum)
                                        //{
                                        //    VPIDpassVerF = (int)this.MyHid.DeviceAttributes.VersionNumber;
                                        //}
                                        bool flag9 = devicePathName.IndexOf(GuidRead) > -1 
                                            && (int)this.MyHid.DeviceAttributes.VendorID == myVendorID 
                                            && (int)this.MyHid.DeviceAttributes.ProductID == myProductID;
                                        if (flag9)
                                        {
                                            //VPIDpassVerF = (int)this.MyHid.DeviceAttributes.VersionNumber;

                                            this.myDevicePathName = devicePathName;
                                            BoolCreateFile = false;
                                        }
                                        else
                                        {
                                            this.myDeviceDetected = false;
                                            hidHandleMouse.Close();
                                        }
                                    }
                                    else
                                    {
                                        bool flag10 = devicePathName.IndexOf(GuidRead) > -1 
                                            && (int)this.MyHid.DeviceAttributes.VendorID == myVendorID 
                                            && (int)this.MyHid.DeviceAttributes.ProductID == myVendorID;
                                        if (flag10)
                                        {
                                            this.myDevicePathName = devicePathName;
                                            BoolCreateFile = false;
                                        }
                                        else
                                        {
                                            this.myDeviceDetected = false;
                                            hidHandleMouse.Close();
                                        }
                                    }
                                }
                                else
                                {
                                    this.myDeviceDetected = false;
                                    hidHandleMouse.Close();
                                }
                            }
                        }
                        else
                        {
                            try
                            {
                                foreach (string tempOutPath2 in this.devicePathNameList)
                                {
                                    bool flag11 = tempOutPath2.IndexOf(Convert.ToString(myVendorID, 16)) > -1;
                                    if (flag11)
                                    {
                                        bool flag12 = tempOutPath2.IndexOf(GuidWrite) > -1 && tempOutPath2.IndexOf(Convert.ToString(myVendorID, 16)) > -1;
                                        if (flag12)
                                        {
                                            this.myDeviceDetected = true;
                                            this.myDevicePathName64 = tempOutPath2;
                                        }
                                    }
                                }
                            }
                            catch (Exception ex2)
                            {
                            }
                        }
                    }
                    memberIndex++;
                }
                while (!this.myDeviceDetected && DIDResult);
            }
            catch (Exception ex3)
            {
            }
            finally
            {
                bool flag13 = detailDataBuffer != IntPtr.Zero;
                if (flag13)
                {
                    Marshal.FreeHGlobal(detailDataBuffer);
                }
                bool flag14 = deviceInfoSet != IntPtr.Zero;
                if (flag14)
                {
                    DeviceManagement.SetupDiDestroyDeviceInfoList(deviceInfoSet);
                }
            }
            bool result;
            try
            {
                bool flag15 = this.myDeviceDetected;
                if (flag15)
                {
                    try
                    {
                        success = this.MyDeviceManagement.RegisterForDeviceNotifications(this.myDevicePathName, this.FrmMainHandle, hidGuid, ref this.deviceNotificationHandle);
                    }
                    catch (Exception ex4)
                    {
                    }
                    this.MyHid.Capabilities = this.MyHid.GetDeviceCapabilities(hidHandleMouse);
                    bool flag16 = success;
                    if (flag16)
                    {
                        this.hidUsage = this.MyHid.GetHidUsage(this.MyHid.Capabilities);
                        this.GetInputReportBufferSize();
                        endthreadMouse = false;
                        Isfind = true;
                        readHandle = FileIO.CreateFile(this.myDevicePathName, 2147483648U, 3, IntPtr.Zero, 3, 1073741824, 0);
                        writeHandle64 = FileIO.CreateFile(this.myDevicePathName64, 3221225472U, 3, IntPtr.Zero, 3, 0, 0);
                        bool isInvalid = readHandle.IsInvalid;
                        if (!isInvalid)
                        {
                            //FWver = MyHid.DeviceAttributes.VersionNumber;
                            VPIDpassVerF = (int)this.MyHid.DeviceAttributes.VersionNumber;
                            MyHid.FlushQueue(readHandle);
                            endthreadMouse = false;
                            Thread.Sleep(50);
                            ParameterizedThreadStart ParStart = new ParameterizedThreadStart(MyThreadMouseMethod);
                            myMouseThread = new Thread(ParStart);
                            object ThreadPar = null;
                            myMouseThread.Start(ThreadPar);
                            myMouseThread.IsBackground = true;
                        }
                    }
                }
                result = this.myDeviceDetected;
            }
            catch (Exception ex4)
            {
                throw;
            }
            return myDeviceDetected;
        }

        private bool FindTheHidMTK(Int32 myVendorID, Int32 myProductID, Int32 VerNum)
        {
            IsfindMTK = false;
            Guid hidGuid = Guid.Empty;
            int memberIndex = 0;
            //int myProductID = 0;
            //int myVendorID = 0;
            bool success = false;
            bool BoolCreateFile = true;
            this.devicePathNameList.Clear();
            int bufferSize = 0;
            IntPtr detailDataBuffer = IntPtr.Zero;
            IntPtr deviceInfoSet = (IntPtr)0;
            DeviceManagement.SP_DEVICE_INTERFACE_DATA MyDeviceInterfaceData = default(DeviceManagement.SP_DEVICE_INTERFACE_DATA);
            try
            {
                this.myDeviceDetectedMTK = false;
                Hid.HidD_GetHidGuid(ref hidGuid);
                deviceInfoSet = DeviceManagement.SetupDiGetClassDevs(ref hidGuid, IntPtr.Zero, IntPtr.Zero, 18);
                memberIndex = 0;
                MyDeviceInterfaceData.cbSize = Marshal.SizeOf(MyDeviceInterfaceData);
                bool DIDResult;
                do
                {
                    DIDResult = DeviceManagement.SetupDiEnumDeviceInterfaces(deviceInfoSet, IntPtr.Zero, ref hidGuid, memberIndex, ref MyDeviceInterfaceData);
                    bool flag = !DIDResult;
                    if (flag)
                    {
                        DIDResult = false;
                        bool flag2 = !BoolCreateFile;
                        if (flag2)
                        {
                            try
                            {
                                foreach (string tempOutPath in this.devicePathNameList)
                                {
                                    bool flag3 = tempOutPath.IndexOf(Convert.ToString(myProductID, 16)) > -1;
                                    if (flag3)
                                    {
                                        bool flag4 = tempOutPath.IndexOf("mi_03") > -1 && tempOutPath.IndexOf(Convert.ToString(myProductID, 16)) > -1;
                                        if (flag4)
                                        {
                                            this.myDeviceDetectedMTK = true;
                                            this.myDevicePathName64MTK = tempOutPath;
                                        }
                                    }
                                }
                            }
                            catch (Exception ex)
                            {
                            }
                        }
                    }
                    else
                    {
                        DIDResult = DeviceManagement.SetupDiGetDeviceInterfaceDetail(deviceInfoSet, ref MyDeviceInterfaceData, IntPtr.Zero, 0, ref bufferSize, IntPtr.Zero);
                        detailDataBuffer = Marshal.AllocHGlobal(bufferSize);
                        Marshal.WriteInt32(detailDataBuffer, (IntPtr.Size == 4) ? (4 + Marshal.SystemDefaultCharSize) : 8);
                        DIDResult = DeviceManagement.SetupDiGetDeviceInterfaceDetail(deviceInfoSet, ref MyDeviceInterfaceData, detailDataBuffer, bufferSize, ref bufferSize, IntPtr.Zero);
                        IntPtr pDevicePathName = new IntPtr(detailDataBuffer.ToInt64() + 4L);
                        string devicePathName = Marshal.PtrToStringAuto(pDevicePathName);
                        this.devicePathNameList.Add(devicePathName);
                        bool flag5 = BoolCreateFile;
                        if (flag5)
                        {
                            hidHandleMouseMTK = FileIO.CreateFile(devicePathName, 2147483648U, 3, IntPtr.Zero, 3, 1073741824, 0);
                            bool flag6 = !hidHandleMouseMTK.IsInvalid;
                            if (flag6)
                            {
                                this.MyHid.DeviceAttributes.Size = Marshal.SizeOf(this.MyHid.DeviceAttributes);
                                success = Hid.HidD_GetAttributes(hidHandleMouseMTK, ref this.MyHid.DeviceAttributes);
                                bool flag7 = success;
                                if (flag7)
                                {
                                    bool flag8 = myVendorID != 0;
                                    if (flag8)
                                    {
                                        //if ((int)this.MyHid.DeviceAttributes.VendorID == myVendorID && (int)this.MyHid.DeviceAttributes.ProductID == myProductID && (int)this.MyHid.DeviceAttributes.VersionNumber != VerNum)
                                        //{
                                        //    VPIDpassVerF = (int)this.MyHid.DeviceAttributes.VersionNumber;
                                        //}
                                        bool flag9 = devicePathName.IndexOf("mi_03") > -1 && (int)this.MyHid.DeviceAttributes.VendorID == myVendorID && (int)this.MyHid.DeviceAttributes.ProductID == myProductID;
                                        if (flag9)
                                        {
                                            //VPIDpassVerF = (int)this.MyHid.DeviceAttributes.VersionNumber;

                                            this.myDevicePathNameMTK = devicePathName;
                                            BoolCreateFile = false;
                                        }
                                        else
                                        {
                                            this.myDeviceDetectedMTK = false;
                                            hidHandleMouseMTK.Close();
                                        }
                                    }
                                    else
                                    {
                                        bool flag10 = (int)this.MyHid.DeviceAttributes.VendorID == myVendorID && (int)this.MyHid.DeviceAttributes.ProductID == myVendorID;
                                        if (flag10)
                                        {
                                            this.myDevicePathNameMTK = devicePathName;
                                            BoolCreateFile = false;
                                        }
                                        else
                                        {
                                            this.myDeviceDetectedMTK = false;
                                            hidHandleMouseMTK.Close();
                                        }
                                    }
                                }
                                else
                                {
                                    this.myDeviceDetectedMTK = false;
                                    hidHandleMouseMTK.Close();
                                }
                            }
                        }
                        else
                        {
                            try
                            {
                                foreach (string tempOutPath2 in this.devicePathNameList)
                                {
                                    bool flag11 = tempOutPath2.IndexOf(Convert.ToString(myVendorID, 16)) > -1;
                                    if (flag11)
                                    {
                                        bool flag12 = tempOutPath2.IndexOf("mi_03") > -1 && tempOutPath2.IndexOf(Convert.ToString(myVendorID, 16)) > -1;
                                        if (flag12)
                                        {
                                            this.myDeviceDetectedMTK = true;
                                            this.myDevicePathName64MTK = tempOutPath2;
                                        }
                                    }
                                }
                            }
                            catch (Exception ex2)
                            {
                            }
                        }
                    }
                    memberIndex++;
                }
                while (!this.myDeviceDetectedMTK && DIDResult);
            }
            catch (Exception ex3)
            {
            }
            finally
            {
                bool flag13 = detailDataBuffer != IntPtr.Zero;
                if (flag13)
                {
                    Marshal.FreeHGlobal(detailDataBuffer);
                }
                bool flag14 = deviceInfoSet != IntPtr.Zero;
                if (flag14)
                {
                    DeviceManagement.SetupDiDestroyDeviceInfoList(deviceInfoSet);
                }
            }
            bool result;
            try
            {
                bool flag15 = this.myDeviceDetectedMTK;
                if (flag15)
                {
                    try
                    {
                        success = this.MyDeviceManagementMTK.RegisterForDeviceNotifications(this.myDevicePathNameMTK, this.FrmMainHandleMTK, hidGuid, ref this.deviceNotificationHandleMTK);
                    }
                    catch (Exception ex4)
                    {
                    }
                    this.MyHid.CapabilitiesMTK = this.MyHid.GetDeviceCapabilities(hidHandleMouseMTK);
                    bool flag16 = success;
                    if (flag16)
                    {
                        this.hidUsageMTK = this.MyHid.GetHidUsage(this.MyHid.CapabilitiesMTK);
                        this.GetInputReportBufferSizeMTK();
                        endthreadMouseMTK = false;
                        IsfindMTK = true;
                        readHandleMTK = FileIO.CreateFile(this.myDevicePathNameMTK, 2147483648U, 3, IntPtr.Zero, 3, 1073741824, 0);
                        writeHandle64MTK = FileIO.CreateFile(this.myDevicePathName64MTK, 3221225472U, 3, IntPtr.Zero, 3, 0, 0);
                        bool isInvalid = readHandleMTK.IsInvalid;
                        if (!isInvalid)
                        {
                            //FWver = MyHid.DeviceAttributes.VersionNumber;
                            VPIDpassVerFMTK = (int)this.MyHid.DeviceAttributes.VersionNumber;
                            MyHid.FlushQueue(readHandleMTK);
                            endthreadMouseMTK = false;
                            Thread.Sleep(50);
                            ParameterizedThreadStart ParStart = new ParameterizedThreadStart(MyThreadMouseMethodMTK);
                            myMouseThread = new Thread(ParStart);
                            object ThreadPar = null;
                            myMouseThread.Start(ThreadPar);
                            myMouseThread.IsBackground = true;
                        }
                    }
                }
                result = this.myDeviceDetectedMTK;
            }
            catch (Exception ex4)
            {
                throw;
            }
            return myDeviceDetectedMTK;
        }


        private void GetInputReportBufferSize()
        {
            int numberOfInputBuffers = 0;
            try
            {
                bool success = this.MyHid.GetNumberOfInputBuffers(hidHandleMouse, ref numberOfInputBuffers);
            }
            catch (Exception ex_1B)
            {
                throw;
            }
        }

        private void GetInputReportBufferSizeMTK()
        {
            int numberOfInputBuffers = 0;
            try
            {
                bool success = this.MyHid.GetNumberOfInputBuffers(hidHandleMouseMTK, ref numberOfInputBuffers);
            }
            catch (Exception ex_1B)
            {
                throw;
            }
        }


        /// <summary>
        /// 比较VIDPID是否存在
        /// </summary>
        /// <param name="myVendorID"></param>
        /// <param name="myProductID"></param>
        /// <returns></returns>
        //private bool GetVendorAndProductIDCompare(Int32 myVendorID, Int32 myProductID)
        //{
        //    intvp tem;
        //    try
        //    {
        //        foreach (object i in IntVidPid)
        //        {
        //            tem = (intvp)i;
        //            if (tem.vid == myVendorID && tem.pid == myProductID)
        //            {
        //                myVendorID = tem.vid;
        //                myProductID = tem.pid;
        //                return true;
        //            }
        //        }
        //        return false;
        //    }
        //    catch (System.Exception)
        //    {
        //        return false;
        //    }


        //}


        private void GetMouseInputReportBufferSize()
        {
            Int32 numberOfInputBuffers = 0;
            Boolean success;

            try
            {
                success = MyHid.GetNumberOfInputBuffers(hidHandleMouse, ref numberOfInputBuffers);
            }
            catch (Exception)
            {
            }
        }
        public Boolean End = false;

        private void MyThreadMouseMethodMTK(object FormHandle)//更新界面
        {

            IntPtr eventObject = IntPtr.Zero;
            NativeOverlapped HidOverlapped = new NativeOverlapped();
            IntPtr nonManagedBuffer = IntPtr.Zero;
            IntPtr nonManagedOverlapped = IntPtr.Zero;
            Int32 numberOfBytesRead = 0;
            Int32 result = 0;
            Boolean success;
            Byte[] inputReportBuffer = null;
            Byte[] outputBuffer = null;
            Int32 StrLen = 0;
            DateTime keyPre = DateTime.Now;

            try
            {
                inputReportBuffer = new Byte[MyHid.CapabilitiesMTK.InputReportByteLength];
                outputBuffer = new Byte[MyHid.CapabilitiesMTK.InputReportByteLength];
                Hid.HidD_GetNumInputBuffers(readHandleMTK, ref StrLen);
                eventObject = FileIO.CreateEvent(IntPtr.Zero, false, false, "");
                HidOverlapped.OffsetLow = 0;
                HidOverlapped.OffsetHigh = 0;
                HidOverlapped.EventHandle = eventObject;
                nonManagedBuffer = Marshal.AllocHGlobal(inputReportBuffer.Length);//分配内存
                nonManagedOverlapped = Marshal.AllocHGlobal(Marshal.SizeOf(HidOverlapped));//分配对象内存
                Marshal.StructureToPtr(HidOverlapped, nonManagedOverlapped, false);
                while (!endthreadMouse)
                {
                    if (readHandleMTK != null || (!readHandleMTK.IsInvalid))
                    {
                        //读文件
                        success = FileIO.ReadFile(readHandleMTK, nonManagedBuffer, inputReportBuffer.Length, ref numberOfBytesRead, nonManagedOverlapped);

                        FileIO.GetOverlappedResult(readHandleMTK, nonManagedOverlapped, ref numberOfBytesRead, false);
                        Array.Clear(inputReportBuffer, 0, inputReportBuffer.Length);
                        Marshal.Copy(nonManagedBuffer, inputReportBuffer, 0, numberOfBytesRead);//将内容拷贝到inputReportBuffer字节数组
                        if (inputReportBuffer[0] == 2 && inputReportBuffer[2] == 16)
                        {
                        }

                        int dwErr = 0;
                        if (!success)
                        {
                            dwErr = Hid.GetLastError();
                            dwErr = Marshal.GetLastWin32Error();
                        }
                        result = FileIO.WaitForSingleObject(eventObject, 200);

                        switch (result)
                        {
                            case (System.Int32)FileIO.WAIT_OBJECT_0:
                                FileIO.GetOverlappedResult(readHandleMTK, nonManagedOverlapped, ref numberOfBytesRead, false);
                                Array.Clear(inputReportBuffer, 0, inputReportBuffer.Length);
                                Marshal.Copy(nonManagedBuffer, inputReportBuffer, 0, numberOfBytesRead);//将内容拷贝到inputReportBuffer字节数组


                                if (GetMouseInputData_eventMTK != null)
                                {

                                    GetMouseInputData_eventMTK(inputReportBuffer);
                                }

                                break;

                            case FileIO.WAIT_TIMEOUT:
                                if (!readHandleMTK.IsClosed)
                                {
                                    FileIO.CancelIo(readHandleMTK);
                                }
                                break;
                            default:
                                if (!readHandleMTK.IsClosed)
                                {
                                    FileIO.CancelIo(readHandleMTK);
                                }
                                break;
                        }
                    }

                }
            }
            catch (Exception ex)
            {

            }

        }

        private void MyThreadMouseMethod(object FormHandle)//更新界面
        {

            IntPtr eventObject = IntPtr.Zero;
            NativeOverlapped HidOverlapped = new NativeOverlapped();
            IntPtr nonManagedBuffer = IntPtr.Zero;
            IntPtr nonManagedOverlapped = IntPtr.Zero;
            Int32 numberOfBytesRead = 0;
            Int32 result = 0;
            Boolean success;
            Byte[] inputReportBuffer = null;
            Byte[] outputBuffer = null;
            Int32 StrLen = 0;
            DateTime keyPre = DateTime.Now;

            try
            {
                inputReportBuffer = new Byte[MyHid.Capabilities.InputReportByteLength];
                outputBuffer = new Byte[MyHid.Capabilities.InputReportByteLength];
                Hid.HidD_GetNumInputBuffers(readHandle, ref StrLen);
                eventObject = FileIO.CreateEvent(IntPtr.Zero, false, false, "");
                HidOverlapped.OffsetLow = 0;
                HidOverlapped.OffsetHigh = 0;
                HidOverlapped.EventHandle = eventObject;
                nonManagedBuffer = Marshal.AllocHGlobal(inputReportBuffer.Length);//分配内存
                nonManagedOverlapped = Marshal.AllocHGlobal(Marshal.SizeOf(HidOverlapped));//分配对象内存
                Marshal.StructureToPtr(HidOverlapped, nonManagedOverlapped, false);
                while (!endthreadMouse)
                {
                    if (readHandle != null || (!readHandle.IsInvalid))
                    {
                        //读文件
                        success = FileIO.ReadFile(readHandle, nonManagedBuffer, inputReportBuffer.Length, ref numberOfBytesRead, nonManagedOverlapped);

                        FileIO.GetOverlappedResult(readHandle, nonManagedOverlapped, ref numberOfBytesRead, false);
                        Array.Clear(inputReportBuffer, 0, inputReportBuffer.Length);
                        Marshal.Copy(nonManagedBuffer, inputReportBuffer, 0, numberOfBytesRead);//将内容拷贝到inputReportBuffer字节数组
                        if (inputReportBuffer[0] == 2 && inputReportBuffer[2] == 16)
                        {
                        }

                        int dwErr = 0;
                        if (!success)
                        {
                            dwErr = Hid.GetLastError();
                            dwErr = Marshal.GetLastWin32Error();
                        }
                        result = FileIO.WaitForSingleObject(eventObject, 200);

                        switch (result)
                        {
                            case (System.Int32)FileIO.WAIT_OBJECT_0:
                                FileIO.GetOverlappedResult(readHandle, nonManagedOverlapped, ref numberOfBytesRead, false);
                                Array.Clear(inputReportBuffer, 0, inputReportBuffer.Length);
                                Marshal.Copy(nonManagedBuffer, inputReportBuffer, 0, numberOfBytesRead);//将内容拷贝到inputReportBuffer字节数组


                                if (GetMouseInputData_event != null)
                                {

                                    GetMouseInputData_event(inputReportBuffer);
                                }

                                break;

                            case FileIO.WAIT_TIMEOUT:
                                if (!readHandle.IsClosed)
                                {
                                    FileIO.CancelIo(readHandle);
                                }
                                break;
                            default:
                                if (!readHandle.IsClosed)
                                {
                                    FileIO.CancelIo(readHandle);
                                }
                                break;
                        }
                    }

                }
            }
            catch (Exception ex)
            {

            }

        }

        /// <summary>
        /// 获取按键数据
        /// </summary>
        /// <param name="inputReportBuffer"></param>
        /// <param name="i"></param>
        /// <param name="key"></param>
        /// <param name="value"></param>
        /// <returns></returns>
        //public KeySetting GetKey(byte[] inputReportBuffer, int i, int key, byte value)
        //{
        //    //if (i == 5) inputReportBuffer[i] = 0xB0;
        //    KeySetting myKeySetting = new KeySetting();
        //    myKeySetting.DefaultID = value;
        //    myKeySetting.Byte1 = inputReportBuffer[i + 1];
        //    myKeySetting.Byte2 = inputReportBuffer[i + 2];
        //    myKeySetting.Byte3 = inputReportBuffer[i + 3];
        //    myKeySetting.KeyID = CoventKeyID(inputReportBuffer[i], value);
        //    myKeySetting.KeyIndex = (byte)key;
        //    return myKeySetting;
        //}

        /// <summary>
        /// 转换ID
        /// </summary>
        /// <param name="id"></param>
        /// <param name="value"></param>
        /// <returns></returns>
        public byte CoventKeyID(int id, int value)
        {
            byte DATA = 0;
            switch (id)
            {
                case 0xff:
                case 0xfe:
                case 0xfd:
                    DATA = (byte)id;
                    break;
                case 0x00:
                    DATA = (byte)value;
                    break;
                default:
                    DATA = (byte)id;
                    break;
            }
            return DATA;
        }
        [DllImport("user32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern bool IsWindow(IntPtr hWnd);

        public bool isRegis = false;
        public bool isRegisCB = false;

        /// <summary>
        /// 插拔更新设备
        /// </summary>
        /// <param name="m"></param>
        public void OnDeviceChange(int wParam)
        {
            try
            {
                switch (wParam)
                {
                    case DeviceManagement.DBT_DEVNODES_CHANGED:
                        //只注册一次
                        if (!this.myDeviceDetected)
                        {
                            Debug.WriteLine("111");
                            bool isLian = this.ExitfindMyDeviceMouse();
                            if (isLian)
                            {
                                if (!isRegis)
                                {
                                    this.RegisteDeviceNotification();
                                    isRegis = true;
                                }
                                MainWindow.HaveRueslt = true;//做个标记位，处理了不再进来
                                FileIO.PostMessage(FrmMainHandle, FileIO.WM_MYCOSTOM, 1, 1);
                            }
                            //else
                            //{
                            //    FileIO.PostMessage(frmMainHandle, FileIO.WM_MYCOSTOM, 2, 2);
                            //}
                        }

                        break;
                    //case DeviceManagement.DBT_DEVICEARRIVAL://插入

                    //    if (!this.myDeviceDetected)
                    //    {
                    //        Debug.WriteLine("111");
                    //        bool isLian = this.ExitfindMyDeviceMouse();
                    //        if (isLian)
                    //        {
                    //            MainWindow.HaveRueslt = true;//做个标记位，处理了不再进来
                    //            FileIO.PostMessage(frmMainHandle, FileIO.WM_MYCOSTOM, 1, 1);

                    //        }
                    //    }
                    //    break;
                    case DeviceManagement.DBT_DEVICEREMOVECOMPLETE://拔出
                        if (this.myDeviceDetected)
                        {
                            Debug.WriteLine("222");
                            bool isLian = this.ExitfindMyDeviceMouse();
                            if (!isLian)
                            {
                                MainWindow.HaveRueslt = true;//做个标记位，处理了不再进来
                                StopReceivingDeviceNotifications(deviceNotificationHandle);
                                FileIO.PostMessage(FrmMainHandle, FileIO.WM_MYCOSTOM, 2, 2);
                            }
                        }
                        break;
                }
            }
            catch (Exception ex)
            {

            }
        }
        public void OnDeviceChangeCB(int wParam)
        {
            try
            {
                switch (wParam)
                {
                    case DeviceManagement.DBT_DEVNODES_CHANGED:
                        //只注册一次
                        if (!this.myDeviceDetectedMTK)
                        {
                            bool isLian = this.ExitfindMyDeviceMouseMTK();
                            if (isLian)
                            {
                                if (!isRegisCB)
                                {
                                    this.RegisteDeviceNotificationMTK();
                                    isRegisCB = true;
                                }

                                //MainWindow.HaveRuesltCB = true;//做个标记位，处理了不再进来
                                FileIO.PostMessage(FrmMainHandle, FileIO.WM_MYCOSTOM, 3, 3);

                            }
                        }
                        break;
                    //case DeviceManagement.DBT_DEVICEARRIVAL://插入
                    //    if (!this.myDeviceDetectedMTK)
                    //    {
                    //        bool isLian = this.ExitfindMyDeviceMouseMTK();
                    //        if (isLian)
                    //        {
                    //            MainWindow.HaveRuesltCB = true;//做个标记位，处理了不再进来
                    //            FileIO.PostMessage(frmMainHandle, FileIO.WM_MYCOSTOM, 3, 3);

                    //        }
                    //    }
                    //    break;
                    case DeviceManagement.DBT_DEVICEREMOVECOMPLETE://拔出

                        if (this.myDeviceDetectedMTK)
                        {
                            bool isLian = this.ExitfindMyDeviceMouseMTK();
                            if (!isLian)
                            {
                                //MainWindow.HaveRuesltCB = true;//做个标记位，处理了不再进来
                                StopReceivingDeviceNotifications(deviceNotificationHandleMTK);
                                FileIO.PostMessage(FrmMainHandle, FileIO.WM_MYCOSTOM, 4, 4);

                            }
                        }
                        break;

                }


            }
            catch (Exception ex)
            {

            }
        }

        public static IntPtr Handle;

        public void StopReceivingDeviceNotifications(IntPtr deviceNotificationHandle)
        {
            if (deviceNotificationHandle != IntPtr.Zero)
            {
                // 注销设备通知
                bool isLian = UnregisterDeviceNotification(deviceNotificationHandle);
                deviceNotificationHandle = IntPtr.Zero;
                //isRegistered = false; // 重置标记
            }
        }

        /// <summary>
        /// 注册接收的详细的消息
        /// </summary>
        public void RegisteDeviceNotification()
        {
            Guid hidGuid = Guid.Empty;
            Hid.HidD_GetHidGuid(ref hidGuid);//获取GUID
            // bool success = MyDeviceManagement.RegisterForDeviceNotifications(myDevicePathName6464, frmMainHandle, hidGuid, ref deviceNotificationHandle);
            bool successMouse = MyDeviceManagement.RegisterForDeviceNotifications(myDevicePathName, FrmMainHandle, hidGuid, ref deviceNotificationHandle);
            if (successMouse)
            { Handle = deviceNotificationHandle; }
        }

        public void RegisteDeviceNotificationMTK()
        {
            Guid hidGuid = Guid.Empty;
            Hid.HidD_GetHidGuid(ref hidGuid);//获取GUID
            // bool success = MyDeviceManagement.RegisterForDeviceNotifications(myDevicePathName6464, frmMainHandle, hidGuid, ref deviceNotificationHandle);
            bool successMouse = MyDeviceManagementMTK.RegisterForDeviceNotifications(myDevicePathNameMTK, FrmMainHandle, hidGuid, ref deviceNotificationHandleMTK);
            if (successMouse)
            { Handle = deviceNotificationHandle; }
        }


        /// <summary>
        /// 发送数据到设备
        /// </summary>
        /// <param name="outputBuffer"></param>
        /// <returns></returns>
        public Boolean SendToDevice64(Byte[] outputBuffer, int tag)
        {
            Thread.Sleep(30);
            Boolean success = false;
            try
            {
                switch (tag)
                {
                    case 0:
                        if ((writeHandle64 != null) && !writeHandle64.IsInvalid)
                        {

                            int len = 0;
                            success = FileIO.WriteFile(writeHandle64, outputBuffer, outputBuffer.Length, ref len, IntPtr.Zero);

                        }
                        break;
                    case 1:
                        if ((writeHandleMouse != null))
                        {
                            int len = 0;
                            success = FileIO.WriteFile(writeHandleMouse, outputBuffer, outputBuffer.Length, ref len, IntPtr.Zero);
                        }
                        break;
                    case 2:
                        if ((writeHandle64 != null) && !writeHandle64.IsInvalid)
                        {

                            success = Hid.HidD_SetFeature(writeHandle64, outputBuffer, outputBuffer.Length);
                        }
                        break;

                }
                return success;
            }
            catch (Exception ex)
            {
                // MessageBox.Show("发送数据到设备异常：" + ex.ToString());
                return success;
            }
        }
        /// <summary>
        /// 注销消息通知
        /// </summary>
        /// <param name="m"></param>
        /// <param name="mydevicePathName"></param>
        /// <returns></returns>
        public Boolean DeviceNameMatch(Message m, String mydevicePathName)
        {
            Int32 stringSize;

            try
            {
                DEV_BROADCAST_DEVICEINTERFACE_1 devBroadcastDeviceInterface = new DEV_BROADCAST_DEVICEINTERFACE_1();
                DEV_BROADCAST_HDR devBroadcastHeader = new DEV_BROADCAST_HDR();
                Marshal.PtrToStructure(m.LParam, devBroadcastHeader);
                if ((devBroadcastHeader.dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE))
                {
                    stringSize = System.Convert.ToInt32((devBroadcastHeader.dbch_size - 32) / 2);
                    devBroadcastDeviceInterface.dbcc_name = new Char[stringSize + 1];
                    Marshal.PtrToStructure(m.LParam, devBroadcastDeviceInterface);
                    String DeviceNameString = new String(devBroadcastDeviceInterface.dbcc_name, 0, stringSize);
                    if ((String.Compare(DeviceNameString, mydevicePathName, true) == 0))
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
            }
            catch (Exception ex)
            {
                throw;
            }

            return false;
        }

        public Boolean GetDataInReport(ref Byte[] outputBuffer)
        {
            Boolean success = false;
            int len = 0;
            try
            {
                if ((writeHandle64 != null) && (!writeHandle64.IsInvalid))
                {
                    success = Hid.HidD_GetFeature(writeHandle64, outputBuffer, outputBuffer.Length);
                    //Thread.Sleep(50);
                    //success = Hid.HidD_SetFeature(writeHandleMouse, outputBuffer, outputBuffer.Length);

                    //success = FileIO.WriteFile(writeHandleMouse, outputBuffer, outputBuffer.Length, ref len, IntPtr.Zero);
                }
                else
                {

                }
                return success;
            }
            catch (Exception ex)
            {
                return success;
            }
        }

        public int ChangeTheMouseVersion(int HidVison)
        {
            int vison = 0;
            string text = HidVison.ToString("x");
            // text = text.Substring(0, text.Length - 1);

            vison = Convert.ToInt32(text);
            return vison;
        }

        #endregion
    }
}
