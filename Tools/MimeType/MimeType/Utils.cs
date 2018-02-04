using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Charlotte.Tools;

namespace Charlotte
{
	public class Utils
	{
		public static string ExtMimeTypeFilter(string str)
		{
			StringBuilder buff = new StringBuilder();

			foreach (char chr in str)
			{
				if (StringTools.HALF.Contains(chr))
				{
					buff.Append(chr);
				}
			}
			string ret = buff.ToString();

			if (ret == "")
				ret = "<error>";

			return ret;
		}
	}
}
