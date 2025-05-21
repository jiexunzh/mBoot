namespace win_iap_ymodem
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要修改
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.btn_Port = new System.Windows.Forms.Button();
            this.lbl_Port = new System.Windows.Forms.Label();
            this.progressBar1 = new System.Windows.Forms.ProgressBar();
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.lbl_Bps = new System.Windows.Forms.Label();
            this.lbl_Pass = new System.Windows.Forms.Label();
            this.cbx_Port = new System.Windows.Forms.ComboBox();
            this.cbx_Baud = new System.Windows.Forms.ComboBox();
            this.txb_FilePath = new System.Windows.Forms.TextBox();
            this.btn_SelectFile = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.btn_Clear = new System.Windows.Forms.Button();
            this.tbx_show = new System.Windows.Forms.TextBox();
            this.btn_Update = new System.Windows.Forms.Button();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.lbl_bar = new System.Windows.Forms.Label();
            this.label_percent = new System.Windows.Forms.Label();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.SuspendLayout();
            // 
            // btn_Port
            // 
            this.btn_Port.BackColor = System.Drawing.Color.White;
            this.btn_Port.ForeColor = System.Drawing.Color.Black;
            this.btn_Port.Location = new System.Drawing.Point(20, 120);
            this.btn_Port.Name = "btn_Port";
            this.btn_Port.Size = new System.Drawing.Size(155, 32);
            this.btn_Port.TabIndex = 0;
            this.btn_Port.Text = "打开串口";
            this.btn_Port.UseVisualStyleBackColor = true;
            this.btn_Port.Click += new System.EventHandler(this.btn_Port_Click);
            // 
            // lbl_Port
            // 
            this.lbl_Port.AutoSize = true;
            this.lbl_Port.Location = new System.Drawing.Point(20, 41);
            this.lbl_Port.Name = "lbl_Port";
            this.lbl_Port.Size = new System.Drawing.Size(53, 12);
            this.lbl_Port.TabIndex = 1;
            this.lbl_Port.Text = "串口号：";
            // 
            // progressBar1
            // 
            this.progressBar1.BackColor = System.Drawing.SystemColors.ControlLight;
            this.progressBar1.Location = new System.Drawing.Point(220, 355);
            this.progressBar1.Name = "progressBar1";
            this.progressBar1.Size = new System.Drawing.Size(467, 22);
            this.progressBar1.TabIndex = 2;
            this.progressBar1.Click += new System.EventHandler(this.progressBar1_Click);
            // 
            // serialPort1
            // 
            this.serialPort1.BaudRate = 115200;
            this.serialPort1.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.serialPort1_DataReceived);
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.Filter = "所有文件(*.bin;*.hex)|*.bin;*.hex";
            this.openFileDialog1.FileOk += new System.ComponentModel.CancelEventHandler(this.openFileDialog1_FileOk);
            // 
            // lbl_Bps
            // 
            this.lbl_Bps.AutoSize = true;
            this.lbl_Bps.Location = new System.Drawing.Point(20, 81);
            this.lbl_Bps.Name = "lbl_Bps";
            this.lbl_Bps.Size = new System.Drawing.Size(53, 12);
            this.lbl_Bps.TabIndex = 3;
            this.lbl_Bps.Text = "波特率：";
            // 
            // lbl_Pass
            // 
            this.lbl_Pass.AutoSize = true;
            this.lbl_Pass.Location = new System.Drawing.Point(19, 30);
            this.lbl_Pass.Name = "lbl_Pass";
            this.lbl_Pass.Size = new System.Drawing.Size(65, 12);
            this.lbl_Pass.TabIndex = 4;
            this.lbl_Pass.Text = "文件路径：";
            // 
            // cbx_Port
            // 
            this.cbx_Port.FormattingEnabled = true;
            this.cbx_Port.Location = new System.Drawing.Point(79, 38);
            this.cbx_Port.Name = "cbx_Port";
            this.cbx_Port.Size = new System.Drawing.Size(98, 20);
            this.cbx_Port.TabIndex = 5;
            this.cbx_Port.DropDown += new System.EventHandler(this.cbx_Port_DropDown);
            // 
            // cbx_Baud
            // 
            this.cbx_Baud.FormattingEnabled = true;
            this.cbx_Baud.Items.AddRange(new object[] {
            "Custom",
            "4800",
            "9600",
            "14400",
            "19200",
            "38400",
            "56000",
            "57600",
            "115200",
            "128000",
            "256000"});
            this.cbx_Baud.Location = new System.Drawing.Point(79, 78);
            this.cbx_Baud.Name = "cbx_Baud";
            this.cbx_Baud.Size = new System.Drawing.Size(98, 20);
            this.cbx_Baud.TabIndex = 6;
            this.cbx_Baud.SelectedIndexChanged += new System.EventHandler(this.cbx_Baud_SelectedIndexChanged);
            // 
            // txb_FilePath
            // 
            this.txb_FilePath.BackColor = System.Drawing.SystemColors.Window;
            this.txb_FilePath.Location = new System.Drawing.Point(21, 54);
            this.txb_FilePath.Name = "txb_FilePath";
            this.txb_FilePath.Size = new System.Drawing.Size(155, 21);
            this.txb_FilePath.TabIndex = 7;
            this.txb_FilePath.TextChanged += new System.EventHandler(this.txb_FilePath_TextChanged);
            // 
            // btn_SelectFile
            // 
            this.btn_SelectFile.Location = new System.Drawing.Point(112, 25);
            this.btn_SelectFile.Name = "btn_SelectFile";
            this.btn_SelectFile.Size = new System.Drawing.Size(64, 23);
            this.btn_SelectFile.TabIndex = 8;
            this.btn_SelectFile.Text = "选择文件";
            this.btn_SelectFile.UseVisualStyleBackColor = true;
            this.btn_SelectFile.Click += new System.EventHandler(this.btn_SelectFile_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.btn_Port);
            this.groupBox1.Controls.Add(this.lbl_Bps);
            this.groupBox1.Controls.Add(this.lbl_Port);
            this.groupBox1.Controls.Add(this.cbx_Port);
            this.groupBox1.Controls.Add(this.cbx_Baud);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(191, 173);
            this.groupBox1.TabIndex = 10;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "端口操作";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.btn_Clear);
            this.groupBox2.Controls.Add(this.tbx_show);
            this.groupBox2.Location = new System.Drawing.Point(220, 12);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(507, 298);
            this.groupBox2.TabIndex = 11;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "信息视窗";
            // 
            // btn_Clear
            // 
            this.btn_Clear.Location = new System.Drawing.Point(0, 276);
            this.btn_Clear.Name = "btn_Clear";
            this.btn_Clear.Size = new System.Drawing.Size(75, 23);
            this.btn_Clear.TabIndex = 16;
            this.btn_Clear.Text = "清除窗口";
            this.btn_Clear.UseVisualStyleBackColor = true;
            this.btn_Clear.Click += new System.EventHandler(this.btn_Clear_Click);
            // 
            // tbx_show
            // 
            this.tbx_show.BackColor = System.Drawing.SystemColors.HighlightText;
            this.tbx_show.Location = new System.Drawing.Point(1, 20);
            this.tbx_show.Multiline = true;
            this.tbx_show.Name = "tbx_show";
            this.tbx_show.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.tbx_show.Size = new System.Drawing.Size(506, 255);
            this.tbx_show.TabIndex = 9;
            this.tbx_show.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.tbx_show_KeyPress);
            // 
            // btn_Update
            // 
            this.btn_Update.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.btn_Update.ForeColor = System.Drawing.SystemColors.ControlText;
            this.btn_Update.Location = new System.Drawing.Point(20, 108);
            this.btn_Update.Name = "btn_Update";
            this.btn_Update.Size = new System.Drawing.Size(155, 32);
            this.btn_Update.TabIndex = 15;
            this.btn_Update.Text = "更新固件 Update";
            this.btn_Update.UseVisualStyleBackColor = false;
            this.btn_Update.Click += new System.EventHandler(this.btn_Update_Click);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.lbl_Pass);
            this.groupBox3.Controls.Add(this.btn_SelectFile);
            this.groupBox3.Controls.Add(this.txb_FilePath);
            this.groupBox3.Controls.Add(this.btn_Update);
            this.groupBox3.Location = new System.Drawing.Point(12, 204);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(190, 173);
            this.groupBox3.TabIndex = 17;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "文件设置";
            // 
            // lbl_bar
            // 
            this.lbl_bar.AutoSize = true;
            this.lbl_bar.Location = new System.Drawing.Point(218, 332);
            this.lbl_bar.Name = "lbl_bar";
            this.lbl_bar.Size = new System.Drawing.Size(65, 12);
            this.lbl_bar.TabIndex = 18;
            this.lbl_bar.Text = "更新进度：";
            // 
            // label_percent
            // 
            this.label_percent.AutoSize = true;
            this.label_percent.Font = new System.Drawing.Font("宋体", 10F);
            this.label_percent.Location = new System.Drawing.Point(690, 358);
            this.label_percent.Name = "label_percent";
            this.label_percent.Size = new System.Drawing.Size(21, 14);
            this.label_percent.TabIndex = 19;
            this.label_percent.Text = "0%";
            this.label_percent.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.label_percent.Click += new System.EventHandler(this.label1_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.ClientSize = new System.Drawing.Size(743, 416);
            this.Controls.Add(this.label_percent);
            this.Controls.Add(this.lbl_bar);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.progressBar1);
            this.Name = "Form1";
            this.Text = "IAP固件升级工具-V0.3";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btn_Port;
        private System.Windows.Forms.Label lbl_Port;
        private System.Windows.Forms.ProgressBar progressBar1;
        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.Label lbl_Bps;
        private System.Windows.Forms.Label lbl_Pass;
        private System.Windows.Forms.ComboBox cbx_Port;
        private System.Windows.Forms.ComboBox cbx_Baud;
        private System.Windows.Forms.TextBox txb_FilePath;
        private System.Windows.Forms.Button btn_SelectFile;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.TextBox tbx_show;
        private System.Windows.Forms.Button btn_Update;
        private System.Windows.Forms.Button btn_Clear;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Label lbl_bar;
        private System.Windows.Forms.Label label_percent;
    }
}

