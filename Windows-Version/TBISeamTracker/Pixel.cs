using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;
using Emgu.CV;
using Emgu.CV.Structure;
using Emgu.CV.CvEnum;
using System.Drawing;

namespace TBISeamTracker
{
	enum PixelOperationReturnType_t { OK, NOT_OK }

	class Pixel
	{
		private int row;
		private int column;
		private int intensity;

		public Pixel()
		{
			row = 0;
			column = 0;
		}
		public Pixel(int _row, int _col, int _intensity)
		{
			row = _row;
			column = _col;
			intensity = _intensity;
		}

        public void DrawPixel(ref Emgu.CV.Image<Gray, byte> _image)
        {
            _image.Data[row, column, 0] = (byte)intensity;
           
        }

		public int Row
		{
			get { return row; }
			set { row = value; }
		}
		public int Column
		{
			get { return column; }
			set { column = value; }
		}

		public int Intensity
		{
			get { return intensity; }
			set { intensity = value; }
		}

		public int X
		{
			get { return column; }
			set { column = value; }
		}

		public int Y
		{
			get { return row; }
			set { row = value; }
		}
	}
}
