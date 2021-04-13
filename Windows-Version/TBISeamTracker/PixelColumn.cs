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
	enum PixelClusterIntensityTranistion_t { STARTINGCLUSTER, INCREASING, DECREASING, ENDOFCLUSTER };
	enum PixelColumnOperationReturnType_t { OK, NOT_OK}

	class PixelColumn
	{
		//-------------------------------------
		private List<PixelCluster> pixelclusters;
		private int column_number;
		//-------------------------------------
		public PixelColumn()
			{
				pixelclusters = new List<PixelCluster>();
				column_number = 0;
			}
		//-------------------------------------
		public void Add(PixelCluster _pixelcluster)
		{
			if (_pixelcluster == null) return;
			pixelclusters.Add(_pixelcluster);
		}
		//-------------------------------------
        public void DrawColumn(ref Emgu.CV.Image<Gray, byte> _image)
        {
            foreach(PixelCluster _cluster in pixelclusters)
            {
                _cluster.DrawCluster(ref _image);
            }
        }
		public int Column
		{
			get { return column_number; }
			set { column_number = value; }
		}
		//-------------------------------------
		public int Count
		{
			get { return pixelclusters.Count; }
		}
		//-------------------------------------
		public List<PixelCluster> PixelCluster
		{
			get { return pixelclusters; }
		}
		//
	}
}
