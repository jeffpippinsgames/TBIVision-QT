using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TBISeamTracker
{
	class LaserLinePixel
	{
		private double y;
		private int column;

		public LaserLinePixel()
		{
			y = double.NaN;
			column = 0;
		}

		public double Y
		{
			get { return y; }
			set { y = value; }
		}

		public int X
		{
			get { return column; }
			set { column = value; }
		}

		public int Column
		{
			get { return column; }
			set { column = value; }
		}

		public bool IsValid
		{
			get { if (!double.IsNaN(y)) return false; return true; }
		}

		public void MakeInvalid()
		{
			y = double.NaN;
			column = 0;
		}
	}
}
