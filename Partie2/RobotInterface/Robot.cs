using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RobotInterface
{
    class Robot
    {
        public Queue<byte> byteListReceived;
        public string receive_text;

        public byte[] ir_data;

        public byte direction = 2;

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
            ir_data = new byte[5] {0,0,0,0,0};
        }
    }
}
