using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Windows.Forms;

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

	public class Tools
	{
		public static readonly Encoding ENCODING_SJIS = Encoding.GetEncoding(932);

		public static string Join(string[] tokens, string separator)
		{
			StringBuilder buff = new StringBuilder();

			for(int i = 0; i < tokens.Length; i++)
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
	}

	public class EventSet
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
