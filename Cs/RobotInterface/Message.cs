using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RobotInterface
{
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

    class MessageOkEventArgs
    {
        public ushort msg_decoded_func = 0;
        public ushort msg_decoded_payload_length = 0;
        public byte[] msg_decoded_payload;

        public MessageOkEventArgs(ushort _msg_decoded_func, ushort _msg_decoded_payload_length, byte[] _msg_decoded_payload)
        {
            msg_decoded_func = _msg_decoded_func;
            msg_decoded_payload_length = _msg_decoded_payload_length;
            msg_decoded_payload = _msg_decoded_payload;
    }
    }

    class Message
    {
        public static readonly byte EMPTY_TRAME_SIZE = 6;

        private static StateReception rcv_state = StateReception.Waiting;

        private static ushort msg_decoded_func = 0;
        private static ushort msg_decoded_payload_length = 0;
        private static byte[] msg_decoded_payload;
        private static int msg_decoded_payload_index = 0;

        public static event EventHandler<MessageOkEventArgs> MessageOk;

        public Message()
        {

        }

        public static void decodeMsg(byte c)
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
                        MessageOk?.Invoke(null, new MessageOkEventArgs(msg_decoded_func, msg_decoded_payload_length, msg_decoded_payload));
                        

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

        public static byte calcChecksum(ushort msg_func, ushort msg_payload_length, byte[] msg_payload)
        {
            byte ret = 0xFE;

            ret = (byte)(ret ^ msg_func);
            ret = (byte)(ret ^ msg_payload_length);

            for (int i = 0; i < msg_payload_length; i++)
                ret ^= msg_payload[i];

            return ret;
        }
    }
}
