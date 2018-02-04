using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Charlotte.Tools;

namespace Charlotte
{
	public partial class EditDlg : Form
	{
		private string[] OtherExts;

		public EditDlg(string ext, string mimeType, string[] otherExts)
		{
			InitializeComponent();

			this.MinimumSize = this.Size;

			this.Ext.Text = ext;
			this.MimeType.Text = mimeType;

			this.OtherExts = otherExts;
		}

		private void EditDlg_Load(object sender, EventArgs e)
		{
			// noop
		}

		private void BtnCancel_Click(object sender, EventArgs e)
		{
			this.Close();
		}

		public string RetExt;
		public string RetMimeType;

		public bool OKPressed = false;

		private void BtnOk_Click(object sender, EventArgs e)
		{
			try
			{
				this.Check();

				this.RetExt = this.Ext.Text;
				this.RetMimeType = this.MimeType.Text;

				this.OKPressed = true;
				this.Close();
			}
			catch (Exception ex)
			{
				MessageBox.Show(
					this,
					"" + ex,
					"登録できません",
					MessageBoxButtons.OK,
					MessageBoxIcon.Warning
					);
			}
		}

		private void Check()
		{
			if (this.Ext.Text == "")
				throw new Exception("拡張子を入力して下さい。");

			if (this.MimeType.Text == "")
				throw new Exception("MIME Type を入力して下さい。");

			if (Utils.ExtMimeTypeFilter(this.Ext.Text) != this.Ext.Text)
				throw new Exception("拡張子に使用できない文字が含まれています。");

			if (Utils.ExtMimeTypeFilter(this.MimeType.Text) != this.MimeType.Text)
				throw new Exception("MIME Type に使用できない文字が含まれています。");

			foreach (string otherExt in this.OtherExts)
			{
				if (StringTools.EqualsIgnoreCase(otherExt, this.Ext.Text))
				{
					throw new Exception("その拡張子は既に登録されています。");
				}
			}
		}

		private void Ext_KeyPress(object sender, KeyPressEventArgs e)
		{
			if (e.KeyChar == (char)13) // enter
			{
				this.MimeType.Focus();
				e.Handled = true;
			}
		}

		private void MimeType_KeyPress(object sender, KeyPressEventArgs e)
		{
			if (e.KeyChar == (char)13) // enter
			{
				this.BtnOk.Focus();
				e.Handled = true;
			}
		}
	}
}
