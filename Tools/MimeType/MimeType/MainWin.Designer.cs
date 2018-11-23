namespace Charlotte
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
			this.MainSheet = new System.Windows.Forms.DataGridView();
			this.MainSheetMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
			this.選択解除CToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.BtnUp = new System.Windows.Forms.Button();
			this.BtnDown = new System.Windows.Forms.Button();
			this.BtnSave = new System.Windows.Forms.Button();
			this.BtnClose = new System.Windows.Forms.Button();
			this.BtnDelete = new System.Windows.Forms.Button();
			this.BtnAdd = new System.Windows.Forms.Button();
			this.BtnEdit = new System.Windows.Forms.Button();
			this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
			this.拡張子でソートSToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.mIMETypeでソートTToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			((System.ComponentModel.ISupportInitialize)(this.MainSheet)).BeginInit();
			this.MainSheetMenu.SuspendLayout();
			this.SuspendLayout();
			// 
			// MainSheet
			// 
			this.MainSheet.AllowUserToAddRows = false;
			this.MainSheet.AllowUserToDeleteRows = false;
			this.MainSheet.AllowUserToResizeRows = false;
			this.MainSheet.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.MainSheet.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
			this.MainSheet.ContextMenuStrip = this.MainSheetMenu;
			this.MainSheet.Location = new System.Drawing.Point(12, 12);
			this.MainSheet.MultiSelect = false;
			this.MainSheet.Name = "MainSheet";
			this.MainSheet.ReadOnly = true;
			this.MainSheet.RowHeadersVisible = false;
			this.MainSheet.RowTemplate.Height = 21;
			this.MainSheet.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
			this.MainSheet.Size = new System.Drawing.Size(454, 437);
			this.MainSheet.TabIndex = 0;
			this.MainSheet.CellContentClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.MainSheet_CellContentClick);
			this.MainSheet.CellDoubleClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.MainSheet_CellDoubleClick);
			// 
			// MainSheetMenu
			// 
			this.MainSheetMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.選択解除CToolStripMenuItem,
            this.toolStripMenuItem1,
            this.拡張子でソートSToolStripMenuItem,
            this.mIMETypeでソートTToolStripMenuItem});
			this.MainSheetMenu.Name = "MainSheetMenu";
			this.MainSheetMenu.Size = new System.Drawing.Size(185, 98);
			// 
			// 選択解除CToolStripMenuItem
			// 
			this.選択解除CToolStripMenuItem.Name = "選択解除CToolStripMenuItem";
			this.選択解除CToolStripMenuItem.Size = new System.Drawing.Size(184, 22);
			this.選択解除CToolStripMenuItem.Text = "選択解除(&C)";
			this.選択解除CToolStripMenuItem.Click += new System.EventHandler(this.選択解除CToolStripMenuItem_Click);
			// 
			// BtnUp
			// 
			this.BtnUp.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.BtnUp.Location = new System.Drawing.Point(472, 275);
			this.BtnUp.Name = "BtnUp";
			this.BtnUp.Size = new System.Drawing.Size(100, 30);
			this.BtnUp.TabIndex = 4;
			this.BtnUp.Text = "▲(&U)";
			this.BtnUp.UseVisualStyleBackColor = true;
			this.BtnUp.Click += new System.EventHandler(this.BtnUp_Click);
			// 
			// BtnDown
			// 
			this.BtnDown.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.BtnDown.Location = new System.Drawing.Point(472, 311);
			this.BtnDown.Name = "BtnDown";
			this.BtnDown.Size = new System.Drawing.Size(100, 30);
			this.BtnDown.TabIndex = 5;
			this.BtnDown.Text = "▼(&D)";
			this.BtnDown.UseVisualStyleBackColor = true;
			this.BtnDown.Click += new System.EventHandler(this.BtnDown_Click);
			// 
			// BtnSave
			// 
			this.BtnSave.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.BtnSave.Location = new System.Drawing.Point(472, 383);
			this.BtnSave.Name = "BtnSave";
			this.BtnSave.Size = new System.Drawing.Size(100, 30);
			this.BtnSave.TabIndex = 6;
			this.BtnSave.Text = "&OK";
			this.BtnSave.UseVisualStyleBackColor = true;
			this.BtnSave.Click += new System.EventHandler(this.BtnSave_Click);
			// 
			// BtnClose
			// 
			this.BtnClose.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.BtnClose.Location = new System.Drawing.Point(472, 419);
			this.BtnClose.Name = "BtnClose";
			this.BtnClose.Size = new System.Drawing.Size(100, 30);
			this.BtnClose.TabIndex = 7;
			this.BtnClose.Text = "&Cancel";
			this.BtnClose.UseVisualStyleBackColor = true;
			this.BtnClose.Click += new System.EventHandler(this.BtnClose_Click);
			// 
			// BtnDelete
			// 
			this.BtnDelete.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.BtnDelete.Location = new System.Drawing.Point(472, 120);
			this.BtnDelete.Name = "BtnDelete";
			this.BtnDelete.Size = new System.Drawing.Size(100, 30);
			this.BtnDelete.TabIndex = 3;
			this.BtnDelete.Text = "削除(&R)";
			this.BtnDelete.UseVisualStyleBackColor = true;
			this.BtnDelete.Click += new System.EventHandler(this.BtnDelete_Click);
			// 
			// BtnAdd
			// 
			this.BtnAdd.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.BtnAdd.Location = new System.Drawing.Point(472, 48);
			this.BtnAdd.Name = "BtnAdd";
			this.BtnAdd.Size = new System.Drawing.Size(100, 30);
			this.BtnAdd.TabIndex = 1;
			this.BtnAdd.Text = "追加(&A)";
			this.BtnAdd.UseVisualStyleBackColor = true;
			this.BtnAdd.Click += new System.EventHandler(this.BtnAdd_Click);
			// 
			// BtnEdit
			// 
			this.BtnEdit.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.BtnEdit.Location = new System.Drawing.Point(472, 84);
			this.BtnEdit.Name = "BtnEdit";
			this.BtnEdit.Size = new System.Drawing.Size(100, 30);
			this.BtnEdit.TabIndex = 2;
			this.BtnEdit.Text = "編集(&E)";
			this.BtnEdit.UseVisualStyleBackColor = true;
			this.BtnEdit.Click += new System.EventHandler(this.BtnEdit_Click);
			// 
			// toolStripMenuItem1
			// 
			this.toolStripMenuItem1.Name = "toolStripMenuItem1";
			this.toolStripMenuItem1.Size = new System.Drawing.Size(181, 6);
			// 
			// 拡張子でソートSToolStripMenuItem
			// 
			this.拡張子でソートSToolStripMenuItem.Name = "拡張子でソートSToolStripMenuItem";
			this.拡張子でソートSToolStripMenuItem.Size = new System.Drawing.Size(184, 22);
			this.拡張子でソートSToolStripMenuItem.Text = "拡張子でソート(&S)";
			this.拡張子でソートSToolStripMenuItem.Click += new System.EventHandler(this.拡張子でソートSToolStripMenuItem_Click);
			// 
			// mIMETypeでソートTToolStripMenuItem
			// 
			this.mIMETypeでソートTToolStripMenuItem.Name = "mIMETypeでソートTToolStripMenuItem";
			this.mIMETypeでソートTToolStripMenuItem.Size = new System.Drawing.Size(184, 22);
			this.mIMETypeでソートTToolStripMenuItem.Text = "MIME Type でソート(&T)";
			this.mIMETypeでソートTToolStripMenuItem.Click += new System.EventHandler(this.mIMETypeでソートTToolStripMenuItem_Click);
			// 
			// MainWin
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 20F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(584, 461);
			this.Controls.Add(this.BtnEdit);
			this.Controls.Add(this.BtnAdd);
			this.Controls.Add(this.BtnDelete);
			this.Controls.Add(this.BtnClose);
			this.Controls.Add(this.BtnSave);
			this.Controls.Add(this.BtnDown);
			this.Controls.Add(this.BtnUp);
			this.Controls.Add(this.MainSheet);
			this.Font = new System.Drawing.Font("メイリオ", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.Name = "MainWin";
			this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Show;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "MimeType";
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainWin_FormClosing);
			this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.MainWin_FormClosed);
			this.Load += new System.EventHandler(this.MainWin_Load);
			this.Shown += new System.EventHandler(this.MainWin_Shown);
			((System.ComponentModel.ISupportInitialize)(this.MainSheet)).EndInit();
			this.MainSheetMenu.ResumeLayout(false);
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.DataGridView MainSheet;
		private System.Windows.Forms.Button BtnUp;
		private System.Windows.Forms.Button BtnDown;
		private System.Windows.Forms.Button BtnSave;
		private System.Windows.Forms.Button BtnClose;
		private System.Windows.Forms.Button BtnDelete;
		private System.Windows.Forms.Button BtnAdd;
		private System.Windows.Forms.Button BtnEdit;
		private System.Windows.Forms.ContextMenuStrip MainSheetMenu;
		private System.Windows.Forms.ToolStripMenuItem 選択解除CToolStripMenuItem;
		private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
		private System.Windows.Forms.ToolStripMenuItem 拡張子でソートSToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem mIMETypeでソートTToolStripMenuItem;
	}
}

