using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace Charlotte.Tools
{
	public class WorkingDir : IDisposable
	{
		public static WorkingDir Root;

		public static WorkingDir CreateRoot()
		{
			return new WorkingDir(Path.Combine(Environment.GetEnvironmentVariable("TMP"), Program.APP_IDENT));
		}

		private string Dir;

		public WorkingDir(string dir)
		{
			if (Directory.Exists(dir))
				Directory.Delete(dir, true);

			Directory.CreateDirectory(dir);

			this.Dir = dir;
		}

		public WorkingDir Create()
		{
			return new WorkingDir(this.MakePath());
		}

		private long MakePathCounter = 0;

		public string MakePath()
		{
			//return this.GetPath(Guid.NewGuid().ToString("B"));
			//return this.GetPath(SecurityTools.MakePassword_9A());
			return this.GetPath("$" + this.MakePathCounter++);
		}

		public string GetPath(string localName)
		{
			return Path.Combine(this.Dir, localName);
		}

		public void Dispose()
		{
			if (this.Dir != null)
			{
				try
				{
					Directory.Delete(this.Dir, true);
				}
				catch (Exception e)
				{
					Program.PostMessage(e);
				}

				this.Dir = null;
			}
		}
	}
}
