using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows;
using System.Windows.Forms;
using System.Windows.Media;

namespace WpfPanel
{
    /// <summary>
    /// WpfLinePanel.xaml 的交互逻辑
    /// </summary>
    public partial class WpfLinePanel : System.Windows.Controls.UserControl
    {
        public int LinecChangeColorCount = 0;
        private System.Drawing.Point endPoint;
        private List<System.Drawing.Point> ListPoint = new List<System.Drawing.Point>();
        private readonly System.Drawing.Pen PenBlack = new System.Drawing.Pen(System.Drawing.Color.Black, 2);
        private readonly System.Drawing.Pen PenWhite = new System.Drawing.Pen(System.Drawing.Color.White, 2);
        private readonly System.Drawing.Pen PenGreen = new System.Drawing.Pen(System.Drawing.Color.Green, 2);
        private readonly System.Drawing.Pen PenOrange = new System.Drawing.Pen(System.Drawing.Color.Orange, 2);

        private readonly System.Drawing.Pen PenRed = new System.Drawing.Pen(System.Drawing.Color.Red, 2);
        private readonly System.Drawing.Pen PenX1 = new System.Drawing.Pen(System.Drawing.Color.FromArgb(0, 255, 255), 2);
        private readonly System.Drawing.Pen PenX2 = new System.Drawing.Pen(System.Drawing.Color.FromArgb(255, 0, 255), 2);
        private readonly System.Drawing.Pen PenDef = new System.Drawing.Pen(System.Drawing.Color.Black, 2);
        private System.Drawing.Point NewP = new System.Drawing.Point();


        public WpfLinePanel()
        {
            InitializeComponent();
            InitializeWinFormsPanel();
        }
        #region 颜色
        // 静态字段，用于存储绿色画刷
        private static readonly SolidColorBrush brushGreen = new SolidColorBrush(Colors.Green);
        public static SolidColorBrush BrushGreen
        {
            get { return brushGreen; }
        }
        // 静态字段，用于存储绿色画刷
        private static readonly SolidColorBrush brushRed = new SolidColorBrush(Colors.Red);
        public static SolidColorBrush BrushRed
        {
            get { return brushRed; }
        }
        // 静态字段，用于存储绿色画刷
        private static readonly SolidColorBrush brushWhite = new SolidColorBrush(Colors.White);
        public static SolidColorBrush BrushWhite
        {
            get { return brushWhite; }
        }

        private static readonly SolidColorBrush brushYellow = new SolidColorBrush(Colors.Yellow);
        public static SolidColorBrush BrushYellow
        {
            get { return brushYellow; }
        }

        private static readonly SolidColorBrush brushGray = new SolidColorBrush(Colors.Gray);
        public static SolidColorBrush BrushGray
        {
            get { return brushGray; }
        }

        private static readonly SolidColorBrush brushWhiteSmoke = new SolidColorBrush(Colors.WhiteSmoke);
        public static SolidColorBrush BrushWhiteSmoke
        {
            get { return brushWhiteSmoke; }
        }
        private static readonly SolidColorBrush brushBlack = new SolidColorBrush(Colors.Black);
        public static SolidColorBrush BrushBlack
        {
            get { return brushBlack; }
        }
        private static readonly SolidColorBrush brushOrange = new SolidColorBrush(Colors.DarkOrange);
        public static SolidColorBrush BrushOrange
        {
            get { return brushOrange; }
        }
        #endregion

        //属性--测试结果
        public string TestRueslt
        {
            get => (string)GetValue(TestRuesltProperty);
            set => SetValue(TestRuesltProperty, value);
        }
        //属性--测试结果
        public static readonly DependencyProperty TestRuesltProperty =
            DependencyProperty.Register(nameof(TestRueslt), typeof(string), typeof(WpfLinePanel), new PropertyMetadata(ChangeTestRueslt));

        public static bool Reset = false;//是否清除控件到默认状态

        /// <summary>
        /// 属性--测试结果
        /// </summary>
        /// <param name="obj"></param>
        /// <param name="r"></param>
        private static void ChangeTestRueslt(DependencyObject obj, DependencyPropertyChangedEventArgs r)
        {
            try
            {
                WpfLinePanel control = (WpfLinePanel)obj;
                switch ((string)r.NewValue)
                {
                    case "Pass":
                        control.Border.Background = brushGreen;
                        break;
                    case "Fail":
                        control.Border.Background = brushRed;
                        break;
                    case "NA":
                        Reset = true;
                        control.Border.Background = brushGray;
                        control.ListPoint.Clear();
                        control.winFormsPanel.Refresh();
                        break;
                    default:
                        break;
                }

            }
            catch (Exception e)
            {
                System.Windows.MessageBox.Show(e.ToString());
                throw;
            }
        }

        #region winform panel
        private void InitializeWinFormsPanel()
        {
            // 订阅鼠标事件
            winFormsPanel.MouseMove += WinFormsPanel_MouseMove;
            LinecChangeColorCount = int.Parse(LTB.Text);
        }



        private void WinFormsPanel_MouseMove(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            NewP.X = e.X; NewP.Y = e.Y;
            Graphics g = this.winFormsPanel.CreateGraphics();
            //g.DrawLine(new Pen(Color.Silver, 2), endPoint, new Point(e.X, e.Y));   
            if (endPoint.X != 0 && endPoint.Y != 0/* && IsVison*/)//ChkBoxConnect.Checked && 
            {
                switch (e.Button)
                {
                    case MouseButtons.None:
                        //g.DrawLine(PenWhite, endPoint, NewP);
                        //if (ListPoint.Count > LinecChangeColorCount)
                        //{
                        //    TestRueslt = "Pass";
                        //    g.DrawLine(PenDef, endPoint, NewP);
                        //    ListPoint.Remove(ListPoint[0]);
                        //}
                        //else
                        //{
                        //    g.DrawLine(PenWhite, endPoint, NewP);
                        //}
                        break;
                    case MouseButtons.Left:
                   
                        //g.DrawLine(new Pen(Color.FromArgb(0, 255, 0), 2), endPoint, new Point(e.X, e.Y));
                        g.DrawLine(PenGreen, endPoint, NewP);
                        break;
                    case MouseButtons.Right:
                        g.DrawLine(PenRed, endPoint, NewP);
                        break;
                    case MouseButtons.Middle:
                        g.DrawLine(PenOrange, endPoint, NewP);
                        break;
                    case MouseButtons.XButton1:
                        g.DrawLine(PenX1, endPoint, NewP);
                        break;
                    case MouseButtons.XButton2:
                        g.DrawLine(PenX2, endPoint, NewP);
                        break;
                    default:
                        g.DrawLine(PenDef, endPoint, NewP);
                        break;
                }
            }

            System.Drawing.Point nowPoint = new System.Drawing.Point
            {
                X = e.X,
                Y = e.Y
            };
            ListPoint.Add(nowPoint);

            if (ListPoint.Count > LinecChangeColorCount)
            {
                TestRueslt = "Pass";
                ListPoint.Remove(ListPoint[0]);
                g.DrawLine(PenBlack, ListPoint[0], ListPoint[1]);
            }


            endPoint.X = e.X;
            endPoint.Y = e.Y;
        }

        #endregion

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                LinecChangeColorCount = int.Parse(LTB.Text);
                ListPoint.Clear();
                winFormsPanel.Refresh();
                TestRueslt = "NA";
            }
            catch (Exception)
            {
                System.Windows.MessageBox.Show("请输入正确数字");
            }
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            winFormsPanel.Refresh();
        }
    }
}
