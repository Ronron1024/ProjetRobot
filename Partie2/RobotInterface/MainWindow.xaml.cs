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

namespace RobotInterface
{
    /// <summary>
    /// Logique d'interaction pour MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {

        static readonly byte EMPTY_TRAME_SIZE = 6;

        public enum StateReception
        {
            Waiting,
            FunctionMSB,
            FunctionLSB,
            PayloadLengthMSB,
            PayloadLengthLSB,
            Payload,
            CheckSum
        }

        StateReception rcv_state = StateReception.Waiting;
        ushort msg_decoded_func = 0;
        ushort msg_decoded_payload_length = 0;
        byte[] msg_decoded_payload;
        int msg_decoded_payload_index = 0;

        Robot robot;
        ReliableSerialPort serialPort;
        DispatcherTimer timer;

        private readonly KeyboardHookListener m_KeyboardHookManager;

        public MainWindow()
        {
            InitializeComponent();

            robot = new Robot();

            serialPort = new ReliableSerialPort("COM19", 115200, Parity.None, 8, StopBits.One);
            serialPort.DataReceived += SerialPort_DataReceived;
            serialPort.Open();

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
            switch (e.KeyCode) // Utiliser des define
            {
                case Keys.Q:
                    if (robot.direction == 2)
                        uartEncodeAndSendMsg(0x50, 2, new byte[2] { Robot.STATE_ATTENTE, robot.direction });
                    else
                        uartEncodeAndSendMsg(0x50, 2, new byte[2] { Robot.STATE_AVANCE, robot.direction });
                    break;
                case Keys.D:
                    if (robot.direction == 2)
                        uartEncodeAndSendMsg(0x50, 2, new byte[2] { Robot.STATE_ATTENTE, robot.direction });
                    else
                        uartEncodeAndSendMsg(0x50, 2, new byte[2] { Robot.STATE_AVANCE, robot.direction });
                    break;
                case Keys.Z:
                    robot.direction = 2;
                    uartEncodeAndSendMsg(0x50, 2, new byte[2] { Robot.STATE_ATTENTE, robot.direction });
                    break;
                case Keys.S:
                    robot.direction = 2;
                    uartEncodeAndSendMsg(0x50, 2, new byte[2] { Robot.STATE_ATTENTE, robot.direction });
                    break;
                case Keys.LControlKey:
                    if (robot.direction == 2)
                        uartEncodeAndSendMsg(0x50, 2, new byte[2] { Robot.STATE_ATTENTE, robot.direction });
                    else
                        uartEncodeAndSendMsg(0x50, 2, new byte[2] { Robot.STATE_AVANCE, robot.direction });
                    break;
                case Keys.Space:
                    if (robot.direction == 2)
                        uartEncodeAndSendMsg(0x50, 2, new byte[2] { Robot.STATE_ATTENTE, robot.direction });
                    else
                        uartEncodeAndSendMsg(0x50, 2, new byte[2] { Robot.STATE_AVANCE, robot.direction });
                    break;
            }
        }

        private void M_KeyboardHookManager_KeyDown(object sender, System.Windows.Forms.KeyEventArgs e)
        {
            switch (e.KeyCode) // Utiliser des define
            {
                case Keys.Q:
                    if (robot.direction != 2)
                        uartEncodeAndSendMsg(0x50, 2, new byte[2] { Robot.STATE_TOURNE_GAUCHE, robot.direction });
                    else
                        uartEncodeAndSendMsg(0x50, 2, new byte[2] { Robot.STATE_TOURNE_SUR_PLACE_GAUCHE, robot.direction });
                    break;
                case Keys.D:
                    if (robot.direction != 2)
                        uartEncodeAndSendMsg(0x50, 2, new byte[2] { Robot.STATE_TOURNE_DROITE, robot.direction });
                    else
                        uartEncodeAndSendMsg(0x50, 2, new byte[2] { Robot.STATE_TOURNE_SUR_PLACE_DROITE, robot.direction });
                    break;
                case Keys.Z:
                    robot.direction = 1;
                    uartEncodeAndSendMsg(0x50, 2, new byte[2] { Robot.STATE_AVANCE, robot.direction });
                    break;
                case Keys.S:
                    robot.direction = 0;
                    uartEncodeAndSendMsg(0x50, 2, new byte[2] { Robot.STATE_AVANCE, robot.direction });
                    break;
                case Keys.LControlKey:
                    uartEncodeAndSendMsg(0x50, 2, new byte[2] { Robot.STATE_SLOW, robot.direction });
                    break;
                case Keys.Space:
                    uartEncodeAndSendMsg(0x50, 2, new byte[2] { Robot.STATE_FAST, robot.direction });
                    break;
            }
        }


        private byte calcChecksum(ushort msg_func, ushort msg_payload_length, byte[] msg_payload)
        {
            byte ret = 0xFE;

            ret = (byte)(ret ^ msg_func);
            ret = (byte)(ret ^ msg_payload_length);

            for (int i = 0; i < msg_payload_length; i++)
                ret ^= msg_payload[i];

            return ret;
        }

        // Créer une classe UART ?
        private void uartEncodeAndSendMsg(ushort msg_func, ushort msg_payload_length, byte[] msg_payload)
        {
            int i = 0, j = 0;
            byte[] msg = new byte[EMPTY_TRAME_SIZE + msg_payload_length];

            msg[i++] = 0xFE;

            msg[i++] = (byte)(msg_func >> 8);
            msg[i++] = (byte)msg_func;

            msg[i++] = (byte)(msg_payload_length >> 8);
            msg[i++] = (byte)msg_payload_length;

            for (j = 0; j < msg_payload_length; j++)
                msg[i++] = msg_payload[j];

            msg[i++] = calcChecksum(msg_func, msg_payload_length, msg_payload);

            serialPort.Write(msg, 0, msg.Length);
        }

        private void decodeMsg(byte c)
        {
            switch (rcv_state)
            {
                case StateReception.Waiting:
                    if (c == 0xFE)
                        rcv_state = StateReception.FunctionMSB;
                    break;

                case StateReception.FunctionMSB:
                    msg_decoded_func += c;
                    msg_decoded_func <<= 8;
                    rcv_state = StateReception.FunctionLSB;
                    break;

                case StateReception.FunctionLSB:
                    msg_decoded_func += c;
                    rcv_state = StateReception.PayloadLengthMSB;
                    break;

                case StateReception.PayloadLengthMSB:
                    msg_decoded_payload_length += c;
                    msg_decoded_payload_length <<= 8;
                    rcv_state = StateReception.PayloadLengthLSB;
                    break;

                case StateReception.PayloadLengthLSB:
                    msg_decoded_payload_length += c;
                    msg_decoded_payload = new byte[msg_decoded_payload_length];
                    if (msg_decoded_payload_length == 0)
                        rcv_state = StateReception.CheckSum;
                    else
                        rcv_state = StateReception.Payload;
                    break;

                case StateReception.Payload:
                    msg_decoded_payload[msg_decoded_payload_index++] = c;
                    if (msg_decoded_payload_index == msg_decoded_payload_length)
                        rcv_state = StateReception.CheckSum;
                    break;

                case StateReception.CheckSum: // Contrôle d'erreur à rajouter
                    if (calcChecksum(msg_decoded_func, msg_decoded_payload_length, msg_decoded_payload) == c)
                        processDecodedMessage(msg_decoded_func, msg_decoded_payload_length, msg_decoded_payload);
                    
                    msg_decoded_func = 0;
                    msg_decoded_payload_length = 0;
                    msg_decoded_payload = new byte[0];
                    msg_decoded_payload_index = 0;

                    rcv_state = StateReception.Waiting; 
                    break;

                default:
                    rcv_state = StateReception.Waiting;
                    break;
            }
        }

        private void processDecodedMessage(ushort func, ushort payload_length, byte[] payload)
        {
            switch (func)
            {
                case 0x30: // IR data
                    for (int i = 0; i < payload_length; i++)
                        robot.ir_data[i] = payload[i];
                    break;

                case 0x80: // Text transmission
                    robot.receive_text = System.Text.Encoding.Default.GetString(payload);
                    break;

                default: // Unknow command
                    break;
            }
        }

        private void SerialPort_DataReceived(object sender, DataReceivedArgs e)
        {
            for (int i = 0; i < e.Data.Length; i++)
            {
                //robot.byteListReceived.Enqueue(e.Data[i]);
                decodeMsg(e.Data[i]);
            }
        }

        private void Timer_Tick(object sender, EventArgs e)
        {
            while (robot.byteListReceived.Count != 0)
                textbox_out.Text += "0x" + robot.byteListReceived.Dequeue().ToString("X2") + " ";
            if (robot.receive_text != "")
            {
                textbox_out.Text += robot.receive_text;
                robot.receive_text = "";
            }

            // Collection de labels ?
            label_ir0.Content = robot.ir_data[0] + "cm";
            label_ir1.Content = robot.ir_data[1] + "cm";
            label_ir2.Content = robot.ir_data[2] + "cm";
            label_ir3.Content = robot.ir_data[3] + "cm";
            label_ir4.Content = robot.ir_data[4] + "cm";

        }

        private void send(object sender, RoutedEventArgs e)
        {
            if (sender == button)
                uartEncodeAndSendMsg(0x20, 1, new byte[1] { 5 });
            else
            {
                byte[] buff = Encoding.ASCII.GetBytes(textbox_in.Text);
                uartEncodeAndSendMsg(0x80, (ushort)buff.Length, buff);
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
                uartEncodeAndSendMsg(0x20, 2, new byte[2] { 1, 1 });
            else if (sender == checkbox_led2)
                uartEncodeAndSendMsg(0x20, 2, new byte[2] { 2, 1 });
            else if (sender == checkbox_led3)
                uartEncodeAndSendMsg(0x20, 2, new byte[2] { 3, 1 });
        }
        private void checkbox_leds_Unchecked(object sender, RoutedEventArgs e)
        {
            if (sender == checkbox_led1)
                uartEncodeAndSendMsg(0x20, 2, new byte[2] { 1, 0 });
            else if (sender == checkbox_led2)
                uartEncodeAndSendMsg(0x20, 2, new byte[2] { 2, 0 });
            else if (sender == checkbox_led3)
                uartEncodeAndSendMsg(0x20, 2, new byte[2] { 3, 0 });
        }
    }
}
