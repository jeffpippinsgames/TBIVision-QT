using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using Emgu.CV.Structure;

namespace TBISeamTracker
{


	class Point2dDouble
	{
		//--------------------------------------------------------------------------------------------------------------
		public double X;
		public double Y;
        
		public Point2dDouble()
		{
			X = double.NaN;
			Y = double.NaN;
		}
		//--------------------------------------------------------------------------------------------------------------
		public Point2dDouble(double _x, double _y)
		{
			X = _x;
			Y = _y;
		}
        //--------------------------------------------------------------------------------------------------------------
		public Point2dDouble(Point2dDouble _pnt)
		{
			this.X = _pnt.X;
			this.Y = _pnt.Y;
		}
		//--------------------------------------------------------------------------------------------------------------
		public void MakeInvalid()
        {
            X = double.NaN;
            Y = double.NaN;
        }
		//--------------------------------------------------------------------------------------------------------------
		public bool isValidPoint()
		{
			if (double.IsNaN(this.X) || double.IsNaN(this.Y) || double.IsInfinity(this.X) || double.IsInfinity(this.Y))
				return false;
			return true;
		}
		//--------------------------------------------------------------------------------------------------------------
		public Point toPoint()
		{
			Point pnt = new Point(0, 0);
			if (this.isValidPoint())
			{
				pnt.X = (int)X;
				pnt.Y = (int)Y;
			}
			return pnt;
		}
		//--------------------------------------------------------------------------------------------------------------
		public PointF toPointF()
		{
			PointF pnt = new PointF(float.NaN, float.NaN);
			if (this.isValidPoint())
			{
				pnt.X = (float)X;
				pnt.Y = (float)Y;
			}
			return pnt;
		}
		//--------------------------------------------------------------------------------------------------------------
		public double GetAngleFromHorizontalX(Point2dDouble pntd)
		{
			if (!pntd.isValidPoint()) return double.NaN;
			if (!this.isValidPoint()) return double.NaN;
			Point2dDouble vect1 = new Point2dDouble();
			Point2dDouble vect2 = new Point2dDouble();
			double angle;
			vect1.X = X;
			vect1.Y = 0;
			vect2.X = pntd.X - X;
			vect2.Y = -(pntd.Y - Y);
			double crossproduct = (vect1.X * vect2.X) + (vect1.Y * vect2.Y);
			double summagnitudes = Math.Sqrt((vect1.X * vect1.X) + (vect1.Y * vect1.Y)) * Math.Sqrt((vect2.X * vect2.X) + (vect2.Y * vect2.Y));
			if (summagnitudes == 0) return double.NaN;
			angle = 57.2958 * Math.Acos(crossproduct / summagnitudes);
			if (Y < pntd.Y) angle = angle * -1;
			return angle;
		}
		//--------------------------------------------------------------------------------------------------------------
		public void DrawPointToEmguCVImage(ref Emgu.CV.Image<Bgr, Byte> img, Bgr color, float _sizecross = 32, float _sizeradius = 10)
		{
			if (img == null) return;
			if (this.isValidPoint())
			{
				PointF pnt = new PointF((float)this.X, (float)this.Y);
				Cross2DF cross = new Cross2DF(pnt, _sizecross, _sizecross);
				CircleF circle = new CircleF(pnt, _sizeradius);
				img.Draw(cross, color, 1);
				img.Draw(circle, color);
			}
		}
        //--------------------------------------------------------------------------------------------------------------
        public double DistanceBetweenPoints(Point2dDouble _pnt)
        {
            if(!_pnt.isValidPoint() || !this.isValidPoint())
            {
                return double.NaN;
            }

            return Math.Sqrt(Math.Pow((_pnt.X - this.X), 2) + Math.Pow((_pnt.Y - this.Y), 2));      
        }
        //--------------------------------------------------------------------------------------------------------------
        public bool IsEqual(Point2dDouble _pnt)
        {
            if(!this.isValidPoint() || !_pnt.isValidPoint())
            {
                return false;
            }

            if(_pnt.X == this.X)
            {
                if(_pnt.Y == this.Y)
                {
                    return true;
                }
            }

            return false;
        }
		//--------------------------------------------------------------------------------------------------------------
		public void SetEqualTo(Point2dDouble _pnt)
		{
			this.X = _pnt.X;
			this.Y = _pnt.Y;
		}
		//--------------------------------------------------------------------------------------------------------------
	}

	class LineSegment2dDouble : IComparable<LineSegment2dDouble>
	{
		private Point2dDouble p1;
		private Point2dDouble p2;
        private Point2dDouble mp;
		private double anglefromhorizontal;
		private double length;
		
		//--------------------------------------------------------------------------------------------------------------
		public LineSegment2dDouble()
		{
			p1 = new Point2dDouble();
			p2 = new Point2dDouble();
            mp = new Point2dDouble();
			p1.MakeInvalid();
			p2.MakeInvalid();
            mp.MakeInvalid();
			anglefromhorizontal = double.NaN;
			length= double.NaN;
		}
		//--------------------------------------------------------------------------------------------------------------
		public LineSegment2dDouble(Point2dDouble _p1, Point2dDouble _p2)
		{
			
			p1 = new Point2dDouble();
			p2 = new Point2dDouble();
            mp = new Point2dDouble();
			Point1 = _p1;
			Point2 = _p2;
		}
		//--------------------------------------------------------------------------------------------------------------
		public LineSegment2dDouble(LineSegment2dDouble _line)
		{
			this.Point1 = _line.Point1;
			this.Point2 = _line.Point2;
            UpdateAngleFromHorizontal();
            UpdateLength();
		}
		//--------------------------------------------------------------------------------------------------------------
		private void UpdateAngleFromHorizontal()
		{

			if (this.IsValidLineSegment())
			{
				Point2dDouble vect1 = new Point2dDouble();
				Point2dDouble vect2 = new Point2dDouble();
				vect1.X = (this.p2.X - this.p1.X);
				vect1.Y = (this.p2.Y - this.p1.Y);
				vect2.X = 1;
				vect2.Y = 0;
				double vect1length = Math.Sqrt((vect1.X * vect1.X) + (vect1.Y * vect1.Y));
				double vect2length = Math.Sqrt((vect2.X * vect2.X) + (vect2.Y * vect2.Y));
				double crossprod = (vect1.X * vect2.X) + (vect1.Y * vect2.Y);
				anglefromhorizontal = Math.Acos((crossprod) / (vect1length * vect2length));
				anglefromhorizontal = anglefromhorizontal * 57.2958;
				if (vect1.Y > 0) anglefromhorizontal = anglefromhorizontal * -1;
                mp.X = (p1.X + p2.X) / 2;
                mp.Y = (p2.Y + p1.Y) / 2;
			}
			else anglefromhorizontal = double.NaN;
		
		}
		//--------------------------------------------------------------------------------------------------------------
		private void UpdateLength()
		{
			if (this.IsValidLineSegment())
			{
				length = Math.Sqrt((p2.X - p1.X) * (p2.X - p1.X) + (p2.Y - p1.Y) * (p2.Y - p1.Y));
			}
			else length = double.NaN;		
		}
		//--------------------------------------------------------------------------------------------------------------
        public double GetY_at_X(double _xvalue)
        {
            if(IsValidLineSegment())
            {
                //y=mx+b
                //m = deltay / deltax
                double _slope = (p1.Y - p2.Y) / (p1.X - p2.X);
                double _yint = p1.Y - (_slope * p1.X);
                return (_slope * _xvalue) + _yint;
            }
            return double.NaN;
        }
        //--------------------------------------------------------------------------------------------------------------
        public bool IsColinear(LineSegment2dDouble _line, double _distance_threshold = 1, double _angle_threshold = 1)

        {
            if (this.IsValidLineSegment() && _line.IsValidLineSegment())
            {
                if (Math.Abs(_line.AngleFromHorizontal - this.AngleFromHorizontal) <= _angle_threshold)
                {
                    Point2dDouble _npnt1 = new Point2dDouble(_line.p1.X, (this.GetY_at_X(_line.p1.X)));
                    Point2dDouble _npnt2 = new Point2dDouble(_line.p2.X, (this.GetY_at_X(_line.p2.X)));

                    if (_line.p1.DistanceBetweenPoints(_npnt1) <= _distance_threshold)
                    {
                        if (_line.p2.DistanceBetweenPoints(_npnt2) <= _distance_threshold)
                        {
                            return true;
                        }
                    }
                }
            }
            return false;
        }
        //--------------------------------------------------------------------------------------------------------------
        public void MakeInvalid()
        {
            p1.MakeInvalid();
            p2.MakeInvalid();
			mp.MakeInvalid();
			length = double.NaN;
			anglefromhorizontal = double.NaN;
        }
        //--------------------------------------------------------------------------------------------------------------
        public bool IsValidLineSegment()
		{
			if (p1.isValidPoint() && p2.isValidPoint()) return true;
			return false;
		}
		//--------------------------------------------------------------------------------------------------------------
		public void DrawLineToEmguCVImage(ref Emgu.CV.Image<Bgr, Byte> img, Color _color)
		{
			if (img == null) return;
			if (this.IsValidLineSegment())
			{
				Point[] pnts = new Point[2];
				pnts[0].X = (int)this.p1.X;
				pnts[0].Y = (int)this.p1.Y;
				pnts[1].X = (int)this.p2.X;
				pnts[1].Y = (int)this.p2.Y;
				img.DrawPolyline(pnts, false, new Bgr(_color));
			}
		}
		//--------------------------------------------------------------------------------------------------------------
		public double GetDegreesBetweenLines(LineSegment2dDouble line)
		{
			if (!this.IsValidLineSegment() && !line.IsValidLineSegment()) return double.NaN;
			double angle = 0;
			Point2dDouble vect1 = new Point2dDouble();
			Point2dDouble vect2 = new Point2dDouble();
			vect1.X = (this.p2.X - this.p1.X);
			vect1.Y = (this.p2.Y - this.p1.Y);
			vect2.X = (line.p2.X - line.p1.X);
			vect2.Y = (line.p2.Y - line.p1.Y);
			double vect1length = Math.Sqrt((vect1.X * vect1.X) + (vect1.Y * vect1.Y));
			double vect2length = Math.Sqrt((vect2.X * vect2.X) + (vect2.Y * vect2.Y));
			double crossprod = (vect1.X * vect2.X) + (vect1.Y * vect2.Y);
			angle = Math.Acos((crossprod) / (vect1length * vect2length));
			angle = angle * 57.2958;

			return angle;
		}
		//--------------------------------------------------------------------------------------------------------------
		public bool IsPointXInLineSubset(Point2dDouble pnt)
		{
			if (p1.X > p2.X)
			{
				if ((p2.X <= pnt.X) && (p1.X >= pnt.X)) return true;
			}
			else if (p2.X > p1.X)
			{
				if ((p2.X >= pnt.X) && (p1.X <= pnt.X)) return true;
			}
			else if (p2.X == p1.X)
			{
				if (p2.X == pnt.X) return false;
			}

			return false;
		}
		//--------------------------------------------------------------------------------------------------------------
		public double DistanceToLine(Point2dDouble pnt)
		{
			if (!this.IsValidLineSegment()) return double.NaN;
			if (!pnt.isValidPoint()) return double.NaN;
			double distance = double.NaN;
			double denominator = Math.Abs(((p2.Y - p1.Y) * pnt.X) - ((p2.X - p1.X) * pnt.Y) + (p2.X * p1.Y) - (p2.Y * p1.X));
			double numerator = Math.Sqrt(((p2.Y - p1.Y) * (p2.Y - p1.Y)) + ((p2.X - p1.X) * (p2.X - p1.X)));
			if (numerator == 0) return double.NaN;
			distance = denominator / numerator;
			return distance;
		}    
		//--------------------------------------------------------------------------------------------------------------
		public bool SplitMergeHelper_CanLineBeSplit(List<Point2dDouble> datadpnts, ref int indexofsplit, double distancethreshold = 1)
		{
			if (!this.IsValidLineSegment()) return false;
			if (datadpnts.Count < 1) return false;

			int highindex = 0;
			double highdistance = 0;
			double distance;

			for (int i = 0; i < datadpnts.Count; ++i)
			{
				if (this.IsPointXInLineSubset(datadpnts[i]))
				{
					distance = this.DistanceToLine(datadpnts[i]);
					if (!double.IsNaN(distance))
					{
						if (distance > highdistance)
						{
							highdistance = distance;
							highindex = i;
						}
					}
				}
			}

			if (highdistance >= distancethreshold)
			{
				indexofsplit = highindex;
				return true;
			}
			return false;
		}
		//--------------------------------------------------------------------------------------------------------------
		public int CompareTo(LineSegment2dDouble obj)
		{
			return this.p1.X.CompareTo(obj.p1.X);
		}
		//--------------------------------------------------------------------------------------------------------------
		public void SetEqualTo(LineSegment2dDouble _src)
		{
			this.Point1 = _src.Point1;
			this.Point2 = _src.Point2;
		}



		#region Accessors
		//--------------------------------------------------------------------------------------------------------------
		public double Length
		{
			get { return length;}
		}
		//--------------------------------------------------------------------------------------------------------------
		public double AngleFromHorizontal
		{
			get { return anglefromhorizontal; }
		}
		//--------------------------------------------------------------------------------------------------------------
		public Point2dDouble Point1
		{
			get { return p1; }
			set
			{
				p1.SetEqualTo(value);
				if (this.IsValidLineSegment())
				{
					UpdateAngleFromHorizontal();
					UpdateLength();
				}
				else
				{
					this.anglefromhorizontal = double.NaN;
					this.length = double.NaN;
				}
			}
		}
		//--------------------------------------------------------------------------------------------------------------
		public Point2dDouble Point2
		{
			get { return p2; }
			set
			{
				p2.SetEqualTo(value);
				if (this.IsValidLineSegment())
				{
					UpdateAngleFromHorizontal();
					UpdateLength();
				}
				else
				{
					this.anglefromhorizontal = double.NaN;
					this.length = double.NaN;
				}
			}
		}
		//--------------------------------------------------------------------------------------------------------------
		public Point2dDouble MidPoint
		{
			get { return mp; }
		}
		//--------------------------------------------------------------------------------------------------------------
		#endregion


	}
}
