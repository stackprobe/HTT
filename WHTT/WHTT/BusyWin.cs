using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using System.Security.Permissions;

namespace WHTT
{
	/// <summary>
	/// 使い方
	/// new BusyWin(l, t, w, h).Perform(runner);
	/// </summary>
	public partial class BusyWin : Form
	{
		#region ALT_F4 抑止

		[SecurityPermission(SecurityAction.LinkDemand, Flags = SecurityPermissionFlag.UnmanagedCode)]
		protected override void WndProc(ref Message m)
		{
			const int WM_SYSCOMMAND = 0x112;
			const long SC_CLOSE = 0xF060L;

			if (m.Msg == WM_SYSCOMMAND && (m.WParam.ToInt64() & 0xFFF0L) == SC_CLOSE)
			{
				return;
			}
			base.WndProc(ref m);
		}

		#endregion

		private Form OwnerWin; // null == オーナー無し
		private int InitL;
		private int InitT;
		private int WinW;
		private int WinH;

		public BusyWin(Form owner, int init_l, int init_t, int win_w, int win_h)
		{
			this.OwnerWin = owner;
			this.InitL = init_l;
			this.InitT = init_t;
			this.WinW = win_w;
			this.WinH = win_h;

			InitializeComponent();

			//this.Location = new Point(init_l, init_t); // ここで位置設定するとサイズがおかしくなる。-> BusyWin_Load へ
			//this.Size = new Size(win_w, win_h);
		}

		private Thread Runner;

		public void DoStart(Thread runner)
		{
			this.Runner = runner;
			this.Runner.Start();
		}

		private void BusyWin_Load(object sender, EventArgs e)
		{
			this.Location = new Point(this.InitL, this.InitT);
		}

		private bool ActivatedFlag;

		private void BusyWin_Activated(object sender, EventArgs e)
		{
			if (this.ActivatedFlag)
				return;

			this.ActivatedFlag = true;

			this.MT_Enabled = true;
			this.MainTimer.Enabled = true;

			if (this.OwnerWin != null)
				this.OwnerWin.Visible = false;
		}

		private bool MT_Enabled;
		private bool MT_Busy;
		private long MT_Count;

		private void MainTimer_Tick(object sender, EventArgs e)
		{
			if (this.MT_Enabled == false || this.MT_Busy)
				return;

			this.MT_Busy = true;

			try
			{
				if (this.MT_Count < 5)
					return;

				if (this.Runner.IsAlive == false)
				{
					this.MainTimer.Enabled = false;
					this.MT_Enabled = false;

					if (this.OwnerWin != null)
						this.OwnerWin.Visible = true;

					this.Close();
				}
			}
			catch (Exception ex)
			{
				this.MT_Enabled = false;
				throw ex;
			}
			finally
			{
				this.MT_Count++;
				this.MT_Busy = false;
			}
		}
	}
}
