using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Security.Cryptography;

namespace Charlotte.Tools
{
	public class SecurityTools
	{
		public static Random CRandom = new Random(new RNGCryptoServiceProvider());

		public static string MakePassword(string allowChars, int length)
		{
			StringBuilder buff = new StringBuilder();

			for (int index = 0; index < length; index++)
			{
				buff.Append(allowChars[(int)CRandom.GetRandom((uint)allowChars.Length)]);
			}
			return buff.ToString();
		}

		public static string MakePassword()
		{
			return MakePassword(StringTools.DECIMAL + StringTools.ALPHA + StringTools.alpha, 22);
		}

		public static string MakePassword_9A()
		{
			return MakePassword(StringTools.DECIMAL + StringTools.ALPHA, 25);
		}

		public static string MakePassword_9()
		{
			return MakePassword(StringTools.DECIMAL, 39);
		}
	}
}
