#include "./MavLink/common/mavlink.h" //Put Mavlink folder in this sketch folder 
#include <ros.h>
#include <mavros_msgs/RCOut.h>
#include <sensor_msgs/NavSatFix.h>
#include <tf/transform_broadcaster.h>

int thIMUid, thGPSid, thReadMsg, thFSYid,thFOLLOWid;
#define cout Serial
elapsedMillis bootTime;

#define rad2deg 57.2957795131
#define deg2rad 0.01745329251

//Motor control setup


//=======================================

//MAGNETIC DECLINATION
float MAGDEC=-5.95055f;
float wp_radius= 1.0f;
float max_speed=2.0f;


const unsigned long connectTimeout  = 15L * 1000L, // Max time to wait for server connection
responseTimeout = 15L * 1000L; // Max time to wait for data from server


//GPS SETUP
#define GPSECHO  true
//#define GPShwSERIAL 2 // 1 = Serial1, 2 = Serial2, 3 = Serial3, 4 = Serial4 ....
#define GPS_PORT Serial3

uint32_t const BaudDefault = 9600; // default settings

struct flysky {
  int16_t x0;
  int16_t y1;
  int16_t x2;
  int16_t y3;
  uint16_t swa4;
  uint16_t swb5;
  uint16_t swc6;
  uint16_t swd7;
};
struct flysky flysky_msg;

struct LR {
  float L;
  float R;
};
struct LR lr;


//Mode 0 is not operational, 1 is FlySky, 2 if Follow Me, 3 is PurePursuit
int flight_mode = 0;
int motorsActive=0;



struct gps {
  unsigned long      iTOW;        ///< [ms], GPS time of the navigation epoch
  int32_t              ts;
  unsigned char   fixType;      ///< [ND], GNSSfix Type: 0: no fix, 1: dead reckoning only,
  ///< 2: 2D-fix, 3: 3D-fix, 4: GNSS + dead reckoning combined,
  ///< 5: time only fix
  unsigned char   numSV;      ///< [ND], Number of satellites used in Nav Solution
  double          lon1;        ///< [deg], Longitude
  double          lat1;        ///< [deg], Latitude
  double          hMSL;       ///< [m], Height above mean sea level
  double          velN;       ///< [m/s], NED north velocity
  double          velE;       ///< [m/s], NED east velocity
  double          velD;       ///< [m/s], NED down velocity
  double          gSpeed;     ///< [m/s], Ground Speed (2-D)
  double          heading;      ///< [deg], Heading of motion (2-D)
  double          pDOP;       ///< [ND], Position DOP
  double          utcTime;    //composit utc time
};
struct gps rtk;


int32_t Latitude;
int32_t Longitude;
int32_t Altitude;
int16_t Velocity;
int64_t Microseconds;

struct imuSensor {
  float           yaw;
  float           pitch;
  float           roll;
  float           heading;
  float           rotx;
  float           roty;
  float           rotz;
};
struct imuSensor ahrs;
/************************************************************************************************************/


//Helper Fields
int16_t ZControl = 0;
int16_t XControl = 0;
int DEADZONE = 0;
bool MovingBackward = false;

uint8_t motor_control = 0;
float val_spd, val_turn;

uint8_t passThrough_toggle = 0;

/**********************************************

 **********************************************/
/**Define Telemetry Port  */
#define telem Serial1

//Map floating point data
float fmap (float sensorValue, float sensorMin, float sensorMax, float outMin, float outMax)
{
  return (sensorValue - sensorMin) * (outMax - outMin) / (sensorMax - sensorMin) + outMin;
}