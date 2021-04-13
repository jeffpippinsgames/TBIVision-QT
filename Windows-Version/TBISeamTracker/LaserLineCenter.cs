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
	class LaserLineCenter
	{
		private List<LaserLinePixel> points;
        private int highestYindex;
        private int lowestYindex;
        private double highestYvalue;
        private double lowestYvalue;
        //-------------------------------------
        public LaserLineCenter()
		{
			points = new List<LaserLinePixel>();
            highestYindex = 0;
            lowestYindex = 0;
            
            highestYvalue = double.NaN;
            lowestYvalue = double.NaN;
		}
        //-------------------------------------
        public void Add(LaserLinePixel _point)
		{
            if (_point == null) return;
			points.Add(_point);
            if(_point.Y > highestYvalue)
            {
                highestYindex = points.Count() - 1;
                highestYvalue = _point.Y;
            }
            if(_point.Y < lowestYvalue)
            {
                lowestYindex = points.Count() - 1;
                lowestYvalue = _point.Y;
            }
		}
        //-------------------------------------
        public void Clear()
		{
			points.Clear();
            highestYindex = 0;
            lowestYindex = 0;
            highestYvalue = double.NaN;
            lowestYvalue = double.NaN;
        }
        //-------------------------------------
        public LaserLinePixel GetPixelAtIndex(int _index)
        {
            if (points.Count() < 1) return null;
            return points[_index];
        }
        //-------------------------------------
        public void DrawLaserLineCenter(ref Emgu.CV.Image<Gray, byte> _image)
        {
            foreach(LaserLinePixel _pixel in points)
            {
                _image.Data[(int)Math.Round((double)_pixel.X), (int)Math.Round((double)_pixel.Y), 0] = 255;
            }
        }
        //-------------------------------------
        public void DrawLaserLineCenter(ref Emgu.CV.Image<Bgr, byte> _image, Bgr _color)
        {
            foreach (LaserLinePixel _pixel in points)
            {
                _image[(int)Math.Round((double)_pixel.X), (int)Math.Round((double)_pixel.Y)] = _color;
            }
        }
        //-------------------------------------
        public void GetVgrooveLeftTrackingPoint(LineSegment2dDouble _pzl, ref Point2dDouble _trackingpnt, double _distance_threshold = 1.0)
        {
            if(!this.IsValid() || !_pzl.IsValidLineSegment())
            {
                _trackingpnt.MakeInvalid();
                return;
            }

            for(int i = highestYindex; i >= 0; --i)
            {
                Point2dDouble _pt = new Point2dDouble((double)points[i].X, points[i].Y);
                double _dist = _pzl.DistanceToLine(_pt);
                if(_dist <= _distance_threshold)
                {
                    _trackingpnt.X = _pt.X;
                    _trackingpnt.Y = _pt.Y;
                    return;
                }
            }
            _trackingpnt.MakeInvalid();
        }
        //-------------------------------------
        public void GetVgrooveCenterTrackingPoint(ref Point2dDouble _trackingpnt)
        {
            if(!this.IsValid())
            {
                _trackingpnt.MakeInvalid();
                return;
            }

            _trackingpnt.X = (double)points[highestYindex].X;
            _trackingpnt.Y = points[highestYindex].Y;
        }
        //-------------------------------------
        public void GetVGrooveRightTrackingPoint(LineSegment2dDouble _pzr, ref Point2dDouble _trackingpnt, double _distance_threshold = 1.0)
        {
			if (!this.IsValid() || !_pzr.IsValidLineSegment())
			{
				_trackingpnt.MakeInvalid();
				return;
			}

			for (int i = highestYindex; i < points.Count; ++i)
			{
				Point2dDouble _pt = new Point2dDouble((double)points[i].X, points[i].Y);
				double _dist = _pzr.DistanceToLine(_pt);
				if (_dist <= _distance_threshold)
				{
					_trackingpnt.X = _pt.X;
					_trackingpnt.Y = _pt.Y;
					return;
				}
			}
			_trackingpnt.MakeInvalid();
		}
        //-------------------------------------
        public int XValueAtHighestYIndex
        {
            get
            {
                if (double.IsNaN(highestYvalue)) return 0;
                return points[highestYindex].X;
            }
        }
        //-------------------------------------
        public bool IsValid()
        {
                if (points.Count == 0) return false;
                return true;
        }
        //-------------------------------------







        public int Count
        {
            get { return points.Count; }
        }

        public int HighestYValueIndex
        {
            get { return highestYindex; }
        }

        public double HighestYValue
        {
            get { return highestYvalue; }
        }

        public int LowestYIndex
        {
            get { return lowestYindex; }
        }

        public double LowestYValue
        {
            get { return LowestYValue; }
        }

       

       
	}
}
