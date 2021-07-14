#ifndef _ROS_i2cpwm_board_ServoConfig_h
#define _ROS_i2cpwm_board_ServoConfig_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace i2cpwm_board
{

  class ServoConfig : public ros::Msg
  {
    public:
      typedef int16_t _servo_type;
      _servo_type servo;
      typedef int16_t _center_type;
      _center_type center;
      typedef int16_t _range_type;
      _range_type range;
      typedef int16_t _direction_type;
      _direction_type direction;

    ServoConfig():
      servo(0),
      center(0),
      range(0),
      direction(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        int16_t real;
        uint16_t base;
      } u_servo;
      u_servo.real = this->servo;
      *(outbuffer + offset + 0) = (u_servo.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_servo.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->servo);
      union {
        int16_t real;
        uint16_t base;
      } u_center;
      u_center.real = this->center;
      *(outbuffer + offset + 0) = (u_center.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_center.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->center);
      union {
        int16_t real;
        uint16_t base;
      } u_range;
      u_range.real = this->range;
      *(outbuffer + offset + 0) = (u_range.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_range.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->range);
      union {
        int16_t real;
        uint16_t base;
      } u_direction;
      u_direction.real = this->direction;
      *(outbuffer + offset + 0) = (u_direction.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_direction.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->direction);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      union {
        int16_t real;
        uint16_t base;
      } u_servo;
      u_servo.base = 0;
      u_servo.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_servo.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->servo = u_servo.real;
      offset += sizeof(this->servo);
      union {
        int16_t real;
        uint16_t base;
      } u_center;
      u_center.base = 0;
      u_center.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_center.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->center = u_center.real;
      offset += sizeof(this->center);
      union {
        int16_t real;
        uint16_t base;
      } u_range;
      u_range.base = 0;
      u_range.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_range.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->range = u_range.real;
      offset += sizeof(this->range);
      union {
        int16_t real;
        uint16_t base;
      } u_direction;
      u_direction.base = 0;
      u_direction.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_direction.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->direction = u_direction.real;
      offset += sizeof(this->direction);
     return offset;
    }

    const char * getType(){ return "i2cpwm_board/ServoConfig"; };
    const char * getMD5(){ return "d258bdc7108a9660981de680cfa4372d"; };

  };

}
#endif
