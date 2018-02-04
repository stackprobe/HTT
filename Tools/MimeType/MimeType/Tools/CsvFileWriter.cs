using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace Charlotte.Tools
{
	public class CsvFileWriter : IDisposable
	{
		//private const char DELIMITER = ',';
		private const char DELIMITER = '\t';

		private StreamWriter Writer;
		private bool RowHead;

		public CsvFileWriter(string file)
		{
			this.Writer = new StreamWriter(file, false, StringTools.ENCODING_SJIS);
			this.RowHead = true;
		}

		public void WriteCell(string cell)
		{
			if (this.RowHead)
				this.RowHead = false;
			else
				this.Writer.Write(DELIMITER);

			if (
				cell.Contains('"') ||
				cell.Contains('\n') ||
				cell.Contains(DELIMITER)
				)
			{
				this.Writer.Write('"');
				this.Writer.Write(cell.Replace("\"", "\"\""));
				this.Writer.Write('"');
			}
			else
				this.Writer.Write(cell);
		}

		public void EndRow()
		{
			this.Writer.Write('\n');
			this.RowHead = true;
		}

		public void WriteCells(string[] cells)
		{
			foreach (string cell in cells)
			{
				this.WriteCell(cell);
			}
		}

		public void WriteRow(string[] row)
		{
			foreach (string cell in row)
			{
				this.WriteCell(cell);
			}
			this.EndRow();
		}

		public void WriteRows(string[][] rows)
		{
			foreach (string[] row in rows)
			{
				this.WriteRow(row);
			}
		}

		public void Dispose()
		{
			if (this.Writer != null)
			{
				this.Writer.Dispose();
				this.Writer = null;
			}
		}
	}
}
