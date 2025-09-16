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
            // BeLoaderMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(610, 351);
            this.Controls.Add(this.BottonSelectHexFile);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.TextBoxHexFile);
            this.Name = "BeLoaderMain";
            this.Text = "F28069F Loader";
            this.Load += new System.EventHandler(this.BeLoaderMain_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox TextBoxHexFile;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button BottonSelectHexFile;
    }
}

