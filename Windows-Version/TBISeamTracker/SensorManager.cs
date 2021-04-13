using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Emgu.CV;
using Emgu.CV.Structure;
using System.Threading;

namespace TBISeamTracker
{
	//******************************************************
	enum JTPIdentifier_t { LEFT_SIDE = 0, CENTER = 1, RIGHT_SIDE = 2 }
	enum ScanResult_t
	{
		SCAN_NOT_DONE, SCAN_OK, TOTAL_IMAGE_INTENSITY_LARGER_THAN_THRESHOLD, LEFT_PZL_LPD_MATCH_FAILURE, LEFT_PZL_ANGLE_FAILURE, RIGHT_PZL_LPD_MATCH_FAILURE, RIGHT_PZL_ANGLE_FAILURE,
		ROOT_LINE_LENGTH_FAILURE, ROOT_LINE_ANGLE_FAILURE, RIGHT_JTP_BOUNDS_FAILURE, LEFT_JTP_BOUNDS_FAILURE, LDS_MIN_COUNT_FAILURE, LPD_CONTINUITY_FAILURE,
		TIME_DOMAIN_NOT_SATISFIED, DATA_CANNOT_BE_SEGMENTED_FOR_RANSAC, EMGU_CV_ERROR, PASSED_IN_METHOD_PARAMETERS_NOT_VALID, NOT_ENOUGH_PIXEL_CLUSTERS
	}

	//******************************************************
	

	//******************************************************
	
	//******************************************************
	
	//******************************************************
	class SensorManager
	{
		//******************************************************
		#region Private Members
		//-------------------------------------
		private SensorSettings sensor_settings;
		private bool isinloop;
		private Emgu.CV.Image<Gray, byte> cameraimage;
		private Emgu.CV.Image<Gray, byte> blurimage;
		private Emgu.CV.Image<Gray, byte> thresholdimage;
		private Emgu.CV.Image<Gray, byte> skeletalimage;
		public Emgu.CV.Image<Bgr, byte> operationimage;
        public Point2dDouble tracktopoint;
        public Point2dDouble tracktopoint_offset;
        private VGrooveScanManager vgroovescanner;

		//-------------------------------------
		#endregion
		//******************************************************

		//******************************************************
		#region Constructors DeConstructors
		//-------------------------------------
		public SensorManager()
		{
			//instantiate settings object
			sensor_settings = new SensorSettings();
			isinloop = false;
			//instaniate Emgu Images
			cameraimage = new Image<Gray, byte>(sensor_settings.CameraFrameWidth, sensor_settings.CameraFrameHeight);
			blurimage = new Image<Gray, byte>(sensor_settings.CameraFrameWidth, sensor_settings.CameraFrameHeight);
			thresholdimage = new Image<Gray, byte>(sensor_settings.CameraFrameWidth, sensor_settings.CameraFrameHeight);
			skeletalimage = new Image<Gray, byte>(sensor_settings.CameraFrameWidth, sensor_settings.CameraFrameHeight);
			operationimage = new Image<Bgr, byte>(sensor_settings.CameraFrameWidth, sensor_settings.CameraFrameHeight);
            tracktopoint = new Point2dDouble(double.NaN, double.NaN);
            tracktopoint_offset = new Point2dDouble(0, 0);
            vgroovescanner = new VGrooveScanManager();
            
            
		}
		//-------------------------------------
		#endregion
		//******************************************************

		//******************************************************
		#region Methods
		//-------------------------------------
		public void SaveSettingsToFile()
		{
			this.sensor_settings.SaveSettingsToFile();
		}
		//-------------------------------------
		public void LoadSettingsFromFile()
		{
			this.sensor_settings.LoadSettingsFromFile();
			
		}
		//-------------------------------------
		public void DoLoop(Emgu.CV.Image<Gray, byte> _cameraimage, ref MotorController _motor_controller)
		{
			//Make Sure Camera Image is Ok.
			if (_cameraimage == null) return;

			//Make Sure Not in the Loop
			if (isinloop) return;
			isinloop = true;

			//Do Image Processing
			cameraimage = _cameraimage.Clone();
			blurimage = cameraimage.SmoothGaussian(sensor_settings.BlurrValue);
			thresholdimage = blurimage.ThresholdToZero(new Gray(sensor_settings.ThresholdValue));

            vgroovescanner.ProcessVJoint(this.SensorSettingsInterface, ref thresholdimage, ref skeletalimage, ref operationimage);

			//Process The Image Data
			//Base Image Drawing is Done inside the Sensor Data Class
			//sensor_data.ProcessSensorData(sensor_settings as ISensorSettings, ref thresholdimage, ref skeletalimage, ref operationimage, ref columniteimage, tracktopoint_offset);

            //Process Correct Tracking Mode State. Cannot Do Any Tracking If System is Not Calibrated and the Tracking Points are Not Valid
            //if (tracktopoint.isValidPoint() && sensor_data.JointTrackingPoint.isValidPoint())
                {
                    if (sensor_settings.TrackingModeSensorState == TrackingModeSensorState_t.AUTOMODE_ON_FULL)
                    {
                        if (sensor_settings.IsCalibrated)
                        {
                           
                        }
                    }
                    else if (sensor_settings.TrackingModeSensorState == TrackingModeSensorState_t.AUTOMODE_ON_HEIGHT_ONLY)
                    {
                        if (sensor_settings.IsCalibrated)
                        {
                           
                        }
                    }
                    else if (sensor_settings.TrackingModeSensorState == TrackingModeSensorState_t.AUTOMODE_CALIBRATING)
                    {
                    
                        DoAutomaticCalibration(ref _motor_controller);
                    }               
            }

            //Process The Setting of TrackTo Point For Each the Automode Full and Height Only Modes
            if (sensor_settings.TrackingModeSensorState == TrackingModeSensorState_t.SETTRACKTO_AUTOMODEFULL)
            {
                
            }
            else if (sensor_settings.TrackingModeSensorState == TrackingModeSensorState_t.SETTRACKTO_AUTOMODEHEIGHTONLY)
            {
               
            }
            else if (sensor_settings.TrackingModeSensorState == TrackingModeSensorState_t.SETTRACKTO_AUTOCALIBRATE)
            {

            }
            isinloop = false;
		}
        //-------------------------------------
        public void DoAutomaticCalibration(ref MotorController _motor_controller)
        {
            if (sensor_settings.TrackingModeSensorState == TrackingModeSensorState_t.AUTOMODE_CALIBRATING)
            {
                switch (sensor_settings.AutoModeCalibrationState)
                {
                    case AutoModeTrackingCalibrationState_t.NOT_AUTOMODE_CALIBRATING:
                       
                        if (tracktopoint.isValidPoint())
                        {
                            sensor_settings.AutoModeCalibrationState = AutoModeTrackingCalibrationState_t.MOVING_Z;
                            _motor_controller.MoveZ(10);
                        }
                        else
                        {
                            sensor_settings.TrackingModeSensorState = TrackingModeSensorState_t.AUTOMODE_OFF;
                            sensor_settings.AutoModeCalibrationState = AutoModeTrackingCalibrationState_t.NOT_AUTOMODE_CALIBRATING;
                        }
                        break;
                    case AutoModeTrackingCalibrationState_t.MOVING_Z:
                        if (_motor_controller.GetStatusZ == MotorControllerStatus_t.OK)
                        {
                            Point2dDouble _calpnt = new Point2dDouble();
                            if (_calpnt.isValidPoint())
                            {
                                sensor_settings.PixelsPerMMZ = 10.0 / Math.Abs(tracktopoint.Y - _calpnt.Y);
                                sensor_settings.AutoModeCalibrationState = AutoModeTrackingCalibrationState_t.MOVING_X;
                                _motor_controller.MoveX(10);
                            }
                            else
                            {
                                sensor_settings.TrackingModeSensorState = TrackingModeSensorState_t.AUTOMODE_OFF;
                                sensor_settings.AutoModeCalibrationState = AutoModeTrackingCalibrationState_t.NOT_AUTOMODE_CALIBRATING;
                                tracktopoint.MakeInvalid();
                            }
                        }
                        break;
                    case AutoModeTrackingCalibrationState_t.MOVING_X:
                        if (_motor_controller.GetStatusX == MotorControllerStatus_t.OK)
                        {
                            Point2dDouble _calpnt = new Point2dDouble();
                            if (_calpnt.isValidPoint())
                            {
                                sensor_settings.PixelsPerMMX = 10.0 / Math.Abs(tracktopoint.X - _calpnt.X);
                                sensor_settings.AutoModeCalibrationState = AutoModeTrackingCalibrationState_t.NOT_AUTOMODE_CALIBRATING;
                                sensor_settings.TrackingModeSensorState = TrackingModeSensorState_t.AUTOMODE_OFF;
                                _motor_controller.MoveX(-10);
                                _motor_controller.MoveZ(-10);
                            }
                            else
                            {
                                sensor_settings.TrackingModeSensorState = TrackingModeSensorState_t.AUTOMODE_OFF;
                                sensor_settings.AutoModeCalibrationState = AutoModeTrackingCalibrationState_t.NOT_AUTOMODE_CALIBRATING;
                                tracktopoint.MakeInvalid();
                            }
                        }
                        break;
                }
            }
        }

        #endregion
        //******************************************************

        //******************************************************
        #region Accessors
        //-------------------------------------
        public Point2dDouble PointToTrack
        {
            get { return tracktopoint; }
            set { tracktopoint.X = value.X; tracktopoint.Y = value.Y; }
        }
		public ISensorSettings SensorSettingsInterface
		{
			get { return sensor_settings as ISensorSettings; }
		}
		//-------------------------------------
		public Emgu.CV.Image<Bgr, byte> OperationImage
		{
			get { return operationimage; }
		}
		//-------------------------------------
		public Emgu.CV.Image<Gray, byte> CameraImage
		{
			get { return cameraimage; }
		}
		//-------------------------------------
		public Emgu.CV.Image<Gray, byte> BlurImage
		{
			get { return blurimage; }
		}
		//-------------------------------------
		public Emgu.CV.Image<Gray, byte> ThresholdImage
		{
			get { return thresholdimage; }
		}
		//-------------------------------------
		public Emgu.CV.Image<Gray, byte> SkeletonImage
		{
			get { return skeletalimage; }
		}
		//-------------------------------------
		public ref SensorSettings SettingsReference
		{
			get { return ref sensor_settings; }
		}
		//-------------------------------------
		#endregion
		//******************************************************
	}
}
