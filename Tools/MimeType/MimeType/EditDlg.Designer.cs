namespace Charlotte
{
	partial class EditDlg
	{
		/// <summary>
		/// 必要なデザイナー変数です。
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// 使用中のリソースをすべてクリーンアップします。
		/// </summary>
		/// <param name="disposing">マネージ リソースが破棄される場合 true、破棄されない場合は false です。</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows フォーム デザイナーで生成されたコード

		/// <summary>
		/// デザイナー サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディターで変更しないでください。
		/// </summary>
		private void InitializeComponent()
		{
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(EditDlg));
			this.BtnCancel = new System.Windows.Forms.Button();
			this.BtnOk = new System.Windows.Forms.Button();
			this.Ext = new System.Windows.Forms.TextBox();
			this.MimeType = new System.Windows.Forms.TextBox();
			this.label1 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// BtnCancel
			// 
			this.BtnCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.BtnCancel.Font = new System.Drawing.Font("メイリオ", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.BtnCancel.Location = new System.Drawing.Point(441, 77);
			this.BtnCancel.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.BtnCancel.Name = "BtnCancel";
			this.BtnCancel.Size = new System.Drawing.Size(130, 30);
			this.BtnCancel.TabIndex = 5;
			this.BtnCancel.Text = "キャンセル(&C)";
			this.BtnCancel.UseVisualStyleBackColor = true;
			this.BtnCancel.Click += new System.EventHandler(this.BtnCancel_Click);
			// 
			// BtnOk
			// 
			this.BtnOk.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.BtnOk.Font = new System.Drawing.Font("メイリオ", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.BtnOk.Location = new System.Drawing.Point(303, 77);
			this.BtnOk.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.BtnOk.Name = "BtnOk";
			this.BtnOk.Size = new System.Drawing.Size(130, 30);
			this.BtnOk.TabIndex = 4;
			this.BtnOk.Text = "OK(&O)";
			this.BtnOk.UseVisualStyleBackColor = true;
			this.BtnOk.Click += new System.EventHandler(this.BtnOk_Click);
			// 
			// Ext
			// 
			this.Ext.Location = new System.Drawing.Point(12, 32);
			this.Ext.MaxLength = 100;
			this.Ext.Name = "Ext";
			this.Ext.Size = new System.Drawing.Size(200, 27);
			this.Ext.TabIndex = 1;
			this.Ext.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.Ext_KeyPress);
			// 
			// MimeType
			// 
			this.MimeType.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.MimeType.Location = new System.Drawing.Point(218, 32);
			this.MimeType.MaxLength = 1000;
			this.MimeType.Name = "MimeType";
			this.MimeType.Size = new System.Drawing.Size(354, 27);
			this.MimeType.TabIndex = 3;
			this.MimeType.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.MimeType_KeyPress);
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(12, 9);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(48, 20);
			this.label1.TabIndex = 0;
			this.label1.Text = "拡張子";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(214, 9);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(78, 20);
			this.label2.TabIndex = 2;
			this.label2.Text = "MIME Type";
			// 
			// EditDlg
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 20F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(584, 121);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.MimeType);
			this.Controls.Add(this.Ext);
			this.Controls.Add(this.BtnCancel);
			this.Controls.Add(this.BtnOk);
			this.Font = new System.Drawing.Font("メイリオ", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.MinimizeBox = false;
			this.Name = "EditDlg";
			this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Show;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "入力_編集";
			this.Load += new System.EventHandler(this.EditDlg_Load);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Button BtnCancel;
		private System.Windows.Forms.Button BtnOk;
		private System.Windows.Forms.TextBox Ext;
		private System.Windows.Forms.TextBox MimeType;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label2;
	}
}
