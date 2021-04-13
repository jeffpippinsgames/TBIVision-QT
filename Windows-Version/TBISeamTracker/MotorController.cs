using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Ports;
using System.Windows.Forms;


namespace TBISeamTracker
{
    enum MotorControllerStatus_t { OK, BUSY, STOPPING }
    

    class MotorController
    {
        public SerialPort zserialport;
        public SerialPort xserialport;

        MotorControllerStatus_t motorstatusx;
        MotorControllerStatus_t motorstatusz;

        bool in_gamepad_doloop;
        bool is_steppingx;
        bool is_steppingz;
        bool is_joggingx;
        bool is_joggingz;
        bool needs_to_stopx;
        bool needs_to_stopz;

        public MotorController()
        {
            zserialport = new SerialPort("COM4", 9600, Parity.None, 8, StopBits.One);
            xserialport = new SerialPort("COM3", 9600, Parity.None, 8, StopBits.One);
            zserialport.Encoding = Encoding.ASCII;
            xserialport.Encoding = Encoding.ASCII;
            motorstatusx = MotorControllerStatus_t.OK;
            motorstatusz = MotorControllerStatus_t.OK;
            in_gamepad_doloop = false;
            is_steppingx = false;
            is_steppingz = false;
            is_joggingx = false;
            is_joggingz = false;
            needs_to_stopx = false;
            needs_to_stopz = false;
        }

        public void ConnectToMicroControllers()
        {
            try
            {
                zserialport.Open();
            }
            catch (UnauthorizedAccessException ex)
            {
                MessageBox.Show(ex.Message);
                zserialport.Close();
                Application.Exit();
            }
            try
            {
                xserialport.Open();
            }
            catch (UnauthorizedAccessException ex)
            {
                MessageBox.Show(ex.Message);
                xserialport.Close();
                Application.Exit();
            }
            zserialport.DataReceived += new SerialDataReceivedEventHandler(DataReceivedHandlerZ);
            xserialport.DataReceived += new SerialDataReceivedEventHandler(DataReceivedHandlerX);
        }

        public void DisconnectMicrocontrollers()
        {
            if (xserialport.IsOpen) xserialport.Close();
            if (zserialport.IsOpen) zserialport.Close();
        }

        private void DataReceivedHandlerZ(object sender, SerialDataReceivedEventArgs e)
        {
            SerialPort sp = (SerialPort)sender;
            string indata = sp.ReadExisting();

            foreach (char _c in indata)
            {
                switch (_c)
                {
                    case 'O':
                        motorstatusz = MotorControllerStatus_t.OK;
                        break;
                    case 'X':
                        motorstatusz = MotorControllerStatus_t.STOPPING;
                        break;
                    case 'B':
                        motorstatusz = MotorControllerStatus_t.BUSY;
                        break;
                }
            }
        }

        private void DataReceivedHandlerX(object sender, SerialDataReceivedEventArgs e)
        {
            SerialPort sp = (SerialPort)sender;
            string indata = sp.ReadExisting();

            foreach(char _c in indata)
            {
               switch(_c)
                {
                    case 'O':
                        motorstatusx = MotorControllerStatus_t.OK;
                        break;
                    case 'X':
                        motorstatusx = MotorControllerStatus_t.STOPPING;
                        break;
                    case 'B':
                        motorstatusx = MotorControllerStatus_t.BUSY;
                        break;
                   
                }
            }

           
        }

        public void DoGamePadJogLoop(ISensorSettings _sensor_settings, Gamepad _gamepad)
        {
           if(_sensor_settings.TrackingModeSensorState == TrackingModeSensorState_t.AUTOMODE_OFF)
            {
                if (in_gamepad_doloop) return;
                in_gamepad_doloop = true;

                //Get Current Machine States
                MotorControllerStatus_t _statusx = motorstatusx;
                MotorControllerStatus_t _statusz = motorstatusz;

                //Reset the Internal Control Flags
                if(_statusx == MotorControllerStatus_t.OK)
                {
                    needs_to_stopx = false;
                    is_steppingx = false;
                    is_joggingx = false;
                }

                if (_statusz == MotorControllerStatus_t.OK)
                {
                    needs_to_stopz = false;
                    is_steppingz = false;
                    is_joggingz = false;
                }



                //Check to See is Need to Kill
                if (!_gamepad.DPadLeft && !_gamepad.DPadRight && (_statusx == MotorControllerStatus_t.BUSY) && !needs_to_stopx && !is_steppingx) needs_to_stopx = true;
                if (!_gamepad.DPadUp && !_gamepad.DPadDown && (_statusz == MotorControllerStatus_t.BUSY) && !needs_to_stopz && !is_steppingz) needs_to_stopz = true;

                //Send Kill Command If Need To
                bool _return = false;
                if (needs_to_stopx && (_statusx == MotorControllerStatus_t.BUSY))
                {
                    _return = true;
                    needs_to_stopx = false;
                    _statusx = MotorControllerStatus_t.STOPPING;
                    motorstatusx = MotorControllerStatus_t.STOPPING;
                    xserialport.Write("S");
                }
                if (needs_to_stopz && (_statusz == MotorControllerStatus_t.BUSY))
                {
                    _return = true;
                    needs_to_stopz = false;
                    _statusz = MotorControllerStatus_t.STOPPING;
                    motorstatusz = MotorControllerStatus_t.STOPPING;
                    zserialport.Write("S");
                }
                if (_return)
                {
                    in_gamepad_doloop = false;
                    return;
                }

                //Do Stepping First; No Trigger Held Down and Stat
                if(!_gamepad.LeftTrigger && !_gamepad.RightTrigger && (_gamepad.DPadLeft || _gamepad.DPadRight) && (_statusx == MotorControllerStatus_t.OK) && !is_steppingx)
                {
                    is_steppingx = true;
                    _statusx = MotorControllerStatus_t.BUSY;
                    motorstatusx = MotorControllerStatus_t.BUSY;
                    
                    
                    if (_gamepad.DPadLeft) xserialport.Write("M-1F5\r");
                    else xserialport.Write("M1F5\r");              
                }
                if (!_gamepad.LeftTrigger && !_gamepad.RightTrigger && (_gamepad.DPadUp || _gamepad.DPadDown) && (_statusz == MotorControllerStatus_t.OK) && !is_steppingz)
                {
                    is_steppingz = true;
                    _statusz = MotorControllerStatus_t.BUSY;
                    motorstatusz = MotorControllerStatus_t.BUSY;
                    if (_gamepad.DPadDown) zserialport.Write("M-1F5\r");
                    else zserialport.Write("M1F3\r");
                }

                //Processing Jogging
                if(_gamepad.LeftTrigger && (_gamepad.DPadLeft || _gamepad.DPadRight) && (_statusx == MotorControllerStatus_t.OK) && !is_joggingx)
                {
                    is_joggingx = true;
                    _statusx = MotorControllerStatus_t.BUSY;
                    motorstatusx = MotorControllerStatus_t.BUSY;
                    if(_gamepad.RightTrigger)
                    {
                        if (_gamepad.DPadLeft) xserialport.Write("M-500F5\r");
                        else xserialport.Write("M500F5\r");
                    }
                    else
                    {
                        if (_gamepad.DPadLeft) xserialport.Write("M-500F2.5\r");
                        else xserialport.Write("M500F2.5\r");
                    }        
                }
                if (_gamepad.LeftTrigger && (_gamepad.DPadDown || _gamepad.DPadUp) && (_statusz == MotorControllerStatus_t.OK) && !is_joggingz)
                {
                    is_joggingz = true;
                    _statusz = MotorControllerStatus_t.BUSY;
                    motorstatusz = MotorControllerStatus_t.BUSY;
                    if (_gamepad.RightTrigger)
                    {
                        if (_gamepad.DPadDown) zserialport.Write("M-500F5\r");
                        else zserialport.Write("M500F3\r");
                    }
                    else
                    {
                        if (_gamepad.DPadDown) zserialport.Write("M-500F2.5\r");
                        else zserialport.Write("M500F2.5\r");
                    }
                }


                in_gamepad_doloop = false;
            }
           else
            {
                in_gamepad_doloop = false;
            }
        }

        public void MoveX(double _dist = 0, double _feed = 5.0)
        {
            if(motorstatusx == MotorControllerStatus_t.OK)
            {
                motorstatusx = MotorControllerStatus_t.BUSY;
                xserialport.Write("M" + _dist.ToString("##.###") + "F" + _feed.ToString("##.###") + "\r");
            }
        }

        public void MoveZ(double _dist = 0, double _feed = 3.0)
        {
            if (motorstatusz == MotorControllerStatus_t.OK)
            {
                motorstatusz = MotorControllerStatus_t.BUSY;
                zserialport.Write("M" + _dist.ToString("##.###") + "F" + _feed.ToString("##.###") + "\r");
            }
        }

        public MotorControllerStatus_t GetStatusX
        {
            get { return motorstatusx; }
        }

        public MotorControllerStatus_t GetStatusZ
        {
            get { return motorstatusz; }
        }



    }
}
