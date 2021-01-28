using System;
using System.Windows;
using System.Windows.Threading;
using MouseKeyboardActivityMonitor.WinApi;
using MouseKeyboardActivityMonitor;
using System.Windows.Forms;

namespace RobotInterface
{
    public partial class MainWindow : Window
    {
        Robot robot;
        DispatcherTimer timer;
        private readonly KeyboardHookListener m_KeyboardHookManager;

        public MainWindow()
        {
            InitializeComponent();

            robot = new Robot();
            Message.MessageOk += robot.processDecodedMessage;

            timer = new DispatcherTimer();
            timer.Interval = new TimeSpan(0, 0, 0, 0, 100);
            timer.Tick += Timer_Tick;
            timer.Start();

            m_KeyboardHookManager = new KeyboardHookListener(new GlobalHooker());
            m_KeyboardHookManager.Enabled = true;
            m_KeyboardHookManager.KeyDown += M_KeyboardHookManager_KeyDown;
            m_KeyboardHookManager.KeyUp += M_KeyboardHookManager_KeyUp;

            
        }

        private void M_KeyboardHookManager_KeyUp(object sender, System.Windows.Forms.KeyEventArgs e)
        {
            
        }

        private void M_KeyboardHookManager_KeyDown(object sender, System.Windows.Forms.KeyEventArgs e)
        {
            
        }

        private void Timer_Tick(object sender, EventArgs e)
        {
            while (robot.byteListReceived.Count != 0)
                textbox_out.Text += "0x" + robot.byteListReceived.Dequeue().ToString("X2") + " ";

            if (robot.receive_text != "")
            {
                textbox_out.Text += robot.receive_text + "\n";
                robot.receive_text = "";
            }

            textbox_out.ScrollToEnd();

        }

        private void clear()
        {
            textbox_out.Text = "";
        }

        private void buttons_vit_Click(object sender, RoutedEventArgs e)
        {
            System.Windows.Controls.Button src = e.Source as System.Windows.Controls.Button;
            
            //robot.uartEncodeAndSendMsg(0x)
        }
    }
}
