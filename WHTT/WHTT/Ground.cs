using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace WHTT
{
	public class Gnd
	{
		private Gnd()
		{ }

		public static Gnd I = new Gnd();

		public readonly string ALPHA = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		public readonly string alpha = "abcdefghijklmnopqrstuvwxyz";
		public readonly string DIGIT = "0123456789";

		public readonly string ACCESS_LOG_MUTEX_NAME = "{4e08b31a-9280-4ee0-9f9a-2ef462589893}"; // shared_uuid

		public HTTProc HTTProc;

		public enum ShowConsoleMode_e
		{
			HIDE,
			SHOW_MIN,
			SHOW_NORMAL,
		};

		public int PortNo = 80;
		public ShowConsoleMode_e ShowConsoleMode = ShowConsoleMode_e.HIDE; // 隠しモード
		public List<DomainInfo> DomainInfoList = new List<DomainInfo>();

		public int MainWin_L = int.MinValue;
		public int MainWin_T = int.MinValue;
		public int MainWin_W = int.MinValue;
		public int MainWin_H = int.MinValue;
		public bool KeepWinPosFlag = true; // 隠しモード

		public readonly int DOMAIN_INFO_NUM_MIN = 1;
		public readonly int DOMAIN_INFO_NUM_MAX = 1000;

		public void LoadData()
		{
			try
			{
				string[] lines = File.ReadAllLines("WHTT.dat", Tools.ENCODING_SJIS);
				int c = 0;

				// ---- data ----

				this.PortNo = Tools.ToRange(int.Parse(lines[c++]), 1, 65535);
				this.ShowConsoleMode = (ShowConsoleMode_e)Tools.ToRange(int.Parse(lines[c++]), 0, 2);
				this.MainWin_L = int.Parse(lines[c++]);
				this.MainWin_T = int.Parse(lines[c++]);
				this.KeepWinPosFlag = int.Parse(lines[c++]) != 0;

				// ----

				File.Delete("WHTT.exe.dat"); // old ver
			}
			catch
			{ }

			this.DomainInfoList.Clear();

			try
			{
				string[] lines = File.ReadAllLines("DocRoot.dat", Tools.ENCODING_SJIS);

				foreach (string line in lines)
				{
					int sp = line.IndexOf(' ');

					DomainInfo di = new DomainInfo()
					{
						Domain = line.Substring(0, sp),
						RootDir = line.Substring(sp + 1),
					};

					if (di.Domain == "")
						di.Domain = "default";

					if (di.RootDir == "")
						di.RootDir = ".";

					this.DomainInfoList.Add(di);
				}
			}
			catch
			{ }
		}

		public void SaveData()
		{
			try
			{
				List<string> lines = new List<string>();

				// ---- data ----

				lines.Add("" + this.PortNo);
				lines.Add("" + (int)this.ShowConsoleMode);
				lines.Add("" + this.MainWin_L);
				lines.Add("" + this.MainWin_T);
				lines.Add("" + (this.KeepWinPosFlag ? 1 : 0));

				// ----

				File.WriteAllLines("WHTT.dat", lines, Encoding.ASCII);
			}
			catch
			{ }

			try
			{
				List<string> lines = new List<string>();

				foreach (DomainInfo di in this.DomainInfoList)
				{
					lines.Add(di.Domain + " " + di.RootDir);
				}
				File.WriteAllLines("DocRoot.dat", lines.ToArray(), Tools.ENCODING_SJIS);
			}
			catch
			{ }
		}

		public bool Is初回起動()
		{
			return File.Exists("WHTT.dat") == false; // ? SaveData()を1度も実行していない。
		}
	}

	public class DomainInfo
	{
		public string Domain;
		public string RootDir;
	}
}
