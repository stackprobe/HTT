using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace WHTT
{
	public partial class PortNoWin : Form
	{
		public PortNoWin()
		{
			InitializeComponent();
		}

		private void PortNoWin_Load(object sender, EventArgs e)
		{
			this.PortNo.Text = "" + Gnd.I.PortNo;
			this.PortNo.SelectAll();
		}

		private void PortNo_KeyPress(object sender, KeyPressEventArgs e)
		{
			if (e.KeyChar == (char)Keys.Enter)
			{
				e.Handled = true;
				this.Close();
			}
		}

		public static int LastPortNo;

		private void PortNoWin_FormClosed(object sender, FormClosedEventArgs e)
		{
			try
			{
				LastPortNo = int.Parse(this.PortNo.Text);
			}
			catch
			{
				LastPortNo = -1;
			}
		}
	}
}
