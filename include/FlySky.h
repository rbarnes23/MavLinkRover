#include "FlySkyIBus.h"
#include <ODriveArduino.h>



ODriveArduino odrive(Serial4); // instantiate ODrive


void calculateTurn(int nJoyX, int nJoyY);
void modeSwitch(int mode_desired);


void setupFlySky() {
  Serial4.begin(115200);
  IBus.begin(Serial5);
  delay(2000);
  modeSwitch(1);
}




void modeSwitch(int mode_desired) {
  //bool success = odrive.clear_errors(0, 0);
  //mode_desired = 1; //For test only
  if (mode_desired == motorsActive) {
  }
  else {
    if (mode_desired == 1) {
      int requested_state = ODriveArduino::AXIS_STATE_CLOSED_LOOP_CONTROL;
      Serial.println("Engaging motors");
      odrive.run_state(0, requested_state, false);
      odrive.run_state(1, requested_state, false);
    }
    else if (mode_desired == 0) {
      int requested_state = ODriveArduino::AXIS_STATE_IDLE;
      //Serial.println("Disengaging motors");
      odrive.run_state(0, requested_state, false);
      odrive.run_state(1, requested_state, false);
    }
    else {
      Serial.println("Invalid mode selection");
    }
    motorsActive = mode_desired;
    delay(10);
  }
  return;
}

void calculateTurn(int16_t nJoyX, int16_t nJoyY) {
  // Differential Steering Joystick Algorithm
  // ========================================
  //   by Calvin Hass
  //   https://www.impulseadventure.com/elec/robot-differential-steering.html
  //   https://github.com/ImpulseAdventure/
  //
  // Converts a single dual-axis joystick into a differential
  // drive motor control, with support for both drive, turn
  // and pivot operations.
  //

  //Using inputs from Flysky Controller range of 1000 to 2000
  nJoyX = nJoyX - 1000;
  nJoyX = map(nJoyX, 0, 1000, -128, 128);
  nJoyY = nJoyY - 1000;
  nJoyY= map(nJoyY, 0, 1000, -128, 128);

  // INPUTS
  //int     nJoyX;              // Joystick X input                     (-128..+127)
  //int     nJoyY;              // Joystick Y input                     (-128..+127)

  // OUTPUTS
  int     nMotMixL;           // Motor (left)  mixed output           (-128..+127)
  int     nMotMixR;           // Motor (right) mixed output           (-128..+127)

  // CONFIG
  // - fPivYLimt  : The threshold at which the pivot action starts
  //                This threshold is measured in units on the Y-axis
  //                away from the X-axis (Y=0). A greater value will assign
  //                more of the joystick's range to pivot actions.
  //                Allowable range: (0..+127)
  float fPivYLimit = 32.0;

  // TEMP VARIABLES
  float   nMotPremixL;    // Motor (left)  premixed output        (-128..+127)
  float   nMotPremixR;    // Motor (right) premixed output        (-128..+127)
  int     nPivSpeed;      // Pivot Speed                          (-128..+127)
  float   fPivScale;      // Balance scale b/w drive and pivot    (   0..1   )


  // Calculate Drive Turn output due to Joystick X input
  if (nJoyY >= 0) {
    // Forward
    nMotPremixL = (nJoyX >= 0) ? 127.0 : (127.0 + nJoyX);
    nMotPremixR = (nJoyX >= 0) ? (127.0 - nJoyX) : 127.0;
  } else {
    // Reverse
    nMotPremixL = (nJoyX >= 0) ? (127.0 - nJoyX) : 127.0;
    nMotPremixR = (nJoyX >= 0) ? 127.0 : (127.0 + nJoyX);
  }

  // Scale Drive output due to Joystick Y input (throttle)
  nMotPremixL = nMotPremixL * nJoyY / 128.0;
  nMotPremixR = nMotPremixR * nJoyY / 128.0;

  // Now calculate pivot amount
  // - Strength of pivot (nPivSpeed) based on Joystick X input
  // - Blending of pivot vs drive (fPivScale) based on Joystick Y input
  nPivSpeed = nJoyX;
  fPivScale = (abs(nJoyY) > fPivYLimit) ? 0.0 : (1.0 - abs(nJoyY) / fPivYLimit);

  // Calculate final mix of Drive and Pivot
  nMotMixL = (1.0 - fPivScale) * nMotPremixL + fPivScale * ( nPivSpeed);
  nMotMixR = (1.0 - fPivScale) * nMotPremixR + fPivScale * (-nPivSpeed);

  // Convert to Motor PWM range
  float deadZone = .25;
  lr.L = fmap(nMotMixL, -128, 127, -3.0, 3.0);
  lr.R = fmap(nMotMixR, -128, 127, -3.0, 3.0);
  ((lr.L > 0 and lr.L < deadZone) || (lr.L < 0 && lr.L > (-1 * deadZone))) ? lr.L = 0 : lr.L;
  ((lr.R > 0 and lr.R < deadZone) || (lr.R < 0 && lr.R > (-1 * deadZone))) ? lr.R = 0 : lr.R;
  odrive.SetVelocity(1, -lr.L);
  odrive.SetVelocity(0, lr.R);
}
/*
    //Change this so it takes Current latlng and desired latlng and then creates a vector of magnitude and direction
    void calculateWheelVelocity(PVector current, PVector desired) {
      double Kp = 1, Ki = 1, Kd = 1;
      //Define Variables we'll be connecting to
      double Setpoint, Input, Output;
      desired.set(10, 180);
      float R = .127; //10 inch tires in meters
      float V = desired.getX();//Magnitude
      float O = radians(desired.getY());//current.angleRad(desired);
      //float O = -1 * desired; //PID This
      float L = .6604;
      float V_r = 2 * V + (O * L) / (2 * R);
      float V_l = 2 * V - (O * L) / (2 * R);
      PVector master;
      float e = atan2(sin(master.angleBetween(desired, current)), cos(master.angleBetween(desired, current)));

      printf( "VlM is %f and VrM is %f using speed %f and direction of %f\n", V_l, V_r, V, O);

      Setpoint = e;
      Input = e;
      PID errorPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);
      //PID errorPID(e, Output, e, Kp, Ki, Kd, DIRECT);
      //PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);
      //turn the PID on
      //errorPID.SetMode(AUTOMATIC);
      //errorPID.Compute();

      Serial.println("ERROR: " + String(e, 6));
      V_r = map(V_r, -PI, PI, 0, 255);
      V_l = map(V_l, -PI, PI, 0, 255);
      //V_r = map(V_r, 0, 10, 0, 255);
      //V_l = map(V_l, 0, 10, 0, 255);

      //Serial.println("Vlb:" + String(V_l) + '\t' + "Vr:" + String(V_r));
      printf( "VlMb is %f and VrM is %f using speed %f and direction of %f\n", V_l, V_r, V, O);

      V_r = constrain(V_r, 0, 255);
      V_l = constrain(V_l, 0, 255);

      //if (abs(V_l) - abs(V_r) < 1) {
       // V_r = V_l;
        //}

      char turn;
      if (V_l > V_r)
      {
        turn = 'R';
      } else if (V_l < V_r) {
        turn = 'L';
      } else {
        turn = 'N';
      };
      Serial.print("WHEEL SPEEDS ");
      Serial.println("Vl:" + String(V_l) + '\t' + "Vr:" + String(V_r) + '\t' + "TURN " + String(turn));

      float angle = current.angleBetween(current, desired); // angle is 90
      Serial.print("Angle In Degrees ");
      Serial.println(String(angle, 0) + '\t');
    };
*/