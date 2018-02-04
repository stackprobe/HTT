using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Threading;

namespace Charlotte.Tools
{
	public class FileTools
	{
		public static void Delete(string path)
		{
			if (File.Exists(path))
			{
				for (int c = 1; ; c++)
				{
					try
					{
						File.Delete(path);
					}
					catch
					{ }

					if (File.Exists(path) == false)
						break;

					if (10 <= c)
						throw new Exception("ファイルを削除出来ません。" + path);

					Thread.Sleep(c * 100);
				}
			}
			else
			{
				for (int c = 1; ; c++)
				{
					try
					{
						Directory.Delete(path, true);
					}
					catch
					{ }

					if (Directory.Exists(path) == false)
						break;

					if (10 <= c)
						throw new Exception("ディレクトリを削除出来ません。" + path);

					Thread.Sleep(c * 100);
				}
			}
		}

		public static void CreateDir(string dir)
		{
			for (int c = 1; ; c++)
			{
				try
				{
					Directory.CreateDirectory(dir);
				}
				catch
				{ }

				if (Directory.Exists(dir))
					break;

				if (10 <= c)
					throw new Exception("ディレクトリを作成出来ません。" + dir);

				Thread.Sleep(c * 100);
			}
		}
	}
}
