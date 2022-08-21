using System;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace ChatHostExample
{
    
    public class DLLMain
    {
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void SAMP_ChatDelegate(string ChatEntered);

        [DllImport("ChatHostHandler.asi", CallingConvention = CallingConvention.Cdecl)]
        public static void SetChatCallback(SAMP_ChatDelegate aCallback)
        {
        }

        public static void DllMain()
        {
            try
            {
                SAMP_ChatDelegate SAMP_Chat_Thread = new SAMP_ChatDelegate(MainCheat);
                SetChatCallback(SAMP_Chat_Thread);

                while (true) { }
            }
            catch (Exception ex)
            {
               MessageBox.Show(ex.Message);
            }
        }

        public static void MainCheat(string ChatEntered)
        {
            if (ChatEntered == "/Hello")
                MessageBox.Show("Hi bro");
        }
    }

}
