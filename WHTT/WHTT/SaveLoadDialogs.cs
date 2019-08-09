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

	public static class SaveLoadDialogs
	{
		public static string SaveFile(string title, string filterString, string initialFile)
		{
			// filterString == 拡張子1.拡張子2.拡張子3 ...

			string[] exts = filterString.Split('.');

			int filterIndex = exts.Select(ext => "." + ext.ToLower()).ToList().IndexOf(Path.GetExtension(initialFile).ToLower());
			filterIndex += exts.Length + 1;
			filterIndex %= exts.Length + 1;
			filterIndex++;

			return SaveFile(title, filterString, Path.GetDirectoryName(initialFile), Path.GetFileName(initialFile), dlg => dlg.FilterIndex = filterIndex);
		}

		public static string LoadFile(string title, string filterString, string initialFile)
		{
			// filterString == [総称:]拡張子1.拡張子2.拡張子3 ...

			string genericName = null;

			{
				int p = filterString.IndexOf(':');

				if (p != -1)
				{
					genericName = filterString.Substring(0, p);
					filterString = filterString.Substring(p + 1);
				}
			}

			string[] exts = filterString.Split('.');

			if (genericName == null)
				genericName = string.Join("_", exts);

			filterString = string.Join(";", filterString.Split('.').Select(ext => "*." + ext));
			filterString = genericName + "ファイル(" + filterString + ")|" + filterString + "|すべてのファイル(*.*)|*.*";

			int filterIndex = exts.Select(ext => "." + ext.ToLower()).Contains(Path.GetExtension(initialFile).ToLower()) ? 1 : 2;

			return LoadFile(title, "", Path.GetDirectoryName(initialFile), Path.GetFileName(initialFile), dlg =>
			{
				dlg.Filter = filterString;
				dlg.FilterIndex = filterIndex;
			});
		}

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
					//dlg.FilterIndex = GF_FilterIndex; // del
					dlg.FilterIndex = 1;
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

		//private static int GF_FilterIndex;

		private static string GetFilter(string filterString)
		{
			// filterString == [名前1:]拡張子1.[名前2:]拡張子2.[名前3:]拡張子3 ...

			StringBuilder buff = new StringBuilder();

			//GF_FilterIndex = 1;

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

				//GF_FilterIndex++;

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
