namespace TBISeamTracker
{
	partial class MainForm
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.components = new System.ComponentModel.Container();
			this.menuStrip1 = new System.Windows.Forms.MenuStrip();
			this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.loadToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
			this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
			this.exitProgramToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripSeparator7 = new System.Windows.Forms.ToolStripSeparator();
			this.shutDownComputerToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.viewToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.operationViewToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
			this.imageProcessingViewsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.rawCameraFrameToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.blurrFrameToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.thresholdFrameToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.skeletonFrameToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripSeparator6 = new System.Windows.Forms.ToolStripSeparator();
			this.testFrameToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripSeparator5 = new System.Windows.Forms.ToolStripSeparator();
			this.drawColumniteImageOnOperationImageToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.viewRawDataScanToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.viewTrackingLinesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.viewZeroLinesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.viewTrackingPointsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.settingsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.sensorModeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.vGrooveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.equalOffsetToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.leftSideTransistionOffsetToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.rightSideTransitionOffsetToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.buttToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.filletToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.seamTypeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.roundSeamToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.straightSeamToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.dSPFiltersToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
			this.manualCalibrationToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.autoCalibrationToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripSeparator8 = new System.Windows.Forms.ToolStripSeparator();
			this.cameraSelectionToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.aboutTBIToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.panAndZoomPictureBox_view = new Emgu.CV.UI.PanAndZoomPictureBox();
			this.panel_operationViewTracking = new System.Windows.Forms.Panel();
			this.panel1 = new System.Windows.Forms.Panel();
			this.label_track_to_point = new System.Windows.Forms.Label();
			this.label_x_mmperpixel = new System.Windows.Forms.Label();
			this.label_z_mmperpixel = new System.Windows.Forms.Label();
			this.label_zgrbl = new System.Windows.Forms.Label();
			this.label_xgrbl = new System.Windows.Forms.Label();
			this.label_trackingmode = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.label_controllerstatus = new System.Windows.Forms.Label();
			this.panel_rawcameraview = new System.Windows.Forms.Panel();
			this.label1 = new System.Windows.Forms.Label();
			this.panel_blurrview = new System.Windows.Forms.Panel();
			this.trackBar_blur = new System.Windows.Forms.TrackBar();
			this.label_blur = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.panel_thresholdview = new System.Windows.Forms.Panel();
			this.trackBar_threshold = new System.Windows.Forms.TrackBar();
			this.label_threshold = new System.Windows.Forms.Label();
			this.label4 = new System.Windows.Forms.Label();
			this.label_camera_status = new System.Windows.Forms.Label();
			this.label_mainlooptime = new System.Windows.Forms.Label();
			this.panel_skeletalcamraview = new System.Windows.Forms.Panel();
			this.trackBar_column_intensity_filter = new System.Windows.Forms.TrackBar();
			this.label_column_intensity_filter = new System.Windows.Forms.Label();
			this.panel_lineextract_subpixelweightedmean = new System.Windows.Forms.Panel();
			this.label6 = new System.Windows.Forms.Label();
			this.label5 = new System.Windows.Forms.Label();
			this.timer_gampadpoll = new System.Windows.Forms.Timer(this.components);
			this.timer_camerawatchdog = new System.Windows.Forms.Timer(this.components);
			this.label_camerawatchdog = new System.Windows.Forms.Label();
			this.timer_uiupdater = new System.Windows.Forms.Timer(this.components);
			this.menuStrip1.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.panAndZoomPictureBox_view)).BeginInit();
			this.panel_operationViewTracking.SuspendLayout();
			this.panel1.SuspendLayout();
			this.panel_rawcameraview.SuspendLayout();
			this.panel_blurrview.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.trackBar_blur)).BeginInit();
			this.panel_thresholdview.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.trackBar_threshold)).BeginInit();
			this.panel_skeletalcamraview.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.trackBar_column_intensity_filter)).BeginInit();
			this.panel_lineextract_subpixelweightedmean.SuspendLayout();
			this.SuspendLayout();
			// 
			// menuStrip1
			// 
			this.menuStrip1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(64)))), ((int)(((byte)(64)))));
			this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.viewToolStripMenuItem,
            this.settingsToolStripMenuItem,
            this.cameraSelectionToolStripMenuItem,
            this.aboutToolStripMenuItem});
			this.menuStrip1.Location = new System.Drawing.Point(0, 0);
			this.menuStrip1.Name = "menuStrip1";
			this.menuStrip1.Size = new System.Drawing.Size(1264, 24);
			this.menuStrip1.TabIndex = 0;
			this.menuStrip1.Text = "menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.loadToolStripMenuItem,
            this.toolStripSeparator2,
            this.saveToolStripMenuItem,
            this.toolStripSeparator3,
            this.exitProgramToolStripMenuItem,
            this.toolStripSeparator7,
            this.shutDownComputerToolStripMenuItem});
			this.fileToolStripMenuItem.ForeColor = System.Drawing.Color.White;
			this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
			this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
			this.fileToolStripMenuItem.Text = "File";
			// 
			// loadToolStripMenuItem
			// 
			this.loadToolStripMenuItem.Name = "loadToolStripMenuItem";
			this.loadToolStripMenuItem.Size = new System.Drawing.Size(189, 22);
			this.loadToolStripMenuItem.Text = "Load State";
			this.loadToolStripMenuItem.Click += new System.EventHandler(this.loadToolStripMenuItem_Click);
			// 
			// toolStripSeparator2
			// 
			this.toolStripSeparator2.Name = "toolStripSeparator2";
			this.toolStripSeparator2.Size = new System.Drawing.Size(186, 6);
			// 
			// saveToolStripMenuItem
			// 
			this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
			this.saveToolStripMenuItem.Size = new System.Drawing.Size(189, 22);
			this.saveToolStripMenuItem.Text = "Save State";
			this.saveToolStripMenuItem.Click += new System.EventHandler(this.saveToolStripMenuItem_Click);
			// 
			// toolStripSeparator3
			// 
			this.toolStripSeparator3.Name = "toolStripSeparator3";
			this.toolStripSeparator3.Size = new System.Drawing.Size(186, 6);
			// 
			// exitProgramToolStripMenuItem
			// 
			this.exitProgramToolStripMenuItem.Name = "exitProgramToolStripMenuItem";
			this.exitProgramToolStripMenuItem.Size = new System.Drawing.Size(189, 22);
			this.exitProgramToolStripMenuItem.Text = "Exit Application";
			this.exitProgramToolStripMenuItem.Click += new System.EventHandler(this.exitProgramToolStripMenuItem_Click);
			// 
			// toolStripSeparator7
			// 
			this.toolStripSeparator7.Name = "toolStripSeparator7";
			this.toolStripSeparator7.Size = new System.Drawing.Size(186, 6);
			// 
			// shutDownComputerToolStripMenuItem
			// 
			this.shutDownComputerToolStripMenuItem.Name = "shutDownComputerToolStripMenuItem";
			this.shutDownComputerToolStripMenuItem.Size = new System.Drawing.Size(189, 22);
			this.shutDownComputerToolStripMenuItem.Text = "Shut Down Computer";
			this.shutDownComputerToolStripMenuItem.Click += new System.EventHandler(this.shutDownComputerToolStripMenuItem_Click);
			// 
			// viewToolStripMenuItem
			// 
			this.viewToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.operationViewToolStripMenuItem,
            this.toolStripSeparator1,
            this.imageProcessingViewsToolStripMenuItem,
            this.toolStripSeparator5,
            this.drawColumniteImageOnOperationImageToolStripMenuItem,
            this.viewRawDataScanToolStripMenuItem,
            this.viewTrackingLinesToolStripMenuItem,
            this.viewZeroLinesToolStripMenuItem,
            this.viewTrackingPointsToolStripMenuItem});
			this.viewToolStripMenuItem.ForeColor = System.Drawing.Color.White;
			this.viewToolStripMenuItem.Name = "viewToolStripMenuItem";
			this.viewToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
			this.viewToolStripMenuItem.Text = "View";
			// 
			// operationViewToolStripMenuItem
			// 
			this.operationViewToolStripMenuItem.Checked = true;
			this.operationViewToolStripMenuItem.CheckState = System.Windows.Forms.CheckState.Checked;
			this.operationViewToolStripMenuItem.Name = "operationViewToolStripMenuItem";
			this.operationViewToolStripMenuItem.Size = new System.Drawing.Size(204, 22);
			this.operationViewToolStripMenuItem.Text = "Operation View";
			this.operationViewToolStripMenuItem.Click += new System.EventHandler(this.operationViewToolStripMenuItem_Click);
			// 
			// toolStripSeparator1
			// 
			this.toolStripSeparator1.Name = "toolStripSeparator1";
			this.toolStripSeparator1.Size = new System.Drawing.Size(201, 6);
			// 
			// imageProcessingViewsToolStripMenuItem
			// 
			this.imageProcessingViewsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.rawCameraFrameToolStripMenuItem,
            this.blurrFrameToolStripMenuItem,
            this.thresholdFrameToolStripMenuItem,
            this.skeletonFrameToolStripMenuItem,
            this.toolStripSeparator6,
            this.testFrameToolStripMenuItem});
			this.imageProcessingViewsToolStripMenuItem.Name = "imageProcessingViewsToolStripMenuItem";
			this.imageProcessingViewsToolStripMenuItem.Size = new System.Drawing.Size(204, 22);
			this.imageProcessingViewsToolStripMenuItem.Text = "Image Processing Views";
			// 
			// rawCameraFrameToolStripMenuItem
			// 
			this.rawCameraFrameToolStripMenuItem.Name = "rawCameraFrameToolStripMenuItem";
			this.rawCameraFrameToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
			this.rawCameraFrameToolStripMenuItem.Text = "Raw Camera Frame";
			this.rawCameraFrameToolStripMenuItem.Click += new System.EventHandler(this.rawCameraFrameToolStripMenuItem_Click);
			// 
			// blurrFrameToolStripMenuItem
			// 
			this.blurrFrameToolStripMenuItem.Name = "blurrFrameToolStripMenuItem";
			this.blurrFrameToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
			this.blurrFrameToolStripMenuItem.Text = "Blurr Frame";
			this.blurrFrameToolStripMenuItem.Click += new System.EventHandler(this.blurrFrameToolStripMenuItem_Click);
			// 
			// thresholdFrameToolStripMenuItem
			// 
			this.thresholdFrameToolStripMenuItem.Name = "thresholdFrameToolStripMenuItem";
			this.thresholdFrameToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
			this.thresholdFrameToolStripMenuItem.Text = "Threshold Frame";
			this.thresholdFrameToolStripMenuItem.Click += new System.EventHandler(this.thresholdFrameToolStripMenuItem_Click);
			// 
			// skeletonFrameToolStripMenuItem
			// 
			this.skeletonFrameToolStripMenuItem.Name = "skeletonFrameToolStripMenuItem";
			this.skeletonFrameToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
			this.skeletonFrameToolStripMenuItem.Text = "Skeleton Frame";
			this.skeletonFrameToolStripMenuItem.Click += new System.EventHandler(this.skeletonFrameToolStripMenuItem_Click);
			// 
			// toolStripSeparator6
			// 
			this.toolStripSeparator6.Name = "toolStripSeparator6";
			this.toolStripSeparator6.Size = new System.Drawing.Size(177, 6);
			// 
			// testFrameToolStripMenuItem
			// 
			this.testFrameToolStripMenuItem.Name = "testFrameToolStripMenuItem";
			this.testFrameToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
			this.testFrameToolStripMenuItem.Text = "Test Frame";
			this.testFrameToolStripMenuItem.Click += new System.EventHandler(this.testFrameToolStripMenuItem_Click);
			// 
			// toolStripSeparator5
			// 
			this.toolStripSeparator5.Name = "toolStripSeparator5";
			this.toolStripSeparator5.Size = new System.Drawing.Size(201, 6);
			// 
			// drawColumniteImageOnOperationImageToolStripMenuItem
			// 
			this.drawColumniteImageOnOperationImageToolStripMenuItem.Name = "drawColumniteImageOnOperationImageToolStripMenuItem";
			this.drawColumniteImageOnOperationImageToolStripMenuItem.Size = new System.Drawing.Size(204, 22);
			this.drawColumniteImageOnOperationImageToolStripMenuItem.Text = "Draw Columnite Image";
			this.drawColumniteImageOnOperationImageToolStripMenuItem.Click += new System.EventHandler(this.drawColumniteImageOnOperationImageToolStripMenuItem_Click);
			// 
			// viewRawDataScanToolStripMenuItem
			// 
			this.viewRawDataScanToolStripMenuItem.Name = "viewRawDataScanToolStripMenuItem";
			this.viewRawDataScanToolStripMenuItem.Size = new System.Drawing.Size(204, 22);
			this.viewRawDataScanToolStripMenuItem.Text = "View Raw Data Scan";
			this.viewRawDataScanToolStripMenuItem.Click += new System.EventHandler(this.viewRawDataScanToolStripMenuItem_Click);
			// 
			// viewTrackingLinesToolStripMenuItem
			// 
			this.viewTrackingLinesToolStripMenuItem.Name = "viewTrackingLinesToolStripMenuItem";
			this.viewTrackingLinesToolStripMenuItem.Size = new System.Drawing.Size(204, 22);
			this.viewTrackingLinesToolStripMenuItem.Text = "View Tracking Lines";
			this.viewTrackingLinesToolStripMenuItem.Click += new System.EventHandler(this.viewTrackingLinesToolStripMenuItem_Click);
			// 
			// viewZeroLinesToolStripMenuItem
			// 
			this.viewZeroLinesToolStripMenuItem.Name = "viewZeroLinesToolStripMenuItem";
			this.viewZeroLinesToolStripMenuItem.Size = new System.Drawing.Size(204, 22);
			this.viewZeroLinesToolStripMenuItem.Text = "View Predicted Zero Line";
			this.viewZeroLinesToolStripMenuItem.Click += new System.EventHandler(this.viewZeroLinesToolStripMenuItem_Click);
			// 
			// viewTrackingPointsToolStripMenuItem
			// 
			this.viewTrackingPointsToolStripMenuItem.Name = "viewTrackingPointsToolStripMenuItem";
			this.viewTrackingPointsToolStripMenuItem.Size = new System.Drawing.Size(204, 22);
			this.viewTrackingPointsToolStripMenuItem.Text = "View Tracking Points";
			this.viewTrackingPointsToolStripMenuItem.Click += new System.EventHandler(this.viewTrackingPointsToolStripMenuItem_Click);
			// 
			// settingsToolStripMenuItem
			// 
			this.settingsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.sensorModeToolStripMenuItem,
            this.seamTypeToolStripMenuItem,
            this.dSPFiltersToolStripMenuItem,
            this.toolStripSeparator4,
            this.manualCalibrationToolStripMenuItem,
            this.autoCalibrationToolStripMenuItem,
            this.toolStripSeparator8});
			this.settingsToolStripMenuItem.ForeColor = System.Drawing.Color.White;
			this.settingsToolStripMenuItem.Name = "settingsToolStripMenuItem";
			this.settingsToolStripMenuItem.Size = new System.Drawing.Size(61, 20);
			this.settingsToolStripMenuItem.Text = "Settings";
			// 
			// sensorModeToolStripMenuItem
			// 
			this.sensorModeToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.vGrooveToolStripMenuItem,
            this.buttToolStripMenuItem,
            this.filletToolStripMenuItem});
			this.sensorModeToolStripMenuItem.Name = "sensorModeToolStripMenuItem";
			this.sensorModeToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
			this.sensorModeToolStripMenuItem.Text = "Joints";
			// 
			// vGrooveToolStripMenuItem
			// 
			this.vGrooveToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.equalOffsetToolStripMenuItem,
            this.leftSideTransistionOffsetToolStripMenuItem,
            this.rightSideTransitionOffsetToolStripMenuItem});
			this.vGrooveToolStripMenuItem.Name = "vGrooveToolStripMenuItem";
			this.vGrooveToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
			this.vGrooveToolStripMenuItem.Text = "V-Groove";
			this.vGrooveToolStripMenuItem.Click += new System.EventHandler(this.vGrooveToolStripMenuItem_Click);
			// 
			// equalOffsetToolStripMenuItem
			// 
			this.equalOffsetToolStripMenuItem.Name = "equalOffsetToolStripMenuItem";
			this.equalOffsetToolStripMenuItem.Size = new System.Drawing.Size(227, 22);
			this.equalOffsetToolStripMenuItem.Text = "Equal Offset";
			this.equalOffsetToolStripMenuItem.Click += new System.EventHandler(this.equalOffsetToolStripMenuItem_Click);
			// 
			// leftSideTransistionOffsetToolStripMenuItem
			// 
			this.leftSideTransistionOffsetToolStripMenuItem.Name = "leftSideTransistionOffsetToolStripMenuItem";
			this.leftSideTransistionOffsetToolStripMenuItem.Size = new System.Drawing.Size(227, 22);
			this.leftSideTransistionOffsetToolStripMenuItem.Text = "Left Side - Transistion/Offset";
			this.leftSideTransistionOffsetToolStripMenuItem.Click += new System.EventHandler(this.leftSideTransistionOffsetToolStripMenuItem_Click);
			// 
			// rightSideTransitionOffsetToolStripMenuItem
			// 
			this.rightSideTransitionOffsetToolStripMenuItem.Name = "rightSideTransitionOffsetToolStripMenuItem";
			this.rightSideTransitionOffsetToolStripMenuItem.Size = new System.Drawing.Size(227, 22);
			this.rightSideTransitionOffsetToolStripMenuItem.Text = "Right Side - Transition/Offset";
			this.rightSideTransitionOffsetToolStripMenuItem.Click += new System.EventHandler(this.rightSideTransitionOffsetToolStripMenuItem_Click);
			// 
			// buttToolStripMenuItem
			// 
			this.buttToolStripMenuItem.Name = "buttToolStripMenuItem";
			this.buttToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
			this.buttToolStripMenuItem.Text = "Butt";
			this.buttToolStripMenuItem.Click += new System.EventHandler(this.buttToolStripMenuItem_Click);
			// 
			// filletToolStripMenuItem
			// 
			this.filletToolStripMenuItem.Name = "filletToolStripMenuItem";
			this.filletToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
			this.filletToolStripMenuItem.Text = "Fillet";
			this.filletToolStripMenuItem.Click += new System.EventHandler(this.filletToolStripMenuItem_Click);
			// 
			// seamTypeToolStripMenuItem
			// 
			this.seamTypeToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.roundSeamToolStripMenuItem,
            this.straightSeamToolStripMenuItem});
			this.seamTypeToolStripMenuItem.Name = "seamTypeToolStripMenuItem";
			this.seamTypeToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
			this.seamTypeToolStripMenuItem.Text = "Seam Type";
			// 
			// roundSeamToolStripMenuItem
			// 
			this.roundSeamToolStripMenuItem.Name = "roundSeamToolStripMenuItem";
			this.roundSeamToolStripMenuItem.Size = new System.Drawing.Size(147, 22);
			this.roundSeamToolStripMenuItem.Text = "Round Seam";
			this.roundSeamToolStripMenuItem.Click += new System.EventHandler(this.roundSeamToolStripMenuItem_Click);
			// 
			// straightSeamToolStripMenuItem
			// 
			this.straightSeamToolStripMenuItem.Name = "straightSeamToolStripMenuItem";
			this.straightSeamToolStripMenuItem.Size = new System.Drawing.Size(147, 22);
			this.straightSeamToolStripMenuItem.Text = "Straight Seam";
			this.straightSeamToolStripMenuItem.Click += new System.EventHandler(this.straightSeamToolStripMenuItem_Click);
			// 
			// dSPFiltersToolStripMenuItem
			// 
			this.dSPFiltersToolStripMenuItem.Name = "dSPFiltersToolStripMenuItem";
			this.dSPFiltersToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
			this.dSPFiltersToolStripMenuItem.Text = "DSP Filters";
			// 
			// toolStripSeparator4
			// 
			this.toolStripSeparator4.Name = "toolStripSeparator4";
			this.toolStripSeparator4.Size = new System.Drawing.Size(177, 6);
			// 
			// manualCalibrationToolStripMenuItem
			// 
			this.manualCalibrationToolStripMenuItem.Name = "manualCalibrationToolStripMenuItem";
			this.manualCalibrationToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
			this.manualCalibrationToolStripMenuItem.Text = "Manual Calibration";
			// 
			// autoCalibrationToolStripMenuItem
			// 
			this.autoCalibrationToolStripMenuItem.Name = "autoCalibrationToolStripMenuItem";
			this.autoCalibrationToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
			this.autoCalibrationToolStripMenuItem.Text = "Auto Calibration";
			this.autoCalibrationToolStripMenuItem.Click += new System.EventHandler(this.autoCalibrationToolStripMenuItem_Click);
			// 
			// toolStripSeparator8
			// 
			this.toolStripSeparator8.Name = "toolStripSeparator8";
			this.toolStripSeparator8.Size = new System.Drawing.Size(177, 6);
			// 
			// cameraSelectionToolStripMenuItem
			// 
			this.cameraSelectionToolStripMenuItem.ForeColor = System.Drawing.Color.White;
			this.cameraSelectionToolStripMenuItem.Name = "cameraSelectionToolStripMenuItem";
			this.cameraSelectionToolStripMenuItem.Size = new System.Drawing.Size(111, 20);
			this.cameraSelectionToolStripMenuItem.Text = "Camera Selection";
			// 
			// aboutToolStripMenuItem
			// 
			this.aboutToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.aboutTBIToolStripMenuItem});
			this.aboutToolStripMenuItem.ForeColor = System.Drawing.Color.White;
			this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
			this.aboutToolStripMenuItem.Size = new System.Drawing.Size(52, 20);
			this.aboutToolStripMenuItem.Text = "About";
			this.aboutToolStripMenuItem.Click += new System.EventHandler(this.aboutToolStripMenuItem_Click);
			// 
			// aboutTBIToolStripMenuItem
			// 
			this.aboutTBIToolStripMenuItem.Name = "aboutTBIToolStripMenuItem";
			this.aboutTBIToolStripMenuItem.Size = new System.Drawing.Size(127, 22);
			this.aboutTBIToolStripMenuItem.Text = "About TBI";
			this.aboutTBIToolStripMenuItem.Click += new System.EventHandler(this.aboutTBIToolStripMenuItem_Click);
			// 
			// panAndZoomPictureBox_view
			// 
			this.panAndZoomPictureBox_view.BackColor = System.Drawing.Color.DimGray;
			this.panAndZoomPictureBox_view.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.panAndZoomPictureBox_view.Location = new System.Drawing.Point(12, 73);
			this.panAndZoomPictureBox_view.Name = "panAndZoomPictureBox_view";
			this.panAndZoomPictureBox_view.Size = new System.Drawing.Size(720, 540);
			this.panAndZoomPictureBox_view.TabIndex = 1;
			this.panAndZoomPictureBox_view.TabStop = false;
			this.panAndZoomPictureBox_view.MouseClick += new System.Windows.Forms.MouseEventHandler(this.panAndZoomPictureBox_view_MouseClick);
			// 
			// panel_operationViewTracking
			// 
			this.panel_operationViewTracking.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(64)))), ((int)(((byte)(64)))));
			this.panel_operationViewTracking.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.panel_operationViewTracking.Controls.Add(this.panel1);
			this.panel_operationViewTracking.Controls.Add(this.label2);
			this.panel_operationViewTracking.Location = new System.Drawing.Point(755, 75);
			this.panel_operationViewTracking.Name = "panel_operationViewTracking";
			this.panel_operationViewTracking.Size = new System.Drawing.Size(485, 540);
			this.panel_operationViewTracking.TabIndex = 2;
			// 
			// panel1
			// 
			this.panel1.BackColor = System.Drawing.Color.Black;
			this.panel1.Controls.Add(this.label_track_to_point);
			this.panel1.Controls.Add(this.label_x_mmperpixel);
			this.panel1.Controls.Add(this.label_z_mmperpixel);
			this.panel1.Controls.Add(this.label_zgrbl);
			this.panel1.Controls.Add(this.label_xgrbl);
			this.panel1.Controls.Add(this.label_trackingmode);
			this.panel1.Location = new System.Drawing.Point(7, 56);
			this.panel1.Name = "panel1";
			this.panel1.Size = new System.Drawing.Size(459, 467);
			this.panel1.TabIndex = 2;
			// 
			// label_track_to_point
			// 
			this.label_track_to_point.AutoSize = true;
			this.label_track_to_point.Font = new System.Drawing.Font("Microsoft YaHei UI", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label_track_to_point.ForeColor = System.Drawing.Color.Yellow;
			this.label_track_to_point.Location = new System.Drawing.Point(6, 399);
			this.label_track_to_point.Name = "label_track_to_point";
			this.label_track_to_point.Size = new System.Drawing.Size(123, 16);
			this.label_track_to_point.TabIndex = 13;
			this.label_track_to_point.Text = "Track To Point @ (x,y)";
			this.label_track_to_point.Visible = false;
			// 
			// label_x_mmperpixel
			// 
			this.label_x_mmperpixel.AutoSize = true;
			this.label_x_mmperpixel.Font = new System.Drawing.Font("Microsoft YaHei UI", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label_x_mmperpixel.ForeColor = System.Drawing.Color.Yellow;
			this.label_x_mmperpixel.Location = new System.Drawing.Point(6, 298);
			this.label_x_mmperpixel.Name = "label_x_mmperpixel";
			this.label_x_mmperpixel.Size = new System.Drawing.Size(100, 16);
			this.label_x_mmperpixel.TabIndex = 12;
			this.label_x_mmperpixel.Text = "X:mm per pixel = ";
			// 
			// label_z_mmperpixel
			// 
			this.label_z_mmperpixel.AutoSize = true;
			this.label_z_mmperpixel.Font = new System.Drawing.Font("Microsoft YaHei UI", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label_z_mmperpixel.ForeColor = System.Drawing.Color.Yellow;
			this.label_z_mmperpixel.Location = new System.Drawing.Point(6, 282);
			this.label_z_mmperpixel.Name = "label_z_mmperpixel";
			this.label_z_mmperpixel.Size = new System.Drawing.Size(97, 16);
			this.label_z_mmperpixel.TabIndex = 11;
			this.label_z_mmperpixel.Text = "Z:mm per pixel =";
			// 
			// label_zgrbl
			// 
			this.label_zgrbl.AutoSize = true;
			this.label_zgrbl.Font = new System.Drawing.Font("Microsoft YaHei UI", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label_zgrbl.ForeColor = System.Drawing.Color.Lime;
			this.label_zgrbl.Location = new System.Drawing.Point(4, 23);
			this.label_zgrbl.Name = "label_zgrbl";
			this.label_zgrbl.Size = new System.Drawing.Size(82, 17);
			this.label_zgrbl.TabIndex = 9;
			this.label_zgrbl.Text = "Z Controller";
			// 
			// label_xgrbl
			// 
			this.label_xgrbl.AutoSize = true;
			this.label_xgrbl.Font = new System.Drawing.Font("Microsoft YaHei UI", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label_xgrbl.ForeColor = System.Drawing.Color.Lime;
			this.label_xgrbl.Location = new System.Drawing.Point(4, 3);
			this.label_xgrbl.Name = "label_xgrbl";
			this.label_xgrbl.Size = new System.Drawing.Size(82, 17);
			this.label_xgrbl.TabIndex = 8;
			this.label_xgrbl.Text = "X Controller";
			// 
			// label_trackingmode
			// 
			this.label_trackingmode.AutoSize = true;
			this.label_trackingmode.Font = new System.Drawing.Font("Microsoft YaHei UI", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label_trackingmode.ForeColor = System.Drawing.Color.Lime;
			this.label_trackingmode.Location = new System.Drawing.Point(5, 432);
			this.label_trackingmode.Name = "label_trackingmode";
			this.label_trackingmode.Size = new System.Drawing.Size(121, 22);
			this.label_trackingmode.TabIndex = 5;
			this.label_trackingmode.Text = "Manual Mode";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label2.ForeColor = System.Drawing.Color.White;
			this.label2.Location = new System.Drawing.Point(10, 18);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(322, 20);
			this.label2.TabIndex = 1;
			this.label2.Text = "Main Operation: Tracking Control Panel\r\n";
			// 
			// label_controllerstatus
			// 
			this.label_controllerstatus.AutoSize = true;
			this.label_controllerstatus.Font = new System.Drawing.Font("Microsoft YaHei UI", 20.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label_controllerstatus.ForeColor = System.Drawing.Color.Red;
			this.label_controllerstatus.Location = new System.Drawing.Point(707, 632);
			this.label_controllerstatus.Name = "label_controllerstatus";
			this.label_controllerstatus.Size = new System.Drawing.Size(537, 72);
			this.label_controllerstatus.TabIndex = 3;
			this.label_controllerstatus.Text = "WARNING! Controller Not Connected!\r\n\r\n";
			// 
			// panel_rawcameraview
			// 
			this.panel_rawcameraview.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(64)))), ((int)(((byte)(64)))));
			this.panel_rawcameraview.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.panel_rawcameraview.Controls.Add(this.label1);
			this.panel_rawcameraview.Location = new System.Drawing.Point(755, 75);
			this.panel_rawcameraview.Name = "panel_rawcameraview";
			this.panel_rawcameraview.Size = new System.Drawing.Size(485, 540);
			this.panel_rawcameraview.TabIndex = 3;
			this.panel_rawcameraview.Visible = false;
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label1.ForeColor = System.Drawing.Color.White;
			this.label1.Location = new System.Drawing.Point(10, 18);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(183, 20);
			this.label1.TabIndex = 0;
			this.label1.Text = "Raw Camera Controls";
			// 
			// panel_blurrview
			// 
			this.panel_blurrview.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(64)))), ((int)(((byte)(64)))));
			this.panel_blurrview.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.panel_blurrview.Controls.Add(this.trackBar_blur);
			this.panel_blurrview.Controls.Add(this.label_blur);
			this.panel_blurrview.Controls.Add(this.label3);
			this.panel_blurrview.Location = new System.Drawing.Point(755, 75);
			this.panel_blurrview.Name = "panel_blurrview";
			this.panel_blurrview.Size = new System.Drawing.Size(485, 540);
			this.panel_blurrview.TabIndex = 4;
			this.panel_blurrview.Visible = false;
			// 
			// trackBar_blur
			// 
			this.trackBar_blur.Location = new System.Drawing.Point(14, 79);
			this.trackBar_blur.Maximum = 51;
			this.trackBar_blur.Minimum = 1;
			this.trackBar_blur.Name = "trackBar_blur";
			this.trackBar_blur.Size = new System.Drawing.Size(287, 45);
			this.trackBar_blur.TabIndex = 4;
			this.trackBar_blur.Value = 1;
			this.trackBar_blur.ValueChanged += new System.EventHandler(this.trackBar_blur_ValueChanged);
			// 
			// label_blur
			// 
			this.label_blur.AutoSize = true;
			this.label_blur.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label_blur.ForeColor = System.Drawing.Color.White;
			this.label_blur.Location = new System.Drawing.Point(11, 60);
			this.label_blur.Name = "label_blur";
			this.label_blur.Size = new System.Drawing.Size(79, 16);
			this.label_blur.TabIndex = 3;
			this.label_blur.Text = "Blur Value";
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(64)))), ((int)(((byte)(64)))));
			this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label3.ForeColor = System.Drawing.Color.White;
			this.label3.Location = new System.Drawing.Point(10, 18);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(200, 20);
			this.label3.TabIndex = 2;
			this.label3.Text = "Blur Filter Control Panel";
			// 
			// panel_thresholdview
			// 
			this.panel_thresholdview.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(64)))), ((int)(((byte)(64)))));
			this.panel_thresholdview.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.panel_thresholdview.Controls.Add(this.trackBar_threshold);
			this.panel_thresholdview.Controls.Add(this.label_threshold);
			this.panel_thresholdview.Controls.Add(this.label4);
			this.panel_thresholdview.Location = new System.Drawing.Point(755, 75);
			this.panel_thresholdview.Name = "panel_thresholdview";
			this.panel_thresholdview.Size = new System.Drawing.Size(485, 540);
			this.panel_thresholdview.TabIndex = 5;
			this.panel_thresholdview.Visible = false;
			// 
			// trackBar_threshold
			// 
			this.trackBar_threshold.Location = new System.Drawing.Point(14, 79);
			this.trackBar_threshold.Maximum = 255;
			this.trackBar_threshold.Minimum = 20;
			this.trackBar_threshold.Name = "trackBar_threshold";
			this.trackBar_threshold.Size = new System.Drawing.Size(287, 45);
			this.trackBar_threshold.TabIndex = 6;
			this.trackBar_threshold.Value = 20;
			this.trackBar_threshold.ValueChanged += new System.EventHandler(this.trackBar_threshold_ValueChanged);
			// 
			// label_threshold
			// 
			this.label_threshold.AutoSize = true;
			this.label_threshold.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label_threshold.ForeColor = System.Drawing.Color.White;
			this.label_threshold.Location = new System.Drawing.Point(11, 60);
			this.label_threshold.Name = "label_threshold";
			this.label_threshold.Size = new System.Drawing.Size(122, 16);
			this.label_threshold.TabIndex = 5;
			this.label_threshold.Text = "Threshold Value";
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label4.ForeColor = System.Drawing.Color.White;
			this.label4.Location = new System.Drawing.Point(3, 18);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(206, 20);
			this.label4.TabIndex = 2;
			this.label4.Text = "Threshold Filter Controls";
			// 
			// label_camera_status
			// 
			this.label_camera_status.AutoSize = true;
			this.label_camera_status.ForeColor = System.Drawing.Color.White;
			this.label_camera_status.Location = new System.Drawing.Point(13, 39);
			this.label_camera_status.Name = "label_camera_status";
			this.label_camera_status.Size = new System.Drawing.Size(35, 13);
			this.label_camera_status.TabIndex = 6;
			this.label_camera_status.Text = "label4";
			// 
			// label_mainlooptime
			// 
			this.label_mainlooptime.AutoSize = true;
			this.label_mainlooptime.ForeColor = System.Drawing.Color.White;
			this.label_mainlooptime.Location = new System.Drawing.Point(13, 643);
			this.label_mainlooptime.Name = "label_mainlooptime";
			this.label_mainlooptime.Size = new System.Drawing.Size(35, 13);
			this.label_mainlooptime.TabIndex = 7;
			this.label_mainlooptime.Text = "label4";
			// 
			// panel_skeletalcamraview
			// 
			this.panel_skeletalcamraview.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(64)))), ((int)(((byte)(64)))));
			this.panel_skeletalcamraview.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.panel_skeletalcamraview.Controls.Add(this.trackBar_column_intensity_filter);
			this.panel_skeletalcamraview.Controls.Add(this.label_column_intensity_filter);
			this.panel_skeletalcamraview.Controls.Add(this.panel_lineextract_subpixelweightedmean);
			this.panel_skeletalcamraview.Controls.Add(this.label5);
			this.panel_skeletalcamraview.Location = new System.Drawing.Point(755, 75);
			this.panel_skeletalcamraview.Name = "panel_skeletalcamraview";
			this.panel_skeletalcamraview.Size = new System.Drawing.Size(485, 540);
			this.panel_skeletalcamraview.TabIndex = 8;
			this.panel_skeletalcamraview.Visible = false;
			// 
			// trackBar_column_intensity_filter
			// 
			this.trackBar_column_intensity_filter.Location = new System.Drawing.Point(22, 79);
			this.trackBar_column_intensity_filter.Maximum = 20000;
			this.trackBar_column_intensity_filter.Minimum = 200;
			this.trackBar_column_intensity_filter.Name = "trackBar_column_intensity_filter";
			this.trackBar_column_intensity_filter.Size = new System.Drawing.Size(287, 45);
			this.trackBar_column_intensity_filter.TabIndex = 6;
			this.trackBar_column_intensity_filter.Value = 200;
			this.trackBar_column_intensity_filter.ValueChanged += new System.EventHandler(this.trackBar_column_intensity_filter_ValueChanged);
			// 
			// label_column_intensity_filter
			// 
			this.label_column_intensity_filter.AutoSize = true;
			this.label_column_intensity_filter.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label_column_intensity_filter.ForeColor = System.Drawing.Color.White;
			this.label_column_intensity_filter.Location = new System.Drawing.Point(19, 60);
			this.label_column_intensity_filter.Name = "label_column_intensity_filter";
			this.label_column_intensity_filter.Size = new System.Drawing.Size(203, 16);
			this.label_column_intensity_filter.TabIndex = 5;
			this.label_column_intensity_filter.Text = "Column Intensity Filter Value";
			// 
			// panel_lineextract_subpixelweightedmean
			// 
			this.panel_lineextract_subpixelweightedmean.BackColor = System.Drawing.Color.Gray;
			this.panel_lineextract_subpixelweightedmean.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.panel_lineextract_subpixelweightedmean.Controls.Add(this.label6);
			this.panel_lineextract_subpixelweightedmean.Location = new System.Drawing.Point(14, 250);
			this.panel_lineextract_subpixelweightedmean.Name = "panel_lineextract_subpixelweightedmean";
			this.panel_lineextract_subpixelweightedmean.Size = new System.Drawing.Size(452, 273);
			this.panel_lineextract_subpixelweightedmean.TabIndex = 1;
			// 
			// label6
			// 
			this.label6.AutoSize = true;
			this.label6.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label6.ForeColor = System.Drawing.Color.White;
			this.label6.Location = new System.Drawing.Point(3, 9);
			this.label6.Name = "label6";
			this.label6.Size = new System.Drawing.Size(367, 16);
			this.label6.TabIndex = 2;
			this.label6.Text = "Line Extraction Algorythm: Sub Pixel Weighted Mean\r\n";
			// 
			// label5
			// 
			this.label5.AutoSize = true;
			this.label5.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label5.ForeColor = System.Drawing.Color.White;
			this.label5.Location = new System.Drawing.Point(10, 18);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(278, 20);
			this.label5.TabIndex = 0;
			this.label5.Text = "Skeletal Line Processing Controls";
			// 
			// timer_gampadpoll
			// 
			this.timer_gampadpoll.Enabled = true;
			this.timer_gampadpoll.Tick += new System.EventHandler(this.timer_gampadpoll_Tick);
			// 
			// timer_camerawatchdog
			// 
			this.timer_camerawatchdog.Enabled = true;
			this.timer_camerawatchdog.Interval = 500;
			this.timer_camerawatchdog.Tick += new System.EventHandler(this.timer_camerawatchdog_Tick);
			// 
			// label_camerawatchdog
			// 
			this.label_camerawatchdog.AutoSize = true;
			this.label_camerawatchdog.Font = new System.Drawing.Font("Microsoft YaHei UI", 20.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label_camerawatchdog.ForeColor = System.Drawing.Color.Red;
			this.label_camerawatchdog.Location = new System.Drawing.Point(761, 33);
			this.label_camerawatchdog.Name = "label_camerawatchdog";
			this.label_camerawatchdog.Size = new System.Drawing.Size(462, 72);
			this.label_camerawatchdog.TabIndex = 4;
			this.label_camerawatchdog.Text = "ERROR: CAMERA NOT RUNNING\r\n\r\n";
			this.label_camerawatchdog.Click += new System.EventHandler(this.label_camerawatchdog_Click);
			// 
			// timer_uiupdater
			// 
			this.timer_uiupdater.Enabled = true;
			this.timer_uiupdater.Interval = 200;
			this.timer_uiupdater.Tick += new System.EventHandler(this.timer_uiupdater_Tick);
			// 
			// MainForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.BackColor = System.Drawing.Color.Black;
			this.ClientSize = new System.Drawing.Size(1264, 681);
			this.Controls.Add(this.label_mainlooptime);
			this.Controls.Add(this.label_camera_status);
			this.Controls.Add(this.panAndZoomPictureBox_view);
			this.Controls.Add(this.menuStrip1);
			this.Controls.Add(this.label_controllerstatus);
			this.Controls.Add(this.panel_operationViewTracking);
			this.Controls.Add(this.panel_skeletalcamraview);
			this.Controls.Add(this.panel_rawcameraview);
			this.Controls.Add(this.panel_blurrview);
			this.Controls.Add(this.panel_thresholdview);
			this.Controls.Add(this.label_camerawatchdog);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
			this.MainMenuStrip = this.menuStrip1;
			this.Name = "MainForm";
			this.Text = "TBI Vision";
			this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.MainForm_FormClosed);
			this.Load += new System.EventHandler(this.MainForm_Load);
			this.menuStrip1.ResumeLayout(false);
			this.menuStrip1.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.panAndZoomPictureBox_view)).EndInit();
			this.panel_operationViewTracking.ResumeLayout(false);
			this.panel_operationViewTracking.PerformLayout();
			this.panel1.ResumeLayout(false);
			this.panel1.PerformLayout();
			this.panel_rawcameraview.ResumeLayout(false);
			this.panel_rawcameraview.PerformLayout();
			this.panel_blurrview.ResumeLayout(false);
			this.panel_blurrview.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.trackBar_blur)).EndInit();
			this.panel_thresholdview.ResumeLayout(false);
			this.panel_thresholdview.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.trackBar_threshold)).EndInit();
			this.panel_skeletalcamraview.ResumeLayout(false);
			this.panel_skeletalcamraview.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.trackBar_column_intensity_filter)).EndInit();
			this.panel_lineextract_subpixelweightedmean.ResumeLayout(false);
			this.panel_lineextract_subpixelweightedmean.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.MenuStrip menuStrip1;
		private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem loadToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem viewToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem settingsToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem operationViewToolStripMenuItem;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
		private System.Windows.Forms.ToolStripMenuItem imageProcessingViewsToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem rawCameraFrameToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem blurrFrameToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem thresholdFrameToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem skeletonFrameToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem sensorModeToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem cameraSelectionToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem aboutTBIToolStripMenuItem;
		private Emgu.CV.UI.PanAndZoomPictureBox panAndZoomPictureBox_view;
		private System.Windows.Forms.Panel panel_operationViewTracking;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
		private System.Windows.Forms.Panel panel_rawcameraview;
		private System.Windows.Forms.Panel panel_blurrview;
		private System.Windows.Forms.Panel panel_thresholdview;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Label label_camera_status;
		private System.Windows.Forms.Label label_mainlooptime;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Panel panel_skeletalcamraview;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
		private System.Windows.Forms.ToolStripMenuItem exitProgramToolStripMenuItem;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
		private System.Windows.Forms.TrackBar trackBar_blur;
		private System.Windows.Forms.Label label_blur;
		private System.Windows.Forms.TrackBar trackBar_threshold;
		private System.Windows.Forms.Label label_threshold;
		private System.Windows.Forms.Panel panel_lineextract_subpixelweightedmean;
		private System.Windows.Forms.Label label6;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator5;
		private System.Windows.Forms.ToolStripMenuItem viewZeroLinesToolStripMenuItem;
		private System.Windows.Forms.Panel panel1;
		private System.Windows.Forms.Label label_controllerstatus;
		private System.Windows.Forms.Timer timer_gampadpoll;
		private System.Windows.Forms.ToolStripMenuItem viewTrackingLinesToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem viewRawDataScanToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem dSPFiltersToolStripMenuItem;
		private System.Windows.Forms.Timer timer_camerawatchdog;
		private System.Windows.Forms.Label label_camerawatchdog;
		private System.Windows.Forms.ToolStripMenuItem seamTypeToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem roundSeamToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem straightSeamToolStripMenuItem;
		private System.Windows.Forms.TrackBar trackBar_column_intensity_filter;
		private System.Windows.Forms.Label label_column_intensity_filter;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator6;
		private System.Windows.Forms.ToolStripMenuItem testFrameToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem viewTrackingPointsToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem drawColumniteImageOnOperationImageToolStripMenuItem;
        private System.Windows.Forms.Label label_trackingmode;
        private System.Windows.Forms.Timer timer_uiupdater;
		private System.Windows.Forms.ToolStripMenuItem vGrooveToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem buttToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem filletToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem manualCalibrationToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem autoCalibrationToolStripMenuItem;
        private System.Windows.Forms.Label label_zgrbl;
        private System.Windows.Forms.Label label_xgrbl;
        private System.Windows.Forms.Label label_track_to_point;
        private System.Windows.Forms.Label label_x_mmperpixel;
        private System.Windows.Forms.Label label_z_mmperpixel;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator7;
        private System.Windows.Forms.ToolStripMenuItem shutDownComputerToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator8;
        private System.Windows.Forms.ToolStripMenuItem equalOffsetToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem leftSideTransistionOffsetToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem rightSideTransitionOffsetToolStripMenuItem;
    }
}

