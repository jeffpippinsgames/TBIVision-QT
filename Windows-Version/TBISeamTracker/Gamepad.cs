using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using SharpDX.XInput;
using System.Drawing;
using System.Diagnostics;

namespace TBISeamTracker
{
	//******************************************************
	interface IGamepadState
	{
		bool LeftStickUp { get; }
		bool LeftStickDown { get; }
		bool LeftStickRight { get; }
		bool LeftStickLeft { get; }
		bool LeftStickPress { get; }
		bool RightStickUp { get; }
		bool RightStickDown { get; }
		bool RightStickRight { get; }
		bool RightStickLeft { get; }
		bool RightStickPress { get; }
		bool LeftBumper { get; }
		bool RightBumper { get; }
		bool Back { get; }
		bool Start { get; }
		bool LeftTrigger { get; }
		bool RightTrigger { get; }
		bool DPadUp { get; }
		bool DPadDown { get; }
		bool DPadRight { get; }
		bool DPadLeft { get; }
		bool AButton { get; }
		bool BButton { get; }
		bool YButton { get; }
		bool XButton { get; }
	}
	//******************************************************

	//******************************************************
	class GamepadState : IGamepadState
	{
		//******************************************************
		#region Class Members

		bool left_stick_up;
		bool left_stick_down;
		bool left_stick_right;
		bool left_stick_left;
		bool left_stick_press;

		bool right_stick_up;
		bool right_stick_down;
		bool right_stick_left;
		bool right_stick_right;
		bool right_stick_press;

		bool left_trigger;
		bool right_trigger;
		bool left_bumper;
		bool right_bumper;

		bool dpad_up;
		bool dpad_down;
		bool dpad_left;
		bool dpad_right;

		bool back;
		bool start;

		bool a_button;
		bool b_button;
		bool y_button;
		bool x_button;

		

		#endregion
		//******************************************************


		//******************************************************
		#region Constructors
		//-------------------------------------
		public GamepadState()
		{
			left_stick_up = false;
			left_stick_down = false;
			left_stick_right = false;
			left_stick_left = false;

			right_stick_up = false;
			right_stick_down = false;
			right_stick_right = false;
			right_stick_left = false;

			left_trigger = false;
			right_trigger = false;
			left_bumper = false;
			right_bumper = false;

			dpad_up = false;
			dpad_down = false;
			dpad_right = false;
			dpad_left = false;

			back = false;
			start = false;

			a_button = false;
			b_button = false;
			y_button = false;
			x_button = false;
		}
		//-------------------------------------
		//******************************************************
		#endregion

		//******************************************************
		#region Methods
		//-------------------------------------
		public void SetEqual(GamepadState _state)
		{
			this.a_button = _state.AButton;
			this.back = _state.Back;
			this.b_button = _state.BButton;
			this.dpad_down = _state.DPadDown;
			this.dpad_left = _state.DPadLeft;
			this.dpad_right = _state.DPadRight;
			this.dpad_up = _state.DPadUp;
			this.left_bumper = _state.LeftBumper;
			this.left_stick_down = _state.LeftStickDown;
			this.left_stick_left = _state.LeftStickLeft;
			this.left_stick_press = _state.LeftStickPress;
			this.left_stick_right = _state.LeftStickRight;
			this.left_stick_up = _state.LeftStickUp;
			this.left_trigger = _state.LeftTrigger;
			this.right_bumper = _state.RightBumper;
			this.right_stick_down = _state.RightStickDown;
			this.right_stick_left = _state.RightStickLeft;
			this.right_stick_press = _state.RightStickPress;
			this.right_stick_right = _state.RightStickRight;
			this.right_stick_up = _state.RightStickUp;
			this.right_trigger = _state.RightTrigger;
			this.start = _state.Start;
			this.x_button = _state.XButton;
			this.y_button = _state.YButton;
		}
		//-------------------------------------
		public void GetGamepadState(SharpDX.XInput.Controller _controller)
		{
			if (_controller == null) return;
			if (!_controller.IsConnected) return;

			SharpDX.XInput.State _state = _controller.GetState();
			const short deadzone = 20000;

			if ((_state.Gamepad.Buttons & GamepadButtonFlags.A) == GamepadButtonFlags.A) this.a_button = true;
			else this.a_button = false;

			if ((_state.Gamepad.Buttons & GamepadButtonFlags.B) == GamepadButtonFlags.B) this.b_button = true;
			else this.b_button = false;

			if ((_state.Gamepad.Buttons & GamepadButtonFlags.Back) == GamepadButtonFlags.Back) this.back = true;
			else this.back = false;

			if ((_state.Gamepad.Buttons & GamepadButtonFlags.DPadDown) == GamepadButtonFlags.DPadDown) this.dpad_down = true;
			else this.dpad_down = false;

			if ((_state.Gamepad.Buttons & GamepadButtonFlags.DPadLeft) == GamepadButtonFlags.DPadLeft) this.dpad_left = true;
			else this.dpad_left = false;

			if ((_state.Gamepad.Buttons & GamepadButtonFlags.DPadRight) == GamepadButtonFlags.DPadRight) this.dpad_right = true;
			else this.dpad_right = false;

			if ((_state.Gamepad.Buttons & GamepadButtonFlags.DPadUp) == GamepadButtonFlags.DPadUp) this.dpad_up = true;
			else this.dpad_up = false;

			if ((_state.Gamepad.Buttons & GamepadButtonFlags.LeftShoulder) == GamepadButtonFlags.LeftShoulder) this.left_bumper = true;
			else this.left_bumper = false;

			if ((_state.Gamepad.Buttons & GamepadButtonFlags.LeftThumb) == GamepadButtonFlags.LeftThumb) this.left_stick_press = true;
			else this.left_stick_press = false;

			if ((_state.Gamepad.Buttons & GamepadButtonFlags.RightShoulder) == GamepadButtonFlags.RightShoulder) this.right_bumper = true;
			else this.right_bumper = false;

			if ((_state.Gamepad.Buttons & GamepadButtonFlags.RightThumb) == GamepadButtonFlags.RightThumb) this.right_stick_press = true;
			else this.right_stick_press = false;

			if ((_state.Gamepad.Buttons & GamepadButtonFlags.Start) == GamepadButtonFlags.Start) this.start = true;
			else this.start = false;

			if ((_state.Gamepad.Buttons & GamepadButtonFlags.X) == GamepadButtonFlags.X) this.x_button = true;
			else this.x_button = false;

			if ((_state.Gamepad.Buttons & GamepadButtonFlags.Y) == GamepadButtonFlags.Y) this.y_button = true;
			else this.y_button = false;
			//-------------------------------------
			if (_state.Gamepad.LeftThumbX > deadzone) this.left_stick_right = true;
			else this.left_stick_right = false;
			if (_state.Gamepad.LeftThumbX < -deadzone) this.left_stick_left = true;
			else this.left_stick_left = false;
			if (_state.Gamepad.LeftThumbY > deadzone) this.left_stick_up = true;
			else this.left_stick_up = false;
			if (_state.Gamepad.LeftThumbY < -deadzone) this.left_stick_down = true;
			else this.left_stick_down = false;
			//-------------------------------------
			if (_state.Gamepad.RightThumbX > deadzone) this.right_stick_right = true;
			else this.right_stick_right = false;
			if (_state.Gamepad.RightThumbX < -deadzone) this.right_stick_left = true;
			else this.right_stick_left = false;
			if (_state.Gamepad.RightThumbY > deadzone) this.right_stick_up = true;
			else this.right_stick_up = false;
			if (_state.Gamepad.RightThumbY < -deadzone) this.right_stick_down = true;
			else this.right_stick_down = false;
			//-------------------------------------
			if (_state.Gamepad.LeftTrigger > 100) this.left_trigger = true;
			else this.left_trigger = false;
			//-------------------------------------
			if (_state.Gamepad.RightTrigger > 100) this.right_trigger = true;
			else this.right_trigger = false;
		}
		//-------------------------------------
		#endregion
		//******************************************************

		//******************************************************
		#region Accessors
		public IGamepadState GamepadStateInterface
		{
			get { return this as IGamepadState; }
		}
		//-------------------------------------
		public bool LeftStickUp
		{
			get { return left_stick_up; }
		}
		//-------------------------------------
		public bool LeftStickDown
		{
			get { return left_stick_down; }
		}
		//-------------------------------------
		public bool LeftStickRight
		{
			get { return left_stick_right; }
		}
		//-------------------------------------
		public bool LeftStickLeft
		{
			get { return left_stick_left; }
		}
		//-------------------------------------
		public bool LeftStickPress
		{
			get { return left_stick_press; }
		}
		//-------------------------------------
		public bool RightStickUp
		{
			get { return right_stick_up; }
		}
		//-------------------------------------
		public bool RightStickDown
		{
			get { return right_stick_down; }
		}
		//-------------------------------------
		public bool RightStickRight
		{
			get { return right_stick_right; }
		}
		//-------------------------------------
		public bool RightStickLeft
		{
			get { return right_stick_left; }
		}
		//-------------------------------------
		public bool RightStickPress
		{
			get { return right_stick_press; }
		}
		//-------------------------------------
		public bool LeftTrigger
		{
			get { return left_trigger; }
		}
		//-------------------------------------
		public bool RightTrigger
		{
			get { return right_trigger; }
		}
		//-------------------------------------
		public bool LeftBumper
		{
			get { return left_bumper; }
		}
		//-------------------------------------
		public bool RightBumper
		{
			get { return right_bumper; }
		}
		//-------------------------------------
		public bool DPadUp
		{
			get { return dpad_up; }
		}
		//-------------------------------------
		public bool DPadDown
		{
			get { return dpad_down; }
		}
		//-------------------------------------
		public bool DPadRight
		{
			get { return dpad_right; }
		}
		//-------------------------------------
		public bool DPadLeft
		{
			get { return dpad_left; }
		}
		//-------------------------------------
		public bool Back
		{
			get { return back; }
		}
		//-------------------------------------
		public bool Start
		{
			get { return start; }
		}
		//-------------------------------------
		public bool AButton
		{
			get { return a_button; }
		}
		//-------------------------------------
		public bool BButton
		{
			get { return b_button; }
		}
		//-------------------------------------
		public bool YButton
		{
			get { return y_button; }
		}
		//-------------------------------------
		public bool XButton
		{
			get { return x_button; }
		}
		//-------------------------------------
		//******************************************************
		#endregion
	}//End Class Definition
	//******************************************************

	class GamePadEventArg
	{
		public bool left_trigger_pressed;
        public bool right_trigger_pressed;

        public GamePadEventArg()
        {
            left_trigger_pressed = false;
            right_trigger_pressed = false;
        }
	}

	//******************************************************
	class Gamepad
	{
		//******************************************************
		#region Events and Delegates

		public delegate void Delegate_OnLeftStickUp_Pressed();
		public event Delegate_OnLeftStickUp_Pressed OnLeftStickUp_Pressed;
		public delegate void Delegate_OnLeftStickUp_Released();
		public event Delegate_OnLeftStickUp_Released OnLeftStickUp_Released;

		public delegate void Delegate_OnLeftStickDown_Pressed();
		public event Delegate_OnLeftStickDown_Pressed OnLeftStickDown_Pressed;
		public delegate void Delegate_OnLeftStickDown_Released();
		public event Delegate_OnLeftStickDown_Released OnLeftStickDown_Released;

		public delegate void Delegate_OnLeftStickRight_Pressed();
		public event Delegate_OnLeftStickRight_Pressed OnLeftStickRight_Pressed;
		public delegate void Delegate_OnLeftStickRight_Released();
		public event Delegate_OnLeftStickRight_Released OnLeftStickRight_Released;

		public delegate void Delegate_OnLeftStickLeft_Pressed();
		public event Delegate_OnLeftStickLeft_Pressed OnLeftStickLeft_Pressed;
		public delegate void Delegate_OnLeftStickLeft_Released();
		public event Delegate_OnLeftStickLeft_Released OnLeftStickLeft_Released;

		public delegate void Delegate_OnLeftStickPress_Pressed();
		public event Delegate_OnLeftStickPress_Pressed OnLeftStickPress_Pressed;
		public delegate void Delegate_OnLeftStickPress_Released();
		public event Delegate_OnLeftStickPress_Released OnLeftStickPress_Released;
		//-------------------------------------

		//-------------------------------------
		public delegate void Delegate_OnRightStickUp_Pressed();
		public event Delegate_OnRightStickUp_Pressed OnRightStickUp_Pressed;
		public delegate void Delegate_OnRightStickUp_Released();
		public event Delegate_OnRightStickUp_Released OnRightStickUp_Released;

		public delegate void Delegate_OnRightStickDown_Pressed();
		public event Delegate_OnRightStickDown_Pressed OnRightStickDown_Pressed;
		public delegate void Delegate_OnRightStickDown_Released();
		public event Delegate_OnRightStickDown_Released OnRightStickDown_Released;

		public delegate void Delegate_OnRightStickRight_Pressed();
		public event Delegate_OnRightStickRight_Pressed OnRightStickRight_Pressed;
		public delegate void Delegate_OnRightStickRight_Released();
		public event Delegate_OnRightStickRight_Released OnRightStickRight_Released;

		public delegate void Delegate_OnRightStickLeft_Pressed();
		public event Delegate_OnRightStickLeft_Pressed OnRightStickLeft_Pressed;
		public delegate void Delegate_OnRightStickLeft_Released();
		public event Delegate_OnRightStickLeft_Released OnRightStickLeft_Released;

		public delegate void Delegate_OnRightStickPress_Pressed();
		public event Delegate_OnRightStickPress_Pressed OnRightStickPress_Pressed;
		public delegate void Delegate_OnRightStickPress_Released();
		public event Delegate_OnRightStickPress_Released OnRightStickPress_Released;
		//-------------------------------------

		//-------------------------------------
		public delegate void Delegate_LeftBumper_Pressed();
		public event Delegate_LeftBumper_Pressed OnLeftBumper_Pressed;
		public delegate void Delegate_LeftBumper_Released();
		public event Delegate_LeftBumper_Released OnLeftBumper_Released;
		public delegate void Delegate_RightBumper_Pressed();
		public event Delegate_RightBumper_Pressed OnRightBumper_Pressed;
		public delegate void Delegate_RightBumper_Released();
		public event Delegate_RightBumper_Released OnRightBumper_Released;
		public delegate void Delegate_LeftTrigger_Pressed();
		public event Delegate_LeftTrigger_Pressed OnLeftTrigger_Pressed;
		public delegate void Delegate_LeftTrigger_Released();
		public event Delegate_LeftTrigger_Released OnLeftTrigger_Released;
		public delegate void Delegate_RightTrigger_Pressed();
		public event Delegate_RightTrigger_Pressed OnRightTrigger_Pressed;
		public delegate void Delegate_RightTrigger_Released();
		public event Delegate_RightTrigger_Released OnRightTrigger_Released;
		//-------------------------------------

		//-------------------------------------
		public delegate void Delegate_Back_Pressed();
		public event Delegate_Back_Pressed OnBack_Pressed;
		public delegate void Delegate_Back_Released();
		public event Delegate_Back_Released OnBack_Released;
		public delegate void Delegate_Start_Pressed();
		public event Delegate_Start_Pressed OnStart_Pressed;
		public delegate void Delegate_Start_Released();
		public event Delegate_Start_Released OnStart_Released;
		//-------------------------------------

		//-------------------------------------
		public delegate void Delegate_DPadUp_Pressed(GamePadEventArg e);
		public event Delegate_DPadUp_Pressed OnDPadUp_Pressed;
		public delegate void Delegate_DPadUp_Release(GamePadEventArg e);
		public event Delegate_DPadUp_Release OnDPadUp_Released;
		public delegate void Delegate_DPadDown_Pressed(GamePadEventArg e);
		public event Delegate_DPadDown_Pressed OnDPadDown_Pressed;
		public delegate void Delegate_DPadDown_Release(GamePadEventArg e);
		public event Delegate_DPadDown_Release OnDPadDown_Released;
		public delegate void Delegate_DPadRight_Pressed(GamePadEventArg e);
		public event Delegate_DPadRight_Pressed OnDPadRight_Pressed;
		public delegate void Delegate_DPadRight_Release(GamePadEventArg e);
		public event Delegate_DPadRight_Release OnDPadRight_Released;
		public delegate void Delegate_DPadLeft_Pressed(GamePadEventArg e);
		public event Delegate_DPadLeft_Pressed OnDPadLeft_Pressed;
		public delegate void Delegate_DPadLeft_Release(GamePadEventArg e);
		public event Delegate_DPadLeft_Release OnDPadLeft_Released;
		//-------------------------------------

		//-------------------------------------
		public delegate void Delegate_AButton_Pressed(GamePadEventArg e);
		public event Delegate_AButton_Pressed OnAButton_Pressed;
		public delegate void Delegate_AButton_Released(GamePadEventArg e);
		public event Delegate_AButton_Released OnAButton_Released;
		public delegate void Delegate_BButton_Pressed(GamePadEventArg e);
		public event Delegate_BButton_Pressed OnBButton_Pressed;
		public delegate void Delegate_BButton_Released(GamePadEventArg e);
		public event Delegate_BButton_Released OnBButton_Released;
		public delegate void Delegate_YButton_Pressed(GamePadEventArg e);
		public event Delegate_YButton_Pressed OnYButton_Pressed;
		public delegate void Delegate_YButton_Released();
		public event Delegate_YButton_Released OnYButton_Released;
		public delegate void Delegate_XButton_Pressed(GamePadEventArg e);
		public event Delegate_XButton_Pressed OnXButton_Pressed;
		public delegate void Delegate_XButton_Released();
		public event Delegate_XButton_Released OnXButton_Released;
		//-------------------------------------

		//-------------------------------------
		public delegate void Delegate_Controller_Connected();
		public event Delegate_Controller_Connected OnControllerConnected;
		public delegate void Delegate_Controller_Disconnected();
		public event Delegate_Controller_Disconnected OnControllerDisconnected;
		//-------------------------------------




		#endregion
		//******************************************************

		//******************************************************
		#region Class Members

		Controller gamepad;
		GamepadState current_state;
		GamepadState prev_state;
		Thread polling_thread;
		ThreadStart polling_thread_ref;
		bool runmainloop;

		Stopwatch dpad_right_stopwatch_holddown;
		Stopwatch dpad_left_stopwatch_holddown;
		Stopwatch dpad_right_stopwatch_repeat;
		Stopwatch dpad_left_stopwatch_repeat;

        Stopwatch dpad_up_stopwatch_holddown;
        Stopwatch dpad_down_stopwatch_holddown;
        Stopwatch dpad_up_stopwatch_repeat;
        Stopwatch dpad_down_stopwatch_repeat;

        const long HOLDDOWNTIME = 10;
		const long REPEATTIME = 20;

		#endregion
		//******************************************************


		//******************************************************
		#region Constructors
		//-------------------------------------
		public Gamepad()
		{
			gamepad = null;
			current_state = new GamepadState();
			prev_state = new GamepadState();
			runmainloop = true;
			polling_thread_ref = new ThreadStart(DoThreadedLoop);
			polling_thread = new Thread(polling_thread_ref);
			dpad_left_stopwatch_holddown = new Stopwatch();
			dpad_right_stopwatch_holddown = new Stopwatch();
			dpad_left_stopwatch_repeat = new Stopwatch();
			dpad_right_stopwatch_repeat = new Stopwatch();

            dpad_up_stopwatch_holddown = new Stopwatch();
            dpad_down_stopwatch_holddown = new Stopwatch();
            dpad_up_stopwatch_repeat = new Stopwatch();
            dpad_down_stopwatch_repeat = new Stopwatch();

        }
		#endregion
		//-------------------------------------
		//******************************************************

		//******************************************************
		#region Methods
		//-------------------------------------
		public void StartPollingThread()
		{
			runmainloop = true;
			polling_thread.Start();
		}
        //-------------------------------------
        public void EndPollingThread()
		{
			runmainloop = false;
		}
		//-------------------------------------
		private void DoThreadedLoop()
		{
			while(runmainloop)
			{
				DoLoop();
			}
		}
		//-------------------------------------
		public void DoLoop ()
		{
			if (DoGamepadWatchdog()) //A Valid Gampad is Connected
			{
				//Read Current State
				current_state.GetGamepadState(gamepad);
				GamePadEventArg e = new GamePadEventArg();
				e.left_trigger_pressed = current_state.LeftTrigger;
                e.right_trigger_pressed = current_state.RightTrigger;
                
				//Compare States Fire Events
				if ((OnLeftStickUp_Pressed != null) && current_state.LeftStickUp && !prev_state.LeftStickUp) OnLeftStickUp_Pressed();
				if ((OnLeftStickUp_Released != null) && !current_state.LeftStickUp && prev_state.LeftStickUp) OnLeftStickUp_Released();
				if ((OnLeftStickDown_Pressed != null) && current_state.LeftStickDown && !prev_state.LeftStickDown) OnLeftStickDown_Pressed();
				if ((OnLeftStickDown_Released != null) && !current_state.LeftStickDown && prev_state.LeftStickDown) OnLeftStickDown_Released();
				if ((OnLeftStickRight_Pressed != null) && current_state.LeftStickRight && !prev_state.LeftStickRight) OnLeftStickRight_Pressed();
				if ((OnLeftStickRight_Released != null) && !current_state.LeftStickRight && prev_state.LeftStickRight) OnLeftStickRight_Released();
				if ((OnLeftStickLeft_Pressed != null) && current_state.LeftStickLeft && !prev_state.LeftStickLeft) OnLeftStickLeft_Pressed();
				if ((OnLeftStickLeft_Released != null) && !current_state.LeftStickLeft && prev_state.LeftStickLeft) OnLeftStickLeft_Released();
				if ((OnLeftStickPress_Pressed != null) && current_state.LeftStickPress && !prev_state.LeftStickPress) OnLeftStickPress_Pressed();
				if ((OnLeftStickPress_Released != null) && !current_state.LeftStickPress && prev_state.LeftStickPress) OnLeftStickPress_Released();

				if ((OnRightStickUp_Pressed != null) && current_state.RightStickUp && !prev_state.RightStickUp) OnRightStickUp_Pressed();
				if ((OnRightStickUp_Released != null) && !current_state.RightStickUp && prev_state.RightStickUp) OnRightStickUp_Released();
				if ((OnRightStickDown_Pressed != null) && current_state.RightStickDown && !prev_state.RightStickDown) OnRightStickDown_Pressed();
				if ((OnRightStickDown_Released != null) && !current_state.RightStickDown && prev_state.RightStickDown) OnRightStickDown_Released();
				if ((OnRightStickRight_Pressed != null) && current_state.RightStickRight && !prev_state.RightStickRight) OnRightStickRight_Pressed();
				if ((OnRightStickRight_Released != null) && !current_state.RightStickRight && prev_state.RightStickRight) OnRightStickRight_Released();
				if ((OnRightStickLeft_Pressed != null) && current_state.RightStickLeft && !prev_state.RightStickLeft) OnRightStickLeft_Pressed();
				if ((OnRightStickLeft_Released != null) && !current_state.RightStickLeft && prev_state.RightStickLeft) OnRightStickLeft_Released();
				if ((OnRightStickPress_Pressed != null) && current_state.RightStickPress && !prev_state.RightStickPress) OnRightStickPress_Pressed();
				if ((OnRightStickPress_Released != null) && !current_state.RightStickPress && prev_state.RightStickPress) OnRightStickPress_Released();

				if ((OnLeftBumper_Pressed != null) && current_state.LeftBumper && !prev_state.LeftBumper) OnLeftBumper_Pressed();
				if ((OnLeftBumper_Released != null) && !current_state.LeftBumper && prev_state.LeftBumper) OnLeftBumper_Released();
				if ((OnLeftTrigger_Pressed != null) && current_state.LeftTrigger && !prev_state.LeftTrigger) OnLeftTrigger_Pressed();
				if ((OnLeftTrigger_Released != null) && !current_state.LeftTrigger && prev_state.LeftTrigger) OnLeftTrigger_Released();
				if ((OnRightBumper_Pressed != null) && current_state.RightBumper && !prev_state.RightBumper) OnRightBumper_Pressed();
				if ((OnRightBumper_Released != null) && !current_state.RightBumper && prev_state.RightBumper) OnRightBumper_Released();
				if ((OnRightTrigger_Pressed != null) && current_state.RightTrigger && !prev_state.RightTrigger) OnRightTrigger_Pressed();
				if ((OnRightTrigger_Released != null) && !current_state.RightTrigger && prev_state.RightTrigger) OnRightTrigger_Released();


				if ((OnDPadUp_Pressed != null) && current_state.DPadUp && !prev_state.DPadUp) OnDPadUp_Pressed(e);
				if ((OnDPadUp_Released != null) && !current_state.DPadUp && prev_state.DPadUp) OnDPadUp_Released(e);
				if ((OnDPadDown_Pressed != null) && current_state.DPadDown && !prev_state.DPadDown) OnDPadDown_Pressed(e);
				if ((OnDPadDown_Released != null) && !current_state.DPadDown && prev_state.DPadDown) OnDPadDown_Released(e);
				if ((OnDPadRight_Pressed != null) && current_state.DPadRight && !prev_state.DPadRight) OnDPadRight_Pressed(e);
				if ((OnDPadRight_Released != null) && !current_state.DPadRight && prev_state.DPadRight) OnDPadRight_Released(e);
				if ((OnDPadLeft_Pressed != null) && current_state.DPadLeft && !prev_state.DPadLeft) OnDPadLeft_Pressed(e);
				if ((OnDPadLeft_Released != null) && !current_state.DPadLeft && prev_state.DPadLeft) OnDPadLeft_Released(e);

				if ((OnAButton_Pressed != null) && current_state.AButton && !prev_state.AButton) OnAButton_Pressed(e);
				if ((OnAButton_Released != null) && !current_state.AButton && prev_state.AButton) OnAButton_Released(e);
				if ((OnBButton_Pressed != null) && current_state.BButton && !prev_state.BButton) OnBButton_Pressed(e);
				if ((OnBButton_Released != null) && !current_state.BButton && prev_state.BButton) OnBButton_Released(e);
				if ((OnYButton_Pressed != null) && current_state.YButton && !prev_state.YButton) OnYButton_Pressed(e);
				if ((OnYButton_Released != null) && !current_state.YButton && prev_state.YButton) OnYButton_Released();
				if ((OnXButton_Pressed != null) && current_state.XButton && !prev_state.XButton) OnXButton_Pressed(e);
				if ((OnXButton_Released != null) && !current_state.XButton && prev_state.XButton) OnXButton_Released();

				if ((OnStart_Pressed != null) && current_state.Start && !prev_state.Start) OnStart_Pressed();
				if ((OnStart_Released != null) && !current_state.Start && prev_state.Start) OnStart_Released();
				if ((OnBack_Pressed != null) && current_state.Back && !prev_state.Back) OnBack_Pressed();
				if ((OnBack_Released != null) && !current_state.Back && prev_state.Back) OnBack_Released();

                if ((OnLeftTrigger_Pressed != null) && current_state.LeftTrigger && !prev_state.LeftTrigger) OnLeftTrigger_Pressed();
                if ((OnLeftTrigger_Released != null) && !current_state.LeftTrigger && prev_state.LeftTrigger) OnLeftTrigger_Released();
                if ((OnRightTrigger_Pressed != null) && current_state.RightTrigger && !prev_state.RightTrigger) OnRightTrigger_Pressed();
                if ((OnRightTrigger_Released != null) && !current_state.RightTrigger && prev_state.LeftTrigger) OnRightTrigger_Released();

                //Hold Down Repeat Events.

                /*
                //start holddown stopwatches
                if (current_state.DPadRight && !prev_state.DPadRight) dpad_right_stopwatch_holddown.Start();
				if(current_state.DPadLeft && !prev_state.DPadLeft) dpad_left_stopwatch_holddown.Start();
                if (current_state.DPadUp && !prev_state.DPadUp) dpad_up_stopwatch_holddown.Start();
                if (current_state.DPadDown && !prev_state.DPadDown) dpad_down_stopwatch_holddown.Start();

                

                //start the repeat stopwatches
                if (dpad_right_stopwatch_holddown.IsRunning)
				{
					if ((dpad_right_stopwatch_holddown.ElapsedMilliseconds > HOLDDOWNTIME) && !dpad_right_stopwatch_repeat.IsRunning) dpad_right_stopwatch_repeat.Start();
				}
				if (dpad_left_stopwatch_holddown.IsRunning)
				{
					if ((dpad_left_stopwatch_holddown.ElapsedMilliseconds > HOLDDOWNTIME) && !dpad_left_stopwatch_repeat.IsRunning) dpad_left_stopwatch_repeat.Start();
				}
                if (dpad_up_stopwatch_holddown.IsRunning)
                {
                    if ((dpad_up_stopwatch_holddown.ElapsedMilliseconds > HOLDDOWNTIME) && !dpad_up_stopwatch_repeat.IsRunning) dpad_up_stopwatch_repeat.Start();
                }
                if (dpad_down_stopwatch_holddown.IsRunning)
                {
                    if ((dpad_down_stopwatch_holddown.ElapsedMilliseconds > HOLDDOWNTIME) && !dpad_down_stopwatch_repeat.IsRunning) dpad_down_stopwatch_repeat.Start();
                }

                //stop all stopwatches
                if (!current_state.DPadRight && prev_state.DPadRight)
				{
					dpad_right_stopwatch_holddown.Reset();
					dpad_right_stopwatch_repeat.Reset();
				}

				if (!current_state.DPadLeft && prev_state.DPadLeft)
				{
					dpad_left_stopwatch_holddown.Reset();
					dpad_left_stopwatch_repeat.Reset();
				}

                if (!current_state.DPadUp && prev_state.DPadUp)
                {
                    dpad_up_stopwatch_holddown.Reset();
                    dpad_up_stopwatch_repeat.Reset();
                }

                if (!current_state.DPadDown && prev_state.DPadDown)
                {
                    dpad_down_stopwatch_holddown.Reset();
                    dpad_down_stopwatch_repeat.Reset();
                }


                //Fire Holddown Events on Stopwatches At Correct Time ONLY if Left Trigger is Held DOWN
                    if (dpad_right_stopwatch_repeat.IsRunning && (dpad_right_stopwatch_repeat.ElapsedMilliseconds >= REPEATTIME))
                    {
                        dpad_right_stopwatch_repeat.Restart();
                        if ((OnDPadRight_Pressed != null) && e.left_trigger_pressed) OnDPadRight_Pressed(e);
                    }

                    if (dpad_left_stopwatch_repeat.IsRunning && (dpad_left_stopwatch_repeat.ElapsedMilliseconds >= REPEATTIME))
                    {
                        dpad_left_stopwatch_repeat.Restart();
                        if ((OnDPadLeft_Pressed != null) && e.left_trigger_pressed) OnDPadLeft_Pressed(e);
                    }

                    if (dpad_up_stopwatch_repeat.IsRunning && (dpad_up_stopwatch_repeat.ElapsedMilliseconds >= REPEATTIME))
                    {
                        dpad_up_stopwatch_repeat.Restart();
                        if ((OnDPadUp_Pressed != null) && e.left_trigger_pressed) OnDPadUp_Pressed(e);
                    }

                    if (dpad_down_stopwatch_repeat.IsRunning && (dpad_down_stopwatch_repeat.ElapsedMilliseconds >= REPEATTIME))
                    {
                        dpad_down_stopwatch_repeat.Restart();
                        if ((OnDPadDown_Pressed != null) && e.left_trigger_pressed) OnDPadDown_Pressed(e);
                    }
                */

                //Set Prev State to Current State
                prev_state.SetEqual(current_state);
			}
		}
		//-------------------------------------
		// Returns True If a GamePad is Connected
		// Otherwise False;
		private bool DoGamepadWatchdog()
		{
			if (gamepad != null)
			{
				if (gamepad.IsConnected) return true;
			}

			Controller[] _gamepads = { new Controller(UserIndex.One), new Controller(UserIndex.Two), new Controller(UserIndex.Three), new Controller(UserIndex.Four) };
			gamepad = null;
			foreach (Controller _gamepad in _gamepads)
			{
				if (_gamepad.IsConnected)
				{
					gamepad = _gamepad;
					if (OnControllerConnected != null) OnControllerConnected(); //Make Sure There is a Subscriber to Event. Fire Event.
					return true;
				}
			}

			if (gamepad == null)
			{
				if (OnControllerDisconnected != null) OnControllerDisconnected(); //Make Sure There is a Subscriber to Event. Fire Event.
			}
			return false;
		}

        #endregion
        //******************************************************



        //******************************************************
        #region Accessors
        public bool LeftStickUp
        {
            get
            {
                return current_state.LeftStickUp;
            }
               
        }
        public bool LeftStickDown
        {
            get
            {
                return current_state.LeftStickDown;
            }
        }
        public bool LeftStickRight
        {
            get
            {
                return current_state.LeftStickRight;
            }

        }
        public bool LeftStickLeft
        {
            get
            {
                return current_state.LeftStickLeft;
            }
        }
        public bool LeftStickPress
        {
            get
            {
                return current_state.LeftStickPress;
            }
        }
        public bool RightStickUp
        {
            get
            {
                if (current_state.RightStickUp) return true;
                return false;
            }
        }
        public bool RightStickDown
        {
            get
            {
                return current_state.RightStickDown;
            }
        }
        public bool RightStickRight
        {
            get
            {
                return current_state.RightStickRight;
            }
        }
        public bool RightStickLeft
        {
            get
            {
                return current_state.RightStickLeft;
            }
        }
        public bool RightStickPress
        {
            get
            {
                return current_state.RightStickPress;
            }
        }
        public bool LeftBumper
        {
            get
            {
                return current_state.LeftBumper;
            }
        }
        public bool RightBumper
        {
            get
            {
                return current_state.RightBumper;
            }
        }
        public bool Back
        {
            get
            {
                return current_state.Back;
            }
        }
        public bool Start
        {
            get
            {
                return current_state.Start;
            }
        }
        public bool LeftTrigger
        {
            get
            {
                return current_state.LeftTrigger;
            }
        }
        public bool RightTrigger
        {
            get
            {
                return current_state.RightTrigger;
            }

        }
        public bool DPadUp
        {
            get
            {
                return current_state.DPadUp;
            }
        }
        public bool DPadDown
        {
            get
            {
                return current_state.DPadDown;
            }
        }
        public bool DPadRight
        {
            get
            {
                return current_state.DPadRight;
            }
        }
        public bool DPadLeft
        {
            get
            {
                return current_state.DPadLeft;
            }
        }
        public bool AButton
        {
            get
            {
                return current_state.AButton;
            }
        }
        public bool BButton
        {
            get
            {
                return current_state.BButton;
            }
        }
        public bool YButton
        {
            get
            {
                return current_state.YButton;
            }
        }
        public bool XButton
        {
            get
            {
                return current_state.XButton;
            }
        }
        #endregion
        //******************************************************



    }//End Class Definition
	 //******************************************************

}//End Namespace
