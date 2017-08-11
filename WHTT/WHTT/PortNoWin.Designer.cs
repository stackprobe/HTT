namespace WHTT
{
	partial class PortNoWin
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(PortNoWin));
			this.PortNo = new System.Windows.Forms.TextBox();
			this.SuspendLayout();
			// 
			// PortNo
			// 
			this.PortNo.Font = new System.Drawing.Font("メイリオ", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.PortNo.Location = new System.Drawing.Point(12, 12);
			this.PortNo.MaxLength = 5;
			this.PortNo.Name = "PortNo";
			this.PortNo.Size = new System.Drawing.Size(204, 36);
			this.PortNo.TabIndex = 0;
			this.PortNo.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
			this.PortNo.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.PortNo_KeyPress);
			// 
			// PortNoWin
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(228, 63);
			this.Controls.Add(this.PortNo);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "PortNoWin";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "ポート番号";
			this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.PortNoWin_FormClosed);
			this.Load += new System.EventHandler(this.PortNoWin_Load);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.TextBox PortNo;

	}
}
