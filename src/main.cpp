#include <Arduino.h>

/********************************************************\
   Modified by:   mjs513
          Date:   May 22, 2018
      Bot Type:   Land Rover
  Motor Config:  4WH RC Car
   Discription:   MavLink Communication via MavESP8266 with the
                 Teensy 3.x.
 *********************************************************/
#define USE_TEENSY_HW_SERIAL
#include "globals.h"
#include "mavGlobals.h"
#include <TinyGPS++.h>
// The TinyGPS++ object
TinyGPSPlus gps;
#include "smavHelpers.h"
#include "FlySky.h"
#include "sMavlink.h"
//#include "motorControl.h"
#include <Wire.h>
#include "./MavLink/common/mavlink.h"

#include <inttypes.h>


#include "Streaming.h"
#include <string>

#include <TeensyThreads.h>
//#include "EEPROM.h"
//#include <PWMServo.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>


//#include "FlySky.h"
#include "Imu.h"
#include "Bno055_Config.h"
#include "A_Threads.h"

void toggleGPS();
void passThroughMode();

//PWM SETUP................
//PWMServo pwm_spd, pwm_turn;

bool once = false;

void setup() {
  cout.begin(115200);
  //telem.setRX(21);  //using alternate RX pin here
  telem.begin(115200);
  Wire.setSDA(18);
  Wire.setSCL(19);
  Serial3.begin(BaudDefault);

  cout.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  cout.println(F("by Mikal Hart"));
  cout.println();

  //==================================================
  // Initial Adafruit BNO055
  //==================================================

  //BNO055_Init();
  //setupImu();
  //==================================================
  // setupFlySky
  //==================================================

  setupFlySky();

  //============================================================
  //.println(F("\nInitialising ESP8266 WiFi/UDP Setup ..."));

  Serial.println();

  //PWM Setup
  //pwm_spd.attach(5, 1872, 1022); // pin, min/max setting
  //pwm_turn.attach(23, 1975, 1141);

  //=================================================================

  // put your setup code here, to run once:
  thGPSid = threads.addThread(readGPS);
  thIMUid = threads.addThread(readIMU);
  thFSYid = threads.addThread(readFSY);
  thFOLLOWid = threads.addThread(readFOLLOW);


  threads.setTimeSlice(0, 1);
  threads.setTimeSlice(thGPSid, 10);
  threads.setTimeSlice(thIMUid, 1);
  threads.setTimeSlice(thFSYid, 5);
  threads.setTimeSlice(thFOLLOWid, 5);

  if (threads.getState(thGPSid) == Threads::RUNNING) cout.println("GPS thread started");
  if (threads.getState(thIMUid) == Threads::RUNNING) cout.println("IMU thread started");
  if (threads.getState(thFSYid) == Threads::RUNNING) cout.println("FSY thread started");
  if (threads.getState(thFOLLOWid) == Threads::RUNNING) cout.println("Follow Path thread started");

  //Initial heartbeat
  heartbeat.base_mode = MAV_MODE_MANUAL_ARMED; //MAV_MODE_MANUAL_ARMED;
  heartbeat.custom_mode = 0;
  heartbeat.system_status = MAV_STATE_ACTIVE; //MAV_STATE_ACTIVE
  //create_waypoint();
  //mav_set_mode(AUTO);
  //send_mission_req();
  delay(500);
}


void loop() {

  if (cout.available() > 0)
  {
    int val = cout.read();    //read input commands
    switch (val)
    {
      case 'p':
        toggleGPS();
        break;
      case 'w':
        if (wpm_rcvd == 1)
          print_waypoints();
        break;
      case 'm':
        print_setMode();
        break;
      default:
        break;
    }
  }


  //kill motors if lost connection with base station (ONLY USE THIS FOR GROUND BOTS!)
  if ((millis() - heartbeatTimer_RX) > heartbeatInterval_RX)
  {
    //setup failsafe values for motor control
    motor_control = 0;
    killMotors();

    if (motorsActive == 1) {
      modeSwitch(0);
    }

  }

  /*
     Begin MAVLINK
  */
  current_arm = true;
  //mav_arm_pack(current_arm);  //Not Working

  //Pack and send heartbeat at specific interval to the GCS
  if ((millis() - heartbeatTimer_TX) > heartbeatInterval_TX)
  {
    heartbeatTimer_TX = millis();
    send_heartbeat();
    //mav_heartbeat_pack();
    send_status();  //fake data sent for now.  Seen this at .5sec
  }

  // Pack and send GPS reading at the set interval
  if ((millis() - gpsTimer) > gpsInterval)
  {
    send_gps_msg();
    gpsTimer = millis(); // reset the timer
  }

  if ((millis() - ahrsTimer) > ahrsInterval) {
    send_ahrs_msg();
    ahrsTimer = millis();
  }

  // Pack and send FlySky reading at the set interval
  if ((millis() - fsyTimer) > fsyInterval)
  {
    send_fsy_msg();

    send_servo_output_raw();
    fsyTimer = millis(); // reset the timer
  }

  // Pack and send GPS reading at the set interval
  if ((millis() - followTimer) > followInterval)
  {
    //send_gps_msg();
    followPath(0);
    followTimer = millis(); // reset the timer
  }


  handle_Messages();

}



//==================================
void toggleGPS() {
  if (passThrough_toggle == 0) {
    passThrough_toggle = 1;
    passThroughMode();
  } else {
    passThrough_toggle = 0;
  }
}


void passThroughMode() {

  int val;
  while (1) {
    if (cout.available() > 0) {
      val = cout.read();  //read telem input commands

      switch (val)
      {
        case 'n':
          passThrough_toggle = 1;
          toggleGPS();
          return;
      }
    }
    if (cout.available()) {      // If anything comes in Serial (USB),
      GPS_PORT.write(cout.read());   // read it and send it out Serial1 (pins 0 & 1)
    }

    if (GPS_PORT.available()) {     // If anything comes in Serial1 (pins 0 & 1)
      cout.write(GPS_PORT.read());   // read it and send it out Serial (USB)
    }
  }
}