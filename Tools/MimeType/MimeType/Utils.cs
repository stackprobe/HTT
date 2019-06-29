using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Charlotte.Tools;
using System.Windows.Forms;
using System.Reflection;

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
				ret = "error";

			return ret;
		}

		public static void EnableDoubleBuffer(Control control)
		{
			control.GetType().InvokeMember(
				"DoubleBuffered",
				BindingFlags.NonPublic | BindingFlags.Instance | BindingFlags.SetProperty,
				null,
				control,
				new object[] { true }
				);
		}
	}
}
