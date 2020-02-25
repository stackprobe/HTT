using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using System.IO;

namespace WHTT
{
	public class Counter
	{
		private int Count;

		public Counter(int initCount)
		{
			this.Count = initCount;
		}

		public void Increment()
		{
			this.Count++;
		}

		public void Decrement()
		{
			if (0 < this.Count)
			{
				this.Count--;
			}
		}

		public bool IsZero()
		{
			return this.Count == 0;
		}

		public LocalIncrementer IncrementSection()
		{
			return new LocalIncrementer(this);
		}

		public class LocalIncrementer : IDisposable
		{
			private Counter _c;

			public LocalIncrementer(Counter c)
			{
				this._c = c;
				this._c.Increment();
			}

			public void Dispose()
			{
				if (_c != null)
				{
					this._c.Decrement();
					this._c = null;
				}
			}
		}
	}

	public static class Tools
	{
		public static readonly Encoding ENCODING_SJIS = Encoding.GetEncoding(932);

		public static string Join(string[] tokens, string separator)
		{
			StringBuilder buff = new StringBuilder();

			for (int i = 0; i < tokens.Length; i++)
			{
				if (1 <= i)
				{
					buff.Append(separator);
				}
				buff.Append(tokens[i]);
			}
			return buff.ToString();
		}

		public static int ToRange(int value, int minval, int maxval)
		{
			value = Math.Max(value, minval);
			value = Math.Min(value, maxval);

			return value;
		}

		public static void ErrorDlg(Exception e)
		{
			try
			{
				MessageBox.Show("" + e, Program.APP_TITLE + " / エラー", MessageBoxButtons.OK, MessageBoxIcon.Error);
			}
			catch
			{ }
		}

		// sync > @ AntiWindowsDefenderSmartScreen

		public static void AntiWindowsDefenderSmartScreen()
		{
			WriteLog("awdss_1");

			if (Is初回起動())
			{
				WriteLog("awdss_2");

				foreach (string exeFile in Directory.GetFiles(BootTools.SelfDir, "*.exe", SearchOption.TopDirectoryOnly))
				{
					try
					{
						WriteLog("awdss_exeFile: " + exeFile);

						if (exeFile.ToLower() == BootTools.SelfFile.ToLower())
						{
							WriteLog("awdss_self_noop");
						}
						else
						{
							byte[] exeData = File.ReadAllBytes(exeFile);
							File.Delete(exeFile);
							File.WriteAllBytes(exeFile, exeData);
						}
						WriteLog("awdss_OK");
					}
					catch (Exception e)
					{
						WriteLog(e);
					}
				}
				WriteLog("awdss_3");
			}
			WriteLog("awdss_4");
		}

		// < sync

		public static bool Is初回起動()
		{
			return Gnd.I.Is初回起動();
		}

		private static string LogFile = null;
		private static long WL_Count = 0;

		public static void WriteLog(object message)
		{
			try
			{
				if (LogFile == null)
					LogFile = Path.Combine(BootTools.SelfDir, Path.GetFileNameWithoutExtension(BootTools.SelfFile) + ".log");

				using (StreamWriter writer = new StreamWriter(LogFile, WL_Count++ % 1000 != 0, Encoding.UTF8))
				{
					writer.WriteLine("[" + DateTime.Now + "] " + message);
				}
			}
			catch
			{ }
		}

		// sync > @ PostShown

		public static void PostShown_GetAllControl(Form f, Action<Control> reaction)
		{
			Queue<Control.ControlCollection> controlTable = new Queue<Control.ControlCollection>();

			controlTable.Enqueue(f.Controls);

			while (1 <= controlTable.Count)
			{
				foreach (Control control in controlTable.Dequeue())
				{
					GroupBox gb = control as GroupBox;

					if (gb != null)
					{
						controlTable.Enqueue(gb.Controls);
					}
					TabControl tc = control as TabControl;

					if (tc != null)
					{
						foreach (TabPage tp in tc.TabPages)
						{
							controlTable.Enqueue(tp.Controls);
						}
					}
					SplitContainer sc = control as SplitContainer;

					if (sc != null)
					{
						controlTable.Enqueue(sc.Panel1.Controls);
						controlTable.Enqueue(sc.Panel2.Controls);
					}
					Panel p = control as Panel;

					if (p != null)
					{
						controlTable.Enqueue(p.Controls);
					}
					reaction(control);
				}
			}
		}

		public static void PostShown(Form f)
		{
			PostShown_GetAllControl(f, control =>
			{
				Control c = new Control[]
				{
					control as TextBox,
					control as NumericUpDown,
				}
				.FirstOrDefault(v => v != null);

				if (c != null)
				{
					if (c.ContextMenuStrip == null)
					{
						ToolStripMenuItem item = new ToolStripMenuItem();

						item.Text = "項目なし";
						item.Enabled = false;

						ContextMenuStrip menu = new ContextMenuStrip();

						menu.Items.Add(item);

						c.ContextMenuStrip = menu;
					}
				}
			});
		}

		// < sync
	}

	public static class EventSet
	{
		[DllImport("kernel32.dll")]
		private static extern IntPtr CreateEvent(IntPtr lpEventAttributes, bool bManualReset, bool bInitialState, string lpName);

		[DllImport("kernel32.dll")]
		private static extern bool SetEvent(IntPtr hEvent);

		[DllImport("kernel32.dll", SetLastError = true)]
		[return: MarshalAs(UnmanagedType.Bool)]
		private static extern bool CloseHandle(IntPtr hObject);

		public static void Perform(string eventName)
		{
			try
			{
				IntPtr hdl = CreateEvent((IntPtr)0, false, false, eventName);

				if (hdl == (IntPtr)0)
					throw new Exception();

				SetEvent(hdl);
				CloseHandle(hdl);
			}
			catch
			{ }
		}
	}
}
