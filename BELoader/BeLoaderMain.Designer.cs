namespace BELoader
{
    partial class BeLoaderMain
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
            this.TextBoxHexFile = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.BottonSelectHexFile = new System.Windows.Forms.Button();
            this.TextBoxMessageToHumanity = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.LabelStartAddress = new System.Windows.Forms.Label();
            this.LabelEndAddress = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.LabelAdler32 = new System.Windows.Forms.Label();
            this.ButtonDownload = new System.Windows.Forms.Button();
            this.label9 = new System.Windows.Forms.Label();
            this.comboPCAN = new System.Windows.Forms.ComboBox();
            this.label10 = new System.Windows.Forms.Label();
            this.ButtonConnectPCAN = new System.Windows.Forms.Button();
            this.NumericEcuID = new System.Windows.Forms.NumericUpDown();
            ((System.ComponentModel.ISupportInitialize)(this.NumericEcuID)).BeginInit();
            this.SuspendLayout();
            // 
            // TextBoxHexFile
            // 
            this.TextBoxHexFile.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.TextBoxHexFile.Location = new System.Drawing.Point(40, 54);
            this.TextBoxHexFile.Name = "TextBoxHexFile";
            this.TextBoxHexFile.Size = new System.Drawing.Size(376, 26);
            this.TextBoxHexFile.TabIndex = 0;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(37, 28);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(63, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Records file";
            // 
            // BottonSelectHexFile
            // 
            this.BottonSelectHexFile.Location = new System.Drawing.Point(437, 53);
            this.BottonSelectHexFile.Name = "BottonSelectHexFile";
            this.BottonSelectHexFile.Size = new System.Drawing.Size(117, 31);
            this.BottonSelectHexFile.TabIndex = 2;
            this.BottonSelectHexFile.Text = "Select Hex";
            this.BottonSelectHexFile.UseVisualStyleBackColor = true;
            this.BottonSelectHexFile.Click += new System.EventHandler(this.BottonSelectHexFile_Click);
            // 
            // TextBoxMessageToHumanity
            // 
            this.TextBoxMessageToHumanity.Location = new System.Drawing.Point(25, 336);
            this.TextBoxMessageToHumanity.Multiline = true;
            this.TextBoxMessageToHumanity.Name = "TextBoxMessageToHumanity";
            this.TextBoxMessageToHumanity.Size = new System.Drawing.Size(520, 87);
            this.TextBoxMessageToHumanity.TabIndex = 3;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(36, 297);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(182, 20);
            this.label2.TabIndex = 4;
            this.label2.Text = "Message to Humanity";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(37, 96);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(69, 13);
            this.label3.TabIndex = 5;
            this.label3.Text = "Start address";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(37, 122);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(66, 13);
            this.label4.TabIndex = 6;
            this.label4.Text = "End address";
            this.label4.Click += new System.EventHandler(this.label4_Click);
            // 
            // LabelStartAddress
            // 
            this.LabelStartAddress.AutoSize = true;
            this.LabelStartAddress.Location = new System.Drawing.Point(197, 96);
            this.LabelStartAddress.Name = "LabelStartAddress";
            this.LabelStartAddress.Size = new System.Drawing.Size(93, 13);
            this.LabelStartAddress.TabIndex = 7;
            this.LabelStartAddress.Text = "Not yet calculated";
            // 
            // LabelEndAddress
            // 
            this.LabelEndAddress.AutoSize = true;
            this.LabelEndAddress.Location = new System.Drawing.Point(197, 122);
            this.LabelEndAddress.Name = "LabelEndAddress";
            this.LabelEndAddress.Size = new System.Drawing.Size(93, 13);
            this.LabelEndAddress.TabIndex = 8;
            this.LabelEndAddress.Text = "Not yet calculated";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(37, 149);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(123, 13);
            this.label5.TabIndex = 9;
            this.label5.Text = "Code checksum Adler32";
            // 
            // LabelAdler32
            // 
            this.LabelAdler32.AutoSize = true;
            this.LabelAdler32.Location = new System.Drawing.Point(197, 149);
            this.LabelAdler32.Name = "LabelAdler32";
            this.LabelAdler32.Size = new System.Drawing.Size(93, 13);
            this.LabelAdler32.TabIndex = 10;
            this.LabelAdler32.Text = "Not yet calculated";
            // 
            // ButtonDownload
            // 
            this.ButtonDownload.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(128)))), ((int)(((byte)(0)))));
            this.ButtonDownload.Enabled = false;
            this.ButtonDownload.Location = new System.Drawing.Point(25, 206);
            this.ButtonDownload.Name = "ButtonDownload";
            this.ButtonDownload.Size = new System.Drawing.Size(193, 28);
            this.ButtonDownload.TabIndex = 12;
            this.ButtonDownload.TabStop = false;
            this.ButtonDownload.Text = "Download FW";
            this.ButtonDownload.UseVisualStyleBackColor = false;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(448, 122);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(75, 13);
            this.label9.TabIndex = 18;
            this.label9.Text = "J1939 CAN ID";
            // 
            // comboPCAN
            // 
            this.comboPCAN.Enabled = false;
            this.comboPCAN.FormattingEnabled = true;
            this.comboPCAN.Location = new System.Drawing.Point(297, 257);
            this.comboPCAN.Name = "comboPCAN";
            this.comboPCAN.Size = new System.Drawing.Size(257, 21);
            this.comboPCAN.TabIndex = 21;
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(294, 236);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(82, 13);
            this.label10.TabIndex = 22;
            this.label10.Text = "PCAN channels";
            // 
            // ButtonConnectPCAN
            // 
            this.ButtonConnectPCAN.Enabled = false;
            this.ButtonConnectPCAN.Location = new System.Drawing.Point(401, 203);
            this.ButtonConnectPCAN.Name = "ButtonConnectPCAN";
            this.ButtonConnectPCAN.Size = new System.Drawing.Size(155, 24);
            this.ButtonConnectPCAN.TabIndex = 23;
            this.ButtonConnectPCAN.Text = "Connect";
            this.ButtonConnectPCAN.UseVisualStyleBackColor = true;
            this.ButtonConnectPCAN.Click += new System.EventHandler(this.ButtonConnectPCAN_Click);
            // 
            // NumericEcuID
            // 
            this.NumericEcuID.Location = new System.Drawing.Point(451, 147);
            this.NumericEcuID.Maximum = new decimal(new int[] {
            255,
            0,
            0,
            0});
            this.NumericEcuID.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.NumericEcuID.Name = "NumericEcuID";
            this.NumericEcuID.Size = new System.Drawing.Size(103, 20);
            this.NumericEcuID.TabIndex = 25;
            this.NumericEcuID.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // BeLoaderMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(586, 447);
            this.Controls.Add(this.NumericEcuID);
            this.Controls.Add(this.ButtonConnectPCAN);
            this.Controls.Add(this.label10);
            this.Controls.Add(this.comboPCAN);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.ButtonDownload);
            this.Controls.Add(this.LabelAdler32);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.LabelEndAddress);
            this.Controls.Add(this.LabelStartAddress);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.TextBoxMessageToHumanity);
            this.Controls.Add(this.BottonSelectHexFile);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.TextBoxHexFile);
            this.Name = "BeLoaderMain";
            this.Text = "F28069F Loader";
            this.Load += new System.EventHandler(this.BeLoaderMain_Load);
            ((System.ComponentModel.ISupportInitialize)(this.NumericEcuID)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox TextBoxHexFile;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button BottonSelectHexFile;
        private System.Windows.Forms.TextBox TextBoxMessageToHumanity;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label LabelStartAddress;
        private System.Windows.Forms.Label LabelEndAddress;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label LabelAdler32;
        private System.Windows.Forms.Button ButtonDownload;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.ComboBox comboPCAN;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Button ButtonConnectPCAN;
        private System.Windows.Forms.NumericUpDown NumericEcuID;
    }
}

