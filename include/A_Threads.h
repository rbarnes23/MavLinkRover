/*double getRoll(double q0, double q1, double q2, double q3) {
  return -atan2(2 * (q0 * q1 + q2 * q3), 1 - 2 * (q1 * q1 + q2 * q2));
  }
  double getPitch(double q0, double q1, double q2, double q3) {
  return asin(2 * (q0 * q2 - q3 * q1));
  }
  double getYaw(double q0, double q1, double q2, double q3) {
  return -atan2(2 * (q0 * q3 + q1 * q2), 1 - 2 * (q2 * q2 + q3 * q3)) - PI / 2;
  }

  double getHeading(double q0, double q1, double q2, double q3, float mag_decl) {
  double hDegrees = (getYaw(q0, q1, q2, q3) + mag_decl) * RAD_TO_DEG;
  (hDegrees < 0) ? (hDegrees += 360) : hDegrees; //degrees after Compass Adjustment could be more than 360 or less than 0
  (hDegrees > 360) ? (hDegrees -= 360) : hDegrees;
  return hDegrees;
  }
*/
void readIMU() {
  while (1) {
    //{ Threads::Scope scope(drdy_lock);
    /* Display calibration status for each sensor. */
    //displayCalStatus();
    sensors_event_t sensor;
    bno.getEvent(&sensor);
    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    imu::Vector<3> accel_linear = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
    imu::Vector<3> rot_rate = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
    imu::Quaternion quat = bno.getQuat();
    ahrs.heading = getHeading(quat.w(), quat.x(), quat.y(), quat.z(), MAGDEC);

    ahrs.roll = (float)sensor.orientation.y * deg2rad;
    ahrs.pitch = (float)sensor.orientation.z * deg2rad;
    ahrs.yaw = (float)sensor.orientation.x * deg2rad;

    ahrs.rotx = (float) rot_rate.x();
    ahrs.roty = (float) rot_rate.y();
    ahrs.rotz = (float) rot_rate.z();  //heading

    //ahrs.accelx = (float) accel_linear.x();
    //ahrs.accely = (float) accel_linear.y();
    //ahrs.accelz = (float) accel_linear.z();

    // Adjust heading to account for declination
    //ahrs.heading += DEC_ANGLE;

    //telem << "Compass Yar/dec Heading: " << yar_heading << " , " << heading << endl;

    // Correct for when signs are reversed.
    //f(ahrs.heading < 0)
    //  ahrs.heading += 360.;

    // Check for wrap due to addition of declination.
    //if(ahrs.heading > 360.)
    //  ahrs.heading -= 360.;
    getAtmosphericData();
    //telem << roll << "\t" << pitch << "\t" << yar_heading << endl;
    //telem << "Changed heading: " << (float)sensor.orientation.x << endl;
    threads.delay(BNO055_SAMPLERATE_DELAY_MS);
    //}
  }
}

void readGPS() {
  while (1) {
    //{ Threads::Scope scope(drdy_lock);
    while (1) {
      // Dispatch incoming characters
      while (Serial3.available() > 0)
        gps.encode(Serial3.read());

      if (gps.location.isUpdated())
      {

        rtk.iTOW = gps.time.age();
        rtk.fixType = 3;
        rtk.numSV = gps.satellites.value();
        rtk.pDOP = gps.hdop.value();
        rtk.lat1 = (int)(gps.location.lat() * 1e7);
        rtk.lon1 = (int)(gps.location.lng() * 1e7);
        rtk.heading = (int)(gps.course.deg() * 1e3);
        rtk.gSpeed = (int)(gps.speed.value() * 1e3); //gps.speed.value();
        rtk.hMSL = (int)(gps.altitude.value());
        /*TinyGPSCustom magneticVariation(gps, "GPRMC", 10);
          if (magneticVariation.isUpdated())
          {
          rtk.hMSL = (int)(magneticVariation.value()) * 1e3;
          }*/
      }
      threads.delay(BNO055_SAMPLERATE_DELAY_MS);
    }
    //}
  }

}

void readFSY() {
  while (1) {
    IBus.loop();
    //Initialize to zero in case it is turned off
    /*
      flysky_msg.x0 = 0;
      flysky_msg.y1 = 0;
      flysky_msg.x2 = 0;
      flysky_msg.y3 = 0;
      flysky_msg.swa4 = 0;
      flysky_msg.swb5 = 0;
      flysky_msg.swc6 = 0;
    */
    for (int i = 0; i <= 9; i++) {
      uint16_t reading =  IBus.readChannel(i);

      switch (i) {
        case 0:
          //flySky.x = reading;
          flysky_msg.x0 = reading;// - 1000;
          //flysky_msg.x0 = map(flysky_msg.x0, 0, 1000, -128, 128);
          break;
        case 1:
          //flySky.y = reading;
          flysky_msg.y1 = reading;// - 1000;
          //flysky_msg.y1 = map(flysky_msg.y1, 0, 1000, -128, 128);
          break;
        case 2:
          //flySky.x = reading;
          flysky_msg.x2 = reading;
          break;
        case 3:
          //flySky.y = reading;
          flysky_msg.y3 = reading;
          break;
        case 4:
          flysky_msg.swa4 = reading;
          if (flysky_msg.swa4 == 2000) {
            modeSwitch(1);//(int)flysky_msg.swa4 > ENGAGE_THRESHOLD && !tilt_limit_exceeded);
          } else {
            //modeSwitch(0);
          }
          break;
        case 5:
          flysky_msg.swb5 = reading;
          break;

        case 6:
          flysky_msg.swc6 = reading;

          switch (flysky_msg.swc6) {
            case 2000:  //FlySky Controller Mode
              flight_mode = 1;

              if (motorsActive != 1) {
                modeSwitch(1);
              }
              break;
            case 1500:  //Follow Me Mode - Get Remote Coordinates
              //modeSwitch(0);
              if (flysky_msg.swa4 == 2000 && flysky_msg.swb5 == 1000) {
                flight_mode = 2;//Follow Me

              } //Follow Me mode
              if (flysky_msg.swb5 == 2000 && flysky_msg.swa4 == 1000) {
                flight_mode = 3;//Pure Pursuit
              } //Pure Pursuit
              if (flysky_msg.swb5 == 1000 && flysky_msg.swa4 == 1000) {

              }
              break;
            case 1000:
              if (motorsActive == 1) {
                modeSwitch(0);
              }

              //modeSwitch(0);
              break;
            default:
              //modeSwitch(0);
              break;
          }

          break;
        case 7:
          flysky_msg.swd7 = reading;
          switch (flysky_msg.swd7) {
            case 1000:  //Manual
              //Follow from wp1
              //flight_mode = 1;
              break;
            case 2000:  //Auto
              //Go to nearest wp and start
              flight_mode = 2;
              break;
          }
          break;

        default:
          break;
      }

    }
  }
}

void readFOLLOW(){}