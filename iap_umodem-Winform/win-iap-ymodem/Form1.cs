using Microsoft.Win32;
using System;
using System.ComponentModel;
using System.Data;
using System.IO;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using System.Timers;
using System.Drawing;

namespace win_iap_ymodem        // 相当于一个文件夹
{
    public partial class Form1 : Form
    {
        public enum ACKResult
        {
            SUCCESS = 0,
            ERROR,
            TIMEOUT
        }
        // public static class GlobalPara
        public class GlobalPara
        {
            public const ushort WAIT_TIME = 5000;
            volatile public static bool SerialRecvFlag = false;
            volatile public static bool TimeOutFlag = false;
        }

        private bool hasOpenPort = false;   // 打开串口标志位

        public bool HasOpenPort
        {
            get { return hasOpenPort; }
            set
            {
                hasOpenPort = value;
                if (hasOpenPort && hasSelectBin)
                    openControlBtn();
                else
                    closeControlBtn();
            }
        }
        private bool hasSelectBin = false;     
        public bool HasSelectBin        // HasSelectBin属性
        {
            get { return hasSelectBin; }    // get访问器：获取 hasSelectBin
            set // set访问器：设置 hasSelectBin，并执行一些操作
            {
                hasSelectBin = value;
                if (hasOpenPort && hasSelectBin)    // 已经打开串口和选择bin文件
                    openControlBtn();
                else
                    closeControlBtn();
            }
        }

        public static string filePath = "";
        string sendCmd = "";
        int fsLen = 0;

        /* packet define */
        const byte C = 67;
        byte STX = 2;


        const int FILE_NAME_LENGTH = 256;
        const byte FILE_SIZE_LENGTH = 16;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            closeControlBtn();
            EnumComportfromReg(cbx_Port);
            serialPort1.Encoding = Encoding.GetEncoding("gb2312");//串口接收编码GB2312码
            System.Windows.Forms.Control.CheckForIllegalCrossThreadCalls = false;//忽略程序跨越线程运行导致的错误.没有此句将会产生错误
            cbx_Baud.SelectedIndex = 8;    // 选择默认波特率
        }

        /// <summary>
        /// enabled all button
        /// </summary>
        private void openControlBtn()
        {
            btn_Update.Enabled = true;
        }

        /// <summary>
        /// disabled all button
        /// </summary>
        private void closeControlBtn()
        {
            btn_Update.Enabled = false;
        }


        /// <summary>
        /// the button for select bin file or hex file.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btn_SelectFile_Click(object sender, EventArgs e)
        {
            openFileDialog1.ShowDialog();
        }

        /// <summary>
        /// has been selected the right file.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void openFileDialog1_FileOk(object sender, CancelEventArgs e)
        {
            filePath = openFileDialog1.FileName;
            //Get the extension of the file
            string extName = Path.GetExtension(filePath);
            Console.WriteLine(filePath);
            if (extName == ".hex")
            {
                //we shoule convert the hex file to bin file.
                if (HexToBin.convertHexToBin(filePath))     // 文件路径也会自动更改为.bin
                {
                    tbx_show.AppendText("HEX文件转换完成!\r\n");
                }
                else
                {
                    tbx_show.AppendText("HEX文件转换失败!\r\n");
                }
            }

            txb_FilePath.Text = filePath;
            Console.WriteLine(txb_FilePath.Text);

            HasSelectBin = true;   // 将 hasSelectBin 的值设置为 true

            FileStream fileStream = new FileStream(@filePath, FileMode.Open, FileAccess.Read);
            fsLen = (int)fileStream.Length;
            tbx_show.AppendText("文件大小: " + fsLen.ToString() + "\r\n");
        }

        /// <summary>
        /// get all port and add to cbx_Port.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void cbx_Port_DropDown(object sender, EventArgs e)
        {
            if (!HasOpenPort)
            {
                EnumComportfromReg(cbx_Port);
            }
        }

        /// <summary>
        /// open or close port.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btn_Port_Click(object sender, EventArgs e)
        {
            if (serialPort1.IsOpen)//try to close
            {
                try
                {
                    serialPort1.Close();
                    btn_Port.Text = "打开串口";
                    btn_Port.ForeColor = System.Drawing.Color.Black;
                    btn_Port.BackColor = System.Drawing.Color.White;
                    HasOpenPort = false;
                }
                catch (Exception)
                {
                    MessageBox.Show("关闭失败");
                    return;
                }
            }
            else //try to open
            {
                if (cbx_Port.Items.Count == 0)
                    return;
                int baud = int.Parse(cbx_Baud.Text);
                serialPort1.PortName = cbx_Port.Text;
                serialPort1.BaudRate = baud;
                try
                {
                    serialPort1.Open();
                    btn_Port.Text = "关闭串口";
                    btn_Port.ForeColor = System.Drawing.Color.White;
                    btn_Port.BackColor = System.Drawing.Color.ForestGreen;
                    HasOpenPort = true;     // HasOpenPort属性设置为true
                }
                catch (Exception)
                {
                    MessageBox.Show("打开串口失败！\r\n请选择正确的串口或该串口被占用！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error, MessageBoxDefaultButton.Button1, MessageBoxOptions.ServiceNotification);
                    return;
                }
            }
        }


        /// <summary>
        /// Get the port list from the registry
        /// </summary>
        /// <param name="Combobox"></param>
        private void EnumComportfromReg(ComboBox Combobox)
        {
            Combobox.Items.Clear();
            ///定义注册表子Path
            string strRegPath = @"Hardware\\DeviceMap\\SerialComm";
            ///创建两个RegistryKey类，一个将指向Root Path，另一个将指向子Path
            RegistryKey regRootKey;
            RegistryKey regSubKey;
            ///定义Root指向注册表HKEY_LOCAL_MACHINE节点
            regRootKey = Registry.LocalMachine;
            regSubKey = regRootKey.OpenSubKey(strRegPath);
            if (regSubKey.GetValueNames() == null)
            {
                MessageBox.Show("获取串口设备失败", "提示");
                return;
            }
            string[] strCommList = regSubKey.GetValueNames();
            foreach (string VName in strCommList)
            {
                //向listbox1中添加字符串的名称和数据，数据是从rk对象中的GetValue(it)方法中得来的
                Combobox.Items.Add(regSubKey.GetValue(VName));
            }
            if (Combobox.Items.Count > 0)
            {
                Combobox.SelectedIndex = 0;
            }
            else
            {
                Combobox.Text = "";
            }
            regSubKey.Close();
            regRootKey.Close();
        }


        /// <summary>
        /// once has date in. we should show it on the txb.(找到了bug的原因，就是有数据来了之后首先主动去读取了一次数据，然后又通过这个服务去被动读取了一次，所以会出现问题。)
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void serialPort1_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            try
            {
                string revData = serialPort1.ReadExisting();
                tbx_show.AppendText("\r\nRX: \r\n");
                tbx_show.AppendText(revData);
            }
            catch
            {
                MessageBox.Show("串口出现故障", "提示");
            }
        }
        private void Update_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            GlobalPara.SerialRecvFlag = true;
        }


        /// <summary>
        /// A thread to update firmware.
        /// </summary>
        private void updateFileThread()
        {
            bool xReturn = CustomUpdateFile(txb_FilePath.Text);    // 开始更新固件   
            serialPort1.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(serialPort1_DataReceived);   // RX显示在接收窗口
            openControlBtn();   // 重新开启按键 

            // 会阻塞，应该放在最后
            if (xReturn == true)
            {
                tbx_show.AppendText("固件更新完毕！\r\n");
                MessageBox.Show("固件更新完毕！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information, MessageBoxDefaultButton.Button1, MessageBoxOptions.ServiceNotification);
            }
            else
            {
                tbx_show.AppendText("固件更新失败！\r\n");
                MessageBox.Show("固件更新失败！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error, MessageBoxDefaultButton.Button1, MessageBoxOptions.ServiceNotification);
            }                                  
        }

        /// <summary>
        /// 固件更新按键点击事件处理程序
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btn_Update_Click(object sender, EventArgs e)
        {
            if (!File.Exists(@txb_FilePath.Text) || (Path.GetExtension(@txb_FilePath.Text) != ".bin"))
            {
                MessageBox.Show("请选择有效的bin文件", "提示");
                return;
            }

            closeControlBtn();      // 禁用button，防止被重复按下

            // FirmwareUpdateStart();   // “btn_Update_Click”仍未执行完之前禁用按键无效，按下依然会重复触发
            Thread UpdateStartThread = new Thread(FirmwareUpdateStart);
            UpdateStartThread.Start();
        }

        private void FirmwareUpdateStart()
        {
            progressBar1.Value = 0; // 重置进度条
            label_percent.Text = "0%";  // 重置百分比
            serialPort1.DataReceived -= new System.IO.Ports.SerialDataReceivedEventHandler(serialPort1_DataReceived);

            string send_str = "update\r\n";
            string ack_str = "OK\r\n";
            ACKResult ack_result = SendUpdateCommand(send_str, ack_str);

            if (ack_result == ACKResult.SUCCESS)
            {
                Thread.Sleep(10);
                send_str = "BINSIZE=" + fsLen.ToString() + "\r\n";
                ack_str = fsLen.ToString() + "\r\n";
                ack_result = SendUpdateCommand(send_str, ack_str);
            }

            // 收到 OK fsLen
            if (ack_result == ACKResult.SUCCESS)
            {
                tbx_show.AppendText("收到MCU应答, 开始更新固件...\r\n");
                Thread UploadThread = new Thread(updateFileThread);
                UploadThread.Start();
            }
            else
            {
                openControlBtn();   // 重新打开button
                serialPort1.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(serialPort1_DataReceived);

                if (ack_result == ACKResult.ERROR)
                {
                    tbx_show.AppendText("MCU update或BINSIZE=响应异常\r\n");
                }
                else if (ack_result == ACKResult.TIMEOUT)
                {
                    tbx_show.AppendText("MCU update或BINSIZE=响应超时\r\n");
                }
                else
                { }
                string tem_str = "MCU响应超时或异常，\r\n固件更新失败！";
                MessageBox.Show(tem_str, "提示", MessageBoxButtons.OK, MessageBoxIcon.Error, MessageBoxDefaultButton.Button1, MessageBoxOptions.ServiceNotification);
            }
        }

        private ACKResult SendUpdateCommand(String cmd, String ack)
        {
            int retryTime = 100;    // 100ms
            int retryCount = 0;

            while (retryCount < 100)     // 10秒
            {
                // 发送"cmd"命令
                serialPort1.Write(cmd);

                // 等待MCU回应
                Thread.Sleep(retryTime);

                // 检查是否有回应
                if (serialPort1.BytesToRead > 0)
                {
                    Thread.Sleep(20);   // 等待数据接收完整！
                    string response = serialPort1.ReadExisting();
                    if (response.Contains(ack))
                    {
                        return ACKResult.SUCCESS;
                    }
                    else
                    {   

                        tbx_show.AppendText("收到: " + response);
                        return ACKResult.ERROR;
                    }
                }
                tbx_show.AppendText("MCU update无应答, 重新发送更新命令...\r\n");
                retryCount++;
            }
            return ACKResult.TIMEOUT;
        }

        /// <summary>
        /// clear tbx_show window when the button has been checked.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btn_Clear_Click(object sender, EventArgs e)
        {
            tbx_show.Text = "";
        }

        /// <summary>
        /// when user is input the cmd on the tbx_show, it should be send to stm32.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void tbx_show_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == (char)Keys.Enter)
            {
                //tbx_show.AppendText("\r\n" + sendCmd + "\r\n");
                switch (sendCmd)
                {
                    case "update":
                        btn_Update_Click(null, null);
                        break;
                    default:
                        serialPort1.Write(sendCmd + "\r\n");
                        break;
                }
                sendCmd = "";
            }
            else if (e.KeyChar == (char)Keys.Back)
            {
                if (sendCmd.Length > 0)
                    sendCmd = sendCmd.Substring(0, sendCmd.Length - 1);
            }
            else
            {
                sendCmd += e.KeyChar.ToString();
            }
        }

        private bool CustomUpdateFile(string filePath)
        {
            return SendFirwareFunc(filePath);                        
        }
        private bool SendFirwareFunc(string filePath)
        {
            const byte HEADER = 0xA5;  // Start Of Head
            const ushort DATA_SIZE = 1024;      // 包数据大小
            const ushort PACKET_SIZE = DATA_SIZE + 4;    // 包大小
            const byte Invalid_Byte = 0xFF;

            ushort crc;
            byte[] packet = new byte[PACKET_SIZE];

            /* get the file */
            FileStream fileStream = new FileStream(@filePath, FileMode.Open, FileAccess.Read);
            // 包长
            int packetCnt = (fileStream.Length % DATA_SIZE) == 0 ? (int)(fileStream.Length / DATA_SIZE) : (int)(fileStream.Length / DATA_SIZE) + 1;
            progressBar1.Maximum = packetCnt;   // 设置进度条满度等于包数量

            if (packetCnt > 255)
            {
                MessageBox.Show("BIN文件包长错误：" + packetCnt);
                return false;
            }

            // 创建一个计时器来监视接收超时
            System.Timers.Timer timeoutTimer = new System.Timers.Timer(GlobalPara.WAIT_TIME);  // 设置超时时间
            timeoutTimer.Elapsed += TimeoutTimer_Elapsed;   //到达时间的时候执行事件
            timeoutTimer.AutoReset = false;  // 只触发一次

            /* 按 帧头 + 包序号 + 1KB data + CRC-CCITT 进行组包 */
            for (int i = 0; i < packetCnt; i++)
            {
                Array.Clear(packet, 0, PACKET_SIZE);    // 清空数组
                packet[0] = HEADER;
                packet[1] = (byte)i;    // 包序号
                // offset: 目标数组 data 中的起始位置，FileStream会根据读取的数据更新文件指针（向后移动）
                int fileReadCount = fileStream.Read(packet, 2, DATA_SIZE);
                if (fileReadCount == 0)
                {
                    Console.WriteLine("文件读取错误\r\n");
                    return false;
                }

                /* if this is the last packet fill the remaining bytes with Invalid_Byte */
                if (fileReadCount != DATA_SIZE)
                {
                    for (int j = fileReadCount; j < DATA_SIZE; j++)
                        packet[j + 2] = Invalid_Byte;
                }

                crc = CrcCcitt.Compute(packet, PACKET_SIZE - 2);    // 低八位CRC_L, 高八位CRC_H
                packet[PACKET_SIZE - 2] = (byte)(crc & 0xFF);   // 低位在前
                packet[PACKET_SIZE - 1] = (byte)(crc >> 8);     // 高位在后

                //tbx_show.AppendText(BitConverter.ToString(packet));
                //tbx_show.AppendText("\r\n");

                ushort err_num = 0;

                retryHandle:           // ----错误重传goto----
                Thread.Sleep(10);  // 间隔10ms，给MCU反应时间

                /* 开启串口接收事件 Update_DataReceived 回调 */
                GlobalPara.SerialRecvFlag = false;      /* 如果不加这句，多次重复更新固件SerialRecvFlag偶尔还是会被重复触发 */
                serialPort1.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(Update_DataReceived);    
                if (err_num > 0)
                {
                    tbx_show.AppendText("重传：");
                }
                tbx_show.AppendText("正在传输第"+(i+1)+"个数据包...");
                serialPort1.Write(packet, 0, PACKET_SIZE);  // 发送数据包

                GlobalPara.TimeOutFlag = false;
                timeoutTimer.Start();            // 启动定时器

                /* wait for MCU ACK */
                do
                {
                    /* 收到应答（区别于空闲中断，此时接收不一定完整） */
                    if (GlobalPara.SerialRecvFlag == true)
                    {
                        /* 关闭串口接收事件 Update_DataReceived 回调 */
                        serialPort1.DataReceived -= new System.IO.Ports.SerialDataReceivedEventHandler(Update_DataReceived);
                        GlobalPara.SerialRecvFlag = false;  // 清除串口接收标志位

                        Thread.Sleep(20);   // 等待数据接收完整！

                        /* 取消计时器并处理数据 */
                        timeoutTimer.Stop();
                        string revData = serialPort1.ReadExisting();
                        if (revData.Contains("RECV\r\n"))   // 数据包传输正确
                        {
                            /* 更新进度条 */
                            progressBar1.Value = i + 1;

                            /* 计算百分比 */
                            double percentage = ((double)(i + 1) / packetCnt) * 100;
                            string percentStr = percentage.ToString("F1");  // 保留 1 位小数

                            /* 更新百分比 */
                            string str1 = percentStr.Substring(0, percentStr.Length - 2);   // 去掉最右边2个字符
                            label_percent.Text = str1 + "%";

                            tbx_show.AppendText(percentStr + "%\r\n");

                            break;  
                        }
                        else  // 未收到正确应答
                        {
                            if (revData.Contains("ERROR\r\n"))  // 传输错误
                            {
                                tbx_show.AppendText("\r\n数据包传输有误...\r\n");
                                err_num++;
                                if (err_num >= 3)   // 错误3次以上则退出
                                {
                                    return false;
                                }
                                else
                                {
                                    goto retryHandle;   // 重新传输本包
                                }
                            }
                            else   // 响应异常
                            {
                                tbx_show.AppendText(revData + "\r\nMCU应答异常\r\n");
                                return false;
                            }
                        }
                    }
                } while (GlobalPara.TimeOutFlag == false);      /* MCU响应未超时 */

                if (GlobalPara.TimeOutFlag == true)
                {
                    tbx_show.AppendText("MCU无应答，数据接收超时\r\n");
                    GlobalPara.TimeOutFlag = false;
                    return false;
                }
            }

            return true;
        }

        private void TimeoutTimer_Elapsed(object sender, ElapsedEventArgs e)
        {
            GlobalPara.TimeOutFlag = true;
        }
        private byte Calculate_Checksum(byte[] array, int length)
        {
            byte sum = 0;

            for (int i = 0; i < length; i++)
            {
                sum += array[i];
            }

            sum = (byte)(sum % 256); // 强制转换，保证sum保持在0-255范围内

            return sum;
        }

        public class CrcCcitt
        {
            private static readonly ushort[] CrcTable = new ushort[256];

            // 预计算 CRC-CCITT 查找表（多项式 0x1021）
            static CrcCcitt()
            {
                for (ushort i = 0; i < 256; i++)
                {
                    ushort crc = (ushort)(i << 8);
                    for (int j = 0; j < 8; j++)
                    {
                        if ((crc & 0x8000) != 0)
                            crc = (ushort)((crc << 1) ^ 0x1021);
                        else
                            crc <<= 1;
                    }
                    CrcTable[i] = crc;
                }
            }

            public static ushort Compute(byte[] data, int length)
            {
                if (data == null || length <= 0 || length > data.Length)
                    throw new ArgumentException("Invalid data length");

                ushort crc = 0xFFFF;

                for (int i = 0; i < length; i++)
                {
                    // 确保 crc 在计算后仍然是 ushort
                    crc = (ushort)(((crc << 8) ^ CrcTable[((crc >> 8) ^ data[i]) & 0xFF]) & 0xFFFF);    
                }
                return crc;
            }
        }


        private void txb_FilePath_TextChanged(object sender, EventArgs e)
        {

        }

        private void progressBar1_Click(object sender, EventArgs e)
        {

        }

        private void cbx_Baud_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void label1_Click(object sender, EventArgs e)
        {

        }
    }
}
