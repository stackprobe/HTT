using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Security.AccessControl;
using System.Security.Principal;
using System.Windows.Forms;

// ^ sync @ GlobalProcMtx

namespace Charlotte
{
	// sync > @ GlobalProcMtx

	public class GlobalProcMtx
	{
		private static Mutex ProcMtx;

		public static bool Create(string procMtxName, string title)
		{
			try
			{
				MutexSecurity security = new MutexSecurity();

				security.AddAccessRule(
					new MutexAccessRule(
						new SecurityIdentifier(
							WellKnownSidType.WorldSid,
							null
							),
						MutexRights.FullControl,
						AccessControlType.Allow
						)
					);

				bool createdNew;
				ProcMtx = new Mutex(false, @"Global\Global_" + procMtxName, out createdNew, security);

				if (ProcMtx.WaitOne(0))
					return true;

				ProcMtx.Close();
				ProcMtx = null;
			}
			catch
			{ }

			CloseProcMtx();

			MessageBox.Show(
				"Already started on the other logon session !",
				title + " / Error",
				MessageBoxButtons.OK,
				MessageBoxIcon.Error
				);

			return false;
		}

		public static void Release()
		{
			CloseProcMtx();
		}

		private static void CloseProcMtx()
		{
			try { ProcMtx.ReleaseMutex(); }
			catch { }

			try { ProcMtx.Close(); }
			catch { }

			ProcMtx = null;
		}
	}

	// < sync
}
