using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace WHTT
{
	public static class LogTools
	{
		private static string Format = null; // "" == no format

		public static string FormatFilter(string text)
		{
			string formatFile = Path.Combine(BootTools.SelfDir, "WHTT_AccessLogFormat.conf");

			if (Format == null)
			{
				if (File.Exists(formatFile))
				{
					Format = File.ReadAllText(formatFile);

					Format = Format.Replace("\r", "");
					Format = Format.Replace("\n", "\r\n");
				}
				else
				{
					Format = "";
				}
			}
			if (Format != "")
			{
				text = DoFormatFilter(text);
			}
			return text;
		}

		private class Record_t
		{
			public string TimeStamp;
			public string ClientIP;
			public string Request;
			public string Host;
			public string UserAgent;
			public string Domain;
			public string TargetPath;
			public string Status;
		}

		private static Record_t Record;
		private static StringBuilder Dest;

		private static string DoFormatFilter(string src)
		{
			src = src.Replace("\r", "");
			string[] lines = src.Split('\n');

			Record = null;
			Dest = new StringBuilder();

			foreach (string line in lines)
			{
				if (line == "")
					continue;

				if (IsTimeStamp(line))
				{
					if (Record != null)
						WriteRecord();

					Record = new Record_t();
				}
				AddToRecord(line);
			}
			if (Record != null)
			{
				WriteRecord();
				Record = null;
			}
			string ret = Dest.ToString().Trim();
			Dest = null;
			return ret;
		}

		private const string DECIMAL = "0123456789";

		private static bool IsTimeStamp(string line)
		{
			return DECIMAL.Contains(line[0]) && line.Contains('/');
		}

		private static void AddToRecord(string line)
		{
			if (Record == null)
				return;

			switch (line[0])
			{
				case 'R': Record.Request = line.Substring(1); break;
				case 'H': Record.Host = line.Substring(1); break;
				case 'A': Record.UserAgent = line.Substring(1); break;
				case 'D': Record.Domain = line.Substring(1); break;
				case 'P': Record.TargetPath = line.Substring(1); break;
				case 'S': Record.Status = line.Substring(1); break;

				default:
					if (IsTimeStamp(line))
						Record.TimeStamp = line;
					else
						Record.ClientIP = line;

					break;
			}
		}

		private static void WriteRecord()
		{
			string ret = Format;

			ret = Replace(ret, "$TimeStamp", Record.TimeStamp);
			ret = Replace(ret, "$ClientIP", Record.ClientIP);
			ret = Replace(ret, "$Request", Record.Request);
			ret = Replace(ret, "$Host", Record.Host);
			ret = Replace(ret, "$UserAgent", Record.UserAgent);
			ret = Replace(ret, "$Domain", Record.Domain);
			ret = Replace(ret, "$TargetPath", Record.TargetPath);
			ret = Replace(ret, "$Status", Record.Status);

			Dest.Append(ret);

			if (300000 < Dest.Length)
				Dest.Remove(0, 100000);
		}

		private static string Replace(string str, string srcPtn, string destPtn)
		{
			if (destPtn == null)
				destPtn = "当該項目なし";
			else if (destPtn == "")
				destPtn = "値なし";

			return str.Replace(srcPtn, destPtn);
		}
	}
}
