namespace WHTT
{
	partial class MainWin
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainWin));
			this.menuStrip1 = new System.Windows.Forms.MenuStrip();
			this.アプリケーションAToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.Mnu開始 = new System.Windows.Forms.ToolStripMenuItem();
			this.Mnu停止 = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
			this.Mnu終了 = new System.Windows.Forms.ToolStripMenuItem();
			this.設定SToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.Mnuポート番号 = new System.Windows.Forms.ToolStripMenuItem();
			this.Mnu公開フォルダ = new System.Windows.Forms.ToolStripMenuItem();
			this.アクセスログLToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.Mnuアクセスログ = new System.Windows.Forms.ToolStripMenuItem();
			this.statusStrip1 = new System.Windows.Forms.StatusStrip();
			this.Status = new System.Windows.Forms.Label();
			this.MainTimer = new System.Windows.Forms.Timer(this.components);
			this.menuStrip1.SuspendLayout();
			this.SuspendLayout();
			// 
			// menuStrip1
			// 
			this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.アプリケーションAToolStripMenuItem,
            this.設定SToolStripMenuItem,
            this.アクセスログLToolStripMenuItem});
			this.menuStrip1.Location = new System.Drawing.Point(0, 0);
			this.menuStrip1.Name = "menuStrip1";
			this.menuStrip1.Size = new System.Drawing.Size(324, 26);
			this.menuStrip1.TabIndex = 0;
			this.menuStrip1.Text = "menuStrip1";
			// 
			// アプリケーションAToolStripMenuItem
			// 
			this.アプリケーションAToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.Mnu開始,
            this.Mnu停止,
            this.toolStripMenuItem1,
            this.Mnu終了});
			this.アプリケーションAToolStripMenuItem.Name = "アプリケーションAToolStripMenuItem";
			this.アプリケーションAToolStripMenuItem.Size = new System.Drawing.Size(134, 22);
			this.アプリケーションAToolStripMenuItem.Text = "アプリケーション(&A)";
			// 
			// Mnu開始
			// 
			this.Mnu開始.Name = "Mnu開始";
			this.Mnu開始.Size = new System.Drawing.Size(118, 22);
			this.Mnu開始.Text = "開始(&S)";
			this.Mnu開始.Click += new System.EventHandler(this.Mnu開始_Click);
			// 
			// Mnu停止
			// 
			this.Mnu停止.Name = "Mnu停止";
			this.Mnu停止.Size = new System.Drawing.Size(118, 22);
			this.Mnu停止.Text = "停止(&T)";
			this.Mnu停止.Click += new System.EventHandler(this.Mnu停止_Click);
			// 
			// toolStripMenuItem1
			// 
			this.toolStripMenuItem1.Name = "toolStripMenuItem1";
			this.toolStripMenuItem1.Size = new System.Drawing.Size(115, 6);
			// 
			// Mnu終了
			// 
			this.Mnu終了.Name = "Mnu終了";
			this.Mnu終了.Size = new System.Drawing.Size(118, 22);
			this.Mnu終了.Text = "終了(&X)";
			this.Mnu終了.Click += new System.EventHandler(this.Mnu終了_Click);
			// 
			// 設定SToolStripMenuItem
			// 
			this.設定SToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.Mnuポート番号,
            this.Mnu公開フォルダ});
			this.設定SToolStripMenuItem.Name = "設定SToolStripMenuItem";
			this.設定SToolStripMenuItem.Size = new System.Drawing.Size(62, 22);
			this.設定SToolStripMenuItem.Text = "設定(&S)";
			// 
			// Mnuポート番号
			// 
			this.Mnuポート番号.Name = "Mnuポート番号";
			this.Mnuポート番号.Size = new System.Drawing.Size(165, 22);
			this.Mnuポート番号.Text = "ポート番号(&P)";
			this.Mnuポート番号.Click += new System.EventHandler(this.Mnuポート番号_Click);
			// 
			// Mnu公開フォルダ
			// 
			this.Mnu公開フォルダ.Name = "Mnu公開フォルダ";
			this.Mnu公開フォルダ.Size = new System.Drawing.Size(165, 22);
			this.Mnu公開フォルダ.Text = "公開フォルダ(&F)";
			this.Mnu公開フォルダ.Click += new System.EventHandler(this.Mnu公開フォルダ_Click);
			// 
			// アクセスログLToolStripMenuItem
			// 
			this.アクセスログLToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.Mnuアクセスログ});
			this.アクセスログLToolStripMenuItem.Name = "アクセスログLToolStripMenuItem";
			this.アクセスログLToolStripMenuItem.Size = new System.Drawing.Size(74, 22);
			this.アクセスログLToolStripMenuItem.Text = "ツール(&T)";
			// 
			// Mnuアクセスログ
			// 
			this.Mnuアクセスログ.Name = "Mnuアクセスログ";
			this.Mnuアクセスログ.Size = new System.Drawing.Size(165, 22);
			this.Mnuアクセスログ.Text = "アクセスログ(&L)";
			this.Mnuアクセスログ.Click += new System.EventHandler(this.Mnuアクセスログ_Click);
			// 
			// statusStrip1
			// 
			this.statusStrip1.Location = new System.Drawing.Point(0, 105);
			this.statusStrip1.Name = "statusStrip1";
			this.statusStrip1.Size = new System.Drawing.Size(324, 22);
			this.statusStrip1.SizingGrip = false;
			this.statusStrip1.TabIndex = 1;
			this.statusStrip1.Text = "statusStrip1";
			// 
			// Status
			// 
			this.Status.AutoSize = true;
			this.Status.Font = new System.Drawing.Font("メイリオ", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.Status.Location = new System.Drawing.Point(36, 52);
			this.Status.Name = "Status";
			this.Status.Size = new System.Drawing.Size(164, 28);
			this.Status.TabIndex = 2;
			this.Status.Text = "現在のステータス";
			// 
			// MainTimer
			// 
			this.MainTimer.Enabled = true;
			this.MainTimer.Interval = 2000;
			this.MainTimer.Tick += new System.EventHandler(this.MainTimer_Tick);
			// 
			// MainWin
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(324, 127);
			this.Controls.Add(this.Status);
			this.Controls.Add(this.statusStrip1);
			this.Controls.Add(this.menuStrip1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MainMenuStrip = this.menuStrip1;
			this.MaximizeBox = false;
			this.Name = "MainWin";
			this.Text = "HTT";
			this.Activated += new System.EventHandler(this.MainWin_Activated);
			this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.MainWin_FormClosed);
			this.Load += new System.EventHandler(this.MainWin_Load);
			this.menuStrip1.ResumeLayout(false);
			this.menuStrip1.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.MenuStrip menuStrip1;
		private System.Windows.Forms.StatusStrip statusStrip1;
		private System.Windows.Forms.ToolStripMenuItem アプリケーションAToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem Mnu終了;
		private System.Windows.Forms.ToolStripMenuItem 設定SToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem Mnuポート番号;
		private System.Windows.Forms.ToolStripMenuItem Mnu公開フォルダ;
		private System.Windows.Forms.ToolStripMenuItem Mnu開始;
		private System.Windows.Forms.ToolStripMenuItem Mnu停止;
		private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
		private System.Windows.Forms.Label Status;
		private System.Windows.Forms.Timer MainTimer;
		private System.Windows.Forms.ToolStripMenuItem アクセスログLToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem Mnuアクセスログ;


	}
}

