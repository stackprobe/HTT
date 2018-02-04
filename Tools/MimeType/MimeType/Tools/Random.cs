using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Security.Cryptography;

namespace Charlotte.Tools
{
	public class Random
	{
		private RandomNumberGenerator Rng;

		public Random(RandomNumberGenerator rng)
		{
			this.Rng = rng;
		}

		private const int CACHE_SIZE = 4096;
		private byte[] Cache = new byte[CACHE_SIZE];
		private int RIndex = CACHE_SIZE;

		public byte GetByte()
		{
			if (CACHE_SIZE <= this.RIndex)
			{
				this.Rng.GetBytes(this.Cache);
				this.RIndex = 0;
			}
			return this.Cache[this.RIndex++];
		}

		public uint GetUInt16()
		{
			return
				((uint)this.GetByte() << 8) |
				((uint)this.GetByte() << 0);
		}

		public uint GetUInt()
		{
			return
				(this.GetUInt16() << 16) |
				(this.GetUInt16() << 0);
		}

		public ulong GetUInt64()
		{
			return
				((ulong)this.GetUInt() << 32) |
				((ulong)this.GetUInt() << 0);
		}

		public ulong GetRandom64(ulong modulo)
		{
			if (modulo == 0UL)
				throw new ArgumentOutOfRangeException("modulo == 0");

			if (modulo == 1UL)
				return 0UL;

			ulong r_mod = (ulong.MaxValue % modulo + 1UL) % modulo;
			ulong r;

			do
			{
				r = this.GetUInt64();
			}
			while (r < r_mod);

			r %= modulo;

			return r;
		}

		public uint GetRandom(uint modulo)
		{
			return (uint)this.GetRandom64((ulong)modulo);
		}

		public long GetRange64(long minval, long maxval)
		{
			return (long)this.GetRandom64((ulong)(maxval + 1L - minval)) + minval;
		}

		public int GetRange(int minval, int maxval)
		{
			return (int)this.GetRandom((uint)(maxval + 1 - minval)) + minval;
		}
	}
}
