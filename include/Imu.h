//#include <Adafruit_Sensor.h>
//#include <Adafruit_BNO055.h>
#include <DFRobot_BMP280.h>

#include <geometry_msgs/Quaternion.h>
#include <sensor_msgs/Imu.h>
#include <SimpleKalmanFilter.h>
//SimpleKalmanFilter balanceKalmanFilter(10, 10, 0.001);
SimpleKalmanFilter gyroXKalmanFilter(10, 10, 0.001);
SimpleKalmanFilter eulerZKalmanFilter(10, 10, 0.001);
#include <ros.h>
#include <mavros_msgs/RCOut.h>
#include <sensor_msgs/NavSatFix.h>
#include <tf/transform_broadcaster.h>

/* Set the delay between fresh samples for BNO055*/
#define BNO055_SAMPLERATE_DELAY_MS (100)

//Publishers
geometry_msgs::TransformStamped tfs;
tf::TransformBroadcaster broadcaster;
// Set the connection to rosserial socket server
ros::NodeHandle nh;

void motionController();
void  getAtmosphericData();
//float calcSeaLevelPressure(float altitude, float press, int std);
float calcSeaLevelPressure(float alt, float press, float apt_temp, int std);
double sealevel(double P, double A, bool imperial = false);
//double sealevelI(double P, double A) ;

// Check I2C device address and correct line below (by default address is 0x29 or 0x28)
//                                   id, address
Adafruit_BNO055 bno = Adafruit_BNO055();

typedef DFRobot_BMP280_IIC    BMP;    // ******** use abbreviations instead of full names ********

BMP   bmp(&Wire, BMP::eSdo_low);

//#define SEA_LEVEL_PRESSURE    1024.10f   // sea level pressure-
//#define SEA_LEVEL_PRESSURE    1013.9f   // sea level pressure
double sea_level_pressure = 1015.0;
char frameid_imu[] = "/world";
char child_imu[] = "/imu_frame";

//Publishers

geometry_msgs::TransformStamped t;
sensor_msgs::Imu imu_msg;
ros::Publisher imu_stat_pub("imu_status", &imu_msg);

geometry_msgs::Vector3 bmp_msg;
ros::Publisher bmp280_pub("bmp280_status", &bmp_msg);

geometry_msgs::Vector3 rpy_msg;
ros::Publisher rpy_pub("rpy_stat", &rpy_msg);

double getRoll(double q0, double q1, double q2, double q3) {
  return -atan2(2 * (q0 * q1 + q2 * q3), 1 - 2 * (q1 * q1 + q2 * q2));
}
double getPitch(double q0, double q1, double q2, double q3) {
  return asin(2 * (q0 * q2 - q3 * q1));
}
double getYaw(double q0, double q1, double q2, double q3) {
  return -atan2(2 * (q0 * q3 + q1 * q2), 1 - 2 * (q2 * q2 + q3 * q3)) - PI / 2;
}

double getHeading(double q0, double q1, double q2, double q3, float mag_decl) {
  //double hDegrees = (getYaw(q0, q1, q2, q3) + mag_decl) * RAD_TO_DEG;
  double hDegrees = (getYaw(q0, q1, q2, q3)  * RAD_TO_DEG) + + mag_decl;
  (hDegrees < 0) ? (hDegrees += 360) : hDegrees; //degrees after Compass Adjustment could be more than 360 or less than 0
  (hDegrees > 360) ? (hDegrees -= 360) : hDegrees;
  return hDegrees;
}

float eulerZ = 0.0;




void setupImu() {
  // Check alt,hum,temp sensor
  bmp.reset();
  while (bmp.begin() != BMP::eStatusOK) {
    Serial.println("bmp begin failed");
    //printLastOperateStatus(bmp.lastOperateStatus);
    delay(2000);
  }

  /* Initialise the sensor */
  if (!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }

  bno.setExtCrystalUse(false);

  sea_level_pressure = sealevel(30.03, 89.9, true);
  Serial.println(sea_level_pressure);

  nh.getHardware()->setBaud(57600);  // Set baudrate
  nh.initNode();  // Initialize the node, use Serial1 to connect ROS
  // Let the node be able to publish the message by the publisher
//  nh.advertise(chatter);


  // Set the connection to rosserial socket server
  //nh.initNode("Serial1");

  // Start to be polite
  broadcaster.init(nh);
  nh.advertise(rpy_pub);
  nh.advertise(bmp280_pub);
  nh.advertise(imu_stat_pub);

  //int hDegrees = 0;
  //int compassAdjustment = 0;
  delay(500);

}

void motionController() {
  // IMU sampling
  /*
    Serial.println("Pitch: " + String(rpy_msg.y));//Pitch
    Serial.println("Roll: " + String(rpy_msg.x));//Roll
    Serial.println("Heading: " + String(rpy_msg.z));//Heading
    Serial.println("GX: " + String(imu_msg.angular_velocity.x));
    Serial.println("EZ: " + String(imu_msg.angular_velocity.z));

    Serial.println("EulerZ: " + String(eulerZ));
  */
  /*
    if (abs(rpy_msg.y) > TILT_LIMIT) {

    tilt_limit_exceeded = true;
    Serial.println("TILT EX");
    }
    else {
    tilt_limit_exceeded = false;
    }

    // balance controller
    //The EulerZ and or gyroscope.x()aka imu_msg.angular_velocity.x are possibility fluctuating wildly
    float filteredEulerZ = eulerZKalmanFilter.updateEstimate(eulerZ);
    float filteredGyroX = gyroXKalmanFilter.updateEstimate(imu_msg.angular_velocity.x);
    //float balanceControllerOutput = EulerZ * KP_BALANCE + imu_msg.angular_velocity.x * KD_BALANCE;
    float balanceControllerOutput = filteredEulerZ * KP_BALANCE + filteredGyroX * KD_BALANCE;
    //float balanceControllerOutput = rpy_msg.y * KP_BALANCE + imu_msg.angular_velocity.x * KD_BALANCE;
    //float filteredBalanceControllerOutput = balanceKalmanFilter.updateEstimate(balanceControllerOutput);

    // planar controllera (lateral position and steering angle)
    float positionControllerOutput = KP_POSITION * ((float)flysky_msg.y1 - PWM_CENTER);

    float steeringControllerOutput = KP_STEERING * ((float)flysky_msg.x0 - PWM_CENTER);
    float controllerOutput_right = balanceControllerOutput + positionControllerOutput + steeringControllerOutput;
    float controllerOutput_left  = balanceControllerOutput + positionControllerOutput - steeringControllerOutput;
    if (motorsActive == true) {
    //Serial.println("BC: " + String (balanceControllerOutput));
    //Serial.println("PC: " + String (positionControllerOutput));
    //Serial.println("SC: " + String (steeringControllerOutput));

    Serial.println("COR: " + String(controllerOutput_right));
    Serial.println("COL: " + String(controllerOutput_left));
    controllerOutput_right=.5;
    controllerOutput_left=0.45;
    //odrive.SetCurrent(0, MOTORDIR_0 * controllerOutput_left);
    //odrive.SetCurrent(1, MOTORDIR_1 * controllerOutput_right);
    odrive.SetPosition(0, MOTORDIR_0 * controllerOutput_left);
    odrive.SetPosition(1, MOTORDIR_1 * controllerOutput_right);

    }*/
}

/* getAtmosphericData posts temperature,pressure and altitude*/
void getAtmosphericData() {
  bmp_msg.x = (float)bmp.getTemperature();
  bmp_msg.y = (uint32_t)bmp.getPressure();
  bmp_msg.z = (float)bmp.calAltitude(sea_level_pressure, bmp_msg.y);
  bmp280_pub.publish(&bmp_msg);
}

/* Calc sea level pressure from current altitude and pressure*/
float calcSeaLevelPressure(float alt, float press, float airport_temp, int std) {
  float slp;
  if (std == 0) {
    slp = (press * exp (alt / (29.3 * (airport_temp + 273.15))));
  } else {
    slp = (press * exp (alt / (29.3 * (airport_temp + 273.15))) * 0.0295301);
  }
  return slp;
}

double sealevel(double P, double A, bool imperial)
// Given a pressure P (mb) taken at a specific altitude (meters),
// return the equivalent pressure (mb) at sea level.
// This produces pressure readings that can be used for weather measurements.
// If A and P in feet and inHg then multiply conversion factors
{
  double conv = 1;
  if (imperial) {
    P = P * 33.8639; //inHg to mbar
    A = A * .3048;  //Feet to meters
    //conv = 0.0295301;
  }

  return (P / pow(1 - (A / 44330.0), 5.255)) * conv;
}
/*
  double sealevelI(double P, double A) {
  P = P * 33.8639; //inHg to mbar
  A = A * .3048;  //Feet to meters
  return (P / pow(1 - (A / 44330.0), 5.255)) * 0.0295301;
  }
*/