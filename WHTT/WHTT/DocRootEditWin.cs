using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace WHTT
{
	public partial class DocRootEditWin : Form
	{
		private DomainInfo DI;

		public DocRootEditWin(DomainInfo di)
		{
			this.DI = di;

			InitializeComponent();

			this.Domain.Text = di.Domain;
			this.RootDir.Text = di.RootDir;
			//this.RootDir.BackColor = this.Domain.BackColor;
			this.RootDir.SelectionStart = this.RootDir.Text.Length;
		}

		private void DocRootEditWin_Load(object sender, EventArgs e)
		{
			this.ErrorMessage.Text = "";
			this.MinimumSize = this.Size;
		}

		private void RootDir_Click(object sender, EventArgs e)
		{
			// noop
		}

		private void RootDir_TextChanged(object sender, EventArgs e)
		{
			// noop
		}

		private void DocRootEditWin_FormClosed(object sender, FormClosedEventArgs e)
		{
			this.DI.Domain = this.Domain.Text;
			this.DI.RootDir = this.RootDir.Text;

			this.DI.Domain = this.DomainFltr(this.DI.Domain);
			this.DI.RootDir = this.RootDirFltr(this.DI.RootDir);
		}

		private string DomainFltr(string domain)
		{
			if (domain == "")
				return "default";

			List<char> buff = new List<char>();

			foreach (char chr in domain)
			{
				if ((Gnd.I.ALPHA + Gnd.I.alpha + Gnd.I.DIGIT + ".-").IndexOf(chr) != -1)
				{
					buff.Add(chr);
				}
			}
			domain = new string(buff.ToArray());

			if (domain == "")
				domain = "missing-or-incorrect-domain-name";

			return domain;
		}

		private string RootDirFltr(string rootDir)
		{
			if (rootDir == "")
				rootDir = ".";

			return rootDir;
		}

		private void Domain_KeyPress(object sender, KeyPressEventArgs e)
		{
			if (e.KeyChar == (char)Keys.Enter)
			{
				e.Handled = true;
				this.Close();
			}
		}

		private void RootDir_KeyPress(object sender, KeyPressEventArgs e)
		{
			if (e.KeyChar == (char)Keys.Enter)
			{
				e.Handled = true;
				this.Close();
			}
			if (e.KeyChar == (char)Keys.Space)
			{
				e.Handled = true;
				this.BtnRootDir_Click(null, null);
			}
			if (e.KeyChar == (char)1) // CTRL_A
			{
				e.Handled = true;
				this.RootDir.SelectAll();
			}
		}

		private void Domain_TextChanged(object sender, EventArgs e)
		{
			if (this.Domain.Text != this.DomainFltr(this.Domain.Text))
				this.ErrorMessage.Text = "ドメイン名に問題があります。";
			else
				this.ErrorMessage.Text = "";
		}

		private void BtnRootDir_Click(object sender, EventArgs e)
		{
			this.ErrorMessage.Text = "";

			try
			{
				string homeDir = Directory.GetCurrentDirectory();

				try
				{
					using (FolderBrowserDialog fbd = new FolderBrowserDialog())
					{
						{
							string selPath = this.RootDir.Text;

							try
							{
								selPath = Path.GetFullPath(selPath);
							}
							catch
							{
								selPath = Directory.GetCurrentDirectory();
							}
							fbd.SelectedPath = selPath;
						}

						fbd.Description = "公開フォルダを選択して下さい。";

						if (fbd.ShowDialog() == DialogResult.OK) // using fbd
						{
							string selPath = fbd.SelectedPath;

							if (selPath.StartsWith("\\\\"))
								throw new Exception("ネットワークフォルダは指定できません。");

							if (4 <= homeDir.Length) // ? not root-dir
								if (selPath.StartsWith(homeDir + "\\", StringComparison.OrdinalIgnoreCase))
									selPath = selPath.Substring(homeDir.Length + 1);

							if (selPath != Tools.ENCODING_SJIS.GetString(Tools.ENCODING_SJIS.GetBytes(selPath)))
								throw new Exception("Shift_JIS に変換出来ない文字を含むパスは指定できません。");

							this.RootDir.Text = selPath;
							this.RootDir.Focus();
							this.RootDir.SelectAll();
						}
					}
				}
				finally
				{
					Directory.SetCurrentDirectory(homeDir);
				}
			}
			catch (Exception ex)
			{
				this.ErrorMessage.Text = ex.Message;
			}
		}
	}
}
