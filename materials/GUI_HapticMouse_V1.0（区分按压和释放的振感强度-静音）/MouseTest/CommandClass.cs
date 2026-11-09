using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Input;
using System.Windows.Markup;
using static MouseTest.MainWindow;

namespace MouseTest
{
    public class CommandClass
    {
        public byte[] MAC_ADDR = new byte[7];


        /// <summary>
        /// 写MES 2.4G
        /// </summary>
        public (bool res, byte[] buffer) SetMes(string mesText)
        {
            //写条码
            byte[] mes = new byte[mesText.Trim().Length];
            for (int i = 0; i < mesText.Length; i++)
            {
                mes[i] = Convert.ToByte(mesText[i]);
            }

            byte[] hidOutputReport = new byte[65];
            hidOutputReport[0] = 0;
            hidOutputReport[1] = 0xFF;
            hidOutputReport[2] = 0x03;
            hidOutputReport[3] = mes[0];
            hidOutputReport[4] = mes[1];
            hidOutputReport[5] = mes[2];
            hidOutputReport[6] = mes[3];
            hidOutputReport[7] = mes[4];
            hidOutputReport[8] = mes[5];
            hidOutputReport[9] = mes[6];
            hidOutputReport[10] = mes[7];
            hidOutputReport[11] = mes[8];
            hidOutputReport[12] = mes[9];
            hidOutputReport[13] = mes[10];
            hidOutputReport[14] = mes[11];
            hidOutputReport[15] = mes[12];
            hidOutputReport[16] = mes[13];
            hidOutputReport[17] = mes[14];
            hidOutputReport[18] = mes[15];
            hidOutputReport[19] = mes[16];
            hidOutputReport[20] = mes[17];
            hidOutputReport[21] = mes[18];

            MyHID.SendDevice64(hidOutputReport, 1);
            return (true, hidOutputReport);
        }

        /// <summary>
        /// 读MES 2.4G
        /// </summary>
        public (bool res, byte[] buffer) GetMes()
        {
            byte[] hidOutputReport = new byte[65];
            hidOutputReport[0] = 0;
            hidOutputReport[1] = 0xFF;
            hidOutputReport[2] = 0x04;

            MyHID.SendDevice64(hidOutputReport, 1);
            return (true, hidOutputReport);
        }


        /// <summary>
        ///  2.4G Pair
        /// </summary>
        public (bool res, byte[] buffer) Pair()
        {
            byte[] hidOutputReport = new byte[65];
            hidOutputReport[0] = 0;
            hidOutputReport[1] = 0xFF;
            hidOutputReport[2] = 0x10;

            MyHID.SendDevice64(hidOutputReport, 1);
            return (true, hidOutputReport);
        }



        /// <summary>
        /// 读电量 2.4G
        /// </summary>
        public (bool res, byte[] buffer) GetBattery(byte id, byte checkSum)
        {
            byte[] hidOutputReport = new byte[65];
            hidOutputReport[0] = 0;
            hidOutputReport[1] = id;
            hidOutputReport[2] = 0;
            hidOutputReport[3] = 0x08;
            hidOutputReport[64] = checkSum;

            MyHID.SendDevice64(hidOutputReport, 1);
            return (true, hidOutputReport);
        }



        /// <summary>
        /// 写MES 有线
        /// </summary>
        public (bool res, byte[] buffer) SetMesUSB(string mesText)
        {
            //写条码
            byte[] mes = new byte[mesText.Length];
            for (int i = 0; i < mesText.Length; i++)
            {
                mes[i] = Convert.ToByte(mesText[i]);
            }

            byte[] hidOutputReport = new byte[65];
            hidOutputReport[0] = 0;
            hidOutputReport[1] = 0xFF;
            hidOutputReport[2] = 0x04;
            hidOutputReport[3] = mes[0];
            hidOutputReport[4] = mes[1];
            hidOutputReport[5] = mes[2];
            hidOutputReport[6] = mes[3];
            hidOutputReport[7] = mes[4];
            hidOutputReport[8] = mes[5];
            hidOutputReport[9] = mes[6];
            hidOutputReport[10] = mes[7];
            hidOutputReport[11] = mes[8];
            hidOutputReport[12] = mes[9];
            hidOutputReport[13] = mes[10];
            hidOutputReport[14] = mes[11];
            hidOutputReport[15] = mes[12];
            hidOutputReport[16] = mes[13];
            hidOutputReport[17] = mes[14];
            hidOutputReport[18] = mes[15];
            hidOutputReport[19] = mes[16];
            hidOutputReport[20] = mes[17];
            hidOutputReport[21] = mes[18];

            MyHID.SendDevice64(hidOutputReport, 1);
            return (true, hidOutputReport);
        }


        /// <summary>
        /// 有线读条码
        /// </summary>
        public (bool res, byte[] buffer) GetMesUSB()
        {
            byte[] hidOutputReport = new byte[65];
            hidOutputReport[0] = 0;
            hidOutputReport[1] = 0xFF;
            hidOutputReport[2] = 0x05;

            MyHID.SendDevice64(hidOutputReport, 1);
            return (true, hidOutputReport);
        }


        /// <summary>
        /// 有线读颜色
        /// </summary>
        public (bool res, byte[] buffer) GetMouseColor()
        {
            byte[] hidOutputReport = new byte[65];
            hidOutputReport[0] = 0;
            hidOutputReport[1] = 0xFF;
            hidOutputReport[2] = 0x0a;

            MyHID.SendDevice64(hidOutputReport, 1);
            return (true, hidOutputReport);
        }

        /// <summary>
        /// 有线写颜色
        /// </summary>
        public (bool res, byte[] buffer) SetMouseColor(byte colorId)
        {
            byte[] hidOutputReport = new byte[65];
            hidOutputReport[0] = 0;
            hidOutputReport[1] = 0xFF;
            hidOutputReport[2] = 0x09;
            hidOutputReport[3] = colorId;

            MyHID.SendDevice64(hidOutputReport, 1);
            return (true, hidOutputReport);
        }


        /// <summary>
        /// 进入测试模式 有线
        /// </summary>
        public (bool res, byte[] buffer) SetMesUsbTest()
        {
            byte[] hidOutputReport = new byte[65];
            hidOutputReport[0] = 0;
            hidOutputReport[1] = 0xFF;
            hidOutputReport[2] = 0;

            MyHID.SendDevice64(hidOutputReport, 1);
            return (true, hidOutputReport);
        }


        /// <summary>
        /// 退出测试模式 有线
        /// </summary>
        public (bool res, byte[] buffer) OffUsbTest()
        {
            byte[] hidOutputReport = new byte[65];
            hidOutputReport[0] = 0;
            hidOutputReport[1] = 0xFF;
            hidOutputReport[2] = 1;

            MyHID.SendDevice64(hidOutputReport, 1);
            return (true, hidOutputReport);
        }



        /// <summary>
        /// 0x01:读dongle id，0X02:读mouse id
        /// </summary>
        public (bool res, byte[] buffer) GetMid(byte id)
        {
            byte[] hidOutputReport = new byte[65];
            hidOutputReport[0] = 0;
            hidOutputReport[1] = 0xFF;
            hidOutputReport[2] = id;

            MyHID.SendDevice64(hidOutputReport, 1);
            return (true, hidOutputReport);
        }


        /// <summary>
        /// 读电量
        /// </summary>
        public (bool res, byte[] buffer) CheckPower()
        {
            byte[] hidOutputReport = new byte[65];
            hidOutputReport[0] = 0;
            hidOutputReport[1] = 0xFF;
            hidOutputReport[2] = 0x08;

            MyHID.SendDevice64(hidOutputReport, 1);
            return (true, hidOutputReport);
        }


        /// <summary>
        /// adc校准
        /// </summary>
        public (bool res, byte[] buffer) CheckADC()
        {
            byte[] hidOutputReport = new byte[65];
            hidOutputReport[0] = 0;
            hidOutputReport[1] = 0xFF;
            hidOutputReport[2] = 0x12;

            MyHID.SendDevice64(hidOutputReport, 1);
            return (true, hidOutputReport);
        }


        /// <summary>
        /// 获取MAC地址 
        /// </summary>
        public bool GetMAC()
        {
            byte[] hidOutputReport = new byte[64];
            hidOutputReport[0] = 0x06;
            hidOutputReport[1] = 0x04;
            MyHID.SendDevice64(hidOutputReport, 3);//直接get
            bool a = false;
            for (int i = 5; i < 12; i++)
            {
                if (hidOutputReport[i] != 0)
                {
                    a = true;
                }
            }
            if (!a || hidOutputReport[0] != 0x06 || hidOutputReport[1] != 0x04)
            {
                Thread.Sleep(50);
                return false;

            }
            for (int i = 5; i < 12; i++)
            {
                MAC_ADDR[i - 5] = hidOutputReport[i];
            }
            Thread.Sleep(50);
            return true;

        }


        /// <summary>
        /// start scan 开始扫描 
        /// </summary>
        public (bool res, byte[] buffer) StartScan()
        {
            byte[] hidOutputReport = new byte[65];
            hidOutputReport[0] = 0;
            hidOutputReport[1] = 0xff;
            hidOutputReport[2] = 0x0b;

            MyHID.SendDevice64(hidOutputReport, 2);
            Thread.Sleep(50);

            // 尝试最多10次
            for (int attempt = 0; attempt < 10; attempt++)
            {
                MyHID.SendDevice64(hidOutputReport, 3);
                Thread.Sleep(50);

                // 检查响应是否符合要求（根据StartScanAck方法中的成功条件）
                if (hidOutputReport[4] == 0x03)
                {
                    for (int i = 5; i < 12; i++)
                    {
                        MAC_ADDR[i - 5] = hidOutputReport[i];
                    }
                    return (true, hidOutputReport);
                }

                // 如果不是最后一次尝试，重置缓冲区
                if (attempt < 9)
                {
                    Array.Clear(hidOutputReport, 0, hidOutputReport.Length);
                    hidOutputReport[0] = 0x06;
                    hidOutputReport[1] = 0x04;
                    hidOutputReport[2] = 0x01;
                    hidOutputReport[4] = 0x01;
                }
            }

            // 3次尝试后仍未成功
            return (false, hidOutputReport);
        }


        /// <summary>
        /// 进入配对
        /// </summary>
        public (bool res, byte[] buffer) Pairing()
        {
            byte[] hidOutputReport = new byte[65];
            hidOutputReport[0] = 0;
            hidOutputReport[1] = 0xFF;
            hidOutputReport[2] = 0x0B;


            //for (int i = 0; i < MAC_ADDR.Length - 1; i++)
            //{
            //    hidOutputReport[5 + i] = MAC_ADDR[i];
            //}


            MyHID.SendDevice64(hidOutputReport, 1);
            return (true, hidOutputReport);
        }


        /// <summary>
        /// dongle上电 先get feature 20次，间隔5ms，这20次的数据不做处理，用于清空fifo
        /// </summary>
        public bool CleanTheBuffer()
        {
            try
            {
                byte[] hidOutputReport = new byte[64];

                int times = 0;
                while (times < 20)
                {
                    hidOutputReport[0] = 0x06;
                    hidOutputReport[1] = 0x04;
                    MyHID.SendDevice64(hidOutputReport, 3);
                    Thread.Sleep(5);
                    times++;

                }

                return true; // 未找到完整序列
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                return false;
            }
        }

        /// <summary>
        /// 切换白色通道
        /// </summary>
        /// <returns></returns>
        public bool ChangeChannel()
        {
            byte[] hidOutputReport = new byte[64];
            hidOutputReport[0] = 0x06;
            hidOutputReport[1] = 0x05;
            hidOutputReport[2] = 0x08;
            hidOutputReport[4] = 0x01;
            hidOutputReport[11] = 0xf6;

            for (int i = 0; i < MAC_ADDR.Length - 1; i++)
            {
                hidOutputReport[5 + i] = MAC_ADDR[i];
            }

            return MyHID.SendDevice64(hidOutputReport, 2);
        }

        /// <summary>
        /// 灯色 
        /// </summary>
        public bool Led_Set(string color)
        {
            try
            {
                byte[] hidOutputReport = new byte[65];
                hidOutputReport[0] = 0;
                hidOutputReport[1] = 0xff;
                //hidOutputReport[2] = 0x03;
                switch (color)
                {
                    case "red":
                        hidOutputReport[2] = 0x07;
                        break;
                    case "green":
                        hidOutputReport[2] = 0x08;
                        break;
                    case "blue":
                        hidOutputReport[2] = 0x09;
                        break;
                        //case "close":
                        //    hidOutputReport[2] = 0x00;
                        //    break;
                }
                MyHID.SendDevice64(hidOutputReport, 1);
                Thread.Sleep(50);
                return true;

            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                throw;
            }

        }


        /// <summary>
        /// 2.4g进入suspend，睡眠电流 
        /// </summary>
        public bool Suspend()
        {
            byte[] hidOutputReport = new byte[65];
            hidOutputReport[0] = 0;
            hidOutputReport[1] = 0xff;
            hidOutputReport[2] = 0x05;
            return MyHID.SendDevice64(hidOutputReport, 1);
        }

        /// <summary>
        /// 有线进入suspend，睡眠电流 
        /// </summary>
        public bool SuspendUsb()
        {
            byte[] hidOutputReport = new byte[65];
            hidOutputReport[0] = 0;
            hidOutputReport[1] = 0xff;
            hidOutputReport[2] = 0x06;
            return MyHID.SendDevice64(hidOutputReport, 1);
        }



        /// <summary>
        /// 恢复默认、清除快连配对信息
        /// </summary>
        /// <returns></returns>
        public bool SetDefault()
        {
            byte[] hidOutputReport = new byte[65];
            hidOutputReport[0] = 0;
            hidOutputReport[1] = 0xff;
            hidOutputReport[2] = 0x07;
            return MyHID.SendDevice64(hidOutputReport, 1);
        }


        /// <summary>
        /// 恢复默认、清除快连配对信息
        /// </summary>
        /// <returns></returns>
        public bool SetClearBLE()
        {
            byte[] hidOutputReport = new byte[65];
            hidOutputReport[0] = 0;
            hidOutputReport[1] = 0xff;
            hidOutputReport[2] = 0x15;
            return MyHID.SendDevice64(hidOutputReport, 1);
        }


        /// <summary>
        /// 切换到level 2
        /// </summary>
        /// <returns></returns>
        public bool ChangeLevel2()
        {
            byte[] hidOutputReport = new byte[64];
            hidOutputReport[0] = 0x06;
            hidOutputReport[1] = 0x04;
            hidOutputReport[2] = 0x04;
            hidOutputReport[4] = 0x08;
            hidOutputReport[5] = 0x02;
            return MyHID.SendDevice64(hidOutputReport, 2);
        }


        public byte[] ReturnMAC_ADDR()
        {
            return MAC_ADDR;
        }

        /// <summary>
        /// 获取自检值
        /// </summary>
        /// <returns></returns>
        internal bool CheckSelf()
        {
            byte[] hidOutputReport = new byte[65];
            hidOutputReport[0] = 0;
            hidOutputReport[1] = 0xff;
            hidOutputReport[2] = 0x0f;
            return MyHID.SendDevice64(hidOutputReport, 1);
        }

        /// <summary>
        /// 校准增益值 cali_count是上位机发给鼠标的 校准的次数，现定义为0x96（150次）。开始校准时，byte2写0xD2。在结束校准的时候，byte2写0xFF，鼠标收到这个之后认为校准结束，然后上位机发退出测试模式的命令。鼠标进行保存的操作，延迟300ms后显示PASS或FAIL。
        /// </summary>
        /// <returns></returns>
        internal bool CheckGain()
        {
            byte[] hidOutputReport = new byte[65];
            hidOutputReport[0] = 0;
            hidOutputReport[1] = 0xff;
            hidOutputReport[2] = 0x11;
            hidOutputReport[3] = 0xd2;
            hidOutputReport[4] = 0x96;
            return MyHID.SendDevice64(hidOutputReport, 1);
        }

        /// <summary>
        /// 校准结束保存
        /// </summary>
        /// <returns></returns>
        internal bool CheckGainSave()
        {
            byte[] hidOutputReport = new byte[65];
            hidOutputReport[0] = 0;
            hidOutputReport[1] = 0xff;
            hidOutputReport[2] = 0x11;
            hidOutputReport[3] = 0xff;
            return MyHID.SendDevice64(hidOutputReport, 1);
        }


        /// <summary>
        /// 获取rawdata
        /// </summary>
        /// <returns></returns>
        internal bool GetRawdata()
        {
            byte[] hidOutputReport = new byte[65];
            hidOutputReport[0] = 0;
            hidOutputReport[1] = 0xff;
            hidOutputReport[2] = 0x0b;
            return MyHID.SendDevice64(hidOutputReport, 1);
        }


        /// <summary>
        /// 获取rawdata
        /// </summary>
        /// <returns></returns>
        internal bool CheckRawdata()
        {
            byte[] hidOutputReport = new byte[65];
            hidOutputReport[0] = 0;
            hidOutputReport[1] = 0xff;
            hidOutputReport[2] = 0x0b;
            return MyHID.SendDevice64(hidOutputReport, 1);
        }

        /// <summary>
        /// 获取顶部top baseline
        /// </summary>
        /// <returns></returns>
        internal bool CheckTop()
        {
            byte[] hidOutputReport = new byte[65];
            hidOutputReport[0] = 0;
            hidOutputReport[1] = 0xff;
            hidOutputReport[2] = 0x0d;
            return MyHID.SendDevice64(hidOutputReport, 1);
        }

        /// <summary>
        /// 获取底部bottom baseline
        /// </summary>
        /// <returns></returns>
        internal bool CheckBottom()
        {
            byte[] hidOutputReport = new byte[65];
            hidOutputReport[0] = 0;
            hidOutputReport[1] = 0xff;
            hidOutputReport[2] = 0x0e;
            return MyHID.SendDevice64(hidOutputReport, 1);
        }


        /// <summary>
        /// 获取底部bottom baseline
        /// </summary>
        /// <returns></returns>
        internal bool GetBottomTop()
        {
            byte[] hidOutputReport = new byte[65];
            hidOutputReport[0] = 0;
            hidOutputReport[1] = 0xff;
            hidOutputReport[2] = 0x0b;
            return MyHID.SendDevice64(hidOutputReport, 1);
        }


        /// <summary>
        /// 设置RT
        /// </summary>
        /// <param name="bmEnableMask"></param>
        /// <param name="apValueH"></param>
        /// <param name="apValueL"></param>
        /// <param name="rtPressH"></param>
        /// <param name="rtPressL"></param>
        /// <param name="rtReleaseH"></param>
        /// <param name="rtReleaseL"></param>
        /// <exception cref="NotImplementedException"></exception>
        internal void SetRT(byte key, byte bmEnableMask, byte apValueH, byte apValueL, byte rtPressH, byte rtPressL, byte rtReleaseH, byte rtReleaseL )
        {
            //byte checkSum = 0;

            byte[] hidOutputReport = new byte[65];
            hidOutputReport[0] = 0;
            hidOutputReport[1] = 0x55;
            hidOutputReport[2] = 0x02;
            hidOutputReport[3] = 0x01;
            //hidOutputReport[4] = checkSum;//checkSum
            hidOutputReport[5] = 0x0c;
            hidOutputReport[6] = 0x00;
            hidOutputReport[7] = 0x02;
            hidOutputReport[8] = 0x03;
            hidOutputReport[9] = 0x01;
            hidOutputReport[10] = key;
            hidOutputReport[11] = 0;
            hidOutputReport[12] = bmEnableMask;
            hidOutputReport[13] = apValueL;
            hidOutputReport[14] = apValueH;
            hidOutputReport[15] = rtPressL;
            hidOutputReport[16] = rtPressH;
            hidOutputReport[17] = rtReleaseL;
            hidOutputReport[18] = rtReleaseH;

            hidOutputReport[4] = CalculateChecksum(hidOutputReport);//checkSum
            Thread.Sleep(50);

            MyHID.SendDevice64(hidOutputReport, 1);

        }

        /// <summary>
        /// 计算校验和：包含 Byte1、Byte2 以及 Byte4 及之后的所有字节，排除 Byte0 和 Byte3。
        /// </summary>
        /// <param name="data">字节数组</param>
        /// <returns>校验和（低8位，自动截断）</returns>
        public static byte CalculateChecksum(byte[] data)
        {
            if (data == null || data.Length == 0)
                return 0;

            int sum = 0;

            for (int i = 0; i < data.Length; i++)
            {
                // 跳过 Byte0 (索引0) 和 Byte3 (索引3)
                if (i == 1 || i == 4)
                    continue;

                sum += data[i];
            }

            // 返回低8位作为校验和（等效于模256）
            return (byte)sum;
        }

        internal void SetVoltage(byte key, byte bmEnableMask, byte coilTurnOnTimeL, byte coilTurnOnTimeH)
        {
            byte[] hidOutputReport = new byte[65];
            hidOutputReport[0] = 0;
            hidOutputReport[1] = 0x55;
            hidOutputReport[2] = 0x02;
            hidOutputReport[3] = 0x01;
            //hidOutputReport[4] = checkSum;//checkSum
            hidOutputReport[5] = 0x08;
            hidOutputReport[6] = 0x00;
            hidOutputReport[7] = 0x02;
            hidOutputReport[8] = 0x03;
            hidOutputReport[9] = 0x02;
            hidOutputReport[10] = key;
            hidOutputReport[11] = 0;
            hidOutputReport[12] = bmEnableMask;
            hidOutputReport[13] = coilTurnOnTimeL;
            hidOutputReport[14] = coilTurnOnTimeH;
         

            hidOutputReport[4] = CalculateChecksum(hidOutputReport);//checkSum
            Thread.Sleep(50);

            MyHID.SendDevice64(hidOutputReport, 1);
        }

        /// <summary>
        /// 设置释放振感（release coil）配置
        /// </summary>
        internal void SetReleaseVoltage(byte key, byte bmEnableMask, byte coilTurnOnTimeL, byte coilTurnOnTimeH)
        {
            byte[] hidOutputReport = new byte[65];
            hidOutputReport[0] = 0;
            hidOutputReport[1] = 0x55;
            hidOutputReport[2] = 0x02;
            hidOutputReport[3] = 0x01;
            hidOutputReport[5] = 0x08;
            hidOutputReport[6] = 0x00;
            hidOutputReport[7] = 0x02;
            hidOutputReport[8] = 0x03;
            hidOutputReport[9] = 0x03;
            hidOutputReport[10] = key;
            hidOutputReport[11] = 0;
            hidOutputReport[12] = bmEnableMask;
            hidOutputReport[13] = coilTurnOnTimeL;
            hidOutputReport[14] = coilTurnOnTimeH;

            hidOutputReport[4] = CalculateChecksum(hidOutputReport);
            Thread.Sleep(50);

            MyHID.SendDevice64(hidOutputReport, 1);
        }

        /// <summary>
        /// 设置全局驱动方式：bDriveMode 0=默认正常模式 1=静音模式（按下/释放共用）
        /// </summary>
        internal void SetDriveMode(byte bDriveMode)
        {
            byte[] hidOutputReport = new byte[65];
            hidOutputReport[0] = 0;
            hidOutputReport[1] = 0x55;
            hidOutputReport[2] = 0x02;
            hidOutputReport[3] = 0x01;
            hidOutputReport[5] = 0x05;
            hidOutputReport[6] = 0x00;
            hidOutputReport[7] = 0x02;
            hidOutputReport[8] = 0x03;
            hidOutputReport[9] = 0x06;
            hidOutputReport[10] = 0xFF;
            hidOutputReport[11] = bDriveMode;

            hidOutputReport[4] = CalculateChecksum(hidOutputReport);
            Thread.Sleep(50);

            MyHID.SendDevice64(hidOutputReport, 1);
        }

        /// <summary>
        /// 读取释放线圈配置（与 set 同命令码 0x03，get 标志 byte[8]=0x02）：
        /// 回复 byte[12]=使能标志，byte[13..14]=释放强度
        /// </summary>
        internal void GetReleaseCoil(byte key)
        {
            byte[] hidOutputReport = new byte[65];
            hidOutputReport[0] = 0;
            hidOutputReport[1] = 0x55;
            hidOutputReport[2] = 0x02;
            hidOutputReport[3] = 0x01;
            hidOutputReport[5] = 0x04;
            hidOutputReport[6] = 0x00;
            hidOutputReport[7] = 0x02;
            hidOutputReport[8] = 0x02;
            hidOutputReport[9] = 0x03;
            hidOutputReport[10] = key;

            hidOutputReport[4] = CalculateChecksum(hidOutputReport);//checkSum
            Thread.Sleep(50);

            MyHID.SendDevice64(hidOutputReport, 1);
        }

        /// <summary>
        /// 读取全局驱动方式（一包回复：bDriveMode 0=默认正常模式 1=静音模式）
        /// </summary>
        internal void GetDriveMode()
        {
            byte[] hidOutputReport = new byte[65];
            hidOutputReport[0] = 0;
            hidOutputReport[1] = 0x55;
            hidOutputReport[2] = 0x02;
            hidOutputReport[3] = 0x01;
            hidOutputReport[5] = 0x04;
            hidOutputReport[6] = 0x00;
            hidOutputReport[7] = 0x02;
            hidOutputReport[8] = 0x02;
            hidOutputReport[9] = 0x06;
            hidOutputReport[10] = 0xFF;

            hidOutputReport[4] = CalculateChecksum(hidOutputReport);//checkSum
            Thread.Sleep(50);

            MyHID.SendDevice64(hidOutputReport, 1);
        }

        /// <summary>
        /// 设置全局线圈 PWM 表：startIndex 0x00/0x32（0/50），data 固定 50 字节
        /// </summary>
        internal void SetPwm(byte startIndex, byte[] data)
        {
            byte[] hidOutputReport = new byte[65];
            hidOutputReport[0] = 0;
            hidOutputReport[1] = 0x55;
            hidOutputReport[2] = 0x02;
            hidOutputReport[3] = 0x01;
            hidOutputReport[5] = 0x37;
            hidOutputReport[6] = 0x00;
            hidOutputReport[7] = 0x02;
            hidOutputReport[8] = 0x03;
            hidOutputReport[9] = 0x07;
            hidOutputReport[10] = 0xFF;
            hidOutputReport[11] = startIndex;
            Array.Copy(data, 0, hidOutputReport, 12, 50);

            hidOutputReport[4] = CalculateChecksum(hidOutputReport);
            Thread.Sleep(50);

            MyHID.SendDevice64(hidOutputReport, 1);
        }

        /// <summary>
        /// 读取全局线圈 PWM 表：startIndex 0x00/0x32，回复 50 字节数据
        /// </summary>
        internal void GetPwm(byte startIndex)
        {
            byte[] hidOutputReport = new byte[65];
            hidOutputReport[0] = 0;
            hidOutputReport[1] = 0x55;
            hidOutputReport[2] = 0x02;
            hidOutputReport[3] = 0x01;
            hidOutputReport[5] = 0x05;
            hidOutputReport[6] = 0x00;
            hidOutputReport[7] = 0x02;
            hidOutputReport[8] = 0x02;
            hidOutputReport[9] = 0x07;
            hidOutputReport[10] = 0xFF;
            hidOutputReport[11] = startIndex;

            hidOutputReport[4] = CalculateChecksum(hidOutputReport);//checkSum
            Thread.Sleep(50);

            MyHID.SendDevice64(hidOutputReport, 1);
        }

        internal void GetHall(byte info)
        {
            byte[] hidOutputReport = new byte[65];
            hidOutputReport[0] = 0;
            hidOutputReport[1] = 0x55;
            hidOutputReport[2] = 0x02;
            hidOutputReport[3] = 0x01;
            //hidOutputReport[4] = checkSum;//checkSum
            hidOutputReport[5] = 0x04;
            hidOutputReport[6] = 0x00;
            hidOutputReport[7] = 0x02;
            hidOutputReport[8] = 0x01;
            hidOutputReport[9] = 0x03;
            hidOutputReport[10] = info;

            hidOutputReport[4] = CalculateChecksum(hidOutputReport);//checkSum
            Thread.Sleep(50);

            MyHID.SendDevice64(hidOutputReport, 1);
        }

        internal void GetRT(byte v)
        {
            byte[] hidOutputReport = new byte[65];
            hidOutputReport[0] = 0;
            hidOutputReport[1] = 0x55;
            hidOutputReport[2] = 0x02;
            hidOutputReport[3] = 0x01;
            //hidOutputReport[4] = checkSum;//checkSum
            hidOutputReport[5] = 0x04;
            hidOutputReport[6] = 0x00;
            hidOutputReport[7] = 0x02;
            hidOutputReport[8] = 0x02;
            hidOutputReport[9] = 0x01;
            hidOutputReport[10] = v;

            hidOutputReport[4] = CalculateChecksum(hidOutputReport);//checkSum
            Thread.Sleep(50);

            MyHID.SendDevice64(hidOutputReport, 1);
        }

        internal void GetCoil(byte v)
        {
            byte[] hidOutputReport = new byte[65];
            hidOutputReport[0] = 0;
            hidOutputReport[1] = 0x55;
            hidOutputReport[2] = 0x02;
            hidOutputReport[3] = 0x01;
            //hidOutputReport[4] = checkSum;//checkSum
            hidOutputReport[5] = 0x04;
            hidOutputReport[6] = 0x00;
            hidOutputReport[7] = 0x02;
            hidOutputReport[8] = 0x02;
            hidOutputReport[9] = 0x02;
            hidOutputReport[10] = v;

            hidOutputReport[4] = CalculateChecksum(hidOutputReport);//checkSum
            Thread.Sleep(50);

            MyHID.SendDevice64(hidOutputReport, 1);
        }

        internal void GetCoilDebug(byte v)
        {
            byte[] hidOutputReport = new byte[65];
            hidOutputReport[0] = 0;
            hidOutputReport[1] = 0x55;
            hidOutputReport[2] = 0x02;
            hidOutputReport[3] = 0x01;
            hidOutputReport[5] = 0x04;
            hidOutputReport[6] = 0x00;
            hidOutputReport[7] = 0x02;
            hidOutputReport[8] = 0x05;
            hidOutputReport[9] = 0x01;
            hidOutputReport[10] = v;

            hidOutputReport[4] = CalculateChecksum(hidOutputReport);//checkSum
            Thread.Sleep(50);

            MyHID.SendDevice64(hidOutputReport, 1);
        }

        internal void SetCoilDebug(byte key, byte bDynOnTimeEn, byte bAutoVibrationEn, byte periodL, byte periodH)
        {
            byte[] hidOutputReport = new byte[65];
            hidOutputReport[0] = 0;
            hidOutputReport[1] = 0x55;
            hidOutputReport[2] = 0x02;
            hidOutputReport[3] = 0x01;
            hidOutputReport[5] = 0x08;
            hidOutputReport[6] = 0x00;
            hidOutputReport[7] = 0x02;
            hidOutputReport[8] = 0x06;
            hidOutputReport[9] = 0x01;
            hidOutputReport[10] = key;
            hidOutputReport[11] = bDynOnTimeEn;
            hidOutputReport[12] = bAutoVibrationEn;
            hidOutputReport[13] = periodL;
            hidOutputReport[14] = periodH;

            hidOutputReport[4] = CalculateChecksum(hidOutputReport);//checkSum
            Thread.Sleep(50);

            MyHID.SendDevice64(hidOutputReport, 1);
        }

        internal void GetHallRaw(byte v)
        {
            byte[] hidOutputReport = new byte[65];
            hidOutputReport[0] = 0;
            hidOutputReport[1] = 0x55;
            hidOutputReport[2] = 0x02;
            hidOutputReport[3] = 0x01;
            //hidOutputReport[4] = checkSum;//checkSum
            hidOutputReport[5] = 0x04;
            hidOutputReport[6] = 0x00;
            hidOutputReport[7] = 0x02;
            hidOutputReport[8] = 0x01;
            hidOutputReport[9] = 0x01;
            hidOutputReport[10] = v;

            hidOutputReport[4] = CalculateChecksum(hidOutputReport);//checkSum
            Thread.Sleep(50);

            MyHID.SendDevice64(hidOutputReport, 1);
        }

        internal void GetHallSegment(byte v)
        {
            byte[] hidOutputReport = new byte[65];
            hidOutputReport[0] = 0;
            hidOutputReport[1] = 0x55;
            hidOutputReport[2] = 0x02;
            hidOutputReport[3] = 0x01;
            //hidOutputReport[4] = checkSum;//checkSum
            hidOutputReport[5] = 0x04;
            hidOutputReport[6] = 0x00;
            hidOutputReport[7] = 0x02;
            hidOutputReport[8] = 0x01;
            hidOutputReport[9] = 0x02;
            hidOutputReport[10] = v;

            hidOutputReport[4] = CalculateChecksum(hidOutputReport);//checkSum
            Thread.Sleep(50);

            MyHID.SendDevice64(hidOutputReport, 1);
        }
    }
}
