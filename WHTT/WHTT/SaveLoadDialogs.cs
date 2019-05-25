using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Drawing;
using System.Windows.Forms;

// ^ sync @ SaveLoadDialogs

namespace WHTT
{
	// sync > @ SaveLoadDialogs

	public class SaveLoadDialogs
	{
		public static string SaveFile(string title, string filterString, string initialDir, string initialFile, Action<FileDialog> initializer = null)
		{
			return SaveLoadFile(title, filterString, initialDir, initialFile, initializer, true);
		}

		public static string LoadFile(string title, string filterString, string initialDir, string initialFile, Action<FileDialog> initializer = null)
		{
			return SaveLoadFile(title, filterString, initialDir, initialFile, initializer, false);
		}

		private static string SaveLoadFile(string title, string filterString, string initialDir, string initialFile, Action<FileDialog> initializer, bool saveFlag)
		{
			string homeDir = Directory.GetCurrentDirectory();
			try
			{
				using (FileDialog dlg = saveFlag ? (FileDialog)new SaveFileDialog() : (FileDialog)new OpenFileDialog())
				{
					dlg.Title = title;
					dlg.Filter = GetFilter(filterString);
					dlg.FilterIndex = GF_FilterIndex;
					//dlg.FilterIndex = 1; // old
					dlg.InitialDirectory = initialDir;
					dlg.FileName = initialFile;

					if (initializer != null)
						initializer(dlg);

					if (dlg.ShowDialog() == DialogResult.OK)
					{
						return dlg.FileName;
					}
				}
			}
			finally
			{
				Directory.SetCurrentDirectory(homeDir);
			}
			return null;
		}

		private static int GF_FilterIndex;

		private static string GetFilter(string filterString)
		{
			StringBuilder buff = new StringBuilder();

			GF_FilterIndex = 1;

			foreach (string fExtension in filterString.Split('.').Select(extension => extension.Trim()).Where(extension => extension != ""))
			{
				string name = fExtension.ToUpper();
				string extension = fExtension.ToLower();

				{
					int p = extension.IndexOf(':');

					if (p != -1)
					{
						name = extension.Substring(0, p);
						extension = extension.Substring(p + 1);
					}
				}

				GF_FilterIndex++;

				buff.Append(name + "ファイル(*." + extension + ")|*." + extension + "|");
			}
			buff.Append("すべてのファイル(*.*)|*.*");

			return buff.ToString();
		}

		public static bool SelectFolder(ref string dir, string description, Action<FolderBrowserDialog> initializer = null)
		{
			string homeDir = Directory.GetCurrentDirectory();
			try
			{
				using (FolderBrowserDialog dlg = new FolderBrowserDialog())
				{
					dlg.SelectedPath = dir;
					dlg.Description = description;

					if (initializer != null)
						initializer(dlg);

					if (dlg.ShowDialog() == DialogResult.OK)
					{
						dir = dlg.SelectedPath;
						return true;
					}
				}
			}
			finally
			{
				Directory.SetCurrentDirectory(homeDir);
			}
			return false;
		}

		public static bool SelectColor(ref Color color, int[] customColors = null, Action<ColorDialog> initializer = null)
		{
			if (customColors == null)
				customColors = GetDefCustomColors();

			using (ColorDialog dlg = new ColorDialog())
			{
				dlg.Color = color;
				dlg.CustomColors = customColors;

				if (initializer != null)
					initializer(dlg);

				if (dlg.ShowDialog() == DialogResult.OK)
				{
					color = dlg.Color;
					return true;
				}
			}
			return false;
		}

		private static int[] GetDefCustomColors()
		{
			int[] colors = new int[16];
			Random random = new Random();

			for (int index = 0; index < colors.Length; index++)
			{
				colors[index] = random.Next() & 0xffffff;
			}
			return colors;
		}
	}

	// < sync
}
