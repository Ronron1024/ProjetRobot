using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Utilities;
using ExtendedSerialPort;
using System.IO.Ports;

namespace RobotInterface
{
    class Robot
    {
        public Queue<byte> byteListReceived;
        public string receive_text;

        public byte[] ir_data;

        public byte direction = 2;

        private ReliableSerialPort serialPort;

        // ROBOT STATES
        public static byte STATE_ATTENTE = 0;
        public static byte STATE_AVANCE = 2;
        public static byte STATE_TOURNE_GAUCHE = 4;
        public static byte STATE_TOURNE_DROITE = 6;
        public static byte STATE_TOURNE_SUR_PLACE_GAUCHE = 8;
        public static byte STATE_TOURNE_SUR_PLACE_DROITE = 10;
        public static byte STATE_SLOW = 14;
        public static byte STATE_FAST = 15;

        public Robot()
        {
            byteListReceived = new Queue<byte>();
            receive_text = "";
            ir_data = new byte[5] { 0, 0, 0, 0, 0 };

            serialPort = new ReliableSerialPort("COM23", 115200, Parity.None, 8, StopBits.One);
            serialPort.DataReceived += SerialPort_DataReceived;
            serialPort.Open();
        }

        private void SerialPort_DataReceived(object sender, DataReceivedArgs e)
        {
            for (int i = 0; i < e.Data.Length; i++)
            {
                //robot.byteListReceived.Enqueue(e.Data[i]);
                Message.decodeMsg(e.Data[i]);
            }
        }

        public void processDecodedMessage(object sender, MessageOkEventArgs e)
        {

            ushort func = e.msg_decoded_func;
            ushort payload_length = e.msg_decoded_payload_length;
            byte[] payload = e.msg_decoded_payload;

            switch (func)
            {
                case 0x30: // IR data
                    for (int i = 0; i < payload_length; i++)
                        ir_data[i] = payload[i];
                    break;

                case 0x80: // Text transmission
                    receive_text = System.Text.Encoding.Default.GetString(payload);
                    break;

                case 0x61:
                    string display = "";

                    byte[] timestamp_array = payload.GetRange(0, 4);
                    Array.Reverse(timestamp_array);
                    int timestamp = BitConverter.ToInt32(timestamp_array, 0);

                    byte[] xpos_array = payload.GetRange(4, 4);
                    float xpos = xpos_array.GetFloat();

                    byte[] ypos_array = payload.GetRange(8, 4);
                    float ypos = ypos_array.GetFloat();

                    byte[] angle_array = payload.GetRange(12, 4);
                    float angle = angle_array.GetFloat();

                    byte[] vit_lin_array = payload.GetRange(16, 4);
                    float vit_lin = vit_lin_array.GetFloat();

                    byte[] vit_ang_array = payload.GetRange(20, 4);
                    float vit_ang = vit_ang_array.GetFloat();

                    display = "time=" + timestamp + "\t\t\txpos=" + xpos + "\t\t\typos=" + ypos + "\t\t\tangle=" + angle + "\t\t\tvit_lin=" + vit_lin + "\t\t\tvit_angle=" + vit_ang;

                    receive_text = display;
                    break;

                default: // Unknow command
                    break;
            }
        }

        public void uartEncodeAndSendMsg(ushort msg_func, ushort msg_payload_length, byte[] msg_payload)
        {
            int i = 0, j = 0;
            byte[] msg = new byte[Message.EMPTY_TRAME_SIZE + msg_payload_length];

            msg[i++] = 0xFE;

            msg[i++] = (byte)(msg_func >> 8);
            msg[i++] = (byte)msg_func;

            msg[i++] = (byte)(msg_payload_length >> 8);
            msg[i++] = (byte)msg_payload_length;

            for (j = 0; j < msg_payload_length; j++)
                msg[i++] = msg_payload[j];

            msg[i++] = Message.calcChecksum(msg_func, msg_payload_length, msg_payload);

            serialPort.Write(msg, 0, msg.Length);
        }
    }
}
