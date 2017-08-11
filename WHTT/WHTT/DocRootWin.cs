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
	public partial class DocRootWin : Form
	{
		public DocRootWin()
		{
			InitializeComponent();
		}

		private void DocRootWin_Load(object sender, EventArgs e)
		{
			this.MinimumSize = this.Size;
			this.RefreshUi();

			try
			{
				this.MainList.SelectedIndex = 0;
			}
			catch
			{ }
		}

		private void AddBtn_Click(object sender, EventArgs e)
		{
			if (Gnd.I.DOMAIN_INFO_NUM_MAX <= Gnd.I.DomainInfoList.Count)
				return;

			DomainInfo di = new DomainInfo()
			{
				//Domain = "domain-name", // old
				Domain = "default",
				RootDir = Directory.GetCurrentDirectory(),
			};
			Gnd.I.DomainInfoList.Add(di);

			using (Form f = new DocRootEditWin(di))
			{
				f.ShowDialog();
			}
			this._ドメイン重複チェック();

			this.RefreshUi();
			this.MainList.SelectedIndex = this.MainList.Items.Count - 1;
		}

		private void DelBtn_Click(object sender, EventArgs e)
		{
			if (Gnd.I.DomainInfoList.Count <= Gnd.I.DOMAIN_INFO_NUM_MIN)
				return;

			DomainInfo di = this.GetSelectedDomainInfo();
			Gnd.I.DomainInfoList.Remove(di);

			this.RefreshUi();
			this.MainList.SelectedIndex = Math.Min(this.LastSelIndex, this.MainList.Items.Count - 1);
		}

		private void EdtBtn_Click(object sender, EventArgs e)
		{
			DomainInfo di = this.GetSelectedDomainInfo();

			if (di != null)
			{
				using (Form f = new DocRootEditWin(di))
				{
					f.ShowDialog();
				}
				this._ドメイン重複チェック();
			}
			this.RefreshUi();
			this.MainList.SelectedIndex = this.LastSelIndex;
		}

		private int LastSelIndex = -1;

		private DomainInfo GetSelectedDomainInfo()
		{
			int index = this.MainList.SelectedIndex;

			if (index < 0 || Gnd.I.DomainInfoList.Count <= index)
				return null;

			this.LastSelIndex = index;
			return Gnd.I.DomainInfoList[index];
		}

		private void RefreshUi()
		{
			this.MainList.Items.Clear();

			foreach (DomainInfo di in Gnd.I.DomainInfoList)
			{
				this.MainList.Items.Add(di.Domain + " ==> " + di.RootDir);
			}
		}

		private void UpBtn_Click(object sender, EventArgs e)
		{
			if (this.GetSelectedDomainInfo() == null)
				return;

			if (this.LastSelIndex < 1)
				return;

			DomainInfo di = Gnd.I.DomainInfoList[this.LastSelIndex];
			Gnd.I.DomainInfoList[this.LastSelIndex] = Gnd.I.DomainInfoList[this.LastSelIndex - 1];
			Gnd.I.DomainInfoList[this.LastSelIndex - 1] = di;

			this.RefreshUi();
			this.MainList.SelectedIndex = this.LastSelIndex - 1;
		}

		private void DownBtn_Click(object sender, EventArgs e)
		{
			if (this.GetSelectedDomainInfo() == null)
				return;

			if (this.MainList.Items.Count - 1 <= this.LastSelIndex)
				return;

			DomainInfo di = Gnd.I.DomainInfoList[this.LastSelIndex];
			Gnd.I.DomainInfoList[this.LastSelIndex] = Gnd.I.DomainInfoList[this.LastSelIndex + 1];
			Gnd.I.DomainInfoList[this.LastSelIndex + 1] = di;

			this.RefreshUi();
			this.MainList.SelectedIndex = this.LastSelIndex + 1;
		}

		private void MainTimer_Tick(object sender, EventArgs e)
		{
			if (this.MT_Enabled == false)
				return;

			this.SetEnabled(this.AddBtn, this.MainList.Items.Count < Gnd.I.DOMAIN_INFO_NUM_MAX);
			this.SetEnabled(this.DelBtn, Gnd.I.DOMAIN_INFO_NUM_MIN < this.MainList.Items.Count);
			this.SetEnabled(this.EdtBtn, 1 <= this.MainList.Items.Count);
			this.SetEnabled(this.UpBtn, 1 <= this.MainList.SelectedIndex);
			this.SetEnabled(this.DownBtn, this.MainList.SelectedIndex < this.MainList.Items.Count - 1);
		}

		private void SetEnabled(Control ctrl, bool flag)
		{
			if (ctrl.Enabled != flag)
				ctrl.Enabled = flag;
		}

		private bool ActivatedFlag;
		private bool MT_Enabled;

		private void DocRootWin_Activated(object sender, EventArgs e)
		{
			if (this.ActivatedFlag)
				return;

			this.ActivatedFlag = true;
			this.MT_Enabled = true;
		}

		private void DocRootWin_FormClosed(object sender, FormClosedEventArgs e)
		{
			this.MT_Enabled = false;
		}

		private void _ドメイン重複チェック()
		{
			try
			{
				bool modified;
				int sdlgcnt = 0;

				do
				{
					modified = false;

					for (int ndx2 = 1; ndx2 < Gnd.I.DomainInfoList.Count; ndx2++)
					{
						for (int ndx1 = 0; ndx1 < ndx2; ndx1++)
						{
							DomainInfo di1 = Gnd.I.DomainInfoList[ndx1];
							DomainInfo di2 = Gnd.I.DomainInfoList[ndx2];

							if (string.Compare(di1.Domain, di2.Domain, true) == 0) // ? 一致 -> ドメイン重複
							{
								di2.Domain = DomainInc(di2.Domain);

#if false
								if (sdlgcnt < 1)
									MessageBox.Show(
										(ndx1 + 1) + " 行目と " + (ndx2 + 1) + " 行目のドメイン名が重複したため補正しました。",
										"情報",
										MessageBoxButtons.OK,
										MessageBoxIcon.Information
										);
#endif

								modified = true;
								sdlgcnt++;
							}
						}
					}
				}
				while (modified);
			}
			catch (Exception e)
			{
				Tools.ErrorDlg(e);
			}
		}

		private static string DomainInc(string str)
		{
			string ndxPart = null;

			{
				int dotPos = str.LastIndexOf('.');

				if (dotPos != -1)
				{
					ndxPart = str.Substring(dotPos + 1);
					str = str.Substring(0, dotPos);
				}
			}
			int ndx;

			try
			{
				ndx = int.Parse(ndxPart);
			}
			catch
			{
				ndx = -1;
			}
			return str + "." + (ndx + 1);
		}
	}
}
