using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Diagnostics;
using Basler.Pylon;
using Emgu.CV;
//using Emgu.CV.CvEnum;
using Emgu.CV.Structure;
using Emgu.Util;
using System.Threading;
using System.IO.Ports;

namespace TBISeamTracker
{
	
	

	public partial class MainForm : Form
	{
		//******************************************************
		#region Class Members
		//-------------------------------------
		private Basler.Pylon.Camera camera;
		private bool IsGrabbingCameraFrame;
		private SensorManager sensor_manager;
		private Gamepad gamepad;
		private MotorController motorcontroller;
		private Stopwatch fps_stopwatch;
		private Stopwatch mainloop_stopwatch;
		private double largestmainlooptime;


		//-------------------------------------
		#endregion
		//******************************************************


		//******************************************************
		#region Constructors and DeConstructors
		//-------------------------------------
		public MainForm()
		{
			InitializeComponent();
			IsGrabbingCameraFrame = false;
			sensor_manager = new SensorManager();
			fps_stopwatch = new Stopwatch();
			mainloop_stopwatch = new Stopwatch();
			
			//Main Form Settings
			this.ControlBox = false;
			this.Text = string.Empty;
			this.StartPosition = FormStartPosition.Manual;
			this.Location = new Point(0, 0);
			this.largestmainlooptime = 0;

			//Update All Controls To Match Sensor Settings
			UpdateAllControlsToSettingsValues();

			//Instantiate Gamepad
			gamepad = new Gamepad();
			motorcontroller = new MotorController();

            
		}
		//-------------------------------------
		#endregion
		//******************************************************


		//******************************************************
		#region Main Form Load Method
		//-------------------------------------
		private void MainForm_Load(object sender, EventArgs e)
		{

			//Init Cameras
			AddCamerasToSelectionList();
			InitCamera();

			//Register Gamepad Events
			gamepad.OnControllerConnected += OnGamepadConnected;
			gamepad.OnControllerDisconnected += OnGamepadDisconnected;
			gamepad.OnBack_Pressed += OnGamepadBackPressed;
			gamepad.OnStart_Pressed += OnGamepadStartPressed;
			gamepad.OnDPadUp_Pressed += OnGamepadDPadUpPressed;
			gamepad.OnDPadDown_Pressed += OnGamepadDPadDownPressed;
			gamepad.OnDPadRight_Pressed += OnGamepadDPadRightPressed;
			gamepad.OnDPadLeft_Pressed += OnGamepadDPadLeftPressed;
			gamepad.OnAButton_Pressed += OnGamePadAButtonPressed;
			gamepad.OnBButton_Pressed += OnGamePadBButtonPressed;
			gamepad.OnYButton_Pressed += OnGamePadYButtonPressed;
			gamepad.OnDPadUp_Released += OnGamepadDPadUpReleased;
            gamepad.OnXButton_Pressed += OnGamePadXButtonPressed;
			gamepad.OnDPadDown_Released += OnGamepadDPadDownReleased;
			gamepad.OnDPadRight_Released += OnGamepadDPadRightReleased;
			gamepad.OnDPadLeft_Released += OnGamepadDPadLeftReleased;
			gamepad.OnLeftTrigger_Released += OnGamepadLeftTriggerReleased;

            //For Use When Testing Grbl
            motorcontroller.ConnectToMicroControllers();
			timer_uiupdater.Start();
            //Start Gamepad Thread Multi Threaded Gamepad Polling Not Implimented.
            //gamepad.StartPollingThread();
            //see if default file exsists.
            ISensorSettings _sensor_settings = sensor_manager.SensorSettingsInterface;
            _sensor_settings.CheckAndLoadDefaultFile();





        }
		//-------------------------------------
		#endregion
		//******************************************************
        

		//******************************************************
		#region OnImageGrabbed Method
		//-------------------------------------
		private void OnImageGrabbed(object sender, ImageGrabbedEventArgs e)
		{


			// If called from a different thread, we must use the Invoke method to marshal the call to the proper GUI thread.
			// The grab result will be disposed after the event call. Clone the event arguments for marshaling to the GUI thread.
			if (InvokeRequired)
			{
				BeginInvoke(new EventHandler<ImageGrabbedEventArgs>(OnImageGrabbed), sender, e.Clone());
				return;
			}


			//Check To Make Sure Not Already In Loop
			if (IsGrabbingCameraFrame)
			{
				//Dispose Buffer So Not to Overflow Camera
				e.DisposeGrabResultIfClone();
				return;
			}
			IsGrabbingCameraFrame = true;

			//Reset and Start Main Loop Timers
			mainloop_stopwatch.Reset();
			mainloop_stopwatch.Start();

			//Process Grab Image And Process Frame.
			try
			{
				//MAIN PART OF LOOP************************************************
				if (e.GrabResult.GrabSucceeded)
				{
					//Get Camera Frame, And Do Sensor Manager Loop
					Emgu.CV.Image<Gray, byte> cameraimage = new Image<Gray, byte>(e.GrabResult.Width, e.GrabResult.Height);
					cameraimage.Bytes = e.GrabResult.PixelData as byte[];
					//Do Sensor Manager Loop
					sensor_manager.DoLoop(cameraimage, ref motorcontroller);



					//Process Main ViewPort
					ISensorSettings _sensorsettings_interface = sensor_manager.SensorSettingsInterface;

					if ((_sensorsettings_interface.TrackingModeSensorState == TrackingModeSensorState_t.AUTOMODE_ON_FULL) || (_sensorsettings_interface.TrackingModeSensorState == TrackingModeSensorState_t.AUTOMODE_ON_HEIGHT_ONLY))
					{
						if (sensor_manager.PointToTrack.isValidPoint())
						{
                            Point2dDouble _trckpnt = new Point2dDouble((sensor_manager.tracktopoint.X + sensor_manager.tracktopoint_offset.X), (sensor_manager.tracktopoint.Y + sensor_manager.tracktopoint_offset.Y));
							_trckpnt.DrawPointToEmguCVImage(ref sensor_manager.operationimage, new Bgr(Color.Lime));
						}
					}

					switch (_sensorsettings_interface.CurrentView)
					{
						case ViewType_t.RAWFRAME:
							panAndZoomPictureBox_view.Image = (System.Drawing.Image)sensor_manager.CameraImage.Bitmap.Clone();
							break;
						case ViewType_t.NORMALOPERATION:
							panAndZoomPictureBox_view.Image = (System.Drawing.Image)sensor_manager.OperationImage.Bitmap.Clone();
							break;
						case ViewType_t.BLURFRAME:
							panAndZoomPictureBox_view.Image = (System.Drawing.Image)sensor_manager.BlurImage.Bitmap.Clone();
							break;
						case ViewType_t.SKELETONFRAME:
							panAndZoomPictureBox_view.Image = (System.Drawing.Image)sensor_manager.SkeletonImage.Bitmap.Clone();
							break;
						case ViewType_t.THRESHOLDFRAME:
							panAndZoomPictureBox_view.Image = (System.Drawing.Image)sensor_manager.ThresholdImage.Bitmap.Clone();
							break;
					}

					//Dispay Image Grabbing Information
					fps_stopwatch.Stop();
					double fps = 1000.0 / (double)fps_stopwatch.ElapsedMilliseconds;
					if (fps > largestmainlooptime) largestmainlooptime = fps;
					label_camera_status.Text = "Camera Streaming - FPS = " + fps.ToString("0.0" + "- Image Number " + e.GrabResult.ImageNumber.ToString());
					fps_stopwatch.Reset();
					fps_stopwatch.Start();
				}
				//END MAIN PART OF LOOP************************************************
				else
				{
					label_camera_status.Text = "Camera Not Streaming";
				}
			}
			catch (Exception exception)
			{
				//Show Exception
				MessageBox.Show("Exception caught:\n" + exception.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
			}
			finally
			{
				//Dispose Buffer So Not to Overflow Camera
				e.DisposeGrabResultIfClone();
			}

			//Display Time In Loop
			mainloop_stopwatch.Stop();
			label_mainlooptime.Text = "Time in Loop: " + mainloop_stopwatch.ElapsedMilliseconds.ToString() + " MilliSeconds. Total Ticks: " + mainloop_stopwatch.ElapsedTicks.ToString();

			//Adjust Camera Framerate is Needed
			//double _camera_time = 1.0/camera.Parameters[PLCamera.AcquisitionFrameRateAbs].GetValue();

			//Grbl Code


			//Reset Loop Check
			IsGrabbingCameraFrame = false;
		}
		//-------------------------------------
		#endregion
		//******************************************************


		//******************************************************
		#region Helper Methods
		//-------------------------------------
		private void UpdateAllControlsToSettingsValues()
		{
			SetFormControls_View();
			SetFormControls_BlurSetting();
			SetFormControls_ThresholdSetting();
			SetFormControls_SeamType();
			SetFormControls_ColumnIntensityFilterValue();
            SetFormControls_JointType();
		}
		//-------------------------------------
		private void SetFormControls_View()
		{
			ISensorSettings _sensorsettings_interface = sensor_manager.SensorSettingsInterface;

			panel_blurrview.Visible = false;
			panel_rawcameraview.Visible = false;
			panel_thresholdview.Visible = false;
			panel_operationViewTracking.Visible = false;
			panel_skeletalcamraview.Visible = false;
			operationViewToolStripMenuItem.Checked = false;
			blurrFrameToolStripMenuItem.Checked = false;
			rawCameraFrameToolStripMenuItem.Checked = false;
			thresholdFrameToolStripMenuItem.Checked = false;
			skeletonFrameToolStripMenuItem.Checked = false;
			viewZeroLinesToolStripMenuItem.Checked = false;
			viewTrackingLinesToolStripMenuItem.Checked = false;
			viewRawDataScanToolStripMenuItem.Checked = false;
			testFrameToolStripMenuItem.Checked = false;
			viewTrackingPointsToolStripMenuItem.Checked = false;
			drawColumniteImageOnOperationImageToolStripMenuItem.Checked = false;
			
			switch(_sensorsettings_interface.CurrentView)
			{
				case ViewType_t.BLURFRAME:
					blurrFrameToolStripMenuItem.Checked = true;
					panel_blurrview.Visible = true;
					break;
				case ViewType_t.NONE:
					break;
				case ViewType_t.NORMALOPERATION:
					operationViewToolStripMenuItem.Checked = true;
                    panel_operationViewTracking.Visible = true;
					break;
				case ViewType_t.RAWFRAME:
					rawCameraFrameToolStripMenuItem.Checked = true;
					panel_rawcameraview.Visible = true;
					break;
				case ViewType_t.SKELETONFRAME:
					skeletonFrameToolStripMenuItem.Checked = true;
					panel_skeletalcamraview.Visible = true;
					break;
				case ViewType_t.THRESHOLDFRAME:
					thresholdFrameToolStripMenuItem.Checked = true;
					panel_thresholdview.Visible = true;
					break;
				case ViewType_t.TESTIMAGEFRAME:
					testFrameToolStripMenuItem.Checked = true;
					panel_operationViewTracking.Visible = true;
					break;
			}

			if (_sensorsettings_interface.DrawZeroLineState == DrawPredictedZeroLine_t.DRAWN) viewZeroLinesToolStripMenuItem.Checked = true;
			if (_sensorsettings_interface.DrawTrackingLines == DrawLineSegmentData_t.DRAWN) viewTrackingLinesToolStripMenuItem.Checked = true;
			if (_sensorsettings_interface.DrawRawData == DrawLineDataPoints_t.DRAWN) viewRawDataScanToolStripMenuItem.Checked = true;
			if (_sensorsettings_interface.DrawJointTrackingPoints == DrawJointTrackingPoint_t.DRAWN) viewTrackingPointsToolStripMenuItem.Checked = true;
			if (_sensorsettings_interface.DrawColumiteImageOnOperationsImage == DrawColumiteLineData_t.DRAWN) drawColumniteImageOnOperationImageToolStripMenuItem.Checked = true;

		}
		//-------------------------------------
		private void SetFormControls_BlurSetting()
		{
			ISensorSettings _sensorsettings_interface = sensor_manager.SensorSettingsInterface;

			trackBar_blur.Value = _sensorsettings_interface.BlurrValue;
			label_blur.Text = "Blur Value = " + _sensorsettings_interface.BlurrValue.ToString();
		}
		//-------------------------------------
		private void SetFormControls_ThresholdSetting()
		{
			ISensorSettings _sensorsettings_interface = sensor_manager.SensorSettingsInterface;

			trackBar_threshold.Value = _sensorsettings_interface.ThresholdValue;
			label_threshold.Text = "Threshold Value = " + _sensorsettings_interface.ThresholdValue.ToString();
		}
		//-------------------------------------
		private void SetFormControls_SeamType()
		{
			ISensorSettings _sensorsettings_interface = sensor_manager.SensorSettingsInterface;

			roundSeamToolStripMenuItem.Checked = false;
			straightSeamToolStripMenuItem.Checked = false;

			switch(_sensorsettings_interface.GeometricSeamDescriptor)
			{
				case GeometicSeamDescriptor_t.ROUNDSEAM:
					roundSeamToolStripMenuItem.Checked = true;
					break;
				case GeometicSeamDescriptor_t.STRAIGHTSEAM:
					straightSeamToolStripMenuItem.Checked = true;
					break;
			}
			
		}
		//-------------------------------------
		private void SetFormControls_ColumnIntensityFilterValue()
		{
			ISensorSettings _sensorsettings_interface = sensor_manager.SensorSettingsInterface;

			trackBar_column_intensity_filter.Value = (int)_sensorsettings_interface.ColumnIntensityFilterValue;
			label_column_intensity_filter.Text = "Column Intensity Filter Value = " + _sensorsettings_interface.ColumnIntensityFilterValue.ToString();
		}
		//-------------------------------------
        private void SetFormControls_JointType()
        {
            ISensorSettings _sensor_settings = sensor_manager.SensorSettingsInterface;

            vGrooveToolStripMenuItem.Checked = false;
            buttToolStripMenuItem.Checked = false;
            filletToolStripMenuItem.Checked = false;
            equalOffsetToolStripMenuItem.Checked = false;
            leftSideTransistionOffsetToolStripMenuItem.Checked = false;
            rightSideTransitionOffsetToolStripMenuItem.Checked = false;

            switch(_sensor_settings.JointType)
            {
                case JointType_t.BUTTJOINT:
                    buttToolStripMenuItem.Checked = true;
                    break;
                case JointType_t.FILLET:
                    filletToolStripMenuItem.Checked = true;
                    break;
                case JointType_t.VGROOVE:
                    vGrooveToolStripMenuItem.Checked = true;
                    switch(_sensor_settings.VGrooveJTPLocation)
                    {
                        case VGrooveJTPLocation_t.ALL_JOINT:
                            equalOffsetToolStripMenuItem.Checked = true;
                            break;
                        case VGrooveJTPLocation_t.LEFT_SIDE:
                            leftSideTransistionOffsetToolStripMenuItem.Checked = true;
                            break;
                        case VGrooveJTPLocation_t.RIGHT_SIDE:
                            rightSideTransitionOffsetToolStripMenuItem.Checked = true;
                            break;
                    }
                    break;
            }
        }
		private void AddCamerasToSelectionList()
		{
			try
			{
				cameraSelectionToolStripMenuItem.DropDownItems.Clear();

				// Ask the camera finder for a list of camera devices.
				List<ICameraInfo> allCameras = CameraFinder.Enumerate();
			
				foreach(ICameraInfo cam in allCameras)
				{
					ToolStripMenuItem itm = new ToolStripMenuItem();
					itm.Tag = cam[CameraInfoKey.FullName] as object;
					itm.Text = cam[CameraInfoKey.FriendlyName];
					cameraSelectionToolStripMenuItem.DropDownItems.Add(itm);
				}
								
			}
			catch (Exception exception)
			{
				MessageBox.Show(exception.ToString());
			}
		}
		//-------------------------------------
		private void InitCamera()
		{
			try
			{
				if (cameraSelectionToolStripMenuItem.DropDownItems.Count > 0)
				{
					ISensorSettings _sensorsettings_interface = sensor_manager.SensorSettingsInterface;
					camera = new Basler.Pylon.Camera(CameraSelectionStrategy.FirstFound);
					camera.CameraOpened += Configuration.AcquireContinuous;
					camera.Open();
					//Set Camera TimeOut
					camera.Parameters[PLTransportLayer.HeartbeatTimeout].TrySetValue(1000, IntegerValueCorrection.Nearest);
					//Set Analog Controls
					camera.Parameters[PLCamera.GainAuto].SetValue(PLCamera.GainAuto.Off);
					camera.Parameters[PLCamera.GainSelector].SetValue(PLCamera.GainSelector.All);
					camera.Parameters[PLCamera.GainRaw].SetValue(130);
					camera.Parameters[PLCamera.BlackLevelSelector].SetValue(PLCamera.BlackLevelSelector.All);
					camera.Parameters[PLCamera.BlackLevelRaw].SetValue(0);
					camera.Parameters[PLCamera.GammaEnable].SetValue(false);
					camera.Parameters[PLCamera.GammaSelector].SetValue(PLCamera.GammaSelector.User);
					camera.Parameters[PLCamera.Gamma].SetValue(1);
					camera.Parameters[PLCamera.DigitalShift].SetValue(0);
					//Set Image Format Controls
					camera.Parameters[PLCamera.PixelFormat].SetValue(PLCamera.PixelFormat.Mono8);
					camera.Parameters[PLCamera.ReverseX].SetValue(false);
					camera.Parameters[PLCamera.ReverseY].SetValue(false);
					camera.Parameters[PLCamera.TestImageSelector].SetValue(PLCamera.TestImageSelector.Off);
					camera.Parameters[PLCamera.TestImageResetAndHold].SetValue(false);
					//Set AOI Controls
					camera.Parameters[PLCamera.Height].SetValue((long)_sensorsettings_interface.CameraFrameHeight);
					camera.Parameters[PLCamera.Width].SetValue((long)_sensorsettings_interface.CameraFrameWidth);
					camera.Parameters[PLCamera.AutoFunctionAOIOffsetX].SetValue(0);
					camera.Parameters[PLCamera.AutoFunctionAOIOffsetY].SetValue(0);
					camera.Parameters[PLCamera.BinningHorizontalMode].SetValue(PLCamera.BinningHorizontalMode.Sum);
					camera.Parameters[PLCamera.BinningHorizontal].SetValue(1);
					camera.Parameters[PLCamera.BinningVerticalMode].SetValue(PLCamera.BinningVerticalMode.Sum);
					camera.Parameters[PLCamera.BinningVertical].SetValue(1);
					//Set Image Quality Controls
					camera.Parameters[PLCamera.PgiMode].SetValue(PLCamera.PgiMode.Off);
					//Set Acqusition Controls
					camera.Parameters[PLCamera.AcquisitionMode].SetValue(PLCamera.AcquisitionMode.Continuous);
					camera.Parameters[PLCamera.AcquisitionFrameCount].SetValue(1);
					camera.Parameters[PLCamera.TriggerSelector].SetValue(PLCamera.TriggerSelector.FrameStart);
					camera.Parameters[PLCamera.TriggerMode].SetValue(PLCamera.TriggerMode.Off);
					camera.Parameters[PLCamera.ExposureMode].SetValue(PLCamera.ExposureMode.Timed);
					camera.Parameters[PLCamera.ExposureAuto].SetValue(PLCamera.ExposureAuto.Off);
					camera.Parameters[PLCamera.ExposureTimeAbs].SetValue(10000.0);
					camera.Parameters[PLCamera.AcquisitionFrameRateEnable].SetValue(true);
					camera.Parameters[PLCamera.AcquisitionFrameRateAbs].SetValue(30);
					camera.Parameters[PLCamera.AcquisitionStatusSelector].SetValue(PLCamera.AcquisitionStatusSelector.FrameTriggerWait);
					camera.Parameters[PLCamera.SyncFreeRunTimerEnable].SetValue(false);
					//Set Sequence Control
					camera.Parameters[PLCamera.SequenceEnable].SetValue(false);
					//Set Counters and Timers
					camera.Parameters[PLCamera.TimerSelector].SetValue(PLCamera.TimerSelector.Timer1);
					camera.Parameters[PLCamera.TimerDelayAbs].SetValue(0);
					camera.Parameters[PLCamera.TimerDurationAbs].SetValue(10);
					camera.Parameters[PLCamera.TimerTriggerSource].SetValue(PLCamera.TimerTriggerSource.ExposureStart);
					//Set Transport Layer Settings
					camera.Parameters[PLGigECamera.GevSCPSPacketSize].SetValue(1500); //Standard Packet Size is 1500 Bytes
					camera.Parameters[PLGigECamera.GevSCPD].SetValue(3500); //Inter Packet Delay
					camera.Parameters[PLGigECamera.GevSCFTD].SetValue(1000); //Frame Transmission Delay
					camera.Parameters[PLGigECamera.GevSCBWR].SetValue(10); //Bandwidth Reserve
					camera.Parameters[PLGigECamera.GevSCBWRA].SetValue(10); //Bandwidth Reserve Accum

					//Attach Event Handler
					camera.StreamGrabber.ImageGrabbed += OnImageGrabbed;
					camera.ConnectionLost += OnCameraConnectionLost;
					//Start Grabbing
					camera.StreamGrabber.Start(GrabStrategy.LatestImages, GrabLoop.ProvidedByStreamGrabber);
					//Start fps Timing
					fps_stopwatch.Reset();
					fps_stopwatch.Start();
					//Start Camera Watchdog.
					timer_camerawatchdog.Start();
				}
			}
			catch (Exception exception)
			{
				MessageBox.Show(exception.ToString());
			}
		}
        //-------------------------------------
        #endregion
        //******************************************************


        //******************************************************
        #region GamePad Event Handlers
        //-------------------------------------
        private void OnGamepadConnected()
		{
			label_controllerstatus.ForeColor = Color.LimeGreen;
			label_controllerstatus.Text = "Controller WatchDog:  Controller Connected.";
		}
		//-------------------------------------
		private void OnGamepadDisconnected()
		{
			label_controllerstatus.ForeColor = Color.Red;
			label_controllerstatus.Text = "Controller WatchDog:  WARNING: Controller Not Connected.";
            ISensorSettings _sensor_settings = sensor_manager.SensorSettingsInterface;

            if (!menuStrip1.Focused && (_sensor_settings.CurrentView == ViewType_t.NORMALOPERATION) && (_sensor_settings.TrackingModeSensorState == TrackingModeSensorState_t.AUTOMODE_OFF))
            {
                //if (grbl_manager.GrblStateZ == GrblMachineState_t.JOG) grbl_manager.KillMovementZ();
                //if (grbl_manager.GrblStateX == GrblMachineState_t.JOG) grbl_manager.KillMovementX();
            }

        }
		//-------------------------------------
		private void OnGamepadBackPressed()
		{
			if(menuStrip1.Focused)
			{
				SendKeys.Send("{ESC}");
				return;
			}
		}
		//-------------------------------------
		private void OnGamepadStartPressed()
		{
			if(!menuStrip1.Focused)
			{
				menuStrip1.Focus();
				fileToolStripMenuItem.Select();
				return;
			}
		}
		//-------------------------------------
		private void OnGamepadStartReleased()
		{

		}
		//-------------------------------------
        private void OnGamepadLeftTriggerReleased()
        {
          

           
        }
        //-------------------------------------
        private void OnGamepadDPadUpPressed(GamePadEventArg e)
		{
			ISensorSettings _sensor_settings = sensor_manager.SensorSettingsInterface;

			if (menuStrip1.Focused)
			{
				SendKeys.Send("{UP}");
				return;
			}
			if (!menuStrip1.Focused && (_sensor_settings.CurrentView == ViewType_t.NORMALOPERATION) && (_sensor_settings.TrackingModeSensorState == TrackingModeSensorState_t.AUTOMODE_ON_FULL))
			{
				if (e.left_trigger_pressed)
				{
					if(sensor_manager.tracktopoint.isValidPoint())
                    {
                        double _pixel_to_step = (double)1 / _sensor_settings.PixelsPerMMZ;
                        sensor_manager.tracktopoint_offset.Y += _pixel_to_step;

                    }
				}
			}
			if (!menuStrip1.Focused && (_sensor_settings.CurrentView == ViewType_t.NORMALOPERATION) && (_sensor_settings.TrackingModeSensorState == TrackingModeSensorState_t.AUTOMODE_ON_HEIGHT_ONLY))
			{
				if (e.left_trigger_pressed)
				{

				}
			}
            else if (!menuStrip1.Focused && (_sensor_settings.TrackingModeSensorState == TrackingModeSensorState_t.AUTOMODE_OFF))
            {
                if (!gamepad.LeftTrigger && !gamepad.RightTrigger)
                {
                   // grbl_manager.MoveZ(1);
                }
            }

        }
		//-------------------------------------
        private void OnGamepadDPadUpReleased(GamePadEventArg e)
        {
            ISensorSettings _sensor_settings = sensor_manager.SensorSettingsInterface;

        }
        //-------------------------------------
        private void OnGamepadDPadDownPressed(GamePadEventArg e)
		{
			ISensorSettings _sensor_settings = sensor_manager.SensorSettingsInterface;

			if (menuStrip1.Focused)
			{
				SendKeys.Send("{DOWN}");
				return;
			}
			if (!menuStrip1.Focused && (_sensor_settings.CurrentView == ViewType_t.NORMALOPERATION) && (_sensor_settings.TrackingModeSensorState == TrackingModeSensorState_t.AUTOMODE_ON_FULL))
			{
                if (e.left_trigger_pressed)
                {
                    if (sensor_manager.tracktopoint.isValidPoint())
                    {
                        double _pixel_to_step = (double)1 / _sensor_settings.PixelsPerMMZ;
                        sensor_manager.tracktopoint_offset.Y -= _pixel_to_step;
                    }
                }
            }
			if (!menuStrip1.Focused && (_sensor_settings.CurrentView == ViewType_t.NORMALOPERATION) && (_sensor_settings.TrackingModeSensorState == TrackingModeSensorState_t.AUTOMODE_ON_HEIGHT_ONLY))
			{
				if (e.left_trigger_pressed)
				{

				}
			}
            else if (!menuStrip1.Focused && (_sensor_settings.TrackingModeSensorState == TrackingModeSensorState_t.AUTOMODE_OFF))
            {
                if (!gamepad.LeftTrigger && !gamepad.RightTrigger)
                {
                    //grbl_manager.MoveZ(-1);
                }
            }

        }
		//-------------------------------------
        private void OnGamepadDPadDownReleased(GamePadEventArg e)
        {
            ISensorSettings _sensor_settings = sensor_manager.SensorSettingsInterface;

           
        }
        //-------------------------------------
        private void OnGamepadDPadRightPressed(GamePadEventArg e)
		{
			ISensorSettings _sensor_settings = sensor_manager.SensorSettingsInterface;

			if (menuStrip1.Focused)
			{
				SendKeys.Send("{RIGHT}");
				return;
			}
			else if(!menuStrip1.Focused && (_sensor_settings.CurrentView == ViewType_t.BLURFRAME))
			{
				if(trackBar_blur.Value <= (trackBar_blur.Maximum - 2)) trackBar_blur.Value += 2;
				return;
			}
			else if(!menuStrip1.Focused && (_sensor_settings.CurrentView == ViewType_t.THRESHOLDFRAME))
			{
				if (trackBar_threshold.Value <= (trackBar_threshold.Maximum - 1)) trackBar_threshold.Value += 1;
				return;
			}
			else if (!menuStrip1.Focused && (_sensor_settings.CurrentView == ViewType_t.SKELETONFRAME))
			{
				if (trackBar_column_intensity_filter.Value <= (trackBar_column_intensity_filter.Maximum - 20)) trackBar_column_intensity_filter.Value += 20;
				return;
			}
			else if (!menuStrip1.Focused && (_sensor_settings.CurrentView == ViewType_t.NORMALOPERATION) && (_sensor_settings.TrackingModeSensorState == TrackingModeSensorState_t.AUTOMODE_ON_FULL))
			{
				if(e.left_trigger_pressed)
				{
                    if (sensor_manager.tracktopoint.isValidPoint())
                    {
                        double _pixel_to_step = (double)1 / _sensor_settings.PixelsPerMMX;
                        sensor_manager.tracktopoint_offset.X += _pixel_to_step;
                    }
                }
			}
            else if (!menuStrip1.Focused && ((_sensor_settings.TrackingModeSensorState == TrackingModeSensorState_t.AUTOMODE_OFF) || (_sensor_settings.TrackingModeSensorState == TrackingModeSensorState_t.AUTOMODE_ON_HEIGHT_ONLY)))
            {
                if (!gamepad.LeftTrigger && !gamepad.RightTrigger)
                {
                   // grbl_manager.MoveX(1);
                }
            }


        }
		//-------------------------------------
        private void OnGamepadDPadRightReleased(GamePadEventArg e)
        {
            ISensorSettings _sensor_settings = sensor_manager.SensorSettingsInterface;

            
        }
        //-------------------------------------
        private void OnGamepadDPadLeftPressed(GamePadEventArg e)
		{
			ISensorSettings _sensor_settings = sensor_manager.SensorSettingsInterface;

			if (menuStrip1.Focused)
			{
				SendKeys.Send("{LEFT}");
				return;
			}
			else if (!menuStrip1.Focused && (_sensor_settings.CurrentView == ViewType_t.BLURFRAME))
			{
				if (trackBar_blur.Value >= (trackBar_blur.Minimum + 2)) trackBar_blur.Value -= 2;
				return;
			}
			else if (!menuStrip1.Focused && (_sensor_settings.CurrentView == ViewType_t.THRESHOLDFRAME))
			{
				if (trackBar_threshold.Value >= (trackBar_threshold.Minimum + 1)) trackBar_threshold.Value -= 1;
				return;
			}
			else if (!menuStrip1.Focused && (_sensor_settings.CurrentView == ViewType_t.NORMALOPERATION) && (_sensor_settings.TrackingModeSensorState == TrackingModeSensorState_t.AUTOMODE_ON_FULL))
			{
				if(e.left_trigger_pressed)
				{
                    if (sensor_manager.tracktopoint.isValidPoint())
                    {
                        double _pixel_to_step = (double)1 / _sensor_settings.PixelsPerMMX;
                        sensor_manager.tracktopoint_offset.X -= _pixel_to_step;
                    }
                }
			}
            else if (!menuStrip1.Focused && ((_sensor_settings.TrackingModeSensorState == TrackingModeSensorState_t.AUTOMODE_OFF) || (_sensor_settings.TrackingModeSensorState == TrackingModeSensorState_t.AUTOMODE_ON_HEIGHT_ONLY)))
            {
                if(!gamepad.LeftTrigger && !gamepad.RightTrigger)
                {
                    //grbl_manager.MoveX(-1);
                }
            }
			
		}
		//-------------------------------------
        private void OnGamepadDPadLeftReleased(GamePadEventArg e)
        {
            ISensorSettings _sensor_settings = sensor_manager.SensorSettingsInterface;

            
        }
        //-------------------------------------
        private void OnGamePadAButtonPressed(GamePadEventArg e)
		{
			ISensorSettings _sensor_settings = sensor_manager.SensorSettingsInterface;

			if (menuStrip1.Focused)
			{
				SendKeys.Send("{ENTER}");
				SendKeys.Send("{ESC}");
				return;
			}
			if (!menuStrip1.Focused && (_sensor_settings.CurrentView == ViewType_t.NORMALOPERATION) && (_sensor_settings.TrackingModeSensorState == TrackingModeSensorState_t.AUTOMODE_OFF))
			{
                if (e.left_trigger_pressed && e.right_trigger_pressed)
                {
                    _sensor_settings.TrackingModeSensorState = TrackingModeSensorState_t.SETTRACKTO_AUTOMODEFULL;
                }
			}
		}
        //-------------------------------------
        private void OnGamePadBButtonPressed(GamePadEventArg e)
        {
            ISensorSettings _sensor_settings = sensor_manager.SensorSettingsInterface;

            if (!menuStrip1.Focused && (_sensor_settings.CurrentView == ViewType_t.NORMALOPERATION) && ((_sensor_settings.TrackingModeSensorState == TrackingModeSensorState_t.AUTOMODE_ON_FULL) || (_sensor_settings.TrackingModeSensorState == TrackingModeSensorState_t.AUTOMODE_ON_HEIGHT_ONLY)))
            {
                if (e.left_trigger_pressed && e.right_trigger_pressed)
                {
                    _sensor_settings.TrackingModeSensorState = TrackingModeSensorState_t.AUTOMODE_OFF;
                }
            }
        }
        //-------------------------------------
        private void OnGamePadYButtonPressed(GamePadEventArg e)
        {
            ISensorSettings _sensor_settings = sensor_manager.SensorSettingsInterface;

            if (!menuStrip1.Focused && (_sensor_settings.CurrentView == ViewType_t.NORMALOPERATION) && (_sensor_settings.TrackingModeSensorState == TrackingModeSensorState_t.AUTOMODE_OFF))
            {
                if (e.left_trigger_pressed && e.right_trigger_pressed)
                {
                   _sensor_settings.TrackingModeSensorState = TrackingModeSensorState_t.SETTRACKTO_AUTOMODEHEIGHTONLY;
                }
            }
        }
        //-------------------------------------
        private void OnGamePadXButtonPressed(GamePadEventArg e)
        {
            ISensorSettings _sensor_settings = sensor_manager.SensorSettingsInterface;

            if (!menuStrip1.Focused)
            {
               
            }
        }
        //-------------------------------------
        //-------------------------------------
        //-------------------------------------
        //-------------------------------------
        //-------------------------------------
        //-------------------------------------
        //-------------------------------------
        //-------------------------------------
        //-------------------------------------

        #endregion
        //******************************************************


        //******************************************************
        #region Camera Events
        //-------------------------------------
        private void OnCameraConnectionLost(Object sender, EventArgs e)
		{
			if (InvokeRequired)
			{
				// If called from a different thread, we must use the Invoke method to marshal the call to the proper thread.
				BeginInvoke(new EventHandler<EventArgs>(OnCameraConnectionLost), sender, e);
				return;
			}

			timer_camerawatchdog.Stop();

			// Destroy the camera object.
			try
			{
				if (camera != null)
				{
					camera.Close();
					camera.Dispose();
					camera = null;
				}
			}
			catch (Exception exception)
			{
				label_camera_status.Text = exception.Message;
			}

			AddCamerasToSelectionList();
			InitCamera();
		}
		//-------------------------------------
		#endregion
		//******************************************************


		//******************************************************
		#region Form Control Events
		//-------------------------------------
		private void loadToolStripMenuItem_Click(object sender, EventArgs e)
		{
			ISensorSettings _sensorsettings_interface = sensor_manager.SensorSettingsInterface;
			_sensorsettings_interface.LoadSettingsFromFile();
			UpdateAllControlsToSettingsValues();
		}
		//-------------------------------------
		private void saveToolStripMenuItem_Click(object sender, EventArgs e)
		{
			ISensorSettings _sensorsettings_interface = sensor_manager.SensorSettingsInterface;
			_sensorsettings_interface.SaveSettingsToFile();
		}
		//-------------------------------------
		private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
		{

		}
		//-------------------------------------
		private void MainForm_FormClosed(object sender, FormClosedEventArgs e)
		{
			if (camera != null)
			{
				if (camera.IsOpen)
				{
					if (camera.StreamGrabber.IsGrabbing)
					{
						camera.StreamGrabber.Stop();
					}
					camera.Close();
				}
				camera.Dispose();
			}

			gamepad.EndPollingThread(); // End the Gamepad Polling Thread
		}
		//-------------------------------------	
		private void operationViewToolStripMenuItem_Click(object sender, EventArgs e)
		{
			ISensorSettings _sensorsettings_interface = sensor_manager.SensorSettingsInterface;
			_sensorsettings_interface.CurrentView = ViewType_t.NORMALOPERATION;
			SetFormControls_View();
		}
		//-------------------------------------	
		private void rawCameraFrameToolStripMenuItem_Click(object sender, EventArgs e)
		{
			ISensorSettings _sensorsettings_interface = sensor_manager.SensorSettingsInterface;
			_sensorsettings_interface.CurrentView = ViewType_t.RAWFRAME;
			SetFormControls_View();
		}
		//-------------------------------------	
		private void blurrFrameToolStripMenuItem_Click(object sender, EventArgs e)
		{
			ISensorSettings _sensorsettings_interface = sensor_manager.SensorSettingsInterface;
			_sensorsettings_interface.CurrentView = ViewType_t.BLURFRAME;
			SetFormControls_View();
		}
		//-------------------------------------	
		private void thresholdFrameToolStripMenuItem_Click(object sender, EventArgs e)
		{
			ISensorSettings _sensorsettings_interface = sensor_manager.SensorSettingsInterface;
			_sensorsettings_interface.CurrentView = ViewType_t.THRESHOLDFRAME;
			SetFormControls_View();
		}
		//-------------------------------------	
		private void skeletonFrameToolStripMenuItem_Click(object sender, EventArgs e)
		{
			ISensorSettings _sensorsettings_interface = sensor_manager.SensorSettingsInterface;
			_sensorsettings_interface.CurrentView = ViewType_t.SKELETONFRAME;
			SetFormControls_View();
		}
		//-------------------------------------
		private void testFrameToolStripMenuItem_Click(object sender, EventArgs e)
		{
			ISensorSettings _sensorsettings_interface = sensor_manager.SensorSettingsInterface;
			_sensorsettings_interface.CurrentView = ViewType_t.TESTIMAGEFRAME;
			SetFormControls_View();
		}
		//-------------------------------------
		private void aboutTBIToolStripMenuItem_Click(object sender, EventArgs e)
		{
			using (Form_AboutBox aboutbox = new Form_AboutBox())
			{
				aboutbox.ShowDialog();
			}
		}
		//-------------------------------------
		private void exitProgramToolStripMenuItem_Click(object sender, EventArgs e)
		{
            motorcontroller.DisconnectMicrocontrollers();
			Application.Exit();
		}
		//-------------------------------------
		private void cameraSettingsToolStripMenuItem_Click(object sender, EventArgs e)
		{
			using (Form_CameraSettings csform = new Form_CameraSettings())
			{
				csform.ShowDialog();
			}
		}
		//-------------------------------------
		private void viewZeroLinesToolStripMenuItem_Click(object sender, EventArgs e)
		{
			ISensorSettings _sensorsettings_interface = sensor_manager.SensorSettingsInterface;

			if (viewZeroLinesToolStripMenuItem.Checked)
			{
				_sensorsettings_interface.DrawZeroLineState = DrawPredictedZeroLine_t.NOT_DRAWN;
			}
			else
			{
				_sensorsettings_interface.DrawZeroLineState = DrawPredictedZeroLine_t.DRAWN;
			}

			SetFormControls_View();
		}
		//-------------------------------------
		private void viewTrackingLinesToolStripMenuItem_Click(object sender, EventArgs e)
		{
			ISensorSettings _sensorsettings_interface = sensor_manager.SensorSettingsInterface;

			if(viewTrackingLinesToolStripMenuItem.Checked)
			{
				_sensorsettings_interface.DrawTrackingLines = DrawLineSegmentData_t.NOT_DRAWN;
			}
			else
			{
				_sensorsettings_interface.DrawTrackingLines = DrawLineSegmentData_t.DRAWN;
			}

			SetFormControls_View();
		}
		//-------------------------------------
		private void viewTrackingPointsToolStripMenuItem_Click(object sender, EventArgs e)
		{
			ISensorSettings _sensorsettings_interface = sensor_manager.SensorSettingsInterface;
			if(viewTrackingPointsToolStripMenuItem.Checked)
			{
				_sensorsettings_interface.DrawJointTrackingPoints = DrawJointTrackingPoint_t.NOT_DRAWN;
			}
			else
			{
				_sensorsettings_interface.DrawJointTrackingPoints = DrawJointTrackingPoint_t.DRAWN;
			}

			SetFormControls_View();
		}
		//-------------------------------------
		private void viewRawDataScanToolStripMenuItem_Click(object sender, EventArgs e)
		{
			ISensorSettings _sensorsettings_interface = sensor_manager.SensorSettingsInterface;

			if(viewRawDataScanToolStripMenuItem.Checked)
			{
				_sensorsettings_interface.DrawRawData = DrawLineDataPoints_t.NOT_DRAWN;
			}
			else
			{
				_sensorsettings_interface.DrawRawData = DrawLineDataPoints_t.DRAWN;
			}

			SetFormControls_View();
		}
		//-------------------------------------
		private void drawColumniteImageOnOperationImageToolStripMenuItem_Click(object sender, EventArgs e)
		{
			ISensorSettings _sensorsettings_interface = sensor_manager.SensorSettingsInterface;

			if (drawColumniteImageOnOperationImageToolStripMenuItem.Checked) _sensorsettings_interface.DrawColumiteImageOnOperationsImage = DrawColumiteLineData_t.NOT_DRAWN;
			else _sensorsettings_interface.DrawColumiteImageOnOperationsImage = DrawColumiteLineData_t.DRAWN;

			SetFormControls_View();
		}
		//-------------------------------------
		private void timer_gampadpoll_Tick(object sender, EventArgs e)
		{
            ISensorSettings _sensor_settings = sensor_manager.SensorSettingsInterface;
			gamepad.DoLoop();
            if(!menuStrip1.Focused) motorcontroller.DoGamePadJogLoop(_sensor_settings, gamepad);

        }
		//-------------------------------------
		private void trackBar_blur_ValueChanged(object sender, EventArgs e)
		{
			ISensorSettings _sensorsettings_interface = sensor_manager.SensorSettingsInterface;
			if (trackBar_blur.Value % 2 == 0)
				trackBar_blur.Value += 1;
			_sensorsettings_interface.BlurrValue = trackBar_blur.Value;
			SetFormControls_BlurSetting();
		}
		//-------------------------------------
		private void trackBar_threshold_ValueChanged(object sender, EventArgs e)
		{
			ISensorSettings _sensorsettings_interface = sensor_manager.SensorSettingsInterface;
			_sensorsettings_interface.ThresholdValue = trackBar_threshold.Value;
			SetFormControls_ThresholdSetting();
		}
		//-------------------------------------
		private void timer_camerawatchdog_Tick(object sender, EventArgs e)
		{
			if(camera!=null)
			{
				if(!camera.IsConnected)
				{
					label_camerawatchdog.ForeColor = Color.Red;
					label_camerawatchdog.Text = "WARNING: CAMERA OBJECT IS NOT CONNECTED";
					//timer_camerawatchdog.Stop();
				}
				else if(!camera.IsOpen)
				{
					label_camerawatchdog.ForeColor = Color.Red;
					label_camerawatchdog.Text = "WARNING: CAMERA OBJECT IS NOT OPEN";
					//timer_camerawatchdog.Stop();
				}
				else if(!camera.StreamGrabber.IsGrabbing)
				{
					label_camerawatchdog.ForeColor = Color.Red;
					label_camerawatchdog.Text = "WARNING: CAMERA OBJECT IS NOT STREAMING";
					//timer_camerawatchdog.Stop();
				}
				else
				{
					label_camerawatchdog.ForeColor = Color.LimeGreen;
					label_camerawatchdog.Text = "Camera Watchdog: Camera is Operating Properly.";
				}
			}
			else
			{
				label_camerawatchdog.ForeColor = Color.Red;
				label_camerawatchdog.Text = "WARNING: CAMERA OBJECT IS NULL";
				//timer_camerawatchdog.Stop();
			}
		}
		//-------------------------------------
		private void roundSeamToolStripMenuItem_Click(object sender, EventArgs e)
		{
			ISensorSettings _sensorsettings_interface = sensor_manager.SensorSettingsInterface;

			_sensorsettings_interface.GeometricSeamDescriptor = GeometicSeamDescriptor_t.ROUNDSEAM;
			SetFormControls_SeamType();
		}
		//-------------------------------------
		private void straightSeamToolStripMenuItem_Click(object sender, EventArgs e)
		{
			ISensorSettings _sensorsettings_interface = sensor_manager.SensorSettingsInterface;

			_sensorsettings_interface.GeometricSeamDescriptor = GeometicSeamDescriptor_t.STRAIGHTSEAM;
			SetFormControls_SeamType();
		}
		//-------------------------------------
		private void trackBar_column_intensity_filter_ValueChanged(object sender, EventArgs e)
		{
			ISensorSettings _sensorsettings_interface = sensor_manager.SensorSettingsInterface;

			_sensorsettings_interface.ColumnIntensityFilterValue = (double)trackBar_column_intensity_filter.Value;
			SetFormControls_ColumnIntensityFilterValue();
		}
        //-------------------------------------
        private void panAndZoomPictureBox_view_MouseClick(object sender, MouseEventArgs e)
		{
			if(e.Button == MouseButtons.Right)
			{
				panAndZoomPictureBox_view.SetZoomScale(1, new Point(0, 0));
				panAndZoomPictureBox_view.HorizontalScrollBar.Visible = false;
				panAndZoomPictureBox_view.VerticalScrollBar.Visible = false;
			}
		}
		//-------------------------------------
		private void timer_uiupdater_Tick(object sender, EventArgs e)
        {
            ISensorSettings _sensorsettings_interface = sensor_manager.SensorSettingsInterface;

            if(!double.IsNaN(_sensorsettings_interface.PixelsPerMMX))
            {
                label_x_mmperpixel.ForeColor = Color.LimeGreen;
                label_x_mmperpixel.Visible = true;
                label_x_mmperpixel.Text = "X: mm per pixel = " + _sensorsettings_interface.PixelsPerMMX.ToString("0.###");
            }
            else
            {
                label_x_mmperpixel.ForeColor = Color.Red;
                label_x_mmperpixel.Visible = true;
                label_x_mmperpixel.Text = "X mm per pixel is not calibrated.";
            }
            if (!double.IsNaN(_sensorsettings_interface.PixelsPerMMZ))
            {
                label_z_mmperpixel.ForeColor = Color.LimeGreen;
                label_z_mmperpixel.Visible = true;
                label_z_mmperpixel.Text = "Z: mm per pixel = " + _sensorsettings_interface.PixelsPerMMZ.ToString("0.###");
            }
            else
            {
                label_z_mmperpixel.ForeColor = Color.Red;
                label_z_mmperpixel.Visible = true;
                label_z_mmperpixel.Text = "Z mm per pixel is not calibrated.";
            }
            switch (_sensorsettings_interface.TrackingModeSensorState)
            {
                case TrackingModeSensorState_t.AUTOMODE_OFF:
                    label_trackingmode.ForeColor = Color.Red;
                    label_trackingmode.Text = "Manual Mode.";
                    break;
                case TrackingModeSensorState_t.AUTOMODE_CALIBRATING:
                    label_trackingmode.ForeColor = Color.Yellow;
                    label_trackingmode.Text = "Auto Calibrating.";
                    break;
                case TrackingModeSensorState_t.AUTOMODE_ON_FULL:
                    label_trackingmode.ForeColor = Color.Green;
                    label_trackingmode.Text = "Full Automatic Tracking Mode.";
                    break;
                case TrackingModeSensorState_t.AUTOMODE_ON_HEIGHT_ONLY:
                    label_trackingmode.ForeColor = Color.Yellow;
                    label_trackingmode.Text = "Height Tracking Only Mode.";
                    break;
            }

            switch(motorcontroller.GetStatusX)
            {
                case MotorControllerStatus_t.OK:
                    label_xgrbl.Text = "X Motor Controller State - Motor Idle.";
                    break;
                case MotorControllerStatus_t.BUSY:
                    label_xgrbl.Text = "X Motor Controller State - Moving Motor.";
                    break;
                case MotorControllerStatus_t.STOPPING:
                    label_xgrbl.Text = "X Motor Controller State - Stopping Motor.";
                    break;
            }


            switch (motorcontroller.GetStatusZ)
            {
                case MotorControllerStatus_t.OK:
                    label_zgrbl.Text = "Z Motor Controller State - Motor Idle.";
                    break;
                case MotorControllerStatus_t.BUSY:
                    label_zgrbl.Text = "Z Motor Controller State - Moving Motor.";
                    break;
                case MotorControllerStatus_t.STOPPING:
                    label_zgrbl.Text = "Z Motor Controller State - Stopping Motor.";
                    break;
            }
           
         
        }
		//-------------------------------------
		private void vGrooveToolStripMenuItem_Click(object sender, EventArgs e)
		{
			ISensorSettings _sensorsettings_interface = sensor_manager.SensorSettingsInterface;
			_sensorsettings_interface.JointType = JointType_t.VGROOVE;
			_sensorsettings_interface.TrackingModeSensorState = TrackingModeSensorState_t.AUTOMODE_OFF;
            SetFormControls_JointType();
           
		}
		//-------------------------------------
		private void buttToolStripMenuItem_Click(object sender, EventArgs e)
		{
			ISensorSettings _sensorsettings_interface = sensor_manager.SensorSettingsInterface;
			_sensorsettings_interface.JointType = JointType_t.BUTTJOINT;
			_sensorsettings_interface.TrackingModeSensorState = TrackingModeSensorState_t.AUTOMODE_OFF;
            SetFormControls_JointType();
		}
		//-------------------------------------
		private void filletToolStripMenuItem_Click(object sender, EventArgs e)
		{
			ISensorSettings _sensorsettings_interface = sensor_manager.SensorSettingsInterface;
			_sensorsettings_interface.JointType = JointType_t.FILLET;
			_sensorsettings_interface.TrackingModeSensorState = TrackingModeSensorState_t.AUTOMODE_OFF;
            SetFormControls_JointType();
		}
        //-------------------------------------
        private void autoCalibrationToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ISensorSettings _sensorsettings_interface = sensor_manager.SensorSettingsInterface;
            _sensorsettings_interface.AutoModeCalibrationState = AutoModeTrackingCalibrationState_t.NOT_AUTOMODE_CALIBRATING;
            _sensorsettings_interface.TrackingModeSensorState = TrackingModeSensorState_t.SETTRACKTO_AUTOCALIBRATE;
        }

        //-------------------------------------
       
        private void shutDownComputerToolStripMenuItem_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start("ShutDown", "/s");
        }


        private void equalOffsetToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ISensorSettings _sensor_settings = sensor_manager.SensorSettingsInterface;
            _sensor_settings.VGrooveJTPLocation = VGrooveJTPLocation_t.ALL_JOINT;
            _sensor_settings.JointType = JointType_t.VGROOVE;
            _sensor_settings.TrackingModeSensorState = TrackingModeSensorState_t.AUTOMODE_OFF;
            SetFormControls_JointType();
        }

        private void leftSideTransistionOffsetToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ISensorSettings _sensor_settings = sensor_manager.SensorSettingsInterface;
            _sensor_settings.VGrooveJTPLocation = VGrooveJTPLocation_t.LEFT_SIDE;
            _sensor_settings.JointType = JointType_t.VGROOVE;
            _sensor_settings.TrackingModeSensorState = TrackingModeSensorState_t.AUTOMODE_OFF;
            SetFormControls_JointType();
        }

        private void rightSideTransitionOffsetToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ISensorSettings _sensor_settings = sensor_manager.SensorSettingsInterface;
            _sensor_settings.VGrooveJTPLocation = VGrooveJTPLocation_t.RIGHT_SIDE;
            _sensor_settings.JointType = JointType_t.VGROOVE;
            _sensor_settings.TrackingModeSensorState = TrackingModeSensorState_t.AUTOMODE_OFF;
            SetFormControls_JointType();
        }

		private void label_camerawatchdog_Click(object sender, EventArgs e)
		{

		}










		//-------------------------------------



		//-------------------------------------	
		#endregion
		//******************************************************



	}//End Class Definition
}//End NameSpace
