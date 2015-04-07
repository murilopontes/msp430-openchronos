using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;

namespace msp430f5509_cc1101_lab
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void comboBox_portas_DropDown(object sender, EventArgs e)
        {
            comboBox_portas.Items.Clear();
            foreach (string porta in SerialPort.GetPortNames())
            {
                comboBox_portas.Items.Add(porta);
            }


        }

        private void button_serial_Click(object sender, EventArgs e)
        {
            try
            {
                if (serialPort1.IsOpen)
                {
                    serialPort1.Close();
                    button_serial.Text = "conectar";
                }
                else
                {
                    serialPort1.PortName = (string)comboBox_portas.SelectedItem;
                    serialPort1.BaudRate = 115200;
                    serialPort1.Open();
                    button_serial.Text = "desconectar";

                }

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }



        #region strobes

        string strobe_to_txt(int reg) {
            if (reg == 0x30) return "sres";
            if (reg == 0x31) return "sfstxon";
            if (reg == 0x32) return "sxoff";
            if (reg == 0x33) return "scal";
            if (reg == 0x34) return "srx";
            if (reg == 0x35) return "stx";
            if (reg == 0x36) return "sidle";
            
            //if (reg == 0x37) return "sres";

            if (reg == 0x38) return "swor";
            if (reg == 0x39) return "spwd";
            if (reg == 0x3a) return "sfrx";
            if (reg == 0x3b) return "sftx";
            if (reg == 0x3c) return "sworrst";
            if (reg == 0x3d) return "snop";

            return "[error]";
        }


        private void button_sres_Click(object sender, EventArgs e)
        {
          //  cc1101_strobe(0x30);

        }

        private void button_sfstxon_Click(object sender, EventArgs e)
        {
            cc1101_strobe(0x31);

        }

        private void button_sxoff_Click(object sender, EventArgs e)
        {
            //cc1101_strobe(0x32);

        }

        private void button_scal_Click(object sender, EventArgs e)
        {
            cc1101_strobe(0x33);
        }

        private void button_srx_Click(object sender, EventArgs e)
        {
            cc1101_strobe(0x34);

        }

        private void button_stx_Click(object sender, EventArgs e)
        {
            cc1101_strobe(0x35);

        }

        private void button_sidle_Click(object sender, EventArgs e)
        {
            cc1101_strobe(0x36);

        }

        private void button_swor_Click(object sender, EventArgs e)
        {
            //cc1101_strobe(0x38);

        }

        private void button_spwd_Click(object sender, EventArgs e)
        {
            //cc1101_strobe(0x39);

        }

        private void button_sfrx_Click(object sender, EventArgs e)
        {
            cc1101_strobe(0x3a);

        }

        private void button_sftx_Click(object sender, EventArgs e)
        {
            cc1101_strobe(0x3b);

        }

        private void button_sworrst_Click(object sender, EventArgs e)
        {
            cc1101_strobe(0x3c);

        }

        private void button_snop_Click(object sender, EventArgs e)
        {
            cc1101_strobe(0x3d);
        }



        string status_byte_decode(byte chip) {

            byte mask = 0x70;
            byte st = (byte)((chip & mask) >> 4);
            switch (st) { 
                case 0:
                    return "idle";
                    break;
                case 1:
                    return "rx";
                    break;
                case 2:
                    return "tx";
                    break;
                case 3:
                    return "fstxon";
                    break;
                case 4:
                    return "calibrate";
                    break;
                case 5:
                    return "settling";
                    break;
                case 6:
                    return "rxfifo_overflow";
                    break;
                case 7:
                    return "txfifo_underflow";
                    break;
            }
            return "";
        }

        void cc1101_strobe(byte strobe)
        {
            try
            {
                byte[] cmd = new byte[2];
                cmd[0] = 1;
                cmd[1] = strobe;
                serialPort1.Write(cmd, 0, cmd.Length);
                string hexStringcmd = BitConverter.ToString(cmd);
                textBox_TX.AppendText(DateTime.Now + "-> strobe " + hexStringcmd + "(" + strobe_to_txt(strobe) + ")\n");

                byte[] ba = Encoding.Default.GetBytes(serialPort1.ReadExisting());
                string hexString = BitConverter.ToString(ba);
                textBox_RX.AppendText(DateTime.Now + "-> strobe " + hexString + "(" + strobe_to_txt(ba[1]) + ")(" + status_byte_decode(ba[2]) + ")\n");
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        #endregion


        Series rssi_dbm = new Series("rssi_dbm");

        //const int SMARTRF_SETTING_@RN@ = 0x@ah@;
        const int SMARTRF_SETTING_IOCFG0 = 0x0002;
        const int SMARTRF_SETTING_FIFOTHR = 0x0003;
        const int SMARTRF_SETTING_PKTCTRL0 = 0x0008;
        const int SMARTRF_SETTING_FSCTRL1 = 0x000b;
        const int SMARTRF_SETTING_FREQ2 = 0x000d;
        const int SMARTRF_SETTING_FREQ1 = 0x000e;
        const int SMARTRF_SETTING_FREQ0 = 0x000f;
        const int SMARTRF_SETTING_MDMCFG4 = 0x0010;
        const int SMARTRF_SETTING_MDMCFG3 = 0x0011;
        const int SMARTRF_SETTING_MDMCFG2 = 0x0012;
        const int SMARTRF_SETTING_DEVIATN = 0x0015;
        const int SMARTRF_SETTING_MCSM0 = 0x0018;
        const int SMARTRF_SETTING_FOCCFG = 0x0019;
        const int SMARTRF_SETTING_WORCTRL = 0x0020;
        const int SMARTRF_SETTING_FSCAL3 = 0x0023;
        const int SMARTRF_SETTING_FSCAL2 = 0x0024;
        const int SMARTRF_SETTING_FSCAL1 = 0x0025;
        const int SMARTRF_SETTING_FSCAL0 = 0x0026;
        const int SMARTRF_SETTING_TEST2 = 0x002c;
        const int SMARTRF_SETTING_TEST1 = 0x002d;
        const int SMARTRF_SETTING_TEST0 = 0x002e;


        void cc1101_write_reg(int reg, int valor) {
            try
            {
                byte[] cmd = new byte[3];
                cmd[0] = 3;
                cmd[1] = (byte)reg;
                cmd[2] = (byte)valor;
                serialPort1.Write(cmd, 0, cmd.Length);
                string hexStringcmd = BitConverter.ToString(cmd);
                textBox_TX.AppendText(DateTime.Now + "->" + hexStringcmd + "\n");
                Thread.Sleep(2);
                byte[] ba = Encoding.Default.GetBytes(serialPort1.ReadExisting());
                string hexString = BitConverter.ToString(ba);
                if (ba.Length >= 3)
                {
                    if (ba[2] == valor)
                    {
                        textBox_RX.AppendText(DateTime.Now + "-> write reg " + hexString + "[ok]\n");
                    }
                    else {
                        textBox_RX.AppendText(DateTime.Now + "-> write reg " + hexString + "[error]\n");
                    }
                }
                else {
                    textBox_RX.AppendText(DateTime.Now + "-> write reg " + hexString + "[len error]\n");
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {

            serialPort1.ReadTimeout = 10;

            chart1.Series.Clear();

            rssi_dbm.ChartType = SeriesChartType.Line;

            chart1.Series.Add(rssi_dbm);

           
        }


        private void button_status_regs_Click(object sender, EventArgs e)
        {
            cc1101_strobe(0x36);
            cc1101_strobe(0x36);
            cc1101_strobe(0x36);

            //cc1101_write_reg(SMARTRF_SETTING_@RN@,0x@VH@);


            cc1101_write_reg(SMARTRF_SETTING_FIFOTHR, 0x47);
            cc1101_write_reg(SMARTRF_SETTING_PKTCTRL0, 0x05);
            cc1101_write_reg(SMARTRF_SETTING_FSCTRL1, 0x06);
            cc1101_write_reg(SMARTRF_SETTING_FREQ2, 0x21);
            cc1101_write_reg(SMARTRF_SETTING_FREQ1, 0x62);
            cc1101_write_reg(SMARTRF_SETTING_FREQ0, 0x76);
            cc1101_write_reg(SMARTRF_SETTING_MDMCFG4, 0xF5);
            cc1101_write_reg(SMARTRF_SETTING_MDMCFG3, 0x83);
            cc1101_write_reg(SMARTRF_SETTING_MDMCFG2, 0x13);
            cc1101_write_reg(SMARTRF_SETTING_DEVIATN, 0x15);
            cc1101_write_reg(SMARTRF_SETTING_MCSM0, 0x18);
            cc1101_write_reg(SMARTRF_SETTING_FOCCFG, 0x16);
            cc1101_write_reg(SMARTRF_SETTING_WORCTRL, 0xFB);
            cc1101_write_reg(SMARTRF_SETTING_FSCAL3, 0xE9);
            cc1101_write_reg(SMARTRF_SETTING_FSCAL2, 0x2A);
            cc1101_write_reg(SMARTRF_SETTING_FSCAL1, 0x00);
            cc1101_write_reg(SMARTRF_SETTING_FSCAL0, 0x1F);
            cc1101_write_reg(SMARTRF_SETTING_TEST2, 0x81);
            cc1101_write_reg(SMARTRF_SETTING_TEST1, 0x35);
            cc1101_write_reg(SMARTRF_SETTING_TEST0, 0x09);



            for (byte i = 0x0; i <= 0x3f; i++)
            {
                try
                {
                    byte[] cmd = new byte[2];
                    cmd[0] = 0x02;
                    cmd[1] = i;
                    serialPort1.Write(cmd, 0, cmd.Length);
                    string hexStringcmd = BitConverter.ToString(cmd);
                    textBox_TX.AppendText(DateTime.Now + "->" + hexStringcmd + "\n");

                    byte[] ba = Encoding.Default.GetBytes(serialPort1.ReadExisting());
                    string hexString = BitConverter.ToString(ba);
                    textBox_RX.AppendText(DateTime.Now + "->" + hexString + "\n");
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }

            }
        }

        private void timer_rssi_Tick(object sender, EventArgs e)
        {
            cc1101_strobe(0x34);

            byte[] cmd = new byte[2];
            cmd[0] = 0x02;
            cmd[1] = 0x34;
            serialPort1.Write(cmd, 0, cmd.Length);
            byte[] ba = Encoding.Default.GetBytes(serialPort1.ReadExisting());
            string hexString = BitConverter.ToString(ba);
            if (ba.Length >= 3) {
                textBox_RX.AppendText(DateTime.Now + "->" + hexString + "(" + status_byte_decode(ba[2]) + ")\n");

                rssi_dbm.Points.AddY(rssi_to_dbm(ba[2]));
            }
        }

        int rssi_to_dbm(byte rssireg) {
            int rssi_offset = 74;

            int dbm = 0;
            int rssi = rssireg;
            if (rssi >= 128)
            {
                dbm = ((rssi - 256) / 2) - rssi_offset;
            }
            else {
                dbm = (rssi / 2) - rssi_offset;
            }
            if (dbm < -128) dbm = -128;
            return dbm;
        }

        private void button_rssi_start_Click(object sender, EventArgs e)
        {
            cc1101_strobe(0x33);

            timer_rssi.Start(); 
        }

        private void button_rssi_stop_Click(object sender, EventArgs e)
        {
            timer_rssi.Stop();
        }


    }
}
