using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using Charlotte.Tools;

namespace Charlotte
{
	public partial class MainWin : Form
	{
		public MainWin()
		{
			InitializeComponent();

			this.MinimumSize = this.Size;
		}

		private void MainWin_Load(object sender, EventArgs e)
		{
			// noop
		}

		private void MainWin_Shown(object sender, EventArgs e)
		{
			this.Init();
		}

		private void Init()
		{
			this.MainSheet.Visible = false;

			this.MainSheet.RowCount = 0;
			this.MainSheet.ColumnCount = 0;

			this.MainSheet.ColumnCount = 2;
			this.MainSheet.Columns[0].HeaderText = "拡張子";
			this.MainSheet.Columns[1].HeaderText = "MIME Type";

			this.MainSheet.Columns[0].SortMode = DataGridViewColumnSortMode.NotSortable;
			this.MainSheet.Columns[1].SortMode = DataGridViewColumnSortMode.NotSortable;

			try
			{
#if false
				string file;

				if (File.Exists(Consts.MIME_TYPE_FILE))
				{
					file = Consts.MIME_TYPE_FILE;
				}
				else
				{
					file = Consts.MIME_TYPE_FILE_DEBUG; // devenv
				}
				using (CsvFileReader reader = new CsvFileReader(file))
#else
				if (File.Exists(Consts.MIME_TYPE_FILE) == false)
				{
					File.Copy(Consts.MIME_TYPE_FILE_DEBUG, Consts.MIME_TYPE_FILE); // devenv
				}
				using (CsvFileReader reader = new CsvFileReader(Consts.MIME_TYPE_FILE))
#endif
				{
					for (; ; )
					{
						string[] row = reader.ReadRow();

						if (row == null)
							break;

						this.MainSheet.RowCount++;
						DataGridViewRow msRow = this.MainSheet.Rows[this.MainSheet.RowCount - 1];

						msRow.Cells[0].Value = Utils.ExtMimeTypeFilter(row[0]);
						msRow.Cells[1].Value = Utils.ExtMimeTypeFilter(row[1]);
					}
				}
			}
			catch (Exception ex)
			{
				MessageBox.Show(
					this,
					"" + ex,
					"読み込みに失敗しました",
					MessageBoxButtons.OK,
					MessageBoxIcon.Warning
					);
			}

			this.MainSheet.Visible = true;

			this.MS_AutoResize();
			this.MainSheet.ClearSelection();
		}

		private void MS_AutoResize()
		{
			this.MainSheet.Columns[0].Width = 1000;
			this.MainSheet.Columns[1].Width = 1000;
			this.MainSheet.AutoResizeColumns(DataGridViewAutoSizeColumnsMode.AllCells);
			this.MainSheet.Columns[0].Width += 30;
			this.MainSheet.Columns[1].Width += 30;
		}

		private int GetSelectedRowIndex()
		{
			for (int rowidx = 0; rowidx < this.MainSheet.RowCount; rowidx++)
				if (this.MainSheet.Rows[rowidx].Selected)
					return rowidx;

			return -1;
		}

		private void MS_Swap(int a, int b)
		{
			for (int colidx = 0; colidx < 2; colidx++)
			{
				string tmp = "" + this.MainSheet.Rows[a].Cells[colidx].Value;
				this.MainSheet.Rows[a].Cells[colidx].Value = "" + this.MainSheet.Rows[b].Cells[colidx].Value;
				this.MainSheet.Rows[b].Cells[colidx].Value = tmp;
			}
		}

		private string[] GetOtherExts(int self_rowidx)
		{
			List<string> dest = new List<string>();

			for (int rowidx = 0; rowidx < this.MainSheet.RowCount; rowidx++)
			{
				if (rowidx != self_rowidx)
				{
					dest.Add("" + this.MainSheet.Rows[rowidx].Cells[0].Value);
				}
			}
			return dest.ToArray();
		}

		private void MainWin_FormClosing(object sender, FormClosingEventArgs e)
		{
			// noop
		}

		private void MainWin_FormClosed(object sender, FormClosedEventArgs e)
		{
			// noop
		}

		private void 選択解除CToolStripMenuItem_Click(object sender, EventArgs e)
		{
			this.MainSheet.ClearSelection();
		}

		private void BtnAdd_Click(object sender, EventArgs e)
		{
			using (EditDlg f = new EditDlg("", "", this.GetOtherExts(-1)))
			{
				f.ShowDialog();

				if (f.OKPressed)
				{
					this.MainSheet.RowCount++;
					DataGridViewRow msRow = this.MainSheet.Rows[this.MainSheet.RowCount - 1];

					msRow.Cells[0].Value = f.RetExt;
					msRow.Cells[1].Value = f.RetMimeType;

					this.MS_AutoResize();
					msRow.Selected = true;
					this.MainSheet.FirstDisplayedScrollingRowIndex = this.MainSheet.RowCount - 1;
				}
			}
		}

		private void BtnEdit_Click(object sender, EventArgs e)
		{
			int rowidx = this.GetSelectedRowIndex();

			if (rowidx == -1)
			{
				MessageBox.Show(
					this,
					"選択してね。",
					"編集できません",
					MessageBoxButtons.OK,
					MessageBoxIcon.Warning
					);

				return;
			}
			DataGridViewRow msRow = this.MainSheet.Rows[rowidx];

			using (EditDlg f = new EditDlg("" + msRow.Cells[0].Value, "" + msRow.Cells[1].Value, this.GetOtherExts(rowidx)))
			{
				f.ShowDialog();

				if (f.OKPressed)
				{
					msRow.Cells[0].Value = f.RetExt;
					msRow.Cells[1].Value = f.RetMimeType;

					this.MS_AutoResize();
				}
			}
		}

		private void BtnDelete_Click(object sender, EventArgs e)
		{
			int rowidx = this.GetSelectedRowIndex();

			if (rowidx == -1)
			{
				MessageBox.Show(
					this,
					"選択してね。",
					"削除できません",
					MessageBoxButtons.OK,
					MessageBoxIcon.Warning
					);

				return;
			}
			this.MainSheet.Rows.RemoveAt(rowidx);
			rowidx = Math.Min(rowidx, this.MainSheet.RowCount - 1);

			if (0 <= rowidx)
			{
				this.MainSheet.Rows[rowidx].Selected = true;
				this.MS_ScrollTo(rowidx);
			}
		}

		private void BtnUp_Click(object sender, EventArgs e)
		{
			int rowidx = this.GetSelectedRowIndex();

			if (rowidx < 1)
				return;

			this.MS_Swap(rowidx - 1, rowidx);
			this.MainSheet.Rows[rowidx - 1].Selected = true;
			this.MS_ScrollTo(rowidx - 1);
		}

		private void BtnDown_Click(object sender, EventArgs e)
		{
			int rowidx = this.GetSelectedRowIndex();

			if (rowidx == -1 || this.MainSheet.RowCount <= rowidx + 1)
				return;

			this.MS_Swap(rowidx, rowidx + 1);
			this.MainSheet.Rows[rowidx + 1].Selected = true;
			this.MS_ScrollTo(rowidx + 1);
		}

		private void MS_ScrollTo(int rowidx)
		{
			this.MainSheet.FirstDisplayedScrollingRowIndex = Math.Max(0, rowidx - 9);
		}

		private void BtnSave_Click(object sender, EventArgs e)
		{
			try
			{
				this.Save();

				MessageBox.Show(
					this,
					"保存しました。",
					"成功",
					MessageBoxButtons.OK,
					MessageBoxIcon.Information
					);

				this.Close();
			}
			catch (Exception ex)
			{
				MessageBox.Show(
					this,
					"" + ex,
					"保存に失敗しました",
					MessageBoxButtons.OK,
					MessageBoxIcon.Warning
					);
			}
		}

		private void Save()
		{
			using (CsvFileWriter writer = new CsvFileWriter(Consts.MIME_TYPE_FILE))
			{
				for (int rowidx = 0; rowidx < this.MainSheet.RowCount; rowidx++)
				{
					writer.WriteCell("" + this.MainSheet.Rows[rowidx].Cells[0].Value);
					writer.WriteCell("" + this.MainSheet.Rows[rowidx].Cells[1].Value);
					writer.EndRow();
				}
			}
		}

		private void BtnClose_Click(object sender, EventArgs e)
		{
			this.Close();
		}
	}
}
