namespace WHTT
{
	partial class DocRootWin
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
			this.components = new System.ComponentModel.Container();
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(DocRootWin));
			this.MainList = new System.Windows.Forms.ListBox();
			this.AddBtn = new System.Windows.Forms.Button();
			this.DelBtn = new System.Windows.Forms.Button();
			this.EdtBtn = new System.Windows.Forms.Button();
			this.UpBtn = new System.Windows.Forms.Button();
			this.DownBtn = new System.Windows.Forms.Button();
			this.MainTimer = new System.Windows.Forms.Timer(this.components);
			this.SuspendLayout();
			// 
			// MainList
			// 
			this.MainList.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.MainList.Font = new System.Drawing.Font("メイリオ", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.MainList.FormattingEnabled = true;
			this.MainList.ItemHeight = 24;
			this.MainList.Location = new System.Drawing.Point(12, 73);
			this.MainList.Name = "MainList";
			this.MainList.Size = new System.Drawing.Size(547, 316);
			this.MainList.TabIndex = 5;
			// 
			// AddBtn
			// 
			this.AddBtn.Font = new System.Drawing.Font("メイリオ", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.AddBtn.Location = new System.Drawing.Point(12, 12);
			this.AddBtn.Name = "AddBtn";
			this.AddBtn.Size = new System.Drawing.Size(125, 48);
			this.AddBtn.TabIndex = 0;
			this.AddBtn.Text = "追加";
			this.AddBtn.UseVisualStyleBackColor = true;
			this.AddBtn.Click += new System.EventHandler(this.AddBtn_Click);
			// 
			// DelBtn
			// 
			this.DelBtn.Font = new System.Drawing.Font("メイリオ", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.DelBtn.Location = new System.Drawing.Point(143, 12);
			this.DelBtn.Name = "DelBtn";
			this.DelBtn.Size = new System.Drawing.Size(125, 48);
			this.DelBtn.TabIndex = 1;
			this.DelBtn.Text = "削除";
			this.DelBtn.UseVisualStyleBackColor = true;
			this.DelBtn.Click += new System.EventHandler(this.DelBtn_Click);
			// 
			// EdtBtn
			// 
			this.EdtBtn.Font = new System.Drawing.Font("メイリオ", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.EdtBtn.Location = new System.Drawing.Point(274, 12);
			this.EdtBtn.Name = "EdtBtn";
			this.EdtBtn.Size = new System.Drawing.Size(125, 48);
			this.EdtBtn.TabIndex = 2;
			this.EdtBtn.Text = "変更";
			this.EdtBtn.UseVisualStyleBackColor = true;
			this.EdtBtn.Click += new System.EventHandler(this.EdtBtn_Click);
			// 
			// UpBtn
			// 
			this.UpBtn.Font = new System.Drawing.Font("メイリオ", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.UpBtn.Location = new System.Drawing.Point(405, 12);
			this.UpBtn.Name = "UpBtn";
			this.UpBtn.Size = new System.Drawing.Size(48, 48);
			this.UpBtn.TabIndex = 3;
			this.UpBtn.Text = "▲";
			this.UpBtn.UseVisualStyleBackColor = true;
			this.UpBtn.Click += new System.EventHandler(this.UpBtn_Click);
			// 
			// DownBtn
			// 
			this.DownBtn.Font = new System.Drawing.Font("メイリオ", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.DownBtn.Location = new System.Drawing.Point(459, 12);
			this.DownBtn.Name = "DownBtn";
			this.DownBtn.Size = new System.Drawing.Size(48, 48);
			this.DownBtn.TabIndex = 4;
			this.DownBtn.Text = "▼";
			this.DownBtn.UseVisualStyleBackColor = true;
			this.DownBtn.Click += new System.EventHandler(this.DownBtn_Click);
			// 
			// MainTimer
			// 
			this.MainTimer.Enabled = true;
			this.MainTimer.Tick += new System.EventHandler(this.MainTimer_Tick);
			// 
			// DocRootWin
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(571, 403);
			this.Controls.Add(this.DownBtn);
			this.Controls.Add(this.UpBtn);
			this.Controls.Add(this.EdtBtn);
			this.Controls.Add(this.DelBtn);
			this.Controls.Add(this.AddBtn);
			this.Controls.Add(this.MainList);
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MinimizeBox = false;
			this.Name = "DocRootWin";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "公開フォルダ";
			this.Activated += new System.EventHandler(this.DocRootWin_Activated);
			this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.DocRootWin_FormClosed);
			this.Load += new System.EventHandler(this.DocRootWin_Load);
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.ListBox MainList;
		private System.Windows.Forms.Button AddBtn;
		private System.Windows.Forms.Button DelBtn;
		private System.Windows.Forms.Button EdtBtn;
		private System.Windows.Forms.Button UpBtn;
		private System.Windows.Forms.Button DownBtn;
		private System.Windows.Forms.Timer MainTimer;

	}
}
