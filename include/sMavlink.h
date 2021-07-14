#include "motorControl.h"
void send_parameters();
void print_setMode();
void print_mission_item();
void save_waypoints();
void send_mission_ack();
void send_mission_req();
//void calculateTurn(int nJoyX, int nJoyY);

//******************************************************
//  Read Mavlink Message from GCS
//******************************************************

void handle_Messages() {
  while (telem.available() > 0) {
    uint8_t c = telem.read();

    if (mavlink_parse_char(MAVLINK_COMM_0, c, &receivedMsg, &mav_status)) {
      //print_heartbeat();
      if (receivedMsg.msgid > 0) {
        cout.print(" -> Msg ID: ");
        cout.println(receivedMsg.msgid, DEC);
        if (receivedMsg.msgid == 11) {
          if (mode.base_mode == MAV_MODE_GUIDED_ARMED || mode.base_mode ==
              MAV_MODE_MANUAL_ARMED) {
            Serial.println("SYSTEM ARMED");
            //print_setMode();
            motor_control = 1;
          } else {
            cout.print("--> New Base Mode: ");
            cout.println(mode.base_mode);
          }
        }
      }

      switch (receivedMsg.msgid)
      {
        //cout.println(receivedMsg.msgid);
        case MAVLINK_MSG_ID_SYS_STATUS:  // #1: SYS_STATUS
          {
            /* Message decoding: PRIMITIVE
                  mavlink_msg_sys_status_decode(const mavlink_message_t* msg, mavlink_sys_status_t* sys_status)
            */
            //mavlink_message_t* msg;
            mavlink_sys_status_t sys_status;
            mavlink_msg_sys_status_decode(&msg, &sys_status);
          }
          break;

        case MAVLINK_MSG_ID_MANUAL_CONTROL: // #69: Joystick data
          //[IMPORTANT: TEMPORARY FIX EXPLAINED BELOW]
          mavlink_msg_manual_control_decode(&receivedMsg, &manual_control);
          //Serial.println("Received Manual Message");
          joyStick_control();
          break;

        case MAVLINK_MSG_ID_SET_MODE:            //Get new base mode
          //killMotors();                        //Get bot ready for new mode
          mavlink_msg_set_mode_decode(&receivedMsg, &mode);
          heartbeat.base_mode = mode.base_mode;
          heartbeat.custom_mode = mode.custom_mode;
          //print_setMode();
          break;

        case MAVLINK_MSG_ID_HEARTBEAT: //#0 Get new heartbeat
          //[IMPORTANT: Mavlink C++ generator decodes the heartbeat incorrectly (parameters out of order)]
          heartbeatTimer_RX = millis(); //Reset receive timer
          //print_newHearbeat();
          break;

        case MAVLINK_MSG_ID_PARAM_REQUEST_LIST: //#21
          send_parameters();
          break;

        case MAVLINK_MSG_ID_PARAM_REQUEST_READ:
          send_parameters();
          break;

        case MAVLINK_MSG_ID_MISSION_ITEM:
          mavlink_msg_mission_item_decode(&receivedMsg, &misItem);
          print_mission_item();
          save_waypoints();
          send_mission_ack();
          break;

        case MAVLINK_MSG_ID_MISSION_COUNT:
          mavlink_msg_mission_count_decode(&receivedMsg, &misCount);
          wp_count = misCount.count;
          if (wp_count > 100 || wp_count == 0) {
            wp_count = 0;
            break;
          } else {
            send_mission_req();
          }
          break;

        case MAVLINK_MSG_ID_PARAM_SET:
          //Serial.println("GOT PARAM_SET");
          mavlink_param_set_t packet;
          mavlink_msg_param_set_decode(&paramMsg, &packet);
          //Serial.print("Received set parameter: ");
          //Serial.print(packet.param_id[0]); Serial.print(",  ");
          //Serial.println(packet.param_value);
          send_mission_ack();

          break;

        case MAVLINK_MSG_ID_PARAM_VALUE:
          cout.println("GOT PARAM_VALUE");
          mavlink_param_set_t packet1;
          mavlink_msg_param_set_decode(&paramMsg, &packet1);
          cout.print("Received set parameter: ");
          cout.print(packet1.param_id[0], HEX); Serial.print(",  ");
          cout.println(packet1.param_value);
          break;

        case MAVLINK_MSG_ID_COMMAND_LONG:
          mavlink_msg_command_long_decode(&receivedMsg, &cmd_long);
          cout.print("\tTarget Component: "); Serial.println(cmd_long.target_component);
          cout.print("\tCommand: "); Serial.println(cmd_long.command);
          cout.print("\tParam1: "); Serial.println(cmd_long.param1);
          cout.print("\tParam2: "); Serial.println(cmd_long.param2);
          cout.print("\tParam3: "); Serial.println(cmd_long.param3);
          cout.print("\tParam4: "); Serial.println(cmd_long.param4);
          cout.print("\tParam5: "); Serial.println(cmd_long.param5);
          cout.print("\tParam6: "); Serial.println(cmd_long.param6);
          cout.print("\tParam7: "); Serial.println(cmd_long.param7);
          send_mission_ack();
          break;

      }
    }
  }

}


//=========================================================
//
//=========================================================
void send_parameters() {

  for (uint8_t i = 0; i < paramCount; i++) {
    memset(bufTx, 0xFF, sizeof(bufTx));
    mavlink_msg_param_value_pack(mavlink_system.sysid, mavlink_system.compid,
                                 &paramMsg, local_param[i].param_id, local_param[i].param_value,
                                 local_param[i].param_type, paramCount, i);
    /// Copy the message to send buffer
    uint16_t len = mavlink_msg_to_send_buffer(bufTx, &paramMsg);
    //Write Message
    telem.write(bufTx, len);
  }
  /*
    memset(bufTx, 0xFF, sizeof(bufTx));
    mavlink_msg_rc_channels_override_pack(mavlink_system.sysid, mavlink_system.compid,
     &rc_overMsg, 1, 0, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX,
     UINT16_MAX, UINT16_MAX, UINT16_MAX);
    /// Copy the message to send buffer
    uint16_t len = mavlink_msg_to_send_buffer(bufTx, &rc_overMsg);
    //Write Message
    telem.write(bufTx, len);
    cout.println("Disabled RC INPUT");
  */
}

void mav_heartbeat_pack() {
  mavlink_message_t msg;
  //uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  memset(bufTx, 0xFF, sizeof(bufTx));
  /**
     @brief Pack a heartbeat message
     @param system_id ID of this system
     @param component_id ID of this component (e.g. 200 for IMU)
     @param msg The MAVLink message to compress the data into

     @param type Type of the MAV (quadrotor, helicopter, etc., up to 15 types, defined in MAV_TYPE ENUM)
     @param autopilot Autopilot type / class. defined in MAV_AUTOPILOT ENUM
     @param base_mode System mode bitfield, see MAV_MODE_FLAGS ENUM in mavlink/include/mavlink_types.h
     @param custom_mode A bitfield for use for autopilot-specific flags.
     @param system_status System status flag, see MAV_STATE ENUM
     @return length of the message in bytes (excluding serial stream start sign)
  */
  mavlink_system.sysid = MAV_TYPE_GROUND_ROVER ;
  mavlink_system.compid = MAV_COMP_ID_AUTOPILOT1;
  mavlink_msg_heartbeat_pack(mavlink_system.sysid, mavlink_system.compid, &heartbeatMsg, system_type, autopilot_type, heartbeat.base_mode, heartbeat.custom_mode, heartbeat.system_status);
  uint16_t len = mavlink_msg_to_send_buffer(bufTx, &msg);
  telem.write(bufTx, len);
  heartbeatTimer_TX = millis();
}

void send_heartbeat() {
  memset(bufTx, 0xFF, sizeof(bufTx));
  mavlink_system.sysid = MAV_TYPE_GROUND_ROVER ;
  mavlink_system.compid = MAV_COMP_ID_AUTOPILOT1;

  // Pack the message
  mavlink_msg_heartbeat_pack(mavlink_system.sysid, mavlink_system.compid, &heartbeatMsg, system_type, autopilot_type, heartbeat.base_mode, heartbeat.custom_mode, heartbeat.system_status);

  // Copy the message to send buffer
  uint16_t len = mavlink_msg_to_send_buffer(bufTx, &heartbeatMsg);

  //Write Message
  telem.write(bufTx, len);
  heartbeatTimer_TX = millis();
  //Serial.println("Heartbeat");
}

void send_global_position_setpoint() {
  memset(bufTx, 0xFF, sizeof(bufTx));

  //DOESN'T WORK mavlink_msg_set_global_position_setpoint_int_pack(mavlink_system.sysid, mavlink_system.compid, &globalSetpointMsg, 1, 225500000, -824545000, 333000, 360);

  // Copy the message to send buffer
  uint16_t len = mavlink_msg_to_send_buffer(bufTx, &globalSetpointMsg);

  //Write Message
  telem.write(bufTx, len);
  //heartbeatTimer_TX = millis();
}

void send_servo_output_raw() {
  memset(bufTx, 0xFF, sizeof(bufTx));
  //  mavlink_msg_servo_output_raw_pack(mavlink_system.sysid, mavlink_system.compid, &servoOutputMsg,
  //                   bootTime,0, uint16_t servo1_raw, uint16_t servo2_raw, uint16_t servo3_raw, uint16_t servo4_raw, uint16_t servo5_raw, uint16_t servo6_raw, uint16_t servo7_raw, uint16_t servo8_raw);

  mavlink_msg_servo_output_raw_pack(mavlink_system.sysid, mavlink_system.compid, &servoOutputMsg,
                                    bootTime, 4, (int)abs(lr.L * 1e3), (int)abs(lr.R * 1e3), 0, 0, 0, 0, 0, 0);


  /// Copy the message to send buffer
  uint16_t len = mavlink_msg_to_send_buffer(bufTx, &servoOutputMsg);
  //Write Message
  telem.write(bufTx, len);
}
int no=0;
void send_gps_msg() {
  // As long as we have a fix, proceed with packing and sending GPS data
  //    if(uBloxData.fixType > 2)
  //    {
  memset(bufTx, 0xFF, sizeof(bufTx));
  mavlink_msg_gps_raw_int_pack(mavlink_system.sysid, mavlink_system.compid, &gpsMsg,
                               bootTime, rtk.fixType, (int) rtk.lat1, (int)rtk.lon1, (int)rtk.hMSL, rtk.pDOP,
                               30, rtk.gSpeed, rtk.heading, rtk.numSV);

  /// Copy the message to send buffer
  uint16_t len = mavlink_msg_to_send_buffer(bufTx, &gpsMsg);

  //cout.println("LAT: " + String((int)rtk.lat1) + " LON: " + String((int)rtk.lon1) + " ALT: " + String((int)rtk.hMSL));

//  no++;
//  if (no ==10) {
//    followPath(0);
//    no=0;
//  }


  //Write Message
  telem.write(bufTx, len);

  //     }
}

void send_ahrs_msg() {
  memset(bufTx, 0xFF, sizeof(bufTx));
  mavlink_msg_attitude_pack(mavlink_system.sysid, mavlink_system.compid, &ahrsMsg,
                            bootTime, ahrs.roll, ahrs.pitch, ahrs.yaw,
                            ahrs.rotx, ahrs.roty, ahrs.heading);
  //  cout.println("ROLL: " + String(ahrs.roll) + "PITCH: " + String(ahrs.pitch) + "YAW: " + String(ahrs.yaw)+ " HEADING: " + String(ahrs.heading));
  //  cout.println("Temp: " + String(bmp_msg.x)+ "Press: " + String(bmp_msg.y) + "SeaPress: " + String(bmp_msg.z));
  /// Copy the message to send buffer
  uint16_t len = mavlink_msg_to_send_buffer(bufTx, &ahrsMsg);

  //Write Message
  telem.write(bufTx, len);
}

void send_fsy_msg() {
  memset(bufTx, 0xFF, sizeof(bufTx));
  mavlink_msg_rc_channels_raw_pack(mavlink_system.sysid, mavlink_system.compid, &fsyMsg,
                                   bootTime, 0, flysky_msg.x0, flysky_msg.y1, flysky_msg.x2, flysky_msg.y3,
                                   flysky_msg.swa4, flysky_msg.swb5, flysky_msg.swc6, flysky_msg.swd7, 100);

  calculateTurn(flysky_msg.x0, flysky_msg.y1);
  //cout.println("X0: " + String(flysky_msg.x0) + " Y1: " + String(flysky_msg.y1));
  //cout.println("L: " + String(lr.L) + " R: " + String(lr.R));

  /// Copy the message to send buffer
  uint16_t len = mavlink_msg_to_send_buffer(bufTx, &fsyMsg);

  //Write Message
  telem.write(bufTx, len);
}


void send_status() {
  memset(bufTx, 0xFF, sizeof(bufTx));
  mavlink_msg_sys_status_pack(mavlink_system.sysid, mavlink_system.compid, &statMsg,
                              MAVLINK_SENSOR_PRESENT_DEFAULT, MAVLINK_SENSOR_PRESENT_DEFAULT,
                              MAVLINK_SENSOR_PRESENT_DEFAULT, 500, 7400, 330, 50, 0, 0, 0, 0, 0, 0);

  /// Copy the message to send buffer
  uint16_t len = mavlink_msg_to_send_buffer(bufTx, &statMsg);

  //Write Message
  telem.write(bufTx, len);
}

void send_mission_ack() {
  memset(bufTx, 0xFF, sizeof(bufTx));
  mavlink_msg_mission_ack_pack(mavlink_system.sysid, mavlink_system.compid,
                               &msg, mavlink_system.sysid, mavlink_system.compid, MAV_RESULT_ACCEPTED);
  /// Copy the message to send buffer
  uint16_t len = mavlink_msg_to_send_buffer(bufTx, &msg);
  //Write Message
  telem.write(bufTx, len);
}

void send_mission_req() {
  for (uint16_t i = 0; i < wp_count; i++) {
    memset(bufTx, 0xFF, sizeof(bufTx));
    mavlink_msg_mission_request_pack(mavlink_system.sysid, mavlink_system.compid,
                                     &misReq, mavlink_system.sysid, mavlink_system.compid, i);
    /// Copy the message to send buffer
    uint16_t len = mavlink_msg_to_send_buffer(bufTx, &misReq);
    //Write Message
    telem.write(bufTx, len);
  }
}

void mav_arm_pack(boolean state) {
  //mavlink_message_t msg;
  //uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  memset(bufTx, 0xFF, sizeof(bufTx));

  //Arm the drone
  //400 stands for MAV_CMD_COMPONENT_ARM_DISARM
  // 1 an 8'th argument is for ARM (0 for DISARM)
  if (state) {
    //ARM
    mavlink_msg_command_long_pack(0xFF, 0xBE, &msg, 1, 1, 400, 1, 1.0, 0, 0, 0, 0, 0, 0);
  } else {
    //DISARM
    mavlink_msg_command_long_pack(0xFF, 0xBE, &msg, 1, 1, 400, 1, 0.0, 0, 0, 0, 0, 0, 0);
  }
  uint16_t len = mavlink_msg_to_send_buffer(bufTx, &msg);
  telem.write(bufTx, len);
}

void create_home() {
  //Step 3 of uploading a new waypoint (send HOME coordinates)
  //uint8_t _system_id = 255; // system id of sending station. 255 is Ground control software
  //uint8_t _component_id = 20; // component id of sending station 2 works fine
  uint8_t _target_system = 10; // Pixhawk id
  uint8_t _target_component = 0; // Pixhawk component id, 0 = all (seems to work fine)

  uint16_t seq = 0; // Sequence number
  uint8_t frame = 0; // Set target frame to global default
  uint16_t command = MAV_CMD_NAV_WAYPOINT; // Can be substituted with other commands https://mavlink.io/en/services/mission.html#mission_types
  uint8_t current = 0; // Guided mode waypoint
  uint8_t autocontinue = 0; // Always 0
  float param1 = 0; // Loiter time
  float param2 = 1; // Acceptable range from target - radius in meters
  float param3 = 0; // Pass through waypoint
  float param4 = 0; // Desired yaw angle
  float x = 28.259476; // Latitude - degrees
  float y = -82.2845242; // Longitude - degrees
  float z = 35.900; // Altitude - meters

  // Initialize the required buffers
  mavlink_message_t msg;
  //uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  memset(bufTx, 0xFF, sizeof(bufTx));
  // Pack the message
  mavlink_msg_mission_item_pack(mavlink_system.sysid, mavlink_system.compid, &msg, _target_system, _target_component, seq, frame, command, current, autocontinue, param1, param2, param3, param4, x, y, z);

  //uint16_t mavlink_msg_mission_item_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, uint8_t target_system, uint8_t target_component, uint16_t seq, uint8_t frame, uint16_t command, uint8_t current, uint8_t autocontinue, float param1, float param2, float param3, float param4, float x, float y, float z

  // Copy the message to the send buffer
  uint16_t len = mavlink_msg_to_send_buffer(bufTx, &msg);

  // Send the message (.write sends as bytes)
  telem.write(bufTx, len);

}

void create_waypoint() {
  //Step 3 continuation of uploading a new waypoint (send 1st coordinates)
  //uint8_t _system_id = 255; // system id of sending station. 255 is Ground control software
  //uint8_t _component_id = 20; // component id of sending station 2 works fine
  uint8_t _target_system = 10; // Pixhawk id
  uint8_t _target_component = 0; // Pixhawk component id, 0 = all (seems to work fine)

  uint16_t seq = 1; // Sequence number
  uint8_t frame = 0; // Set target frame to global default
  uint16_t command = MAV_CMD_NAV_WAYPOINT; // Can be substituted with other commands https://mavlink.io/en/services/mission.html#mission_types
  uint8_t current = 1; // Guided mode waypoint
  uint8_t autocontinue = 0; // Always 0
  float param1 = 0; // Loiter time
  float param2 = 0; // Acceptable range from target - radius in meters
  float param3 = 0; // Pass through waypoint
  float param4 = 0; // Desired yaw angle
  float x = 282594760;//15.464217; // Latitude - degrees
  float y = -822845242; //-11.280222; // Longitude - degrees
  float z = 200; // Altitude - meters

  // Initialize the required buffers
  mavlink_message_t msg;
  //uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  memset(bufTx, 0xFF, sizeof(bufTx));
  // Pack the message
  mavlink_msg_mission_item_pack(mavlink_system.sysid, mavlink_system.compid, &msg, _target_system, _target_component, seq, frame, command, current, autocontinue, param1, param2, param3, param4, x, y, z);

  //uint16_t mavlink_msg_mission_item_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, uint8_t target_system, uint8_t target_component, uint16_t seq, uint8_t frame, uint16_t command, uint8_t current, uint8_t autocontinue, float param1, float param2, float param3, float param4, float x, float y, float z

  // Copy the message to the send buffer
  uint16_t len = mavlink_msg_to_send_buffer(bufTx, &msg);

  // Send the message (.write sends as bytes)
  telem.write(bufTx, len);
}

void mav_set_mode(String value) {
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];

  value.trim();

  //SET_MODE
  //Works with 1 at 4'th parameter
  if (value == STABILIZE) {
    mavlink_msg_set_mode_pack(0xFF, 0xBE, &msg, 1, 209, 0);
  }

  if (value == ALTHOLD) {
    mavlink_msg_set_mode_pack(0xFF, 0xBE, &msg, 1, 209, 2);
  }

  if (value == LOITER) {
    mavlink_msg_set_mode_pack(0xFF, 0xBE, &msg, 1, 209, 5);
  }

  if (value == AUTO) {
    mavlink_msg_set_mode_pack(0xFF, 0xBE, &msg, 1, 209, 3);
  }

  if (value == CIRCLE) {
    mavlink_msg_set_mode_pack(0xFF, 0xBE, &msg, 1, 209, 7);
  }

  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  telem.write(buf, len);
}

void mav_set_mode() {
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];

  //Set flight mode 'Stabilize'
  mavlink_msg_set_mode_pack(0xFF, 0xBE, &msg, 1, 209, 0);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  telem.write(buf, len);
}