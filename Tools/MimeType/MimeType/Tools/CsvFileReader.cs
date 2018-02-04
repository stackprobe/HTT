using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace Charlotte.Tools
{
	public class CsvFileReader : IDisposable
	{
		//private const char DELIMITER = ',';
		private const char DELIMITER = '\t';

		private StreamReader Reader;

		public CsvFileReader(string file)
		{
			this.Reader = new StreamReader(file, StringTools.ENCODING_SJIS);
		}

		private int LastChar;

		private int ReadChar()
		{
			do
			{
				this.LastChar = this.Reader.Read();
			}
			while (this.LastChar == '\r');

			return this.LastChar;
		}

		private bool EnclosedCell;

		private string ReadCell()
		{
			StringBuilder buff = new StringBuilder();

			if (this.ReadChar() == '"')
			{
				while (this.ReadChar() != -1 && (this.LastChar != '"' || this.ReadChar() == '"'))
				{
					buff.Append((char)this.LastChar);
				}
				this.EnclosedCell = true;
			}
			else
			{
				while (this.LastChar != -1 && this.LastChar != '\n' && this.LastChar != DELIMITER)
				{
					buff.Append((char)this.LastChar);
					this.ReadChar();
				}
				this.EnclosedCell = false;
			}
			return buff.ToString();
		}

		public string[] ReadRow()
		{
			List<string> row = new List<string>();

			do
			{
				row.Add(this.ReadCell());
			}
			while (this.LastChar != -1 && this.LastChar != '\n');

			if (this.LastChar == -1 && row.Count == 1 && row[0] == "" && this.EnclosedCell == false)
				return null;

			return row.ToArray();
		}

		public string[][] ReadToEnd()
		{
			List<string[]> rows = new List<string[]>();

			for (; ; )
			{
				string[] row = this.ReadRow();

				if (row == null)
					break;

				rows.Add(row);
			}
			return rows.ToArray();
		}

		public void Dispose()
		{
			if (this.Reader != null)
			{
				this.Reader.Dispose();
				this.Reader = null;
			}
		}
	}
}
