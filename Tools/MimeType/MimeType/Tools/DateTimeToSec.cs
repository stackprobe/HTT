using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Charlotte.Tools
{
	public class DateTimeToSec
	{
		public static long ToSec(long dateTime)
		{
			if (dateTime < 10000101000000L || 99991231235959L < dateTime)
				return 0L;

			int s = (int)(dateTime % 100L);
			dateTime /= 100L;
			int i = (int)(dateTime % 100L);
			dateTime /= 100L;
			int h = (int)(dateTime % 100L);
			dateTime /= 100L;
			int d = (int)(dateTime % 100L);
			dateTime /= 100L;
			int m = (int)(dateTime % 100L);
			int y = (int)(dateTime / 100L);

			if (
				y < 1000 || 9999 < y ||
				m < 1 || 12 < m ||
				d < 1 || 31 < d ||
				h < 0 || 23 < h ||
				i < 0 || 59 < i ||
				s < 0 || 59 < s
				)
				return 0L;

			if (m <= 2)
				y--;

			long ret = y / 400;
			ret *= 365 * 400 + 97;

			y %= 400;

			ret += y * 365;
			ret += y / 4;
			ret -= y / 100;

			if (2 < m)
			{
				ret -= 31 * 10 - 4;
				m -= 3;
				ret += (m / 5) * (31 * 5 - 2);
				m %= 5;
				ret += (m / 2) * (31 * 2 - 1);
				m %= 2;
				ret += m * 31;
			}
			else
				ret += (m - 1) * 31;

			ret += d - 1;
			ret *= 24;
			ret += h;
			ret *= 60;
			ret += i;
			ret *= 60;
			ret += s;

			return ret;
		}

		public static long ToDateTime(long sec)
		{
			if (sec < 0L)
				return 10000101000000L;

			int s = (int)(sec % 60L);
			sec /= 60L;
			int i = (int)(sec % 60L);
			sec /= 60L;
			int h = (int)(sec % 24L);
			sec /= 24L;

			if ((long)int.MaxValue < sec)
				return 99991231235959L;

			int day = (int)sec;
			int y = (day / 146097) * 400 + 1;
			int m = 1;
			int d;
			day %= 146097;

			day += Math.Min((day + 306) / 36524, 3);
			y += (day / 1461) * 4;
			day %= 1461;

			day += Math.Min((day + 306) / 365, 3);
			y += day / 366;
			day %= 366;

			if (60 <= day)
			{
				m += 2;
				day -= 60;
				m += (day / 153) * 5;
				day %= 153;
				m += (day / 61) * 2;
				day %= 61;
			}
			m += day / 31;
			day %= 31;
			d = day + 1;

			if (y < 1000)
				return 10000101000000L;

			if (9999 < y)
				return 99991231235959L;

			if (
				//y < 1000 || 9999 < y ||
				m < 1 || 12 < m ||
				d < 1 || 31 < d ||
				h < 0 || 23 < h ||
				m < 0 || 59 < m ||
				s < 0 || 59 < s
				)
				throw null; // never

			return
				y * 10000000000L +
				m * 100000000L +
				d * 1000000L +
				h * 10000L +
				i * 100L +
				s;
		}

		public static class Now
		{
			public static long GetSec()
			{
				return ToSec(GetDateTime());
			}

			public static long GetDateTime()
			{
				DateTime dt = DateTime.Now;

				return
					10000000000L * dt.Year +
					100000000L * dt.Month +
					1000000L * dt.Day +
					10000L * dt.Hour +
					100L * dt.Minute +
					dt.Second;
			}
		}

		public static readonly long POSIX_ZERO = ToSec(19700101000000L);
	}
}
