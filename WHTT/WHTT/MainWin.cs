using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Threading;

namespace WHTT
{
	public partial class MainWin : Form
	{
		public MainWin()
		{
			InitializeComponent();

			Gnd.I.LoadData();
			Gnd.I.HTTProc = new HTTProc();

			this.RefreshUi();
		}

		private void MainWin_Load(object sender, EventArgs e)
		{
			// noop
		}

		private bool ActivatedFlag;
		private Counter TimerOff = new Counter(1);

		private void MainWin_Activated(object sender, EventArgs e)
		{
			if (this.ActivatedFlag)
				return;

			this.ActivatedFlag = true;
			this.TimerOff.Decrement(); // タイマーここから開始

			if (
				Gnd.I.KeepWinPosFlag &&
				Gnd.I.MainWin_L != int.MinValue &&
				Gnd.I.MainWin_T != int.MinValue
				)
			{
				this.Location = new Point(Gnd.I.MainWin_L, Gnd.I.MainWin_T);
			}
			this.SavePos(); // 最小化されると保存されないので、ここで確実に保存しておく..
		}

		private void MainWin_FormClosed(object sender, FormClosedEventArgs e)
		{
			this.TimerOff.Increment(); // タイマーここで死亡
			//this.Status.Text = "終了しています...";
			this.SavePos(); // BusyWinのため..

			if (Gnd.I.HTTProc != null) // SessionEnding 追加により、戻してみた！
			{
				Gnd.I.HTTProc.EndProc(null);
				Gnd.I.HTTProc = null;
			}
			//this.SavePos();
			Gnd.I.SaveData();
		}

		private void Mnu終了_Click(object sender, EventArgs e)
		{
			this.Close();
		}

		private void Mnu開始_Click(object sender, EventArgs e)
		{
			using (this.TimerOff.IncrementSection())
			{
				if (Gnd.I.HTTProc == null)
					Gnd.I.HTTProc = new HTTProc();

				this.RefreshUi();
			}
		}

		private void Mnu停止_Click(object sender, EventArgs e)
		{
			this.SavePos(); // BusyWinのため..

			using (this.TimerOff.IncrementSection())
			{
				if (Gnd.I.HTTProc != null)
				{
					Gnd.I.HTTProc.EndProc(this);
					Gnd.I.HTTProc = null;
				}
				this.RefreshUi();
			}
		}

		private void RefreshUi()
		{
			bool serverOn = Gnd.I.HTTProc != null;

			this.Status.Text = serverOn ? "サーバーは稼動中です" : "サーバーは停止しています";
			this.Status.Enabled = serverOn;

			this.Mnu開始.Enabled = serverOn == false;
			this.Mnu停止.Enabled = serverOn;
		}

		private long MT_Count;

		private void MainTimer_Tick(object sender, EventArgs e)
		{
			if (this.TimerOff.IsZero() == false)
				return;

			using (this.TimerOff.IncrementSection())
			{
				bool uiChange = false;

				if (Gnd.I.HTTProc != null)
				{
					if (Gnd.I.HTTProc.Proc != null)
						if (Gnd.I.HTTProc.Proc.WaitForExit(0)) // ? 終了した
							Gnd.I.HTTProc.Proc = null;

					if (Gnd.I.HTTProc.Proc == null)
					{
						Gnd.I.HTTProc = null;
						uiChange = true;
					}
				}
				if (uiChange)
				{
					this.RefreshUi();
				}
				if (MT_Count % 100 == 0)
				{
					GC.Collect();
				}
				MT_Count++;
			}
		}

		private void SavePos()
		{
			if (this.WindowState != FormWindowState.Normal)
				return;

			Gnd.I.MainWin_L = this.Location.X;
			Gnd.I.MainWin_T = this.Location.Y;
			Gnd.I.MainWin_W = this.Size.Width;
			Gnd.I.MainWin_H = this.Size.Height;
		}

		private void Mnuポート番号_Click(object sender, EventArgs e)
		{
			this.SavePos(); // BusyWinのため..

			using (this.TimerOff.IncrementSection())
			{
				using (Form f = new PortNoWin())
				{
					f.ShowDialog();
				}

				try
				{
					int portNo = PortNoWin.LastPortNo;

					if (portNo < 1 || 65535 < portNo)
					{
						MessageBox.Show(
							"指定されたポート番号は使用できません。\n" +
							"(ポート番号は変更されません)",
							"ポート番号の変更",
							MessageBoxButtons.OK,
							MessageBoxIcon.Warning
							);
					}
					else if (portNo == Gnd.I.PortNo)
					{
						// 変更されなかった。
					}
					else
					{
						Gnd.I.PortNo = portNo;

						MessageBox.Show(
							"ポート番号は " + Gnd.I.PortNo + " に変更されました。",
							"ポート番号の変更",
							MessageBoxButtons.OK,
							MessageBoxIcon.Information
							);
					}
				}
				catch
				{ }

				this.RestartServer();
			}
		}

		private void Mnu公開フォルダ_Click(object sender, EventArgs e)
		{
			this.SavePos(); // BusyWinのため..

			using (this.TimerOff.IncrementSection())
			{
				this.Visible = false;

				using (Form f = new DocRootWin())
				{
					f.ShowDialog();
				}
				this.Visible = true;

				this.RestartServer();
			}
		}

		private void RestartServer()
		{
			if (Gnd.I.HTTProc != null)
			{
				Gnd.I.HTTProc.EndProc(this);
				Gnd.I.HTTProc = new HTTProc();
			}
		}

		private void Mnuアクセスログ_Click(object sender, EventArgs e)
		{
			using (this.TimerOff.IncrementSection())
			{
				this.Visible = false;

				using (Form f = new LogWin())
				{
					f.ShowDialog();
				}
				this.Visible = true;
			}
		}
	}
}
