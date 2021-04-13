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
    
	class VGrooveScanManager
	{
		//Scan Data Entities
		private LaserLineCenter laserline;
		private List<PixelColumn> pixelcolumnlist;
		private uint columnite_intensity;
		private List<LineSegment2dDouble> LSD; //Line Segment Data
		//Geometric Entities
		private LineSegment2dDouble rootline;
		private LineSegment2dDouble PZL;
		private LineSegment2dDouble PZR;
		private Point2dDouble[] JTP;
		private ScanResult_t scan_result;
		bool jtp_time_domain_satisfied;
		Stopwatch jtp_time_domain_stopwatch;

		#region Constructors
		//-------------------------------------
		public VGrooveScanManager()
		{
			laserline = new LaserLineCenter();
			pixelcolumnlist = new List<PixelColumn>();
			LSD = new List<LineSegment2dDouble>();
			JTP = new Point2dDouble[3];
			MakeJTPInvalid();
			columnite_intensity = 0;
			rootline = new LineSegment2dDouble();
			PZL = new LineSegment2dDouble();
			PZR = new LineSegment2dDouble();
			rootline.MakeInvalid();
			PZL.MakeInvalid();
			PZR.MakeInvalid();
			JTP = new Point2dDouble[3];
			JTP[(int)JTPIdentifier_t.LEFT_SIDE] = new Point2dDouble();
			JTP[(int)JTPIdentifier_t.CENTER] = new Point2dDouble();
			JTP[(int)JTPIdentifier_t.RIGHT_SIDE] = new Point2dDouble();
			scan_result = ScanResult_t.SCAN_NOT_DONE;
		}
		//-------------------------------------
		#endregion

		#region Main Process Method - To Use In Loop
		//-------------------------------------
		public ScanResult_t ProcessVJoint(ISensorSettings _sensor_settings, ref Emgu.CV.Image<Gray, byte> _thresholdimage, ref Emgu.CV.Image<Gray, byte> _skelimage,
												 ref Emgu.CV.Image<Bgr, byte> _operationimage)
		{
			//Local Variables
			ScanResult_t _scanresult;

			//Initialize Data
			MakeJTPInvalid();
			PZL.MakeInvalid();
			PZR.MakeInvalid();
			LSD.Clear();
			laserline.Clear();
			pixelcolumnlist.Clear();

			//Check Passed in Data
			if ((_thresholdimage == null) || (_sensor_settings == null) || (_skelimage == null) || (_operationimage == null)) return ScanResult_t.EMGU_CV_ERROR;

			//Build the pixel columnlist data
			_scanresult = BuildPixelColumnList(_sensor_settings, ref pixelcolumnlist, _thresholdimage);
			if(_scanresult != ScanResult_t.SCAN_OK)
			{
				DrawImages(_sensor_settings, ref _skelimage, ref _operationimage, ref _thresholdimage);
				return _scanresult;
			}

            //Build The Laser Center Line
            _scanresult = BuildLaserLineCenterList(_sensor_settings, ref laserline, pixelcolumnlist);
            if (_scanresult != ScanResult_t.SCAN_OK)
            {
                DrawImages(_sensor_settings, ref _skelimage, ref _operationimage, ref _thresholdimage);
                return _scanresult;
            }

            //Build The PZL's
            _scanresult = BuildPredictedZeroLines(_sensor_settings, laserline, ref PZL, ref PZR);
			if (_scanresult != ScanResult_t.SCAN_OK)
			{
				DrawImages(_sensor_settings, ref _skelimage, ref _operationimage, ref _thresholdimage);
				return _scanresult;
			}

            //Get The Tracking Points
            laserline.GetVgrooveLeftTrackingPoint(PZL, ref JTP[(int)JTPIdentifier_t.LEFT_SIDE], 1.0);
			laserline.GetVGrooveRightTrackingPoint(PZR, ref JTP[(int)JTPIdentifier_t.RIGHT_SIDE], 1.0);
			laserline.GetVgrooveCenterTrackingPoint(ref JTP[(int)JTPIdentifier_t.CENTER]);

            //Process The Tracking Points

            //Draw The Images
            DrawImages(_sensor_settings, ref _skelimage, ref _operationimage, ref _thresholdimage);

            return ScanResult_t.SCAN_OK;

		}
        //-------------------------------------
        #endregion

        #region Public Methods
        //-------------------------------------
        public void MakeJTPInvalid()
        {
            if (JTP == null) return;
            JTP[(int)JTPIdentifier_t.LEFT_SIDE].MakeInvalid();
            JTP[(int)JTPIdentifier_t.CENTER].MakeInvalid();
            JTP[(int)JTPIdentifier_t.RIGHT_SIDE].MakeInvalid();
        }
        //-------------------------------------
		public bool IsJTPValid()
		{
			if(JTP[(int)JTPIdentifier_t.CENTER].isValidPoint() && JTP[(int)JTPIdentifier_t.LEFT_SIDE].isValidPoint() && JTP[(int)JTPIdentifier_t.RIGHT_SIDE].isValidPoint())
			{
				return true;
			}
			return false;
		}
		//-------------------------------------
		#endregion

		#region Private Process Methods
		//-------------------------------------
		private ScanResult_t BuildPredictedZeroLines(ISensorSettings _sensor_settings, LaserLineCenter _laserline, ref LineSegment2dDouble _pzl, 
                                                       ref LineSegment2dDouble _pzr, int _min_valid_data_points = 5)
		{
            if (_laserline == null) return ScanResult_t.PASSED_IN_METHOD_PARAMETERS_NOT_VALID;
            if (_pzl == null) return ScanResult_t.PASSED_IN_METHOD_PARAMETERS_NOT_VALID;
            if (_pzr == null) return ScanResult_t.PASSED_IN_METHOD_PARAMETERS_NOT_VALID;

			_pzl.MakeInvalid();
			_pzr.MakeInvalid();

			List<Point2dDouble> _pointdata = new List<Point2dDouble>();
            int highest_index = _laserline.HighestYValueIndex;
            double highest_value = _laserline.HighestYValue;

            for(int i = 0; i < _laserline.Count; ++i)
            {
                Point2dDouble _pnt = new Point2dDouble((double)_laserline.GetPixelAtIndex(i).X, _laserline.GetPixelAtIndex(i).Y);
                _pointdata.Add(_pnt);
            }
				
			if(!DoRANSAC(_sensor_settings, _pointdata, ref _pzl)) _pzl.MakeInvalid();
           
			//Get Right PZL
			_pointdata.Clear();
			
			for(int i = (_laserline.Count - 1); i > highest_index; --i)
			{
                Point2dDouble _pnt = new Point2dDouble((double)_laserline.GetPixelAtIndex(i).X, _laserline.GetPixelAtIndex(i).Y);
                _pointdata.Add(_pnt);
            }

			if (!DoRANSAC(_sensor_settings, _pointdata, ref _pzr)) _pzr.MakeInvalid();

			return ScanResult_t.SCAN_OK;
		}
		//-------------------------------------
		private ScanResult_t BuildPixelColumnList(ISensorSettings _sensor_settings, ref List<PixelColumn> _pixelcolumnlist, Emgu.CV.Image<Gray, byte> _thresholdimage,
                                                  int _min_cluster_size = 5, uint _max_image_intensity = 1500000)
		{
			//Using the PixelColumn, PixelCluster and Pixel Classes This Function Disects the Threshold Image and builds thee laserline class.
			//It Will Build The _laserline data type that represensts the Center of Laser Line.
			// This Method Follows the Following Steps.
			//1. Break the Threshold Image Into a raw byte array
			//2. For the sake of performance avoid nested for loops by using flat looping techniques. 
			//3. Run thru the loop and build the pixelclusters. Add the pixelclusters to the pixel column.
			//4. The addition of the pixel clusters are done with a gausian intensity check.
			//   They must have a starting intensity, increase to a peak, then decrease to zero.
			//   Any other distrobution is a failure and is not included.

			if ((_sensor_settings == null) || (_thresholdimage == null) || (_pixelcolumnlist == null)) return ScanResult_t.PASSED_IN_METHOD_PARAMETERS_NOT_VALID;
			_pixelcolumnlist.Clear();
			
			//---------------------------------------------------------------------
			PixelColumn _pixelcolumn = new PixelColumn();
			PixelCluster _pixelcluster = new PixelCluster();
			PixelClusterIntensityTranistion_t _intensitytrans = PixelClusterIntensityTranistion_t.STARTINGCLUSTER;
			uint _total_intensity = 0;
			byte[] _data = _thresholdimage.Bytes;
			int _length = _thresholdimage.Bytes.Length;
			int _width = _thresholdimage.Width;
			int _height = _thresholdimage.Height;
			int _total_data_elements = _width * _height;
			int _rowindex = 0;
			int _colindex = 0;
			int _dataindex = 0;
			int _counter = 0;

			//The PixelClusterList Must Be Built.
			//Go Thru Each Byte Element.
			//Routine is Complex In Order to Avoid Using Nested For Loops
			//It uses a flat array model.
			do
			{
				//Keep Track of Total Intensity. Error Out if Total Intensity Starts to Become To Big.
				_total_intensity += _data[_counter]; 
				if (_total_intensity > _max_image_intensity) return ScanResult_t.TOTAL_IMAGE_INTENSITY_LARGER_THAN_THRESHOLD;

				//Set The Index for _data at [colindex, rowindex]
				_dataindex = (_width * _rowindex) + _colindex; 

				//Set The Values of the Pixel at the new Index
				//Create a New Pixel and set the values at the new index
				Pixel _pixel = new Pixel();
				_pixel.X = _colindex;
				_pixel.Y = _rowindex;
				_pixel.Intensity = (int)_data[_dataindex];
				
				//Start Building the Pixel Cluster
				//Determine What Stage in the intensity distrobution you are in and decide what to do with the pixel
				switch(_intensitytrans)
				{
					case PixelClusterIntensityTranistion_t.STARTINGCLUSTER: //Looking For A New Start to a Pixel Cluster
						if(_pixel.Intensity != 0)
						{
							//The Begining of the cluster has been found.
							//Add it to the cluster.
							_pixelcluster.AddPixeltoCluster(_pixel);
							//Set The Cluster is Now Officially Increasing.
							_intensitytrans = PixelClusterIntensityTranistion_t.INCREASING;
						}
						break;
					case PixelClusterIntensityTranistion_t.INCREASING:
						if(_pixel.Intensity >= _pixelcluster.LastIntensityValue)
						{
							_pixelcluster.AddPixeltoCluster(_pixel);
							//The Cluster is Still Increasing
							_intensitytrans = PixelClusterIntensityTranistion_t.INCREASING;
						}
						else if(_pixel.Intensity == 0) 
						{
							//The Cluster Died. There was no Decreasing Stage. Invalid
							_pixelcluster.Clear();
							_intensitytrans = PixelClusterIntensityTranistion_t.STARTINGCLUSTER;
						}
						else if(_pixel.Intensity < _pixelcluster.LastIntensityValue)
						{
							_pixelcluster.AddPixeltoCluster(_pixel);
							//The Cluster Has Started to Decrease.
							_intensitytrans = PixelClusterIntensityTranistion_t.DECREASING;
						}
						break;
					case PixelClusterIntensityTranistion_t.DECREASING:
						if(_pixel.Intensity == 0) //The End of The Cluster Add It to the Column
						{
							if (_pixelcluster.Count >= _min_cluster_size) //Only Add if the Cluster Meets the Minimum Cluster Size.
							{
								//The Cluster is Over. Set the Cluster Centroid and Add it to the column
								_pixelcluster.SetCentroid();
								_pixelcolumn.Add(_pixelcluster);
							}
							//Destroy The Old cluster and start a new.
							_pixelcluster = new PixelCluster();
							//Start Looking For a New Cluster in the Column
							_intensitytrans = PixelClusterIntensityTranistion_t.STARTINGCLUSTER;
						}
						else if(_pixel.Intensity > _pixelcluster.LastIntensityValue) 
						{
							//The cluster has started increasing it is invalid.
							//clear it and start over.
							_pixelcluster.Clear();
							_intensitytrans = PixelClusterIntensityTranistion_t.STARTINGCLUSTER;
						}
						else if(_pixel.Intensity <= _pixelcluster.LastIntensityValue)
						{
							//The Cluster is still decreasing.
							_pixelcluster.AddPixeltoCluster(_pixel);
							_intensitytrans = PixelClusterIntensityTranistion_t.DECREASING;
						}
						break;
				}
				//Setup For Next Row.
				++_rowindex;
				//Check for End Of Row And Setup For New Column
				if (_rowindex >= _height)
				{
					//NOTE: Clusters Cannot Run Off the Screen. 
					//If a Cluster is halfway being built it is lost. 
					
					//Add the column to the column list if appropriate.
					if(_pixelcolumn.Count > 0)
					{
						//Set the column number of this pixelcolumn 
						_pixelcolumn.Column = _colindex + 1;
						//Add column to the column list
						_pixelcolumnlist.Add(_pixelcolumn);
					}
					//Renew the column
					_pixelcolumn = new PixelColumn();
					//deal with flat array handlers
					++_colindex;
					_rowindex = 0;
				}
				//Increment Loop Control Counter.
				++_counter;		
			} while (_counter < _total_data_elements - 1);

			//The _pixelclusterlist is now built. 
			return ScanResult_t.SCAN_OK;
		}
		//-------------------------------------
        private ScanResult_t BuildLaserLineCenterList(ISensorSettings _sensor_settings, ref LaserLineCenter _laserline, List<PixelColumn> _pixelcolumnlist, int _min_pixel_count = 400)
        {
            _laserline.Clear();
            //Try And Deal With Columns That Have Multiple Clusters Using Continuity
            //Get First Column with a count of 1
            for (int i = 0; i < _pixelcolumnlist.Count; ++i)
            {
                if (_pixelcolumnlist[i].Count == 1)
                {
                    LaserLinePixel _laserpixel = new LaserLinePixel();
                    _laserpixel.X = _pixelcolumnlist[i].Column;
                    _laserpixel.Y = _pixelcolumnlist[i].PixelCluster[0].Centroid;
                    _laserline.Add(_laserpixel);
                }
            }

            return ScanResult_t.SCAN_OK;
        }
		#endregion

		#region Drawing and Graphics
		//-------------------------------------
		private void DrawImages(ISensorSettings _sensor_settings, ref Emgu.CV.Image<Gray, byte> _skelimage, ref Emgu.CV.Image<Bgr, byte> _operationimage, ref Emgu.CV.Image<Gray, byte> _thresholdimage)
		{

			int _width = _skelimage.Width;
			int _height = _skelimage.Height;

			_skelimage.Dispose();
			_skelimage = new Image<Gray, byte>(_width, _height);
			_operationimage.Dispose();
			_operationimage = new Image<Bgr, byte>(_width, _height);
			double _fontsize = .85;

			//Draw Columite Image to OperationFrame
			if (_sensor_settings.DrawColumiteImageOnOperationsImage == DrawColumiteLineData_t.DRAWN)
			{
				if (pixelcolumnlist.Count > 0)
				{
					Image<Gray, byte> _columniteimage = new Image<Gray, byte>(_width, _height);

					foreach(PixelColumn _column in pixelcolumnlist)
                    {
                        _column.DrawColumn(ref _columniteimage);
                    }
					_operationimage = _columniteimage.Convert<Bgr, byte>();
					string _colint = "Image Total Intensity = " + columnite_intensity.ToString();
					System.Drawing.Point _pnttxt = new System.Drawing.Point(20, 20);
					_operationimage.Draw(_colint, _pnttxt, FontFace.HersheyPlain, _fontsize, new Bgr(Color.AliceBlue));
				}
				else
				{
					_operationimage = _thresholdimage.Convert<Bgr, byte>();
					string _colint = "Image Total Intensity = " + columnite_intensity.ToString();
					System.Drawing.Point _pnttxt = new System.Drawing.Point(20, 20);
					_operationimage.Draw(_colint, _pnttxt, FontFace.HersheyPlain, _fontsize, new Bgr(Color.AliceBlue));
				}
			}

            //Draw the laser line center to the skel image
            laserline.DrawLaserLineCenter(ref _skelimage);
            if (_sensor_settings.DrawRawData == DrawLineDataPoints_t.DRAWN) laserline.DrawLaserLineCenter(ref _operationimage, new Bgr(Color.Red));

			//Draw Tracking Lines
			if (_sensor_settings.DrawTrackingLines == DrawLineSegmentData_t.DRAWN)
			{

				if (LSD != null)
				{
					bool linecolorswitcher = false;
					Color color = new Color();

					foreach (LineSegment2dDouble _line in LSD)
					{
						if (linecolorswitcher) color = Color.AliceBlue;
						else color = Color.CadetBlue;
						_line.DrawLineToEmguCVImage(ref _operationimage, color);
						linecolorswitcher = !linecolorswitcher;
					}

					string _tli = "Number of Tracking Lines = " + LSD.Count();
					System.Drawing.Point _pnttxt = new System.Drawing.Point(20, 40);
					_operationimage.Draw(_tli, _pnttxt, FontFace.HersheyPlain, _fontsize, new Bgr(Color.AliceBlue));
				}
			}

			//DrawJointTrackingPoint
			if (_sensor_settings.DrawJointTrackingPoints == DrawJointTrackingPoint_t.DRAWN)
			{


			}
		}
		//-------------------------------------
		#endregion

		#region Special Algorythms
		//_DoRANSAC(..)
		//_min_point_threshold = The minimum number of good data points that must exist in _lpd
		//_ransac_iterations = The Number of Random Samples the Ransac Does - The More You Do The More Accurate you get
		//_min_inlier_count = The minimum number of inliers the line must have to be considered valid.
		//_threshold_inlier_value = The distance a datapoint must be to the line to be considered close enough to be counted as an inlier.
		//*******************************************************
		private bool DoRANSAC(ISensorSettings _sensor_settings, List<Point2dDouble> _lpd, ref LineSegment2dDouble _line, int _min_point_threshold = 20,
							  int _ransac_iterations = 50, int _minimum_inlier_count = 20, double _threshold_inlier_value = 2.0)
		{
			_line.MakeInvalid();

			if (_lpd == null) return false;
			if (_line == null) return false;
			if (_sensor_settings == null) return false;
			if (_lpd.Count < _min_point_threshold) return false;
			//----------------------------------------------------------------
			//Now Using the Safe Data Do the Ransac
			System.Random _random = new Random();
			int[] _random_indexs = new int[2];
			List<int> _inliers = new List<int>();

			for (int k = 0; k < _ransac_iterations; ++k)
			{
				//Get Two Random Indexs
				//Are Not Considering Duplicates
				_random_indexs[0] = _random.Next(0, _lpd.Count);
				_random_indexs[1] = _random.Next(0, _lpd.Count);
				do
				{
					_random_indexs[1] = _random.Next(0, _lpd.Count);
				} while (_random_indexs[1] == _random_indexs[0]);

				//Got Two Good Points Create A Line Segment

				Point2dDouble _p1_temp = new Point2dDouble();
				Point2dDouble _p2_temp = new Point2dDouble();

				//Put the Points In Order Of X Value
				if (_lpd[_random_indexs[0]].X > _lpd[_random_indexs[1]].X)
				{
					_p1_temp.X = _lpd[_random_indexs[1]].X;
					_p1_temp.Y = _lpd[_random_indexs[1]].Y;
					_p2_temp.X = _lpd[_random_indexs[0]].X;
					_p2_temp.Y = _lpd[_random_indexs[0]].Y;
				}
				else
				{
					_p1_temp.X = _lpd[_random_indexs[0]].X;
					_p1_temp.Y = _lpd[_random_indexs[0]].Y;
					_p2_temp.X = _lpd[_random_indexs[1]].X;
					_p2_temp.Y = _lpd[_random_indexs[1]].Y;
				}
				LineSegment2dDouble _temp_line = new LineSegment2dDouble(_p1_temp, _p2_temp);


				//Fill an Inlier List
				List<int> _temp_inlier_index = new List<int>();
				for (int i = 0; i < _lpd.Count; ++i)
				{
					double _distance = _temp_line.DistanceToLine(_lpd[i]);
					if (_distance <= _threshold_inlier_value)
					{
						_temp_inlier_index.Add(i);
					}
				}

				if (_temp_inlier_index.Count > _inliers.Count)
				{
					_inliers.Clear();
					_inliers = _temp_inlier_index.ToList();
				}
			}

			if (_minimum_inlier_count > _inliers.Count) return false;

			//Now That We Have The Highest inlier Count in all the data series 
			//Do a Linear Regression of the Line
			double _sum_y = 0;
			double _sum_x_squared = 0;
			double _sum_x = 0;
			double _n = 0;
			double _sum_xy = 0;

			foreach (int _inlier_index in _inliers)
			{
				_sum_x += _lpd[_inlier_index].X;
				_sum_y += _lpd[_inlier_index].Y;
				_sum_xy += _lpd[_inlier_index].X * _lpd[_inlier_index].Y;
				_sum_x_squared += _lpd[_inlier_index].X * _lpd[_inlier_index].X;
				++_n;
			}
			double _denominator = (_n * _sum_x_squared) - (_sum_x * _sum_x);
			if (_denominator == 0) return false;
			double _slope = ((_n * _sum_xy) - (_sum_x * _sum_y)) / _denominator;
			double _yint = ((_sum_y * _sum_x_squared) - (_sum_x * _sum_xy)) / _denominator;

			//Set Line
			_line.Point1 = new Point2dDouble(0, _yint);
			_line.Point2 = new Point2dDouble((double)_sensor_settings.CameraFrameWidth, (_slope * (double)_sensor_settings.CameraFrameWidth) + _yint);
			return true;
		}
		#endregion

		#region Accessors
		//Accessors
		public Point2dDouble[] CurrentJointTrackingPoints
		{
			get { return JTP; }
			set
			{
				if (value.Length != 3) return;
				JTP[0].SetEqualTo(value[0]);
				JTP[1].SetEqualTo(value[1]);
				JTP[2].SetEqualTo(value[2]);
			}
		}
		//-------------------------------------
		public LineSegment2dDouble CurrentRootLine
		{
			get { return rootline; }
			set { rootline.SetEqualTo(value); }
		}
		//-------------------------------------
		public LineSegment2dDouble _PZL
		{
			get { return PZL; }
		}
		//-------------------------------------
		public LineSegment2dDouble _PZR
		{
			get { return PZR; }
		}
		//-------------------------------------
		public Point2dDouble[] _JTP
		{
			get { return JTP; }
		}
		//-------------------------------------
		public ScanResult_t ScanResult
		{
			get { return scan_result; }
			set { scan_result = value; }
		}
		//-------------------------------------
		#endregion
	}
}
