
using MouseKeyModel;
using MouseTest.Helper;
using MouseTest.HID;
using System;
using System.CodeDom;
using System.Collections.Generic;
using System.Data.SqlClient;
using System.Diagnostics;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Timers;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Interop;
using System.Windows.Markup;
using System.Windows.Media;
using System.Windows.Threading; // added for DispatcherTimer
using System.Xml;
using WpfPanel;
using static MouseTest.MainWindow;
using static MouseTest.MouseHook;
using static MouseTest.RepoetRateClass;
using static System.Net.Mime.MediaTypeNames;
using Application = System.Windows.Forms.Application;
using Button = System.Windows.Controls.Button;
using MessageBox = System.Windows.MessageBox;

namespace MouseTest
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : System.Windows.Window
    {
        public static HIDCommonClass MyHID;
        public static MainWindow mainWindow;
        private readonly CommandClass commandClass = null;
        // 在类中添加
        private Stopwatch _rateStopwatch = new Stopwatch();

        // 按键速度统计
        private DispatcherTimer _keySpeedCountTimer;
        private DispatcherTimer _keySpeedHoldTimer;
        private int _keyPressCount;
        private enum KeySpeedState { Idle, Counting, Holding }
        private KeySpeedState _keySpeedState = KeySpeedState.Idle;
        private readonly string iniFile = System.IO.Path.GetDirectoryName(Application.ExecutablePath) + "\\DeviceInformation.ini";
        private CIniFileHandler iniFileHandler;

        private string dongleMid = "";
        private string mouseMid = "";

        // 鼠标报告计数器
        //private Stopwatch stopwatch;
        private int moveCount;
        private readonly DispatcherTimer timer;

        public string vid;
        public string pid;
        public string ver;
        public string setPort;
        public string getPort;


        public MainWindow()
        {
            commandClass = new CommandClass();
            mainWindow = this;
            MyHID = new HIDCommonClass();
            InitializeComponent();
            GetInitTetting();
            //SetInitTetting();
            //MyWarPanel.Children.Clear();
            _windowHandle = new WindowInteropHelper(this).Handle;

            #region 报告率初始化
            // 初始化计时器和计数器
            //stopwatch = new Stopwatch();
            moveCount = 0;

            // 使用 DispatcherTimer 替代 WinForms Timer（适合 WPF）
            timer = new DispatcherTimer
            {
                Interval = TimeSpan.FromMilliseconds(350) //350ms
            };
            timer.Tick += Timer_Tick;
            timer.Start();

            // 注册 Raw Input设备
            RegisterRawInputDevices(new RAWINPUTDEVICE[]
            {
                new RAWINPUTDEVICE()
                {
                    usUsagePage =0x01, // HID_USAGE_PAGE_GENERIC
                    usUsage =0x02, // HID_USAGE_GENERIC_MOUSE
                    dwFlags =0, // 默认
                    hwndTarget = this._windowHandle
                }
            }, 1, (uint)Marshal.SizeOf(typeof(RAWINPUTDEVICE)));

            //// 启动计时器
            //stopwatch.Start();
            #endregion

            #region 按键速度统计初始化
            _keySpeedCountTimer = new DispatcherTimer
            {
                Interval = TimeSpan.FromSeconds(1)
            };
            _keySpeedCountTimer.Tick += KeySpeedCountTimer_Tick;

            _keySpeedHoldTimer = new DispatcherTimer
            {
                Interval = TimeSpan.FromSeconds(10)
            };
            _keySpeedHoldTimer.Tick += KeySpeedHoldTimer_Tick;
            #endregion

            timerClean.Elapsed += new System.Timers.ElapsedEventHandler(timerClean_tick);
            timerClean.Interval = 3000;
            MyHID.GetMouseInputData_event += CheckData;//接收数据委托

            // 在构造函数中初始化
            _rateStopwatch.Start();
        }


        /// <summary>
        /// 句柄要在load的时候new
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            MyHID.FrmMainHandle = new WindowInteropHelper(this).Handle;
            MyHID.myDeviceDetected = false;
            MyHID.OnDeviceChange(DeviceManagement.DBT_DEVNODES_CHANGED);

            rbVolYes.Checked += RbVolYes_Checked;
            rbVolNo.Checked += RbVolYes_Checked;
            rbKeyL.Checked += RbKey_Checked;
            rbKeyR.Checked += RbKey_Checked;

            chkDynOnTimeEn.Checked += ChkCoilDebug_Checked;
            chkDynOnTimeEn.Unchecked += ChkCoilDebug_Checked;
            chkAutoVibrationEn.Checked += ChkCoilDebug_Checked;
            chkAutoVibrationEn.Unchecked += ChkCoilDebug_Checked;

            // 在按键速度统计区域监听鼠标按下事件
            testKeySpeed.MouseDown += TestKeySpeed_MouseDown;
        }

        private void RbVolYes_Checked(object sender, RoutedEventArgs e)
        {
            if (_isUpdatingUi) return;
            SaveVoltageSettings();
        }

        private void RbKey_Checked(object sender, RoutedEventArgs e)
        {
            if (_isUpdatingUi) return;
            // 切换左/右键时，把当前选中键的参数绑定到设置 UI
            UpdateSettingsUi();
        }

        private void ChkCoilDebug_Checked(object sender, RoutedEventArgs e)
        {
            if (_isUpdatingUi) return;
            SaveCoilDebugSettings();
        }

        private void SliderAutoVibrationPeriod_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (txtAutoVibrationPeriodDisplay != null)
            {
                txtAutoVibrationPeriodDisplay.Text = $"{(int)sliderAutoVibrationPeriod.Value} ms";
            }
        }

        private void SliderAutoVibrationPeriod_PreviewMouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            SaveCoilDebugSettings();
        }

        private void btnDecreaseAutoVibrationPeriod_Click(object sender, RoutedEventArgs e)
        {
            double newValue = sliderAutoVibrationPeriod.Value - 25;
            if (newValue < sliderAutoVibrationPeriod.Minimum)
                newValue = sliderAutoVibrationPeriod.Minimum;
            sliderAutoVibrationPeriod.Value = newValue;
            SaveCoilDebugSettings();
        }

        private void btnIncreaseAutoVibrationPeriod_Click(object sender, RoutedEventArgs e)
        {
            double newValue = sliderAutoVibrationPeriod.Value + 25;
            if (newValue > sliderAutoVibrationPeriod.Maximum)
                newValue = sliderAutoVibrationPeriod.Maximum;
            sliderAutoVibrationPeriod.Value = newValue;
            SaveCoilDebugSettings();
        }

        private void SliderConductTime_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (txtConductTimeDisplay != null)
            {
                txtConductTimeDisplay.Text = $"{(int)sliderConductTime.Value} ";
            }
            if (txtManualConductTime != null)
            {
                txtManualConductTime.Text = ((int)sliderConductTime.Value).ToString();
            }
        }

        private void SliderConductTime_PreviewMouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            SaveVoltageSettings();
        }

        private void btnDecreaseConductTime_Click(object sender, RoutedEventArgs e)
        {
            double newValue = sliderConductTime.Value - 100;
            if (newValue < sliderConductTime.Minimum)
                newValue = sliderConductTime.Minimum;
            sliderConductTime.Value = newValue;
            SaveVoltageSettings();
        }

        private void btnIncreaseConductTime_Click(object sender, RoutedEventArgs e)
        {
            double newValue = sliderConductTime.Value + 100;
            if (newValue > sliderConductTime.Maximum)
                newValue = sliderConductTime.Maximum;
            sliderConductTime.Value = newValue;
            SaveVoltageSettings();
        }

        private void btnSetManualConductTime_Click(object sender, RoutedEventArgs e)
        {
            if (int.TryParse(txtManualConductTime.Text, out int value))
            {
                if (value >= 0 && value <= 1200)
                {
                    sliderConductTime.Value = value;
                    SaveVoltageSettings();
                }
                else
                {
                    txtVoltageStatus.Text = "请输入0~1200";
                    txtVoltageStatus.Foreground = brushRed;
                }
            }
            else
            {
                txtVoltageStatus.Text = "请输入有效数字";
                txtVoltageStatus.Foreground = brushRed;
            }
        }

        private void txtManualConductTime_PreviewTextInput(object sender, TextCompositionEventArgs e)
        {
            e.Handled = !int.TryParse(e.Text, out _);
        }

        private void SliderReleaseConductTime_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (txtReleaseConductTimeDisplay != null)
            {
                txtReleaseConductTimeDisplay.Text = $"{(int)sliderReleaseConductTime.Value} ";
            }
            if (txtManualReleaseConductTime != null)
            {
                txtManualReleaseConductTime.Text = ((int)sliderReleaseConductTime.Value).ToString();
            }
        }

        private void SliderReleaseConductTime_PreviewMouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            SaveReleaseVoltageSettings();
        }

        private void btnDecreaseReleaseConductTime_Click(object sender, RoutedEventArgs e)
        {
            double newValue = sliderReleaseConductTime.Value - 100;
            if (newValue < sliderReleaseConductTime.Minimum)
                newValue = sliderReleaseConductTime.Minimum;
            sliderReleaseConductTime.Value = newValue;
            SaveReleaseVoltageSettings();
        }

        private void btnIncreaseReleaseConductTime_Click(object sender, RoutedEventArgs e)
        {
            double newValue = sliderReleaseConductTime.Value + 100;
            if (newValue > sliderReleaseConductTime.Maximum)
                newValue = sliderReleaseConductTime.Maximum;
            sliderReleaseConductTime.Value = newValue;
            SaveReleaseVoltageSettings();
        }

        private void btnSetManualReleaseConductTime_Click(object sender, RoutedEventArgs e)
        {
            if (int.TryParse(txtManualReleaseConductTime.Text, out int value))
            {
                if (value >= 0 && value <= 1200)
                {
                    sliderReleaseConductTime.Value = value;
                    SaveReleaseVoltageSettings();
                }
                else
                {
                    txtReleaseVoltageStatus.Text = "请输入0~1200";
                    txtReleaseVoltageStatus.Foreground = brushRed;
                }
            }
            else
            {
                txtReleaseVoltageStatus.Text = "请输入有效数字";
                txtReleaseVoltageStatus.Foreground = brushRed;
            }
        }

        private void txtManualReleaseConductTime_PreviewTextInput(object sender, TextCompositionEventArgs e)
        {
            e.Handled = !int.TryParse(e.Text, out _);
        }

        private void GetInitTetting()
        {
            this.iniFileHandler = new CIniFileHandler(this.iniFile);
            this.vid = this.iniFileHandler.getValue("Device", "vid");
            this.pid = this.iniFileHandler.getValue("Device", "pid");
            this.ver = this.iniFileHandler.getValue("Device", "ver");
            this.setPort = this.iniFileHandler.getValue("Device", "setPort");
            this.getPort = this.iniFileHandler.getValue("Device", "getPort");

            txtVid.Text = this.vid;
            txtPid.Text = this.pid;
            txtVer.Text = this.ver;

        }

        private void timerClean_tick(object sender, ElapsedEventArgs e)
        {
            this.Dispatcher.Invoke(new Action(delegate
            {
                timerClean.Stop();
                CleanBtn_Click(null, null);


                suspend = false;//睡眠标记

            }));
        }






        //开启关闭报告率
        public bool RrIsBegin = false;
        public bool suspend = false; //接收器进入睡眠标记
        // 每秒输出报告率
        private void Timer_Tick(object sender, EventArgs e)
        {
            try
            {
                this.Dispatcher.Invoke(new Action(() =>
                {
                    if (RrIsBegin)
                    {
                        // 计算实际经过的时间（秒）
                        double elapsedSeconds = _rateStopwatch.Elapsed.TotalSeconds;
                        if (elapsedSeconds > 0)
                        {
                            // 计算报告率：总移动次数 / 经过的时间
                            double rate = moveCount / elapsedSeconds;
                            //Rr.Values = Math.Round(rate).ToString();
                        }
                        // 重置计数器和计时器
                        moveCount = 0;
                        _rateStopwatch.Restart();
                    }
                }));
            }
            catch
            {
            }
        }
        public System.Timers.Timer timerClean = new System.Timers.Timer();

        // 静态字段，用于存储绿色画刷
        private static readonly SolidColorBrush brushGreen = new SolidColorBrush(Colors.Green);
        // 静态字段，用于存储画刷
        private static readonly SolidColorBrush brushRed = new SolidColorBrush(Colors.Red);
        // 静态字段，用于存储画刷
        private static readonly SolidColorBrush brushNor = new SolidColorBrush((Color)ColorConverter.ConvertFromString("#4b9498"));
        private static readonly SolidColorBrush brushGray = new SolidColorBrush(Colors.Gray);
        // 创建一个 TaskCompletionSource 来等待条件满足
        private readonly ReusableTaskCompletionSource<bool> tcs = new ReusableTaskCompletionSource<bool>(); //获取到返回数据的变量
        private readonly IntPtr _windowHandle;

        


        // hall 判断参数
        private int checkSelfMin = 0;
        private int checkSelfMax = 0;
        private int gainMin = 0;
        private int gainMax = 0;
        private int noise = 0;
        private int topMin = 0;
        private int topMax = 0;
        private int bottomMin = 0;
        private int bottomMax = 0;
        private int tbDifference = 0;//top-bottom 差值
        private int checkTimes = 0;//top,bottom 读次数
        public int checkSelf = 0;//自检次数


        private int[] noiseSave = new int[3];
        private int[] rawdata = new int[3];
        private int[] rawdataBottom = new int[3];
        private int[] rawdataTop = new int[3];


        public CheckType checkType;
        public enum CheckType
        {
            hallRaw,
            hallSegment,
            coil,
            getReleaseCoil,
            getDriveMode,
            rt,
            hall,
            setRT,
            setVoltage,
            setReleaseVoltage,
            setDriveMode,
            setPwm,
            getPwm,
            coilDebug,
            setCoilDebug,
            Others
        }

        public byte[] tempByte;

        /// <summary>
        /// 单个按键的 MouseInfo 与设置参数缓存
        /// </summary>
        private class MouseKeyInfo
        {
            // Hall
            public ushort VoltageConfig0;
            public ushort VoltageConfig1;
            public ushort VoltageConfig2;

            // RT
            public bool RTEnabled;
            public ushort AP;
            public ushort RTPress;
            public ushort RTRelease;

            // 电压/导通时间
            public bool VoltageEnabled;
            public byte VoltageValue;
            public ushort ConductTime;

            // 释放振感配置
            public bool ReleaseVoltageEnabled;
            public byte ReleaseVoltageValue;
            public ushort ReleaseConductTime;

            // 线圈调试
            public bool DynOnTimeEnabled;
            public bool AutoVibrationEnabled;
            public ushort AutoVibrationPeriod;
        }

        private readonly MouseKeyInfo leftKeyInfo = new MouseKeyInfo();
        private readonly MouseKeyInfo rightKeyInfo = new MouseKeyInfo();

        // 防止 programmatically 更新 UI 时触发事件回调
        private bool _isUpdatingUi = false;

        public void CheckData(byte[] inputBuffer)
        {
            this.Dispatcher.Invoke(delegate ()
            {
                switch (checkType)
                {
                    case CheckType.hallRaw:
                        if (inputBuffer[1] == 0x55 && inputBuffer[10] == 0x01 && inputBuffer[9] == 0x01)
                        {
                            txtInfoCurrentRawL.Text = GetInfo(inputBuffer[11], inputBuffer[12]);
                            txtInfoBaselineBtmL.Text = GetInfo(inputBuffer[13], inputBuffer[14]);
                            txtInfoBaselineTopL.Text = GetInfo(inputBuffer[15], inputBuffer[16]);

                        }
                        else if (inputBuffer[1] == 0x55 && inputBuffer[10] == 0x02 && inputBuffer[9] == 0x01)
                        {
                            txtInfoCurrentRawR.Text = GetInfo(inputBuffer[11], inputBuffer[12]);
                            txtInfoBaselineBtmR.Text = GetInfo(inputBuffer[13], inputBuffer[14]);
                            txtInfoBaselineTopR.Text = GetInfo(inputBuffer[15], inputBuffer[16]);

                        }

                        if (inputBuffer[1] == 0x55 && inputBuffer[10] == 0x01 && inputBuffer[9] == 0x02)
                        {
                            txtInfoCrtSegmentL.Text = GetInfo(inputBuffer[11], inputBuffer[12]);
                            txtInfoMaxSegmentL.Text = GetInfo(inputBuffer[13], inputBuffer[14]);

                        }
                        else if (inputBuffer[1] == 0x55 && inputBuffer[10] == 0x02 && inputBuffer[9] == 0x02)
                        {
                            txtInfoCrtSegmentR.Text = GetInfo(inputBuffer[11], inputBuffer[12]);
                            txtInfoMaxSegmentR.Text = GetInfo(inputBuffer[13], inputBuffer[14]);

                            if (readOnly)
                            {
                                checkType = CheckType.hallRaw;
                            }
                            else
                                checkType = CheckType.Others;
                        }
                        break;
                    case CheckType.hall:
                        if (inputBuffer[1] == 0x55 && inputBuffer[10] == 0x01)
                        {
                            leftKeyInfo.VoltageConfig0 = GetInfoValue(inputBuffer[11], inputBuffer[12]);
                            leftKeyInfo.VoltageConfig1 = GetInfoValue(inputBuffer[13], inputBuffer[14]);
                            leftKeyInfo.VoltageConfig2 = GetInfoValue(inputBuffer[15], inputBuffer[16]);

                            txtInfoVoltageConfig0L.Text = leftKeyInfo.VoltageConfig0.ToString();
                            txtInfoVoltageConfig1L.Text = leftKeyInfo.VoltageConfig1.ToString();
                            txtInfoVoltageConfig2L.Text = leftKeyInfo.VoltageConfig2.ToString();

                            checkType = CheckType.rt;
                            commandClass.GetRT(0x01);
                        }
                        else if (inputBuffer[1] == 0x55 && inputBuffer[10] == 0x02)
                        {
                            rightKeyInfo.VoltageConfig0 = GetInfoValue(inputBuffer[11], inputBuffer[12]);
                            rightKeyInfo.VoltageConfig1 = GetInfoValue(inputBuffer[13], inputBuffer[14]);
                            rightKeyInfo.VoltageConfig2 = GetInfoValue(inputBuffer[15], inputBuffer[16]);

                            txtInfoVoltageConfig0R.Text = rightKeyInfo.VoltageConfig0.ToString();
                            txtInfoVoltageConfig1R.Text = rightKeyInfo.VoltageConfig1.ToString();
                            txtInfoVoltageConfig2R.Text = rightKeyInfo.VoltageConfig2.ToString();

                            checkType = CheckType.rt;
                            commandClass.GetRT(0x02);
                        }
                        break;
                    case CheckType.rt:
                        if (inputBuffer[1] == 0x55 && inputBuffer[10] == 0x01)
                        {
                            leftKeyInfo.RTEnabled = (inputBuffer[12] & 0xF0) != 0;
                            leftKeyInfo.AP = GetInfoValue(inputBuffer[13], inputBuffer[14]);
                            leftKeyInfo.RTPress = GetInfoValue(inputBuffer[15], inputBuffer[16]);
                            leftKeyInfo.RTRelease = GetInfoValue(inputBuffer[17], inputBuffer[18]);

                            txtInfoAPL.Text = leftKeyInfo.AP.ToString();
                            txtInfoRTPressL.Text = leftKeyInfo.RTPress.ToString();
                            txtInfoRTReleaseL.Text = leftKeyInfo.RTRelease.ToString();

                            checkType = CheckType.coil;
                            commandClass.GetCoil(0x01);
                        }
                        else if (inputBuffer[1] == 0x55 && inputBuffer[10] == 0x02)
                        {
                            rightKeyInfo.RTEnabled = (inputBuffer[12] & 0xF0) != 0;
                            rightKeyInfo.AP = GetInfoValue(inputBuffer[13], inputBuffer[14]);
                            rightKeyInfo.RTPress = GetInfoValue(inputBuffer[15], inputBuffer[16]);
                            rightKeyInfo.RTRelease = GetInfoValue(inputBuffer[17], inputBuffer[18]);

                            txtInfoAPR.Text = rightKeyInfo.AP.ToString();
                            txtInfoRTPressR.Text = rightKeyInfo.RTPress.ToString();
                            txtInfoRTReleaseR.Text = rightKeyInfo.RTRelease.ToString();

                            checkType = CheckType.coil;
                            commandClass.GetCoil(0x02);
                        }
                        break;
                    case CheckType.coil://电压，导通时间
                        if (inputBuffer[1] == 0x55 && inputBuffer[10] == 0x01)
                        {
                            leftKeyInfo.VoltageEnabled = (inputBuffer[12] & 0x80) != 0;
                            leftKeyInfo.VoltageValue = (byte)(inputBuffer[12] & 0x0F);
                            leftKeyInfo.ConductTime = GetInfoValue(inputBuffer[13], inputBuffer[14]);

                            txtInfoVoltageConductL.Text = leftKeyInfo.ConductTime.ToString();

                            checkType = CheckType.getReleaseCoil;
                            _chainKey = 0x01;
                            commandClass.GetReleaseCoil(0x01);
                            ArmChainTimeout(500);
                        }
                        else if (inputBuffer[1] == 0x55 && inputBuffer[10] == 0x02)
                        {
                            rightKeyInfo.VoltageEnabled = (inputBuffer[12] & 0x80) != 0;
                            rightKeyInfo.VoltageValue = (byte)(inputBuffer[12] & 0x0F);
                            rightKeyInfo.ConductTime = GetInfoValue(inputBuffer[13], inputBuffer[14]);

                            txtInfoVoltageConductR.Text = rightKeyInfo.ConductTime.ToString();

                            checkType = CheckType.getReleaseCoil;
                            _chainKey = 0x02;
                            commandClass.GetReleaseCoil(0x02);
                            ArmChainTimeout(500);
                        }
                        break;
                    case CheckType.getReleaseCoil://释放线圈配置读（0x03，byte[8]=0x02 为读回复，与 set 的 ack 区分）
                        if (inputBuffer[1] == 0x55 && inputBuffer[8] == 0x02 && inputBuffer[9] == 0x03 && (inputBuffer[10] == 0x01 || inputBuffer[10] == 0x02))
                        {
                            MouseKeyInfo info = GetKeyInfo(inputBuffer[10]);
                            info.ReleaseVoltageEnabled = (inputBuffer[12] & 0x80) != 0;
                            info.ReleaseConductTime = GetInfoValue(inputBuffer[13], inputBuffer[14]);
                            if (inputBuffer[10] == 0x01)
                            {
                                txtInfoVoltageValueL.Text = info.ReleaseConductTime.ToString();
                            }
                            else
                            {
                                txtInfoVoltageValueR.Text = info.ReleaseConductTime.ToString();
                            }

                            _chainTimer.Stop();
                            StartDriveModeRead(inputBuffer[10]);
                        }
                        break;
                    case CheckType.getDriveMode://全局线圈驱动方式（一包）
                        if (inputBuffer[1] == 0x55 && inputBuffer[9] == 0x06 && inputBuffer[10] == 0xFF)
                        {
                            _coilDriveMode = inputBuffer[11];

                            AdvanceAfterDriveMode();
                        }
                        break;
                    case CheckType.setRT:
                        if (inputBuffer[1] == 0x55 && inputBuffer[11] == 0x01)
                        {
                            ShowSetStatus(txtRTStatus, brushGreen);

                            ReadMouseInfo();
                            //设置成功
                        }
                        break;
                    case CheckType.setVoltage:
                        if (inputBuffer[1] == 0x55 && inputBuffer[9] == 0x02 && inputBuffer[11] == 0x01 &&
                            (_applyingGear ? inputBuffer[10] == GearStepKey() : (inputBuffer[10] == 0x01 || inputBuffer[10] == 0x02)))
                        {
                            if (_applyingGear) { GearStepAcked(); break; }

                            ShowSetStatus(txtVoltageStatus, brushGreen);

                            ReadMouseInfo();
                            //设置成功
                        }
                        break;
                    case CheckType.setReleaseVoltage:
                        if (inputBuffer[1] == 0x55 && inputBuffer[9] == 0x03 && inputBuffer[11] == 0x01 &&
                            (_applyingGear ? inputBuffer[10] == GearStepKey() : (inputBuffer[10] == 0x01 || inputBuffer[10] == 0x02)))
                        {
                            if (_applyingGear) { GearStepAcked(); break; }

                            ShowSetStatus(txtReleaseVoltageStatus, brushGreen);

                            ReadMouseInfo();
                        }
                        break;
                    case CheckType.setDriveMode:
                        if (inputBuffer[1] == 0x55 && inputBuffer[9] == 0x06 && inputBuffer[10] == 0xFF && inputBuffer[11] == 0x01)
                        {
                            ShowSetStatus(txtDriveModeStatus, brushGreen);

                            ReadMouseInfo();
                        }
                        break;
                    case CheckType.getPwm:
                        if (inputBuffer[1] == 0x55 && inputBuffer[9] == 0x07 && inputBuffer[10] == 0xFF && inputBuffer[11] == _pwmSeqIndex)
                        {
                            Array.Copy(inputBuffer, 12, _pwmTable, _pwmSeqIndex, 50);
                            if (_pwmSeqIndex == 0x00)
                            {
                                _pwmSeqIndex = 0x32;
                                commandClass.GetPwm(0x32);
                            }
                            else
                            {
                                checkType = CheckType.Others;
                                txtPwmResult.Text = FormatPwmTable(_pwmTable);
                            }
                        }
                        break;
                    case CheckType.setPwm:
                        if (inputBuffer[1] == 0x55 && inputBuffer[9] == 0x07 && inputBuffer[10] == 0xFF && inputBuffer[11] == _pwmSeqIndex && inputBuffer[12] == 0x01)
                        {
                            if (_pwmSeqIndex == 0x00)
                            {
                                _pwmSeqIndex = 0x32;
                                SendPwmPacket(0x32);
                            }
                            else
                            {
                                checkType = CheckType.Others;
                                ShowSetStatus(txtPwmStatus, brushGreen);
                            }
                        }
                        break;
                    case CheckType.setCoilDebug:
                        if (inputBuffer[1] == 0x55 && inputBuffer[11] == 0x01)
                        {
                            ShowSetStatus(txtCoilDebugStatus, brushGreen);

                            ReadMouseInfo();
                        }
                        break;
                    case CheckType.coilDebug:
                        if (inputBuffer[1] == 0x55 && inputBuffer[10] == 0x01)
                        {
                            leftKeyInfo.DynOnTimeEnabled = inputBuffer[11] != 0;
                            leftKeyInfo.AutoVibrationEnabled = inputBuffer[12] != 0;
                            leftKeyInfo.AutoVibrationPeriod = GetInfoValue(inputBuffer[13], inputBuffer[14]);

                            if (_readingMouseInfoAll)
                            {
                                // 读完左键全部参数后，继续读右键
                                checkType = CheckType.hall;
                                commandClass.GetHall(0x02);
                            }
                            else
                            {
                                checkType = CheckType.Others;
                                UpdateSettingsUi();
                                _readingInfo = false; // 读回链路结束
                                if (_wasReadingRaw)
                                {
                                    _wasReadingRaw = false;
                                    StartRawDataReading();
                                }
                            }
                        }
                        else if (inputBuffer[1] == 0x55 && inputBuffer[10] == 0x02)
                        {
                            rightKeyInfo.DynOnTimeEnabled = inputBuffer[11] != 0;
                            rightKeyInfo.AutoVibrationEnabled = inputBuffer[12] != 0;
                            rightKeyInfo.AutoVibrationPeriod = GetInfoValue(inputBuffer[13], inputBuffer[14]);

                            checkType = CheckType.Others;
                            UpdateSettingsUi();
                            _readingInfo = false; // 读回链路结束
                            if (_wasReadingRaw)
                            {
                                _wasReadingRaw = false;
                                StartRawDataReading();
                            }
                        }
                        break;
                    case CheckType.Others:
                        break;
                    default:
                        break;
                }
            });
        }

        private string GetInfo(byte v1, byte v2)
        {
            ushort value = (ushort)((v2 << 8) | v1);   // 0x1234 = 4660
            return value.ToString();
        }

        private ushort GetInfoValue(byte v1, byte v2)
        {
            return (ushort)((v2 << 8) | v1);
        }

        private MouseKeyInfo GetSelectedKeyInfo()
        {
            return rbKeyR.IsChecked == true ? rightKeyInfo : leftKeyInfo;
        }

        private MouseKeyInfo GetKeyInfo(byte key)
        {
            return key == 0x02 ? rightKeyInfo : leftKeyInfo;
        }

        /// <summary>
        /// 将当前选中按键的参数绑定到设置 UI（RT、电压/导通时间、线圈调试）
        /// </summary>
        private void UpdateSettingsUi()
        {
            _isUpdatingUi = true;
            try
            {
                MouseKeyInfo info = GetSelectedKeyInfo();

                // 设置 RT
                rbRTYes.IsChecked = info.RTEnabled;
                rbRTNo.IsChecked = !info.RTEnabled;
                tbAPValue.Text = info.AP.ToString();
                sliderAP.Value = ClampSliderValue(sliderAP, info.AP);
                tbRTPress.Text = info.RTPress.ToString();
                sliderRTPress.Value = ClampSliderValue(sliderRTPress, info.RTPress);
                tbRTRelease.Text = info.RTRelease.ToString();
                sliderRTRelease.Value = ClampSliderValue(sliderRTRelease, info.RTRelease);

                // 设置电压/导通时间
                rbVolYes.IsChecked = info.VoltageEnabled;
                rbVolNo.IsChecked = !info.VoltageEnabled;
                sliderConductTime.Value = ClampSliderValue(sliderConductTime, info.ConductTime);
                txtConductTimeDisplay.Text = $"{info.ConductTime} ";

                // 设置释放振感
                sliderReleaseConductTime.Value = ClampSliderValue(sliderReleaseConductTime, info.ReleaseConductTime);
                txtReleaseConductTimeDisplay.Text = $"{info.ReleaseConductTime} ";

                // 设置驱动方式（0: 默认正常模式, 1: 静音模式）；非 0/1 视为驱动失效
                chkDriveNormal.IsChecked = _coilDriveMode == 0;
                chkDriveSilent.IsChecked = _coilDriveMode == 1;
                UpdateDriveModeUiVisibility();
                if (_coilDriveMode > 1)
                {
                    txtDriveModeStatus.Text = "驱动失效";
                    txtDriveModeStatus.Foreground = brushRed;
                }

                // 设置线圈调试
                chkDynOnTimeEn.IsChecked = info.DynOnTimeEnabled;
                chkAutoVibrationEn.IsChecked = info.AutoVibrationEnabled;
                sliderAutoVibrationPeriod.Value = ClampSliderValue(sliderAutoVibrationPeriod, info.AutoVibrationPeriod);
                txtAutoVibrationPeriodDisplay.Text = $"{info.AutoVibrationPeriod} ms";
                // 释放强度列由 getReleaseCoil 回读或设置时的本地刷新负责，这里不覆盖
            }
            finally
            {
                _isUpdatingUi = false;
            }
        }

        private static double ClampSliderValue(Slider slider, int value)
        {
            if (value < slider.Minimum)
                return slider.Minimum;
            if (value > slider.Maximum)
                return slider.Maximum;
            return value;
        }

        /// <summary>
        /// 显示设置结果，约 1 秒后自动清除，避免残留的成功提示干扰下一次设置判断
        /// </summary>
        private void ShowSetStatus(TextBlock statusText, Brush brush)
        {
            const string message = "设置成功";
            statusText.Text = message;
            statusText.Foreground = brush;

            var timer = new DispatcherTimer { Interval = TimeSpan.FromSeconds(1) };
            timer.Tick += (s, e) =>
            {
                timer.Stop();
                if (statusText.Text == message)
                {
                    statusText.Text = "";
                }
            };
            timer.Start();
        }

        public int pairKeyDown = 0; //配对按键按下次数

        private void PairKey(byte[] inputBuffer)
        {
            int value = (inputBuffer[8] << 24) | (inputBuffer[7] << 16) | (inputBuffer[6] << 8) | inputBuffer[5];
            string result = value.ToString("D6");

            // 映射每位数字 
            char[] key = new char[result.Length];
            for (int i = 0; i < result.Length; i++)
            {
                switch (result[i])
                {
                    case '0':
                        key[i] = 'L';
                        break;
                    case '1':
                        key[i] = 'R';
                        break;
                    case '2':
                        key[i] = 'M';
                        break;
                    default:
                        throw new InvalidOperationException($"无效的数字 {result[i]}，仅允许 0,1,2");
                }
            }



        }

        /// <summary>
        /// byte0是获取到的设备ID号，byte2为08，byte63=checksum，计算方式是1a23-（byte0+...byte62）
        /// </summary>
        /// <param name="id"></param>
        /// <returns></returns>
        private byte CheckSum(byte id)
        {
            int CHECKSUM_CONSTANT = 0x1A23;
            int temp = id + 08;
            // 计算校验和
            return (byte)((CHECKSUM_CONSTANT - temp) & 0xFF);
        }


        public int iPower = 0;//读电量5次求平均值
        public double powerCount = 0.0;//电量总数;


        public int i = 1;
        public int topL = 0;
        public int topR = 0;
        public int tbDiffL = 0;//top-bottom 差值
        public int tbDiffR = 0;//top-bottom 差值



        public int bottomL = 0;
        public int bottomR = 0;







        /// <summary>
        /// 获取初始化
        /// </summary>
        private void SetInitTetting()
        {
            try
            {
                // 读取配置文件
                //string configPath = System.IO.Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "config.ini");
                //string[] data = File.ReadAllLines(configPath);

                // 检查数据完整性
                //if (data.Length < 9)
                //    throw new InvalidOperationException("配置文件行数不足，至少需要9行数据。");

                // 提取配置值（使用Substring前检查长度）
                //QPVID = SafeSubstring(data[0], 4);
                //QPPID = SafeSubstring(data[1], 4);
                //QPVER = SafeSubstring(data[2], 4);
                //VidTB.Text = SafeSubstring(data[3], 5);
                //PidTB.Text = SafeSubstring(data[4], 5);
                //VerTB.Text = SafeSubstring(data[5], 5);
                //VidTBD.Text = SafeSubstring(data[6], 5);
                //PidTBD.Text = SafeSubstring(data[7], 5);
                //VerTBD.Text = SafeSubstring(data[8], 5);

                // 解析XML文件
                string xmlPath = System.IO.Path.Combine(System.Windows.Forms.Application.StartupPath, "TestItem.xml");
                XmlDocument myDoc = new XmlDocument();
                myDoc.Load(xmlPath);

                XmlNode testItemNode = myDoc.SelectSingleNode("//TestItem");
                if (testItemNode == null)
                    throw new InvalidOperationException("XML中未找到<TestItem>节点。");

                // 提取测试项（过滤空条目）
                //teststation1 = GetTestItems(testItemNode, "//TestItem1");
                //teststation2 = GetTestItems(testItemNode, "//TestItem2");
                //teststation3 = GetTestItems(testItemNode, "//TestItem3");
                //teststation4 = GetTestItems(testItemNode, "//TestItem4");
                //teststation5 = GetTestItems(testItemNode, "//TestItem5");
                //teststation6 = GetTestItems(testItemNode, "//TestItem6");
                //teststation7 = GetTestItems(testItemNode, "//TestItem7");
                //teststation8 = GetTestItems(testItemNode, "//TestItem8");
                //teststation9 = GetTestItems(testItemNode, "//TestItem9");
                //teststation10 = GetTestItems(testItemNode, "//TestItem10");
                //teststation11 = GetTestItems(testItemNode, "//TestItem11");
                //teststation12 = GetTestItems(testItemNode, "//TestItem12");
                //teststation13 = GetTestItems(testItemNode, "//TestItem13");

                //teststationMES1 = GetTestItems(testItemNode, "//TestItem1Mes");
                //teststationMES2 = GetTestItems(testItemNode, "//TestItem2Mes");
                //teststationMES3 = GetTestItems(testItemNode, "//TestItem3Mes");
                //teststationMES4 = GetTestItems(testItemNode, "//TestItem4Mes");
                //teststationMES5 = GetTestItems(testItemNode, "//TestItem5Mes");
                //teststationMES6 = GetTestItems(testItemNode, "//TestItem6Mes");
                //teststationMES7 = GetTestItems(testItemNode, "//TestItem7Mes");
                //teststationMES8 = GetTestItems(testItemNode, "//TestItem8Mes");
                //teststationMES9 = GetTestItems(testItemNode, "//TestItem9Mes");
                //teststationMES10 = GetTestItems(testItemNode, "//TestItem10Mes");
                //teststationMES11 = GetTestItems(testItemNode, "//TestItem11Mes");
                //teststationMES12 = GetTestItems(testItemNode, "//TestItem12Mes");
                //teststationMES13 = GetTestItems(testItemNode, "//TestItem13Mes");

                //// 拼接MES字符串（使用StringBuilder优化）
                //teststationMes1 = ConcatTestItems(teststationMES1);
                //teststationMes2 = ConcatTestItems(teststationMES2);
                //teststationMes3 = ConcatTestItems(teststationMES3);
                //teststationMes4 = ConcatTestItems(teststationMES4);
                //teststationMes5 = ConcatTestItems(teststationMES5);
                //teststationMes6 = ConcatTestItems(teststationMES6);
                //teststationMes7 = ConcatTestItems(teststationMES7);
                //teststationMes8 = ConcatTestItems(teststationMES8);
                //teststationMes9 = ConcatTestItems(teststationMES9);
                //teststationMes10 = ConcatTestItems(teststationMES10);
                //teststationMes11 = ConcatTestItems(teststationMES11);
                //teststationMes12 = ConcatTestItems(teststationMES12);
                //teststationMes13 = ConcatTestItems(teststationMES13);
            }
            catch (Exception ex)
            {
                // 记录异常或提示用户
                MessageBox.Show($"配置加载失败：{ex.Message}");
            }
        }

        // 安全截取字符串（避免索引越界）
        private string SafeSubstring(string input, int startIndex)
        {
            if (string.IsNullOrEmpty(input) || input.Length <= startIndex)
                return string.Empty;
            return input.Substring(startIndex);
        }

        // 提取测试项（过滤空条目）
        private string[] GetTestItems(XmlNode parentNode, string xPath)
        {
            XmlNode node = parentNode.SelectSingleNode(xPath);
            return node?.InnerText.Split(new[] { ',' }, StringSplitOptions.RemoveEmptyEntries) ?? Array.Empty<string>();
        }

        // 拼接测试项字符串
        private string ConcatTestItems(string[] items)
        {
            StringBuilder sb = new StringBuilder();
            foreach (var item in items)
                sb.Append(item);
            return sb.ToString();
        }

        //安装鼠标钩子
        MouseHook mouseHook = new MouseHook();
        public void Start(MyMouseEventHandler handler)
        {
            if (mouseHook != null)
            {
                mouseHook.AddMouseHandler(handler);
                mouseHook.Start();
            }
        }

        //卸载鼠标钩子
        public void Stop()
        {
            mouseHook.Stop();
        }

        /// <summary>
        /// 鼠标钩子回调函数
        /// </summary>
        /// <param name="wParam"></param>
        /// <param name="mouseMsg"></param>
        private void Handler(Int32 wParam, MouseHookStruct mouseMsg)
        {
            //Debug.WriteLine(wParam+"/" +mouseMsg.flags + "/" + mouseMsg.dwExtraInfo + "/" + mouseMsg.mouseData);
            //switch (wParam)
            //{
            //    //按下
            //    case WM_MOUSEMOVE:// 鼠标移动
            //        break;
            //    case WM_LBUTTONDOWN:// 鼠标左键按下
            //        if (IsHaveControl(L))
            //        {
            //            L.KeyNameColor = "KeyDown";
            //        }
            //        break;
            //    case WM_RBUTTONDOWN:// 鼠标右键按下
            //        if (IsHaveControl(R))
            //        {
            //            R.KeyNameColor = "KeyDown";
            //        }
            //        break;
            //    case WM_MBUTTONDOWN:// 鼠标中键按下
            //        if (IsHaveControl(M))
            //        {
            //            M.KeyNameColor = "KeyDown";
            //        }
            //        break;
            //    //抬起
            //    case WM_LBUTTONUP:// 鼠标左键抬起
            //        if (IsHaveControl(L))
            //        {
            //            L.Value = (int.Parse(L.Value) + 1).ToString();
            //            L.KeyNameColor = "KeyUp";
            //        }
            //        break;
            //    case WM_RBUTTONUP:// 鼠标右键抬起
            //        if (IsHaveControl(R))
            //        {
            //            R.Value = (int.Parse(R.Value) + 1).ToString();
            //            R.KeyNameColor = "KeyUp";
            //        }
            //        break;
            //    case WM_MBUTTONUP:// 鼠标中键抬起
            //        if (IsHaveControl(M))
            //        {
            //            M.Value = (int.Parse(M.Value) + 1).ToString();
            //            M.KeyNameColor = "KeyUp";
            //        }
            //        break;
            //    case WM_MOUSEWHEEL:// 鼠标滚轮// 获取 wParam 中的高位（滚轮增量）
            //        int delta = GET_WHEEL_DELTA_WPARAM(mouseMsg.mouseData);
            //        if (delta == 120)
            //        {
            //            if (IsHaveControl(WUp))
            //            {
            //                WUp.Value = (int.Parse(WUp.Value) + 1).ToString();
            //            }
            //        }
            //        else if (delta == -120)
            //        {
            //            if (IsHaveControl(WDown))
            //            {
            //                WDown.Value = (int.Parse(WDown.Value) + 1).ToString();
            //            }
            //        }
            //        break;
            //    //K4 K5
            //    case K4K5DOWN:
            //        if (mouseMsg.mouseData == WM_K4)
            //        {
            //            if (IsHaveControl(K4))
            //            {
            //                K4.KeyNameColor = "KeyDown";
            //            }
            //        }
            //        else if (mouseMsg.mouseData == WM_K5)
            //        {
            //            if (IsHaveControl(K5))
            //            {
            //                K5.KeyNameColor = "KeyDown";
            //            }
            //        }
            //        break;
            //    case K4K5UP:
            //        if (mouseMsg.mouseData == WM_K4)
            //        {
            //            if (IsHaveControl(K4))
            //            {
            //                K4.Value = (int.Parse(K4.Value) + 1).ToString();
            //                K4.KeyNameColor = "KeyUp";
            //            }
            //        }
            //        else if (mouseMsg.mouseData == WM_K5)
            //        {
            //            if (IsHaveControl(K5))
            //            {
            //                K5.Value = (int.Parse(K5.Value) + 1).ToString();
            //                K5.KeyNameColor = "KeyUp";
            //            }
            //        }
            //        break;
            //    //case WM_LEFTRIGHTPICK://  左右挑
            //    //    int tempPick = GET_WHEEL_DELTA_WPARAM(mouseMsg.mouseData);
            //    //    if (tempPick == 120)
            //    //    {
            //    //        if (IsHaveControl(Up))
            //    //        {
            //    //            Up.Value = (int.Parse(Up.Value) + 1).ToString();
            //    //        }
            //    //    }
            //    //    else if (tempPick == -120)
            //    //    {
            //    //        if (IsHaveControl(Down))
            //    //        {
            //    //            Down.Value = (int.Parse(Down.Value) + 1).ToString();
            //    //        }
            //    //    }
            //    //    break;
            //}


        }

        /// <summary>
        /// 在按键速度统计区域内按下鼠标
        /// </summary>
        private void TestKeySpeed_MouseDown(object sender, MouseButtonEventArgs e)
        {
            ProcessKeySpeedPress();
        }

        /// <summary>
        /// 处理按键速度统计
        /// </summary>
        private void ProcessKeySpeedPress()
        {
            this.Dispatcher.Invoke(new Action(() =>
            {
                switch (_keySpeedState)
                {
                    case KeySpeedState.Idle:
                        _keyPressCount = 1;
                        _keySpeedState = KeySpeedState.Counting;
                        _keySpeedHoldTimer.Stop();
                        _keySpeedCountTimer.Start();
                        tbKeySpeed.Text = _keyPressCount.ToString();
                        tbKeySpeedStatus.Text = "开始测试";
                        tbKeySpeedStatus.Foreground = brushGreen;
                        break;
                    case KeySpeedState.Counting:
                        _keyPressCount++;
                        tbKeySpeed.Text = _keyPressCount.ToString();
                        break;
                    case KeySpeedState.Holding:
                        // 超过1秒后的按键不纳入统计和显示
                        break;
                }
            }));
        }

        /// <summary>
        /// 1秒统计定时器到期
        /// </summary>
        private void KeySpeedCountTimer_Tick(object sender, EventArgs e)
        {
            this.Dispatcher.Invoke(new Action(() =>
            {
                _keySpeedCountTimer.Stop();
                _keySpeedState = KeySpeedState.Holding;
                _keySpeedHoldTimer.Start();
                tbKeySpeedStatus.Text = "停止测试";
                tbKeySpeedStatus.Foreground = brushGray;
            }));
        }

        /// <summary>
        /// 10秒保持定时器到期
        /// </summary>
        private void KeySpeedHoldTimer_Tick(object sender, EventArgs e)
        {
            this.Dispatcher.Invoke(new Action(() =>
            {
                _keySpeedHoldTimer.Stop();
                _keySpeedState = KeySpeedState.Idle;
                _keyPressCount = 0;
                tbKeySpeed.Text = "0";
                tbKeySpeedStatus.Text = "停止测试";
                tbKeySpeedStatus.Foreground = brushGray;
            }));
        }

        /// <summary>
        /// 判断是否存在这个控件在warppanel中，不排除如果没有此按键会报错
        /// </summary>
        /// <returns></returns>
        public bool IsHaveControl(MouseKey key)
        {
            string targetName = key.Name.ToString(); // 假设你要查找的元素 Name 为 "MyButton"
            foreach (var element in MyWarPanel.Children)
            {
                if (element is MouseKey a && FindName(a.Name) != null && targetName == a.Name)
                {
                    return true;
                }
            }
            return false;
        }

        /// <summary>
        /// 从 wParam 提取滚轮增量
        /// </summary>
        /// <param name="wParam"></param>
        /// <returns></returns>
        private int GET_WHEEL_DELTA_WPARAM(int wParam)
        {
            return (short)(wParam >> 16);  // 获取 wParam 的高16位（增量值）
        }

        protected override void OnSourceInitialized(EventArgs e)
        {
            base.OnSourceInitialized(e);
            this.Win_SourceInitialized(this, e);
        }

        /// <summary>
        /// 委托一个事件 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void Win_SourceInitialized(object sender, EventArgs e)
        {
            if (PresentationSource.FromVisual(this) is HwndSource hwndSource)
            {
                hwndSource.AddHook(new HwndSourceHook(WndProc));
            }

        }
        public static bool HaveRueslt = false;

        protected virtual IntPtr WndProc(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, ref bool handled)
        {
            if (msg == DeviceManagement.WM_DEVICECHANGE)
            {
                if (!HaveRueslt)//找到想要的设备就不再进来
                {
                    MyHID.OnDeviceChange(wParam.ToInt32());
                }
            }
            if (msg == FileIO.WM_MYCOSTOM)
            {
                switch (lParam.ToInt32())
                {
                    // 被测物
                    case 1: // 找到设备
                        if (wParam.ToInt32() == 1)
                        {
                            HaveRueslt = false;
                            MouseConnect();
                        }
                        break;
                    case 2: //退出
                        if (wParam.ToInt32() == 2)
                        {
                            if (!suspend) // 接收器进入睡眠标记不清除
                            {

                                ClearAll();



                            }


                            VerFailTB.Opacity = 0;

                        }
                        break;
                }
            }
            if (msg == FileIO.WM_INPUT)
            {
                uint dwSize = 0;
                GetRawInputData(lParam, 0x10000003, IntPtr.Zero, ref dwSize, (uint)Marshal.SizeOf(typeof(RAWINPUTHEADER)));

                IntPtr rawData = Marshal.AllocHGlobal((int)dwSize);
                if (GetRawInputData(lParam, 0x10000003, rawData, ref dwSize, (uint)Marshal.SizeOf(typeof(RAWINPUTHEADER))) != 0)
                {
                    RAWINPUT rawInput = Marshal.PtrToStructure<RAWINPUT>(rawData);
                    // 处理鼠标移动
                    if (RrIsBegin)//报告率开关
                        if (rawInput.header.dwType == 0) // MOUSE
                        {
                            moveCount++;
                        }
                }

                Marshal.FreeHGlobal(rawData);
            }

            return IntPtr.Zero;
        }

        public void MouseConnect()
        {


            TrueVerTB.Text = HIDCommonClass.VPIDpassVerF.ToString("X2");
            if (TrueVerTB.Text == txtVer.Text)//连接成功
            {
                //ver = TrueVerTB.Text.Trim();
                ConnCheckBox.IsChecked = true;


                ReadMouseInfo(true);   // 连接后先读左键，再读右键 MouseInfo

                //DLine.IsEnabled = true;
                //RrIsBegin = true;
                //Start(Handler); // 启动钩子

            }
            else//版本号错误
            {
                VerFailTB.Opacity = 1;
            }

        }











        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            Stop();
            Environment.Exit(0);
        }



        /// <summary>
        /// 清除测试
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void CleanBtn_Click(object sender, RoutedEventArgs e)//
        {
            ClearAll();



            //测试界面部分
            //DLine.IsEnabled = false;
            //InputShowTB.Text = "INPUT...";
            //InputShowTB.Foreground = brushNor;
            List<string> list = null;
            //list = station1Item;


            if (list == null)//如果没有选择工站就弹出窗口，list是null
            {
                return;
            }
            for (int i = 0; i < list.Count; i++)
            {
                if (FindName(list[i]) is MouseKey)
                {
                    MouseKey b = (MouseKey)FindName(list[i]);
                    b.TestRueslt = "Fail";
                    b.TestRueslt = "NA";
                }
                else if (FindName(list[i]) is ReportingRate.ReportingRate)
                {
                    ReportingRate.ReportingRate b = (ReportingRate.ReportingRate)FindName(list[i]);
                    b.TestRueslt = "NA";
                }
                else if (FindName(list[i]) is WpfLinePanel)
                {
                    WpfLinePanel b = (WpfLinePanel)FindName(list[i]);
                    b.TestRueslt = "NA";

                }
            }


        }

        private void ClearAll()
        {




            mouseMid = "";
            dongleMid = "";



            //其他
            Stop();//钩子
            RrIsBegin = false;//报告率
            HaveRueslt = false;
            //设备1部分
            ConnCheckBox.IsChecked = false;
            TrueVerTB.Text = "";

            //PWM 配置
            txtPwmTable.Text = "";
            txtPwmResult.Text = "";
            txtPwmStatus.Text = "";




            //测试界面部分
            //DLine.IsEnabled = false;
            //InputShowTB.Text = "INPUT...";
            //InputShowTB.Foreground = brushNor;
            List<string> list = null;
            //list = station1Item;


            if (list == null)//如果没有选择工站就弹出窗口，list是null
            {
                return;
            }
            for (int i = 0; i < list.Count; i++)
            {
                if (FindName(list[i]) is MouseKey)
                {
                    MouseKey b = (MouseKey)FindName(list[i]);
                    b.TestRueslt = "Fail";
                    b.TestRueslt = "NA";
                }
                else if (FindName(list[i]) is ReportingRate.ReportingRate)
                {
                    ReportingRate.ReportingRate b = (ReportingRate.ReportingRate)FindName(list[i]);
                    b.TestRueslt = "NA";
                }
                else if (FindName(list[i]) is WpfLinePanel)
                {
                    WpfLinePanel b = (WpfLinePanel)FindName(list[i]);
                    b.TestRueslt = "NA";

                }
            }




        }




        public async void TestLED()
        {
            Task task = Task.Run(async delegate ()
            {
                commandClass.Led_Set("red");
                await Task.Delay(1000);
                commandClass.Led_Set("green");
                await Task.Delay(1000);
                commandClass.Led_Set("blue");
                //await Task.Delay(1000);
                //commandClass.Led_Set("close");

            });
            await task;

        }

        private void SaveRTSettings()
        {
            if (commandClass == null) return;

            byte key = 0;//左右键
            byte bmEnableMask = 0;//是否生效
            byte apValueH = (byte)((int.Parse(tbAPValue.Text.Trim()) >> 8) & 0xFF);//AP值 高位
            byte apValueL = (byte)((int.Parse(tbAPValue.Text.Trim()) & 0xFF));//AP值 低位

            byte rtPressH = (byte)((int.Parse(tbRTPress.Text.Trim()) >> 8) & 0xFF);//RT值按下 高位
            byte rtPressL = (byte)((int.Parse(tbRTPress.Text.Trim()) & 0xFF));//RT值按下 低位

            byte rtReleaseH = (byte)((int.Parse(tbRTRelease.Text.Trim()) >> 8) & 0xFF);//RT值释放 高位
            byte rtReleaseL = (byte)((int.Parse(tbRTRelease.Text.Trim()) & 0xFF));//RT值释放 低位

            if (rbRTYes.IsChecked == true)
                bmEnableMask = 0xf0;
            else if (rbRTNo.IsChecked == true)
                bmEnableMask = 0x0;

            if (rbKeyL.IsChecked == true)
                key = 0x01;
            else if (rbKeyR.IsChecked == true)
                key = 0x02;

            // 同步更新缓存，避免切换按键时显示旧值
            MouseKeyInfo info = GetKeyInfo(key);
            info.RTEnabled = bmEnableMask != 0;
            info.AP = (ushort)((apValueH << 8) | apValueL);
            info.RTPress = (ushort)((rtPressH << 8) | rtPressL);
            info.RTRelease = (ushort)((rtReleaseH << 8) | rtReleaseL);

            checkType = CheckType.setRT;
            commandClass.SetRT(key, bmEnableMask, apValueH, apValueL, rtPressH, rtPressL, rtReleaseH, rtReleaseL);
        }

        private void SliderRT_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (sender is Slider slider)
            {
                if (tbAPValue == null || tbRTPress == null || tbRTRelease == null)
                    return;

                int value = (int)slider.Value;
                if (slider.Name == "sliderAP")
                    tbAPValue.Text = value.ToString();
                else if (slider.Name == "sliderRTPress")
                    tbRTPress.Text = value.ToString();
                else if (slider.Name == "sliderRTRelease")
                    tbRTRelease.Text = value.ToString();
            }
        }

        private void SliderRT_PreviewMouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            SaveRTSettings();
        }

        private void SaveVoltageSettings()
        {
            if (commandClass == null) return;

            byte key = 0;//左右键
            byte bmEnableMask = 0;//是否生效
            int conductTime = (int)sliderConductTime.Value;
            byte CoilTurnOnTimeH = (byte)((conductTime >> 8) & 0xFF);//导通时间 高位
            byte CoilTurnOnTimeL = (byte)(conductTime & 0xFF);//导通时间 低位

            if (rbVolYes.IsChecked == true)
                bmEnableMask = 0x80; // 电压值固定为0
            else if (rbVolNo.IsChecked == true)
                bmEnableMask = 0x00;

            if (rbKeyL.IsChecked == true)
                key = 0x01;
            else if (rbKeyR.IsChecked == true)
                key = 0x02;

            // 同步更新缓存，避免切换按键时显示旧值
            MouseKeyInfo info = GetKeyInfo(key);
            info.VoltageEnabled = bmEnableMask != 0;
            info.ConductTime = (ushort)conductTime;

            checkType = CheckType.setVoltage;
            commandClass.SetVoltage(key, bmEnableMask, CoilTurnOnTimeL, CoilTurnOnTimeH);
        }

        private void SaveReleaseVoltageSettings()
        {
            if (commandClass == null) return;

            byte key = 0;//左右键
            byte bmEnableMask = 0;//是否生效
            int conductTime = (int)sliderReleaseConductTime.Value;
            byte CoilTurnOnTimeH = (byte)((conductTime >> 8) & 0xFF);//导通时间 高位
            byte CoilTurnOnTimeL = (byte)(conductTime & 0xFF);//导通时间 低位

            bmEnableMask = 0x80; // 释放振感固定使能

            if (rbKeyL.IsChecked == true)
                key = 0x01;
            else if (rbKeyR.IsChecked == true)
                key = 0x02;

            // 同步更新缓存，避免切换按键时显示旧值
            MouseKeyInfo info = GetKeyInfo(key);
            info.ReleaseVoltageEnabled = bmEnableMask != 0;
            info.ReleaseConductTime = (ushort)conductTime;

            // 固件暂无释放振感读命令，直接用本地值刷新 MouseInfo 显示
            if (key == 0x01)
                txtInfoVoltageValueL.Text = info.ReleaseConductTime.ToString();
            else
                txtInfoVoltageValueR.Text = info.ReleaseConductTime.ToString();

            checkType = CheckType.setReleaseVoltage;
            commandClass.SetReleaseVoltage(key, bmEnableMask, CoilTurnOnTimeL, CoilTurnOnTimeH);
        }

        // 全局驱动方式缓存（0: 默认正常模式, 1: 静音模式），读取链路暂存的按键与超时定时器
        private byte _coilDriveMode;
        private byte _chainKey;
        private DispatcherTimer _chainTimer;

        /// <summary>
        /// 链路单步超时兜底：某条读命令鼠标不支持/未回复时跳过该步，避免读回链路卡死
        /// </summary>
        private void ArmChainTimeout(double milliseconds)
        {
            if (_chainTimer == null)
            {
                _chainTimer = new DispatcherTimer();
                _chainTimer.Tick += ChainTimeout_Tick;
            }
            _chainTimer.Interval = TimeSpan.FromMilliseconds(milliseconds);
            _chainTimer.Stop();
            _chainTimer.Start();
        }

        private void ChainTimeout_Tick(object sender, EventArgs e)
        {
            _chainTimer.Stop();
            switch (checkType)
            {
                case CheckType.getReleaseCoil:
                    // 固件不回复 0x05 时，释放强度列回退本地缓存值，继续链路
                    txtInfoVoltageValueL.Text = leftKeyInfo.ReleaseConductTime.ToString();
                    txtInfoVoltageValueR.Text = rightKeyInfo.ReleaseConductTime.ToString();
                    StartDriveModeRead(_chainKey);
                    break;
                case CheckType.getDriveMode:
                    AdvanceAfterDriveMode();
                    break;
            }
        }

        /// <summary>
        /// 发送读取全局驱动方式命令
        /// </summary>
        private void StartDriveModeRead(byte key)
        {
            _chainKey = key;
            checkType = CheckType.getDriveMode;
            commandClass.GetDriveMode();
            ArmChainTimeout(500);
        }

        private void AdvanceAfterDriveMode()
        {
            _chainTimer?.Stop();
            checkType = CheckType.coilDebug;
            commandClass.GetCoilDebug(_chainKey);
        }

        private void ChkDriveMode_Changed(object sender, RoutedEventArgs e)
        {
            if (_isUpdatingUi) return;

            UpdateDriveModeUiVisibility();

            if (chkDriveSilent.IsChecked == true)
            {
                SaveDriveMode(1);
            }
            else if (commandClass != null)
            {
                // 从静音切回正常：先下发 0 挡位（按下/释放清零），完成后再切模式
                _pendingDriveMode = 0;
                ApplySilentGear(0);
            }
        }

        // 挡位序列完成后待补发的模式切换命令
        private byte? _pendingDriveMode;

        /// <summary>
        /// 按当前选择的驱动方式调整界面：静音模式隐藏按下/释放振感配置面板、显示挡位；
        /// 正常模式显示强度面板、隐藏挡位
        /// </summary>
        private void UpdateDriveModeUiVisibility()
        {
            bool silent = chkDriveSilent.IsChecked == true;

            testVoltag.Visibility = silent ? Visibility.Collapsed : Visibility.Visible;
            testReleaseVoltag.Visibility = silent ? Visibility.Collapsed : Visibility.Visible;

            Visibility gearVisibility = silent ? Visibility.Visible : Visibility.Collapsed;
            txtGearCaption.Visibility = gearVisibility;
            btnGear0.Visibility = gearVisibility;
            btnGear1.Visibility = gearVisibility;
            btnGear2.Visibility = gearVisibility;
            btnGear3.Visibility = gearVisibility;
            btnGear0.IsEnabled = silent;
            btnGear1.IsEnabled = silent;
            btnGear2.IsEnabled = silent;
            btnGear3.IsEnabled = silent;
        }

        /// <summary>
        /// 静音挡位值表：挡位 0~3 对应按下/释放强度
        /// </summary>
        private static readonly ushort[] SilentGearValues = { 0, 1500, 2000, 2500 };

        // 当前生效挡位对应的按钮（绿色高亮）
        private Button _activeGearBtn;

        private void BtnSilentGear_Click(object sender, RoutedEventArgs e)
        {
            if (commandClass == null) return;

            int gear = int.Parse((string)((Button)sender).Tag);
            ApplySilentGear(SilentGearValues[gear]);

            // 标记当前生效挡位：按钮变绿，切到其他挡位时恢复原样
            if (_activeGearBtn != null)
            {
                _activeGearBtn.ClearValue(Control.BackgroundProperty);
            }
            _activeGearBtn = (Button)sender;
            _activeGearBtn.Background = Brushes.LightGreen;
        }

        /// <summary>
        /// 静音挡位下发状态：0=左按下 1=左释放 2=右按下 3=右释放，逐条发送等回复确认，失败重发一次
        /// </summary>
        private bool _applyingGear;
        private int _gearStep;
        private int _gearRetry;
        private ushort _gearValue;
        private DispatcherTimer _gearTimer;

        private byte GearStepKey()
        {
            return _gearStep < 2 ? (byte)0x01 : (byte)0x02;
        }

        /// <summary>
        /// 应用静音挡位：把挡位值同时设置为左/右键的按下和释放振感强度。
        /// 逐条发送并等待对应回复，300ms 无回应重发一次，仍失败则跳过该条，4 条完成后刷新显示并回读
        /// </summary>
        private void ApplySilentGear(ushort value)
        {
            // 打断进行中的读回链路（如有），其残留的回复按各自分支处理
            _readingInfo = false;
            ReadMouseInfoCancelled();

            // 暂停 RawData 轮询，避免轮询命令与设置命令并发写设备
            _pauseRaw = true;

            _applyingGear = true;
            _gearStep = 0;
            _gearRetry = 0;
            _gearValue = value;
            SendGearStep();
        }

        private void ReadMouseInfoCancelled()
        {
            // 挡位下发的占位状态，回复统一走 Others 忽略
            checkType = CheckType.Others;
        }

        private void SendGearStep()
        {
            byte key = GearStepKey();
            bool press = _gearStep % 2 == 0;
            byte timeL = (byte)(_gearValue & 0xFF);
            byte timeH = (byte)((_gearValue >> 8) & 0xFF);

            if (press)
            {
                // 只改强度，保持各键原有的生效/失效状态
                byte pressMask = (byte)(GetKeyInfo(key).VoltageEnabled ? 0x80 : 0x00);
                checkType = CheckType.setVoltage;
                commandClass.SetVoltage(key, pressMask, timeL, timeH);
            }
            else
            {
                checkType = CheckType.setReleaseVoltage;
                commandClass.SetReleaseVoltage(key, 0x80, timeL, timeH);
            }

            if (_gearTimer == null)
            {
                _gearTimer = new DispatcherTimer();
                _gearTimer.Tick += GearTimeout_Tick;
            }
            _gearTimer.Interval = TimeSpan.FromMilliseconds(300);
            _gearTimer.Stop();
            _gearTimer.Start();
        }

        private void GearStepAcked()
        {
            _gearTimer.Stop();
            _gearRetry = 0;
            _gearStep++;
            if (_gearStep < 4)
            {
                SendGearStep();
            }
            else
            {
                FinishGearApply();
            }
        }

        private void GearTimeout_Tick(object sender, EventArgs e)
        {
            _gearTimer.Stop();
            if (!_applyingGear) return;

            _gearRetry++;
            if (_gearRetry > 1)
            {
                // 重发一次仍无回应，跳过该条继续
                _gearRetry = 0;
                _gearStep++;
                if (_gearStep >= 4)
                {
                    FinishGearApply();
                    return;
                }
            }
            SendGearStep();
        }

        private void FinishGearApply()
        {
            _applyingGear = false;
            _pauseRaw = false;

            foreach (byte key in new byte[] { 0x01, 0x02 })
            {
                MouseKeyInfo info = GetKeyInfo(key);
                info.ConductTime = _gearValue;
                info.ReleaseConductTime = _gearValue;
            }

            // 固件无释放振感读命令，MouseInfo 直接显示本地下发的值
            txtInfoVoltageConductL.Text = _gearValue.ToString();
            txtInfoVoltageConductR.Text = _gearValue.ToString();
            txtInfoVoltageValueL.Text = _gearValue.ToString();
            txtInfoVoltageValueR.Text = _gearValue.ToString();

            UpdateSettingsUi();
            ShowSetStatus(txtDriveModeStatus, brushGreen);

            if (_pendingDriveMode.HasValue)
            {
                // 有挂起的模式切换（静音→正常），先发模式命令，其回复会触发回读
                byte mode = _pendingDriveMode.Value;
                _pendingDriveMode = null;
                SaveDriveMode(mode);
            }
            else
            {
                // 重新发起一次回读确认
                ReadMouseInfo();
            }
        }

        private void SaveDriveMode(byte mode)
        {
            if (commandClass == null) return;

            _coilDriveMode = mode;

            checkType = CheckType.setDriveMode;
            commandClass.SetDriveMode(mode);
        }

        // PWM 波形表缓存（100 点）与分包序号（0x00/0x32）
        private readonly byte[] _pwmTable = new byte[100];
        private byte _pwmSeqIndex;

        private void BtnGetPwm_Click(object sender, RoutedEventArgs e)
        {
            if (commandClass == null) return;

            _pwmSeqIndex = 0x00;
            checkType = CheckType.getPwm;
            commandClass.GetPwm(0x00);
        }

        private void BtnSetPwm_Click(object sender, RoutedEventArgs e)
        {
            if (commandClass == null) return;

            if (!TryParsePwmTable(txtPwmTable.Text, _pwmTable))
            {
                txtPwmStatus.Text = "需要100个0~100的数值";
                txtPwmStatus.Foreground = brushRed;
                return;
            }

            _pwmSeqIndex = 0x00;
            checkType = CheckType.setPwm;
            SendPwmPacket(0x00);
        }

        private void SendPwmPacket(byte startIndex)
        {
            byte[] chunk = new byte[50];
            Array.Copy(_pwmTable, startIndex, chunk, 0, 50);
            commandClass.SetPwm(startIndex, chunk);
        }

        private void TxtPwm_PreviewTextInput(object sender, TextCompositionEventArgs e)
        {
            e.Handled = !(int.TryParse(e.Text, out _) || e.Text == "," || e.Text == " " || e.Text == "\r" || e.Text == "\n");
        }

        /// <summary>
        /// 解析文本框中的 PWM 表：需要恰好 100 个 0~100 的整数
        /// </summary>
        private static bool TryParsePwmTable(string text, byte[] table)
        {
            var values = new List<int>();
            foreach (string part in text.Split(new[] { ',', ' ', '\r', '\n', '\t' }, StringSplitOptions.RemoveEmptyEntries))
            {
                if (!int.TryParse(part.Trim(), out int v) || v < 0 || v > 100)
                    return false;
                values.Add(v);
            }
            if (values.Count != 100) return false;

            for (int i = 0; i < 100; i++)
            {
                table[i] = (byte)values[i];
            }
            return true;
        }

        private static string FormatPwmTable(byte[] table)
        {
            var sb = new StringBuilder();
            for (int i = 0; i < table.Length; i++)
            {
                sb.Append(table[i]);
                sb.Append(i % 10 == 9 ? Environment.NewLine : ", ");
            }
            return sb.ToString();
        }

        private void SaveCoilDebugSettings()
        {
            if (commandClass == null) return;

            byte key = 0;
            byte bDynOnTimeEn = (byte)(chkDynOnTimeEn.IsChecked == true ? 0x01 : 0x00);
            byte bAutoVibrationEn = (byte)(chkAutoVibrationEn.IsChecked == true ? 0x01 : 0x00);
            int period = (int)sliderAutoVibrationPeriod.Value;
            byte periodH = (byte)((period >> 8) & 0xFF);
            byte periodL = (byte)(period & 0xFF);

            if (rbKeyL.IsChecked == true)
                key = 0x01;
            else if (rbKeyR.IsChecked == true)
                key = 0x02;

            // 同步更新缓存，避免切换按键时显示旧值
            MouseKeyInfo info = GetKeyInfo(key);
            info.DynOnTimeEnabled = bDynOnTimeEn != 0;
            info.AutoVibrationEnabled = bAutoVibrationEn != 0;
            info.AutoVibrationPeriod = (ushort)period;

            checkType = CheckType.setCoilDebug;
            commandClass.SetCoilDebug(key, bDynOnTimeEn, bAutoVibrationEn, periodL, periodH);
        }



        public bool readOnly = false; // RawData 轮询默认停止，点“开始读RawData”后才启动
        private bool _wasReadingRaw = false;
        private bool _readingMouseInfoAll = false;
        private bool _readingInfo = false;

        private async void ReadMouseInfo(bool readAll = false)
        {
            if (_readingInfo) return; // 已有读回链路在跑，跳过避免并发

            _readingInfo = true; // 立即占位（含下面的等待期）
            _wasReadingRaw = readOnly;
            _readingMouseInfoAll = readAll;
            readOnly = false;

            if (_wasReadingRaw)
            {
                await Task.Delay(800); // 等待残留的 RawData 回复处理完毕
            }

            if (!_readingInfo) return; // 等待期间被挡位下发等打断，本次作废

            // readAll=true：连接后按 左→右 顺序读取两个按键的 MouseInfo
            // readAll=false：只读取当前选中的左/右键 MouseInfo
            byte key = readAll ? (byte)0x01 : (rbKeyR.IsChecked == true ? (byte)0x02 : (byte)0x01);

            checkType = CheckType.hall;
            commandClass.GetHall(key);
        }

        // 挡位下发等场景暂停 RawData 轮询，避免轮询命令与设置命令并发写设备
        private volatile bool _pauseRaw;

        private void StartRawDataReading()
        {
            checkType = CheckType.hallRaw;
            readOnly = true;
            Task.Run(() =>
            {
                while (readOnly)
                {
                    if (_pauseRaw) { Thread.Sleep(50); continue; }
                    commandClass.GetHallRaw(0x01);
                    Thread.Sleep(100);

                    if (_pauseRaw) { Thread.Sleep(50); continue; }
                    commandClass.GetHallRaw(0x02);
                    Thread.Sleep(100);

                    if (_pauseRaw) { Thread.Sleep(50); continue; }
                    commandClass.GetHallSegment(0x01);
                    Thread.Sleep(100);

                    if (_pauseRaw) { Thread.Sleep(50); continue; }
                    commandClass.GetHallSegment(0x02);
                    Thread.Sleep(100);
                }
            });
        }

        private void btnRawStart_Click(object sender, RoutedEventArgs e)
        {
            StartRawDataReading();
        }

        private void btnRawStop_Click(object sender, RoutedEventArgs e)
        {
            readOnly = false;
        }
    }
}
