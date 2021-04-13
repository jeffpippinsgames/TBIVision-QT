using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using Basler.Pylon;

namespace TBISeamTracker
{
	//******************************************************
	#region Enums and TypeDefs and Consts
	//-------------------------------------
	enum JointType_t {NONE, FILLET, VGROOVE, BUTTJOINT};
	//-------------------------------------
	enum ViewType_t {NONE, NORMALOPERATION, RAWFRAME, BLURFRAME, THRESHOLDFRAME, SKELETONFRAME, TESTIMAGEFRAME};
	//-------------------------------------
	enum TrackingModeSensorState_t { AUTOMODE_OFF, AUTOMODE_ON_FULL, AUTOMODE_ON_HEIGHT_ONLY, AUTOMODE_CALIBRATING, SETTRACKTO_AUTOMODEFULL, SETTRACKTO_AUTOMODEHEIGHTONLY, SETTRACKTO_AUTOCALIBRATE};
	//-------------------------------------
	enum AutoModeTrackingCalibrationState_t { NOT_AUTOMODE_CALIBRATING, MOVING_Z, Z_DONE_MOVING, MEASURE_Z, MOVING_X, X_DONE_MOVING, MEASURE_X, FINISHED_CALIBRATED };
	//-------------------------------------
	enum DrawLineDataPoints_t { NOT_DRAWN, DRAWN};
	//-------------------------------------
	enum DrawPredictedZeroLine_t { NOT_DRAWN, DRAWN};
	//-------------------------------------
	enum DrawLineSegmentData_t { NOT_DRAWN, DRAWN};
	//-------------------------------------
	enum DrawJointTrackingPoint_t { NOT_DRAWN, DRAWN};
	//-------------------------------------
	enum DrawColumiteLineData_t { NOT_DRAWN, DRAWN};
	//-------------------------------------
	enum StepsPerInch_t { XPOS = 0, ZPOS = 1, MAX_AXIS = 2};
	//-------------------------------------
	enum GeometicSeamDescriptor_t {ROUNDSEAM = 0, STRAIGHTSEAM = 1};
    //-------------------------------------
    enum PZLAlgrorythm {};
    //-------------------------------------
    enum VGrooveJTPLocation_t { ALL_JOINT, LEFT_SIDE, RIGHT_SIDE }
    //-------------------------------------
	



    #endregion
    //******************************************************
    interface ISensorSettings
	{
		int CameraFrameWidth { get; }
		int CameraFrameHeight { get; }
		int ThresholdBlurValue { get; set; }
		int ThresholdValue { get; set; }
		int BlurrValue { get; set; }

		DrawLineDataPoints_t DrawRawData { get; set; }
		DrawPredictedZeroLine_t DrawZeroLineState { get; set; }
		DrawLineSegmentData_t DrawTrackingLines { get; set; }
		DrawJointTrackingPoint_t DrawJointTrackingPoints { get; set; }
		DrawColumiteLineData_t DrawColumiteImageOnOperationsImage{ get; set;}
		AutoModeTrackingCalibrationState_t AutoModeCalibrationState { get; set; }
		TrackingModeSensorState_t TrackingModeSensorState { get; set; }
		JointType_t JointType { get; set; }
		ViewType_t CurrentView { get; set; }
		GeometicSeamDescriptor_t GeometricSeamDescriptor { get; set; }

		double PixelsPerMMX { get; }
		double PixelsPerMMZ { get; }
		void SaveSettingsToFile();
		void LoadSettingsFromFile();
        void CheckAndLoadDefaultFile();

        double ColumnIntensityFilterValue { get; set; }
		double TrackingChangeThresholdDistance { get; set; }
		double GrblJogValue { get; }
		double HorizontalLineDefinition { get; }
        bool IsCalibrated { get; }
        VGrooveJTPLocation_t VGrooveJTPLocation { get; set; }
	    int ScanThresholdMinLSDCount { get; set; }
		int ScanThresholdMaxNumberOfDiscontinuities { get; set; }
		double ScanThresholdLPDDiscontiuityValue { get; set; }
		int PZL_RANSAC_Iterations { get; set; }
		double PZL_RANSAC_InlierValue { get; set; }
		int PZL_RANSAC_MinInlierCount { get; set; }
		double ScanThresholdVGrooveJTPtoPZLDistance { get; set; }
		double ScanThresholdJTPBoundingDistance { get; set; }
		double ScanThresholdRootlineLengthDifference { get; set; }
		double ScanThresholdRoolineAngleDifference { get; set; }
		double ScanThresholdPZLAngleDifference { get; set; }
		int ScanThresholdMinConsequetiveScansForReassignment { get; set; }



	}
	//******************************************************
	class SensorSettings : ISensorSettings
	{
		//******************************************************
		#region Private Members
		//-------------------------------------
		//Class Constants
		public const int Camera_Frame_Width = 720;
		public const int Camera_Frame_Height = 540;
		public const int X_Axis = 0;
		public const int Z_Axis = 1;
		public const double GrblJogVal = 500;
		public const double Horizontal_Line_Definition = 2.0;

		//Vgroove Tracking Paramaters
		private double vgroove_rootline_length_difference;
		private double vgroove_rootline_angle_difference;
		private double vgroove_pzl_angle_difference;




		//EmguCV Settings
		int blurr_value;
		int threshold_value;
		int threshold_blurr_value;
		//Sensor Settings
		JointType_t joint_type;
		ViewType_t current_view;
		DrawLineDataPoints_t draw_raw_data;
		DrawPredictedZeroLine_t draw_zero_line;
		DrawLineSegmentData_t draw_tracking_lines;
		DrawJointTrackingPoint_t draw_tracking_points;
		DrawColumiteLineData_t draw_columite_image_on_operation_frame;
		TrackingModeSensorState_t tracking_mode_sensor_state;
		AutoModeTrackingCalibrationState_t automode_tracking_calibration_state;
		GeometicSeamDescriptor_t geometricseamtype;
		VGrooveJTPLocation_t vgroove_jtp_location;
		double[] pixels_per_mm;
		double column_intensity_filter_value;
		double tracking_change_threshold_distance;

		//Scan Threshold Values
		int scan_threshold_min_lsd_count; //The minimum LSD Count to Register a Scan Failure
		int scan_threshold_lpd_max_number_discontinuities; //The Maximum Number Of Discontinuities in the LPD Set Allowed
		double scan_threshold_lpd_discontinoutity_value; //The Value That Defines What a Discontiuitie Is.
		double scan_threshold_vgroove_pzl_jtp_distance; //Distance Needed To Find the Left and Right Side Tracking Points. From PZL to Lowest Point in Joint. For V Grooves
		double scan_threshold_jtp_bounding_distance; //Distance the JTP is allowed to jump between scans.
		double scan_threshold_rootline_length_difference; //The max difference in distance the rootline can change length between scans 
		double scan_threshold_rootline_angle_difference; //The max difference the rootline angles can change between scans
		double scan_threshold_pzl_angle_difference; //The Max difference the PZL angle can be between scans
		int scan_threshold_min_positive_scans_for_reassignment; //The Number of Consequetive Scans needed for Reassignment of the Set To Scan


		//SensorData Algorythm Values
		int pzl_ransac_iterations; //Number of Iterations to Run For the PZL Ransac Calls
		double pzl_ransac_inlier_threshold_value; //The Inlier Value For the PZL Ransac Calls
		int pzl_ransac_min_inlier_count; //The Minimum Number of Inliers For the PZL Rancac To Not Fail



		//-------------------------------------
		#endregion
		//******************************************************

		//******************************************************
		#region Constructors/DeConstructors
		//-------------------------------------
		public SensorSettings()
		{
			//-------------------------------------
			//Basic Img Proc Values
			this.blurr_value = 1;
			this.threshold_value = 200;
			this.threshold_blurr_value = 1;
			this.joint_type = JointType_t.NONE;
			this.current_view = ViewType_t.NORMALOPERATION;
			this.tracking_mode_sensor_state = TrackingModeSensorState_t.AUTOMODE_OFF;
			this.automode_tracking_calibration_state = AutoModeTrackingCalibrationState_t.NOT_AUTOMODE_CALIBRATING;
			this.draw_raw_data = DrawLineDataPoints_t.DRAWN;
			this.draw_zero_line = DrawPredictedZeroLine_t.NOT_DRAWN;
			this.draw_tracking_lines = DrawLineSegmentData_t.NOT_DRAWN;
			this.draw_columite_image_on_operation_frame = DrawColumiteLineData_t.NOT_DRAWN;
			this.draw_tracking_points = DrawJointTrackingPoint_t.NOT_DRAWN;
			this.geometricseamtype = GeometicSeamDescriptor_t.ROUNDSEAM;
			pixels_per_mm = new double[2];
			pixels_per_mm[X_Axis] = double.NaN;
			pixels_per_mm[Z_Axis] = double.NaN;
			column_intensity_filter_value = 1000;
			tracking_change_threshold_distance = 4;
			vgroove_jtp_location = VGrooveJTPLocation_t.ALL_JOINT;
			//-------------------------------------
			//-------------------------------------
			//-------------------------------------
			//Threshold Values For Checking Scans
			scan_threshold_min_lsd_count = 4;
			scan_threshold_lpd_max_number_discontinuities = 4;
			scan_threshold_lpd_discontinoutity_value = 4;
			scan_threshold_vgroove_pzl_jtp_distance = 3;
			scan_threshold_jtp_bounding_distance = 10;
			scan_threshold_rootline_angle_difference = 2;
			scan_threshold_pzl_angle_difference = 2;
			scan_threshold_min_positive_scans_for_reassignment = 30;

		  //-------------------------------------
		  //-------------------------------------
		  //-------------------------------------
		  //SensorData Algorythm Values
		  pzl_ransac_iterations = 75;
			pzl_ransac_inlier_threshold_value = 2;
			pzl_ransac_min_inlier_count = 10;
			scan_threshold_rootline_length_difference = 10;


			//-------------------------------------
		}
		//-------------------------------------
		#endregion
		//******************************************************

		//******************************************************
		#region Methods
		//-------------------------------------
		//-------------------------------------
		public void SaveSettingsToFile()
		{
			if (tracking_mode_sensor_state != TrackingModeSensorState_t.AUTOMODE_OFF) return;

			SaveFileDialog saveDialog = new SaveFileDialog();
			saveDialog.Title = "Save Sensor State";
			saveDialog.Filter = "TBIVision Files (*.tbi)|*.tbi";
			saveDialog.CreatePrompt = true;
			saveDialog.OverwritePrompt = true;
			saveDialog.ValidateNames = true;

			if (saveDialog.ShowDialog() == DialogResult.OK)
			{
				Stream dataStream = saveDialog.OpenFile();
				BinaryWriter writer = new BinaryWriter(dataStream);
				//Data To Write
				writer.Write((int)this.joint_type);
				writer.Write((int)this.current_view);
				writer.Write(this.blurr_value);
				writer.Write(this.threshold_value);
				writer.Write(this.threshold_blurr_value);
				writer.Write(this.pixels_per_mm[(int)StepsPerInch_t.XPOS]);
				writer.Write(this.pixels_per_mm[(int)StepsPerInch_t.ZPOS]);
				writer.Write((int)this.draw_zero_line);
				writer.Write((int)this.draw_tracking_lines);
				writer.Write((int)this.draw_raw_data);
				writer.Write(this.column_intensity_filter_value);
				writer.Write((int)this.draw_tracking_points);
				writer.Write((int)this.draw_columite_image_on_operation_frame);
				writer.Write((int)this.geometricseamtype);
				writer.Write(this.scan_threshold_min_lsd_count);
				writer.Write(this.scan_threshold_lpd_max_number_discontinuities);
				writer.Write(this.scan_threshold_lpd_discontinoutity_value);
				writer.Write(this.pzl_ransac_iterations);
				writer.Write(this.pzl_ransac_inlier_threshold_value);
				writer.Write(this.pzl_ransac_min_inlier_count);
				writer.Write(this.scan_threshold_vgroove_pzl_jtp_distance);
				writer.Write(this.scan_threshold_jtp_bounding_distance);
				writer.Write(this.scan_threshold_rootline_length_difference);
				writer.Write(this.scan_threshold_rootline_angle_difference);
				writer.Write(this.scan_threshold_pzl_angle_difference);
				writer.Write(this.scan_threshold_min_positive_scans_for_reassignment);
			}
		}
		//-------------------------------------
		public void LoadSettingsFromFile()
		{
			if (automode_tracking_calibration_state != AutoModeTrackingCalibrationState_t.NOT_AUTOMODE_CALIBRATING) return;

			OpenFileDialog openDialog = new OpenFileDialog();
			openDialog.Filter = "tbi files (*.tbi)|*.tbi";

			if (openDialog.ShowDialog() == DialogResult.OK)
			{
				Stream dataStream = openDialog.OpenFile();
				BinaryReader reader = new BinaryReader(dataStream);
				//Data To Read
				this.joint_type = (JointType_t)reader.ReadInt32();
				this.current_view = (ViewType_t)reader.ReadInt32();
				this.blurr_value = reader.ReadInt32();
				this.threshold_value = reader.ReadInt32();
				this.threshold_blurr_value = reader.ReadInt32();
				this.pixels_per_mm[(int)StepsPerInch_t.XPOS] = reader.ReadDouble();
				this.pixels_per_mm[(int)StepsPerInch_t.ZPOS] = reader.ReadDouble();
				this.draw_zero_line = (DrawPredictedZeroLine_t)reader.ReadInt32();
				this.draw_tracking_lines = (DrawLineSegmentData_t)reader.ReadInt32();
				this.draw_raw_data = (DrawLineDataPoints_t)reader.ReadInt32();
				this.column_intensity_filter_value = reader.ReadDouble();
				this.draw_tracking_points = (DrawJointTrackingPoint_t)reader.ReadInt32();
				this.draw_columite_image_on_operation_frame = (DrawColumiteLineData_t)reader.ReadInt32();
				this.geometricseamtype = (GeometicSeamDescriptor_t)reader.ReadInt32();
				this.scan_threshold_min_lsd_count = reader.ReadInt32();
				this.scan_threshold_lpd_max_number_discontinuities = reader.ReadInt32();
				this.scan_threshold_lpd_discontinoutity_value = reader.ReadDouble();
				this.pzl_ransac_iterations = reader.ReadInt32();
				this.pzl_ransac_inlier_threshold_value = reader.ReadDouble();
				this.pzl_ransac_min_inlier_count = reader.ReadInt32();
				this.scan_threshold_vgroove_pzl_jtp_distance = reader.ReadDouble();
				this.scan_threshold_jtp_bounding_distance = reader.ReadDouble();
				this.scan_threshold_rootline_length_difference = reader.ReadDouble();
				this.scan_threshold_rootline_angle_difference = reader.ReadDouble();
				this.scan_threshold_pzl_angle_difference = reader.ReadDouble();
				this.scan_threshold_min_positive_scans_for_reassignment = reader.ReadInt32();

			}
		}
		//-------------------------------------
		public void CheckAndLoadDefaultFile()
		{
			if (automode_tracking_calibration_state != AutoModeTrackingCalibrationState_t.NOT_AUTOMODE_CALIBRATING) return;

			if (File.Exists("default_settings.tbi"))
			{
				Stream dataStream = File.Open("default_settings.tbi", FileMode.Open);
				BinaryReader reader = new BinaryReader(dataStream);
				//Data To Read
				this.joint_type = (JointType_t)reader.ReadInt32();
				this.current_view = (ViewType_t)reader.ReadInt32();
				this.blurr_value = reader.ReadInt32();
				this.threshold_value = reader.ReadInt32();
				this.threshold_blurr_value = reader.ReadInt32();
				this.pixels_per_mm[(int)StepsPerInch_t.XPOS] = reader.ReadDouble();
				this.pixels_per_mm[(int)StepsPerInch_t.ZPOS] = reader.ReadDouble();
				this.draw_zero_line = (DrawPredictedZeroLine_t)reader.ReadInt32();
				this.draw_tracking_lines = (DrawLineSegmentData_t)reader.ReadInt32();
				this.draw_raw_data = (DrawLineDataPoints_t)reader.ReadInt32();
				this.column_intensity_filter_value = reader.ReadDouble();
				this.draw_tracking_points = (DrawJointTrackingPoint_t)reader.ReadInt32();
				this.draw_columite_image_on_operation_frame = (DrawColumiteLineData_t)reader.ReadInt32();
				this.geometricseamtype = (GeometicSeamDescriptor_t)reader.ReadInt32();
				this.scan_threshold_min_lsd_count = reader.ReadInt32();
				this.scan_threshold_lpd_max_number_discontinuities = reader.ReadInt32();
				this.scan_threshold_lpd_discontinoutity_value = reader.ReadDouble();
				this.pzl_ransac_iterations = reader.ReadInt32();
				this.pzl_ransac_inlier_threshold_value = reader.ReadDouble();
				this.pzl_ransac_min_inlier_count = reader.ReadInt32();
				this.scan_threshold_vgroove_pzl_jtp_distance = reader.ReadDouble();
				this.scan_threshold_jtp_bounding_distance = reader.ReadDouble();
				this.scan_threshold_rootline_length_difference = reader.ReadDouble();
				this.scan_threshold_rootline_angle_difference = reader.ReadDouble();
				this.scan_threshold_pzl_angle_difference = reader.ReadDouble();
				this.scan_threshold_min_positive_scans_for_reassignment = reader.ReadInt32();
			}

		}


		#endregion
		//******************************************************

		//******************************************************
		#region Accessors
		//-------------------------------------
		public int ScanThresholdMinConsequetiveScansForReassignment
		{
			get { return scan_threshold_min_positive_scans_for_reassignment; }
			set { scan_threshold_min_positive_scans_for_reassignment = value; }
		}
		public double ScanThresholdPZLAngleDifference
		{
			get { return scan_threshold_pzl_angle_difference; }
			set { scan_threshold_pzl_angle_difference = value; }
		}
		//-------------------------------------
		public double ScanThresholdRoolineAngleDifference
		{
			get { return scan_threshold_rootline_angle_difference; }
			set { scan_threshold_rootline_angle_difference = value; }
		}
		//-------------------------------------
		public double ScanThresholdRootlineLengthDifference
		{
			get { return scan_threshold_rootline_length_difference; }
			set { scan_threshold_rootline_length_difference = value; }
		}
		//-------------------------------------
		public double ScanThresholdJTPBoundingDistance
		{
			get { return scan_threshold_jtp_bounding_distance; }
			set { scan_threshold_jtp_bounding_distance = value; }
		}
		//-------------------------------------
		public double ScanThresholdVGrooveJTPtoPZLDistance
		{
			get { return scan_threshold_vgroove_pzl_jtp_distance; }
			set { scan_threshold_vgroove_pzl_jtp_distance = value; }
		}
		//-------------------------------------
		public int PZL_RANSAC_MinInlierCount
		{
			get { return pzl_ransac_min_inlier_count; }
			set { pzl_ransac_min_inlier_count = value; }
		}
		//-------------------------------------
		public double PZL_RANSAC_InlierValue
		{
			get { return pzl_ransac_inlier_threshold_value; }
			set { pzl_ransac_inlier_threshold_value = value; }
		}
		//-------------------------------------
		public int PZL_RANSAC_Iterations
		{
			get { return pzl_ransac_iterations; }
			set { pzl_ransac_iterations = value; }
		}
		//-------------------------------------
		public double ScanThresholdLPDDiscontiuityValue
		{
			get { return scan_threshold_lpd_discontinoutity_value; }
			set { scan_threshold_lpd_discontinoutity_value = value; }
		}
		//-------------------------------------
		public int ScanThresholdMaxNumberOfDiscontinuities
		{
			get { return scan_threshold_lpd_max_number_discontinuities; }
			set { scan_threshold_lpd_max_number_discontinuities = value; }
		}
		//-------------------------------------
		public int ScanThresholdMinLSDCount
		{
			get { return scan_threshold_min_lsd_count; }
			set { scan_threshold_min_lsd_count = value; }
		}
		//-------------------------------------
		public VGrooveJTPLocation_t VGrooveJTPLocation
        {
            get { return vgroove_jtp_location; }
            set { vgroove_jtp_location = value; }
        }
        //-------------------------------------
        public double HorizontalLineDefinition
		{
			get { return Horizontal_Line_Definition; }
		}
		//-------------------------------------
		public GeometicSeamDescriptor_t GeometricSeamDescriptor
		{
			get { return this.geometricseamtype; }
			set { this.geometricseamtype = value; }
		}
		//-------------------------------------
		public double GrblJogValue
        {
            get { return GrblJogVal; }
        }
        //-------------------------------------
        public double TrackingChangeThresholdDistance
		{
			get { return tracking_change_threshold_distance; }
			set { tracking_change_threshold_distance = value; }
		}
		//-------------------------------------
		public DrawColumiteLineData_t DrawColumiteImageOnOperationsImage
		{
			get { return draw_columite_image_on_operation_frame; }
			set { draw_columite_image_on_operation_frame = value; }
		}
		//-------------------------------------
		public double ColumnIntensityFilterValue
		{
			get { return column_intensity_filter_value; }
			set { column_intensity_filter_value = value; }
		}
		//-------------------------------------
		public int CameraFrameWidth
		{
			get { return Camera_Frame_Width; }
		}
		//-------------------------------------
		public int CameraFrameHeight
		{
			get { return Camera_Frame_Height; }
		}
		//-------------------------------------
		public int ThresholdBlurValue
		{
			get { return this.threshold_blurr_value; }
			set { this.threshold_blurr_value = value; }
		}
		//-------------------------------------
		public int ThresholdValue
		{
			get { return this.threshold_value; }
			set { this.threshold_value = value; }
		}
		//-------------------------------------
		public int BlurrValue
		{
			get { return this.blurr_value; }
			set { this.blurr_value = value; }
		}
		//-------------------------------------
		public DrawJointTrackingPoint_t DrawJointTrackingPoints
		{
			get { return draw_tracking_points; }
			set { draw_tracking_points = value; }
		}
		//-------------------------------------
		public DrawLineDataPoints_t DrawRawData
		{
			get { return draw_raw_data; }
			set { draw_raw_data = value; }
		}
		//-------------------------------------
		public DrawLineSegmentData_t DrawTrackingLines
		{
			get { return draw_tracking_lines; }
			set { draw_tracking_lines = value; }
		}
		//-------------------------------------
		public DrawPredictedZeroLine_t DrawZeroLineState
		{
			get { return draw_zero_line; }
			set { draw_zero_line = value; }
		}
		//-------------------------------------
		public AutoModeTrackingCalibrationState_t AutoModeCalibrationState
		{
			get { return this.automode_tracking_calibration_state; }
			set { this.automode_tracking_calibration_state = value; }
		}
		//-------------------------------------
		public TrackingModeSensorState_t TrackingModeSensorState
		{
			get { return this.tracking_mode_sensor_state;}
			set { this.tracking_mode_sensor_state = value; }
		}
		//-------------------------------------
		public JointType_t JointType
		{
			get { return joint_type; }
			set { joint_type = value; }
		}
		//-------------------------------------
		public ViewType_t CurrentView
		{
			get { return current_view; }
			set { current_view = value; }
		}
		//-------------------------------------
		public double PixelsPerMMX
		{
			get { return pixels_per_mm[X_Axis]; }
            set { pixels_per_mm[X_Axis] = value; }
		}
		//-------------------------------------
		public double PixelsPerMMZ
		{
			get { return pixels_per_mm[Z_Axis]; }
            set { pixels_per_mm[Z_Axis] = value; }
		}
		//-------------------------------------
        public bool IsCalibrated
        {
            get
            {
                if(!double.IsNaN(pixels_per_mm[X_Axis]) &&  !double.IsNaN(pixels_per_mm[Z_Axis]))
                {
                    return true;
                }
                return false;
            }
        }
		#endregion
		//******************************************************
	}
}
