namespace WHTT
{
	partial class DocRootEditWin
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(DocRootEditWin));
			this.label1 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.Domain = new System.Windows.Forms.TextBox();
			this.RootDir = new System.Windows.Forms.TextBox();
			this.ErrorMessage = new System.Windows.Forms.Label();
			this.BtnRootDir = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Font = new System.Drawing.Font("メイリオ", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.label1.Location = new System.Drawing.Point(14, 18);
			this.label1.Margin = new System.Windows.Forms.Padding(5, 0, 5, 0);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(90, 24);
			this.label1.TabIndex = 0;
			this.label1.Text = "ドメイン名";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Font = new System.Drawing.Font("メイリオ", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.label2.Location = new System.Drawing.Point(14, 61);
			this.label2.Margin = new System.Windows.Forms.Padding(5, 0, 5, 0);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(106, 24);
			this.label2.TabIndex = 2;
			this.label2.Text = "公開フォルダ";
			// 
			// Domain
			// 
			this.Domain.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.Domain.Font = new System.Drawing.Font("メイリオ", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.Domain.Location = new System.Drawing.Point(130, 15);
			this.Domain.Margin = new System.Windows.Forms.Padding(5, 6, 5, 6);
			this.Domain.MaxLength = 300;
			this.Domain.Name = "Domain";
			this.Domain.Size = new System.Drawing.Size(471, 31);
			this.Domain.TabIndex = 1;
			this.Domain.TextChanged += new System.EventHandler(this.Domain_TextChanged);
			this.Domain.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.Domain_KeyPress);
			// 
			// RootDir
			// 
			this.RootDir.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.RootDir.Font = new System.Drawing.Font("メイリオ", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.RootDir.Location = new System.Drawing.Point(130, 58);
			this.RootDir.Margin = new System.Windows.Forms.Padding(5, 6, 5, 6);
			this.RootDir.Name = "RootDir";
			this.RootDir.ReadOnly = true;
			this.RootDir.Size = new System.Drawing.Size(428, 31);
			this.RootDir.TabIndex = 3;
			this.RootDir.Click += new System.EventHandler(this.RootDir_Click);
			this.RootDir.TextChanged += new System.EventHandler(this.RootDir_TextChanged);
			this.RootDir.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.RootDir_KeyPress);
			// 
			// ErrorMessage
			// 
			this.ErrorMessage.AutoSize = true;
			this.ErrorMessage.BackColor = System.Drawing.Color.Yellow;
			this.ErrorMessage.Font = new System.Drawing.Font("メイリオ", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.ErrorMessage.ForeColor = System.Drawing.Color.Red;
			this.ErrorMessage.Location = new System.Drawing.Point(131, 100);
			this.ErrorMessage.Margin = new System.Windows.Forms.Padding(5, 0, 5, 0);
			this.ErrorMessage.Name = "ErrorMessage";
			this.ErrorMessage.Size = new System.Drawing.Size(117, 24);
			this.ErrorMessage.TabIndex = 5;
			this.ErrorMessage.Text = "ErrorMessage";
			// 
			// BtnRootDir
			// 
			this.BtnRootDir.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.BtnRootDir.Font = new System.Drawing.Font("メイリオ", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.BtnRootDir.Location = new System.Drawing.Point(557, 58);
			this.BtnRootDir.Name = "BtnRootDir";
			this.BtnRootDir.Size = new System.Drawing.Size(44, 31);
			this.BtnRootDir.TabIndex = 4;
			this.BtnRootDir.Text = "...";
			this.BtnRootDir.UseVisualStyleBackColor = true;
			this.BtnRootDir.Click += new System.EventHandler(this.BtnRootDir_Click);
			// 
			// DocRootEditWin
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(10F, 24F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(615, 139);
			this.Controls.Add(this.BtnRootDir);
			this.Controls.Add(this.ErrorMessage);
			this.Controls.Add(this.RootDir);
			this.Controls.Add(this.Domain);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.label1);
			this.Font = new System.Drawing.Font("メイリオ", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Margin = new System.Windows.Forms.Padding(5, 6, 5, 6);
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "DocRootEditWin";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "公開フォルダの編集";
			this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.DocRootEditWin_FormClosed);
			this.Load += new System.EventHandler(this.DocRootEditWin_Load);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.TextBox Domain;
		private System.Windows.Forms.TextBox RootDir;
		private System.Windows.Forms.Label ErrorMessage;
		private System.Windows.Forms.Button BtnRootDir;
	}
}
