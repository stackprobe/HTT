using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Charlotte.Tools
{
	public class StringTools
	{
		public static Encoding ENCODING_SJIS = Encoding.GetEncoding(932);

		public static string BINADECIMAL = "01";
		public static string OCTODECIMAL = "012234567";
		public static string DECIMAL = "0123456789";
		public static string HEXADECIMAL = "0123456789ABCDEF";
		public static string hexadecimal = "0123456789abcdef";

		public static string ALPHA = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		public static string alpha = "abcdefghijklmnopqrstuvwxyz";
		public static string PUNCT =
			GetString_SJISHalfCodeRange(0x21, 0x2f) +
			GetString_SJISHalfCodeRange(0x3a, 0x40) +
			GetString_SJISHalfCodeRange(0x5b, 0x60) +
			GetString_SJISHalfCodeRange(0x7b, 0x7e);

		public static string ASCII = DECIMAL + ALPHA + alpha + PUNCT;
		public static string KANA = GetString_SJISHalfCodeRange(0xa1, 0xdf);

		public static string HALF = ASCII + KANA;

		public static string GetString_SJISHalfCodeRange(int codeMin, int codeMax)
		{
			List<byte> buff = new List<byte>();

			for (int code = codeMin; code <= codeMax; code++)
			{
				buff.Add((byte)code);
			}
			return ENCODING_SJIS.GetString(buff.ToArray());
		}

		public static string EncodeString(string str)
		{
			StringBuilder buff = new StringBuilder();

			foreach (char chr in str)
			{
				if (chr <= 0x20 || chr == '$')
				{
					buff.Append('$');
					buff.Append(((int)chr).ToString("x4"));
				}
				else
					buff.Append(chr);
			}
			return buff.ToString();
		}

		public static string DecodeString(string str)
		{
			StringBuilder buff = new StringBuilder();

			for (int index = 0; index < str.Length; index++)
			{
				char chr = str[index];

				if (chr == '$')
				{
					chr = (char)Convert.ToUInt16(str.Substring(index + 1, 4), 16);
					index += 4;
				}
				buff.Append(chr);
			}
			return buff.ToString();
		}

		public static string ToString(string[] strs)
		{
			List<string> dest = new List<string>();

			foreach (string str in strs)
			{
				dest.Add(EncodeString(str));
			}
			return string.Join("\r\n", dest);
		}

		public static string[] ToStrings(string str)
		{
			List<string> dest = new List<string>();

			foreach (string s in str.Replace("\r", "").Split('\n'))
			{
				dest.Add(DecodeString(s));
			}
			return dest.ToArray();
		}

		public static int Comp(string a, string b)
		{
			return BinTools.Comp(Encoding.UTF8.GetBytes(a), Encoding.UTF8.GetBytes(b));
		}

		public static int CompIgnoreCase(string a, string b)
		{
			return Comp(a.ToLower(), b.ToLower());
		}

		public static bool EqualsIgnoreCase(string a, string b)
		{
			return a.ToLower() == b.ToLower();
		}

		public static int IndexOfIgnoreCase(string str, string ptn)
		{
			return str.ToLower().IndexOf(ptn.ToLower());
		}
	}
}
