namespace msp430f5509_cc1101_lab
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea2 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend2 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series2 = new System.Windows.Forms.DataVisualization.Charting.Series();
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.comboBox_portas = new System.Windows.Forms.ComboBox();
            this.button_serial = new System.Windows.Forms.Button();
            this.chart1 = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.button_snop = new System.Windows.Forms.Button();
            this.button_sworrst = new System.Windows.Forms.Button();
            this.button_sftx = new System.Windows.Forms.Button();
            this.button_sfrx = new System.Windows.Forms.Button();
            this.button_spwd = new System.Windows.Forms.Button();
            this.button_swor = new System.Windows.Forms.Button();
            this.button_sidle = new System.Windows.Forms.Button();
            this.button_stx = new System.Windows.Forms.Button();
            this.button_srx = new System.Windows.Forms.Button();
            this.button_scal = new System.Windows.Forms.Button();
            this.button_sxoff = new System.Windows.Forms.Button();
            this.button_sfstxon = new System.Windows.Forms.Button();
            this.button_sres = new System.Windows.Forms.Button();
            this.textBox_RX = new System.Windows.Forms.TextBox();
            this.button_status_regs = new System.Windows.Forms.Button();
            this.textBox_TX = new System.Windows.Forms.TextBox();
            this.timer_rssi = new System.Windows.Forms.Timer(this.components);
            this.button_rssi_start = new System.Windows.Forms.Button();
            this.button_rssi_stop = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.chart1)).BeginInit();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // comboBox_portas
            // 
            this.comboBox_portas.FormattingEnabled = true;
            this.comboBox_portas.Location = new System.Drawing.Point(692, 12);
            this.comboBox_portas.Name = "comboBox_portas";
            this.comboBox_portas.Size = new System.Drawing.Size(121, 21);
            this.comboBox_portas.TabIndex = 0;
            this.comboBox_portas.DropDown += new System.EventHandler(this.comboBox_portas_DropDown);
            // 
            // button_serial
            // 
            this.button_serial.Location = new System.Drawing.Point(717, 39);
            this.button_serial.Name = "button_serial";
            this.button_serial.Size = new System.Drawing.Size(75, 23);
            this.button_serial.TabIndex = 1;
            this.button_serial.Text = "conectar";
            this.button_serial.UseVisualStyleBackColor = true;
            this.button_serial.Click += new System.EventHandler(this.button_serial_Click);
            // 
            // chart1
            // 
            chartArea2.Name = "ChartArea1";
            this.chart1.ChartAreas.Add(chartArea2);
            legend2.Name = "Legend1";
            this.chart1.Legends.Add(legend2);
            this.chart1.Location = new System.Drawing.Point(12, 14);
            this.chart1.Name = "chart1";
            series2.ChartArea = "ChartArea1";
            series2.Legend = "Legend1";
            series2.Name = "Series1";
            this.chart1.Series.Add(series2);
            this.chart1.Size = new System.Drawing.Size(412, 248);
            this.chart1.TabIndex = 2;
            this.chart1.Text = "chart1";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.button_snop);
            this.groupBox1.Controls.Add(this.button_sworrst);
            this.groupBox1.Controls.Add(this.button_sftx);
            this.groupBox1.Controls.Add(this.button_sfrx);
            this.groupBox1.Controls.Add(this.button_spwd);
            this.groupBox1.Controls.Add(this.button_swor);
            this.groupBox1.Controls.Add(this.button_sidle);
            this.groupBox1.Controls.Add(this.button_stx);
            this.groupBox1.Controls.Add(this.button_srx);
            this.groupBox1.Controls.Add(this.button_scal);
            this.groupBox1.Controls.Add(this.button_sxoff);
            this.groupBox1.Controls.Add(this.button_sfstxon);
            this.groupBox1.Controls.Add(this.button_sres);
            this.groupBox1.Location = new System.Drawing.Point(691, 74);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(161, 402);
            this.groupBox1.TabIndex = 3;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "CC1101 Strobes";
            // 
            // button_snop
            // 
            this.button_snop.Location = new System.Drawing.Point(7, 368);
            this.button_snop.Name = "button_snop";
            this.button_snop.Size = new System.Drawing.Size(148, 23);
            this.button_snop.TabIndex = 12;
            this.button_snop.Text = "SNOP (0x3c)";
            this.button_snop.UseVisualStyleBackColor = true;
            this.button_snop.Click += new System.EventHandler(this.button_snop_Click);
            // 
            // button_sworrst
            // 
            this.button_sworrst.Location = new System.Drawing.Point(6, 339);
            this.button_sworrst.Name = "button_sworrst";
            this.button_sworrst.Size = new System.Drawing.Size(149, 23);
            this.button_sworrst.TabIndex = 11;
            this.button_sworrst.Text = "SWORRST (0x3c)";
            this.button_sworrst.UseVisualStyleBackColor = true;
            this.button_sworrst.Click += new System.EventHandler(this.button_sworrst_Click);
            // 
            // button_sftx
            // 
            this.button_sftx.Location = new System.Drawing.Point(7, 310);
            this.button_sftx.Name = "button_sftx";
            this.button_sftx.Size = new System.Drawing.Size(148, 23);
            this.button_sftx.TabIndex = 10;
            this.button_sftx.Text = "SFTX (0x3B)";
            this.button_sftx.UseVisualStyleBackColor = true;
            this.button_sftx.Click += new System.EventHandler(this.button_sftx_Click);
            // 
            // button_sfrx
            // 
            this.button_sfrx.Location = new System.Drawing.Point(7, 281);
            this.button_sfrx.Name = "button_sfrx";
            this.button_sfrx.Size = new System.Drawing.Size(148, 23);
            this.button_sfrx.TabIndex = 9;
            this.button_sfrx.Text = "SFRX (0x3A)";
            this.button_sfrx.UseVisualStyleBackColor = true;
            this.button_sfrx.Click += new System.EventHandler(this.button_sfrx_Click);
            // 
            // button_spwd
            // 
            this.button_spwd.Location = new System.Drawing.Point(6, 252);
            this.button_spwd.Name = "button_spwd";
            this.button_spwd.Size = new System.Drawing.Size(149, 23);
            this.button_spwd.TabIndex = 8;
            this.button_spwd.Text = "SPWD (0x39) [buga usb]";
            this.button_spwd.UseVisualStyleBackColor = true;
            this.button_spwd.Click += new System.EventHandler(this.button_spwd_Click);
            // 
            // button_swor
            // 
            this.button_swor.Location = new System.Drawing.Point(7, 223);
            this.button_swor.Name = "button_swor";
            this.button_swor.Size = new System.Drawing.Size(148, 23);
            this.button_swor.TabIndex = 7;
            this.button_swor.Text = "SWOR (0x38) [buga usb]";
            this.button_swor.UseVisualStyleBackColor = true;
            this.button_swor.Click += new System.EventHandler(this.button_swor_Click);
            // 
            // button_sidle
            // 
            this.button_sidle.Location = new System.Drawing.Point(6, 194);
            this.button_sidle.Name = "button_sidle";
            this.button_sidle.Size = new System.Drawing.Size(149, 23);
            this.button_sidle.TabIndex = 6;
            this.button_sidle.Text = "SIDLE (0x36)";
            this.button_sidle.UseVisualStyleBackColor = true;
            this.button_sidle.Click += new System.EventHandler(this.button_sidle_Click);
            // 
            // button_stx
            // 
            this.button_stx.Location = new System.Drawing.Point(6, 165);
            this.button_stx.Name = "button_stx";
            this.button_stx.Size = new System.Drawing.Size(149, 23);
            this.button_stx.TabIndex = 5;
            this.button_stx.Text = "STX (0x35)";
            this.button_stx.UseVisualStyleBackColor = true;
            this.button_stx.Click += new System.EventHandler(this.button_stx_Click);
            // 
            // button_srx
            // 
            this.button_srx.Location = new System.Drawing.Point(6, 136);
            this.button_srx.Name = "button_srx";
            this.button_srx.Size = new System.Drawing.Size(149, 23);
            this.button_srx.TabIndex = 4;
            this.button_srx.Text = "SRX (0x34)";
            this.button_srx.UseVisualStyleBackColor = true;
            this.button_srx.Click += new System.EventHandler(this.button_srx_Click);
            // 
            // button_scal
            // 
            this.button_scal.Location = new System.Drawing.Point(7, 107);
            this.button_scal.Name = "button_scal";
            this.button_scal.Size = new System.Drawing.Size(148, 23);
            this.button_scal.TabIndex = 3;
            this.button_scal.Text = "SCAL (0x33)";
            this.button_scal.UseVisualStyleBackColor = true;
            this.button_scal.Click += new System.EventHandler(this.button_scal_Click);
            // 
            // button_sxoff
            // 
            this.button_sxoff.Location = new System.Drawing.Point(7, 78);
            this.button_sxoff.Name = "button_sxoff";
            this.button_sxoff.Size = new System.Drawing.Size(148, 23);
            this.button_sxoff.TabIndex = 2;
            this.button_sxoff.Text = "SXOFF (0x32) [buga usb]";
            this.button_sxoff.UseVisualStyleBackColor = true;
            this.button_sxoff.Click += new System.EventHandler(this.button_sxoff_Click);
            // 
            // button_sfstxon
            // 
            this.button_sfstxon.Location = new System.Drawing.Point(7, 49);
            this.button_sfstxon.Name = "button_sfstxon";
            this.button_sfstxon.Size = new System.Drawing.Size(148, 23);
            this.button_sfstxon.TabIndex = 1;
            this.button_sfstxon.Text = "SFSTXON (0x31)";
            this.button_sfstxon.UseVisualStyleBackColor = true;
            this.button_sfstxon.Click += new System.EventHandler(this.button_sfstxon_Click);
            // 
            // button_sres
            // 
            this.button_sres.Location = new System.Drawing.Point(7, 20);
            this.button_sres.Name = "button_sres";
            this.button_sres.Size = new System.Drawing.Size(148, 23);
            this.button_sres.TabIndex = 0;
            this.button_sres.Text = "SRES (0x30) [buga usb]";
            this.button_sres.UseVisualStyleBackColor = true;
            this.button_sres.Click += new System.EventHandler(this.button_sres_Click);
            // 
            // textBox_RX
            // 
            this.textBox_RX.Location = new System.Drawing.Point(12, 399);
            this.textBox_RX.Multiline = true;
            this.textBox_RX.Name = "textBox_RX";
            this.textBox_RX.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBox_RX.Size = new System.Drawing.Size(673, 77);
            this.textBox_RX.TabIndex = 4;
            // 
            // button_status_regs
            // 
            this.button_status_regs.Location = new System.Drawing.Point(514, 123);
            this.button_status_regs.Name = "button_status_regs";
            this.button_status_regs.Size = new System.Drawing.Size(75, 23);
            this.button_status_regs.TabIndex = 5;
            this.button_status_regs.Text = "Status Regs";
            this.button_status_regs.UseVisualStyleBackColor = true;
            this.button_status_regs.Click += new System.EventHandler(this.button_status_regs_Click);
            // 
            // textBox_TX
            // 
            this.textBox_TX.Location = new System.Drawing.Point(12, 316);
            this.textBox_TX.Multiline = true;
            this.textBox_TX.Name = "textBox_TX";
            this.textBox_TX.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBox_TX.Size = new System.Drawing.Size(673, 77);
            this.textBox_TX.TabIndex = 6;
            // 
            // timer_rssi
            // 
            this.timer_rssi.Tick += new System.EventHandler(this.timer_rssi_Tick);
            // 
            // button_rssi_start
            // 
            this.button_rssi_start.Location = new System.Drawing.Point(530, 181);
            this.button_rssi_start.Name = "button_rssi_start";
            this.button_rssi_start.Size = new System.Drawing.Size(75, 23);
            this.button_rssi_start.TabIndex = 7;
            this.button_rssi_start.Text = "RSSI start";
            this.button_rssi_start.UseVisualStyleBackColor = true;
            this.button_rssi_start.Click += new System.EventHandler(this.button_rssi_start_Click);
            // 
            // button_rssi_stop
            // 
            this.button_rssi_stop.Location = new System.Drawing.Point(530, 210);
            this.button_rssi_stop.Name = "button_rssi_stop";
            this.button_rssi_stop.Size = new System.Drawing.Size(75, 23);
            this.button_rssi_stop.TabIndex = 8;
            this.button_rssi_stop.Text = "RSSI stop";
            this.button_rssi_stop.UseVisualStyleBackColor = true;
            this.button_rssi_stop.Click += new System.EventHandler(this.button_rssi_stop_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(864, 488);
            this.Controls.Add(this.button_rssi_stop);
            this.Controls.Add(this.button_rssi_start);
            this.Controls.Add(this.textBox_TX);
            this.Controls.Add(this.button_status_regs);
            this.Controls.Add(this.textBox_RX);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.chart1);
            this.Controls.Add(this.button_serial);
            this.Controls.Add(this.comboBox_portas);
            this.Name = "Form1";
            this.Text = "MSP430F5509 + CC1101 LAB";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.chart1)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.ComboBox comboBox_portas;
        private System.Windows.Forms.Button button_serial;
        private System.Windows.Forms.DataVisualization.Charting.Chart chart1;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button button_snop;
        private System.Windows.Forms.Button button_sworrst;
        private System.Windows.Forms.Button button_sftx;
        private System.Windows.Forms.Button button_sfrx;
        private System.Windows.Forms.Button button_spwd;
        private System.Windows.Forms.Button button_swor;
        private System.Windows.Forms.Button button_sidle;
        private System.Windows.Forms.Button button_stx;
        private System.Windows.Forms.Button button_srx;
        private System.Windows.Forms.Button button_scal;
        private System.Windows.Forms.Button button_sxoff;
        private System.Windows.Forms.Button button_sfstxon;
        private System.Windows.Forms.Button button_sres;
        private System.Windows.Forms.TextBox textBox_RX;
        private System.Windows.Forms.Button button_status_regs;
        private System.Windows.Forms.TextBox textBox_TX;
        private System.Windows.Forms.Timer timer_rssi;
        private System.Windows.Forms.Button button_rssi_start;
        private System.Windows.Forms.Button button_rssi_stop;
    }
}

