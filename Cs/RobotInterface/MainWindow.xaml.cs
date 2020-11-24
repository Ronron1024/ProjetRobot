using System;
using System.Collections.Generic;
using System.Text;
using System.Windows;
using System.Windows.Input;
using System.IO.Ports;
using ExtendedSerialPort;
using System.Windows.Threading;
using System.Windows.Interop;
using MouseKeyboardActivityMonitor.WinApi;
using MouseKeyboardActivityMonitor;
using System.Windows.Forms;
using Utilities;

namespace RobotInterface
{
    /// <summary>
    /// Logique d'interaction pour MainWindow.xaml
    /// </summary>
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

        private void send(object sender, RoutedEventArgs e)
        {
            if (sender == button)
                robot.uartEncodeAndSendMsg(0x20, 1, new byte[1] { 5 });
            else
            {
                byte[] buff = Encoding.ASCII.GetBytes(textbox_in.Text);
                robot.uartEncodeAndSendMsg(0x80, (ushort)buff.Length, buff);
            }
        }

        private void clear()
        {
            textbox_out.Text = "";
        }

        private void textbox_in_KeyUp(object sender, System.Windows.Input.KeyEventArgs e)
        {
            switch (e.Key)
            {
                case Key.Enter:
                    send(textbox_in, null);
                    break;
                case Key.Escape:
                    clear();
                    break;
            }
        }

        private void checkbox_leds_Checked(object sender, RoutedEventArgs e)
        {
            if (sender == checkbox_led1)
                robot.uartEncodeAndSendMsg(0x20, 2, new byte[2] { 1, 1 });
            else if (sender == checkbox_led2)
                robot.uartEncodeAndSendMsg(0x20, 2, new byte[2] { 2, 1 });
            else if (sender == checkbox_led3)
                robot.uartEncodeAndSendMsg(0x20, 2, new byte[2] { 3, 1 });
        }
        private void checkbox_leds_Unchecked(object sender, RoutedEventArgs e)
        {
            if (sender == checkbox_led1)
                robot.uartEncodeAndSendMsg(0x20, 2, new byte[2] { 1, 0 });
            else if (sender == checkbox_led2)
                robot.uartEncodeAndSendMsg(0x20, 2, new byte[2] { 2, 0 });
            else if (sender == checkbox_led3)
                robot.uartEncodeAndSendMsg(0x20, 2, new byte[2] { 3, 0 });
        }
    }
}
