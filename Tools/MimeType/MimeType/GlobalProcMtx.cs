using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Charlotte
{
	public class GlobalProcMtx
	{
		private static System.Threading.Mutex ProcMtx;

		public static bool Create(string ident, string title)
		{
#if false
			System.Security.AccessControl.MutexSecurity security = new System.Security.AccessControl.MutexSecurity();

			security.AddAccessRule(
				new System.Security.AccessControl.MutexAccessRule(
					new System.Security.Principal.SecurityIdentifier(
						System.Security.Principal.WellKnownSidType.WorldSid,
						null
						),
					System.Security.AccessControl.MutexRights.FullControl,
					System.Security.AccessControl.AccessControlType.Allow
					)
				);

			bool createdNew;
			ProcMtx = new System.Threading.Mutex(false, @"Global\Global_" + ident, out createdNew, security);

			if (ProcMtx.WaitOne(0) == false)
			{
				ProcMtx.Close();
				ProcMtx = null;

				System.Windows.Forms.MessageBox.Show(
					"Already started on the other logon session !",
					title + " / Error",
					System.Windows.Forms.MessageBoxButtons.OK,
					System.Windows.Forms.MessageBoxIcon.Error
					);

				return false;
			}
			return true;
#else
			for (int c = 0; ; c++)
			{
				try
				{
					ProcMtx = new System.Threading.Mutex(false, @"Global\Global_" + ident);

					if (ProcMtx.WaitOne(0))
						break;

					ProcMtx.Close();
					ProcMtx = null;

					Program.PostMessage(new Exception());
				}
				catch (Exception e)
				{
					Program.PostMessage(e);
				}

				CloseProcMtx();

				if (8 < c)
				{
					System.Windows.Forms.MessageBox.Show(
						"Already started on the other logon session !",
						title + " / Error",
						System.Windows.Forms.MessageBoxButtons.OK,
						System.Windows.Forms.MessageBoxIcon.Error
						);

					return false;
				}

				{
					int millis;

					using (System.Security.Cryptography.RNGCryptoServiceProvider cRnd = new System.Security.Cryptography.RNGCryptoServiceProvider())
					{
						byte[] crByte = new byte[1];
						cRnd.GetBytes(crByte);
						millis = (int)crByte[0];
					}
					System.Threading.Thread.Sleep(millis);
				}
			}
			return true;
#endif
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
}
