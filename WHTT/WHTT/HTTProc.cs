using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.Threading;
using System.IO;
using System.Windows.Forms;

namespace WHTT
{
	/// <summary>
	/// HTT.exe, Service.dat の場所とかめんどいので F5 でテストは断念。
	/// </summary>
	public class HTTProc
	{
		public Process Proc;

		public HTTProc()
		{
			try
			{
				Gnd.I.SaveData();

				ProcessStartInfo psi = new ProcessStartInfo();

				psi.FileName = "HTT.exe";
				psi.Arguments = "/P " + Gnd.I.PortNo + " /S Service.dat";
				psi.CreateNoWindow = true;
				psi.UseShellExecute = false;

				if (Gnd.I.ShowConsoleMode == Gnd.ShowConsoleMode_e.SHOW_MIN)
				{
					psi.CreateNoWindow = false;
					psi.UseShellExecute = true; // こうしないと最小化されない！
					psi.WindowStyle = ProcessWindowStyle.Minimized;
				}
				else if (Gnd.I.ShowConsoleMode == Gnd.ShowConsoleMode_e.SHOW_NORMAL)
				{
					psi.CreateNoWindow = false;
				}
				this.Proc = Process.Start(psi);
			}
			catch (Exception e)
			{
				this.Proc = null;

				Tools.ErrorDlg(e);
			}
		}

		public void EndProc(Form owner) // owner: null == オーナー無し
		{
			if (this.Proc == null)
				return;

			owner = null;

			using (BusyWin f = new BusyWin(owner, Gnd.I.MainWin_L, Gnd.I.MainWin_T, Gnd.I.MainWin_W, Gnd.I.MainWin_H))
			{
				f.DoStart(new Thread(this.EndProcMain));
				f.ShowDialog();
			}
		}

		public void EndProcMain()
		{
			if (this.Proc == null)
				return;

			for (int waitcnt = 1; ; waitcnt++)
			{
#if true
				EndProc_EventSetPerform();
#else
				ProcessStartInfo psi = new ProcessStartInfo();

				psi.FileName = "HTT.exe";
				psi.Arguments = "/T";
				psi.CreateNoWindow = true;
				psi.UseShellExecute = false;

				Process.Start(psi).WaitForExit();
#endif

				if (this.Proc.WaitForExit(3000)) // ? 終了した。
					break;

#if false
				if (waitcnt == 10) // ? 長時間終了しない -> 強制終了
				{
					this.Proc.Kill();
					break;
				}
#endif
			}
			this.Proc = null;

			for (int c = 0; c < 3; c++) // 2bs
			{
				Thread.Sleep(100);
				EndProc_EventSetPerform();
			}
		}

		public static void EndProc_EventSetPerform()
		{
			EventSet.Perform("cerulean.charlotte HTT after-school tea time server termination");
		}
	}
}
