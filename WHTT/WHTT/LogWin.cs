using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using System.IO;

namespace WHTT
{
	public partial class LogWin : Form
	{
		public LogWin()
		{
			InitializeComponent();
		}

		private void LogWin_Load(object sender, EventArgs e)
		{
			this.MinimumSize = this.Size;
		}

		private void LogWin_Shown(object sender, EventArgs e)
		{
			this.LoadAccessLog();
			Tools.PostShown(this);
			this.MT_Enabled = true;
		}

		private void LogWin_FormClosed(object sender, FormClosedEventArgs e)
		{
			this.MT_Enabled = false;
		}

		private void LoadAccessLog()
		{
			try
			{
				List<string> accessLog = this.GetAccessLog();

				if (accessLog == null)
					return;

				string text = Tools.Join(accessLog.ToArray(), "\r\n");

				text = LogTools.FormatFilter(text);

				if (this.LogText.Text == text)
					return;

				this.LogText.Text = text;
			}
			catch (Exception e)
			{
				try
				{
					this.LogText.Text = "" + e;
				}
				catch
				{
					try
					{
						this.LogText.Text = "読み込みエラー";
					}
					catch
					{ }
				}
			}

			try
			{
				string text = this.LogText.Text;
				int index = text.LastIndexOf('\n');

				if (index == -1)
					index = text.Length;
				else
					index++;

				this.LogText.SelectionStart = index;
				this.LogText.ScrollToCaret();
			}
			catch
			{ }
		}

		private List<string> GetAccessLog()
		{
			List<string> result = null;

			using (Mutex mtx = new Mutex(false, Gnd.I.ACCESS_LOG_MUTEX_NAME))
			{
				if (mtx.WaitOne(0))
				{
					try
					{
						result = this.GetAccessLog_NoLock();
					}
					catch
					{
						result = null;
					}
					finally
					{
						mtx.ReleaseMutex();
					}
				}
			}
			return result;
		}

		private List<string> GetAccessLog_NoLock()
		{
			List<string> result = new List<string>();

			this.Add(result, "AccessLog0.dat");
			this.Add(result, "AccessLog.dat");

			return result;
		}

		private void Add(List<string> result, string logFile)
		{
			if (File.Exists(logFile))
			{
				foreach (string ro_line in File.ReadAllLines(logFile, Tools.ENCODING_SJIS))
				{
					string line = ro_line;
					const int LINE_LEN_MAX = 1000;

					while (LINE_LEN_MAX < line.Length)
					{
						result.Add(line.Substring(0, LINE_LEN_MAX));
						line = line.Substring(LINE_LEN_MAX);
					}
					result.Add(line);
				}
			}
		}

		private void LogText_KeyPress(object sender, KeyPressEventArgs e)
		{
			char CTRL_A = (char)1;

			if (e.KeyChar == CTRL_A)
			{
				this.LogText.SelectAll();
				e.Handled = true;
			}
		}

		private void _クリップボードに貼り付けBtn_Click(object sender, EventArgs e)
		{
			try
			{
				string text = this.LogText.Text;

				if (text == "")
					Clipboard.Clear();
				else
					Clipboard.SetText(this.LogText.Text);
			}
			catch
			{ }
		}

		private void _クリアBtn_Click(object sender, EventArgs e)
		{
			using (Mutex mtx = new Mutex(false, Gnd.I.ACCESS_LOG_MUTEX_NAME))
			{
				if (mtx.WaitOne(2000))
				{
					try
					{
						File.Delete("AccessLog0.dat");
						File.Delete("AccessLog.dat");
					}
					catch
					{ }

					mtx.ReleaseMutex();
				}
			}
			this.LoadAccessLog();
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
				if (this.CB更新する.Checked)
				{
					this.LoadAccessLog();
				}
			}
			catch
			{
				// ignore
			}
			finally
			{
				this.MT_Count++;
				this.MT_Busy = false;
			}
		}

		private void CB更新する_CheckedChanged(object sender, EventArgs e)
		{
			// noop
		}
	}
}
