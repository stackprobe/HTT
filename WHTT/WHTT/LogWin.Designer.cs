namespace WHTT
{
	partial class LogWin
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(LogWin));
			this.LogText = new System.Windows.Forms.TextBox();
			this._クリップボードに貼り付けBtn = new System.Windows.Forms.Button();
			this._クリアBtn = new System.Windows.Forms.Button();
			this.MainTimer = new System.Windows.Forms.Timer(this.components);
			this.CB更新する = new System.Windows.Forms.CheckBox();
			this.SuspendLayout();
			// 
			// LogText
			// 
			this.LogText.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.LogText.Font = new System.Drawing.Font("メイリオ", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.LogText.Location = new System.Drawing.Point(13, 52);
			this.LogText.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.LogText.MaxLength = 0;
			this.LogText.Multiline = true;
			this.LogText.Name = "LogText";
			this.LogText.ReadOnly = true;
			this.LogText.ScrollBars = System.Windows.Forms.ScrollBars.Both;
			this.LogText.Size = new System.Drawing.Size(421, 271);
			this.LogText.TabIndex = 3;
			this.LogText.WordWrap = false;
			this.LogText.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.LogText_KeyPress);
			// 
			// _クリップボードに貼り付けBtn
			// 
			this._クリップボードに貼り付けBtn.Font = new System.Drawing.Font("メイリオ", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this._クリップボードに貼り付けBtn.Location = new System.Drawing.Point(13, 14);
			this._クリップボードに貼り付けBtn.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this._クリップボードに貼り付けBtn.Name = "_クリップボードに貼り付けBtn";
			this._クリップボードに貼り付けBtn.Size = new System.Drawing.Size(215, 28);
			this._クリップボードに貼り付けBtn.TabIndex = 0;
			this._クリップボードに貼り付けBtn.Text = "クリップボードにコピー";
			this._クリップボードに貼り付けBtn.UseVisualStyleBackColor = true;
			this._クリップボードに貼り付けBtn.Click += new System.EventHandler(this._クリップボードに貼り付けBtn_Click);
			// 
			// _クリアBtn
			// 
			this._クリアBtn.Font = new System.Drawing.Font("メイリオ", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this._クリアBtn.Location = new System.Drawing.Point(236, 14);
			this._クリアBtn.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this._クリアBtn.Name = "_クリアBtn";
			this._クリアBtn.Size = new System.Drawing.Size(83, 28);
			this._クリアBtn.TabIndex = 1;
			this._クリアBtn.Text = "クリア";
			this._クリアBtn.UseVisualStyleBackColor = true;
			this._クリアBtn.Click += new System.EventHandler(this._クリアBtn_Click);
			// 
			// MainTimer
			// 
			this.MainTimer.Enabled = true;
			this.MainTimer.Interval = 2000;
			this.MainTimer.Tick += new System.EventHandler(this.MainTimer_Tick);
			// 
			// CB更新する
			// 
			this.CB更新する.AutoSize = true;
			this.CB更新する.Location = new System.Drawing.Point(341, 17);
			this.CB更新する.Name = "CB更新する";
			this.CB更新する.Size = new System.Drawing.Size(80, 24);
			this.CB更新する.TabIndex = 2;
			this.CB更新する.Text = "自動更新";
			this.CB更新する.UseVisualStyleBackColor = true;
			this.CB更新する.CheckedChanged += new System.EventHandler(this.CB更新する_CheckedChanged);
			// 
			// LogWin
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 20F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(447, 337);
			this.Controls.Add(this.CB更新する);
			this.Controls.Add(this._クリアBtn);
			this.Controls.Add(this._クリップボードに貼り付けBtn);
			this.Controls.Add(this.LogText);
			this.Font = new System.Drawing.Font("メイリオ", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.Name = "LogWin";
			this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Show;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "HTT - アクセスログ";
			this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.LogWin_FormClosed);
			this.Load += new System.EventHandler(this.LogWin_Load);
			this.Shown += new System.EventHandler(this.LogWin_Shown);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.TextBox LogText;
		private System.Windows.Forms.Button _クリップボードに貼り付けBtn;
		private System.Windows.Forms.Button _クリアBtn;
		private System.Windows.Forms.Timer MainTimer;
		private System.Windows.Forms.CheckBox CB更新する;
	}
}
