using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Emgu.CV;
using Emgu.CV.Structure;
using Emgu.CV.CvEnum;
using System.Drawing;

namespace TBISeamTracker
{
	enum PixelClusterOperationReturnType_t { OK, NOT_OK}

	class PixelCluster
	{
		//-------------------------------------
		private List<Pixel> pixels;
		private double centroid;
		private double weightsum = 0.0;
		private double sumrowweightproduct = 0.0;
		//-------------------------------------
		public PixelCluster()
		{
			pixels = new List<Pixel>();
			centroid = double.NaN;
			weightsum = 0.0;
			sumrowweightproduct = 0.0;
		}
		//-------------------------------------
		public void AddPixeltoCluster(Pixel _pixel)
		{
			weightsum += (double)_pixel.Intensity; ;
			sumrowweightproduct += ((double)_pixel.Intensity * (double)_pixel.Row);
			pixels.Add(_pixel);
		}
		//-------------------------------------
		public void SetCentroid()
		{
			if (weightsum == 0) centroid = double.NaN;
			else centroid = sumrowweightproduct / weightsum;
		}
        //-------------------------------------
        public void DrawCluster(ref Emgu.CV.Image<Gray, byte> _image)
        {
            foreach(Pixel _pixel in pixels)
            {
                _image.Data[_pixel.Row, _pixel.Column, 0] = (byte)_pixel.Intensity;
            }

        }
        //-------------------------------------
        public void Clear()
		{
			pixels.Clear();
			centroid = double.NaN;
			weightsum = 0.0;
			sumrowweightproduct = 0.0;
		}
		//-------------------------------------
		public double Centroid
		{
			get { SetCentroid(); return centroid; }
			set { centroid = value; }
		}
		//-------------------------------------
		public int LastIntensityValue
		{
			get
			{
				if (pixels.Count == 0) return 0;
				return pixels[pixels.Count - 1].Intensity;
			}
		}
		//-------------------------------------
		public int Count
		{
			get { return pixels.Count; }
		}
		//-------------------------------------
	}
}
