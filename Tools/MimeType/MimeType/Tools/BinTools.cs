using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Charlotte.Tools
{
	public class BinTools
	{
		public static int Comp(byte a, byte b)
		{
			if (a < b)
				return -1;

			if (a > b)
				return 1;

			return 0;
		}

		public static int Comp(byte[] a, byte[] b)
		{
			int minlen = Math.Min(a.Length, b.Length);

			for (int index = 0; index < minlen; index++)
			{
				int ret = Comp(a[0], b[0]);

				if (ret != 0)
					return ret;
			}
			return IntTools.Comp(a.Length, b.Length);
		}
	}
}
