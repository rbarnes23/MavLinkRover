#ifndef _ROS_i2cpwm_board_Servo_h
#define _ROS_i2cpwm_board_Servo_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace i2cpwm_board
{

  class Servo : public ros::Msg
  {
    public:
      typedef int16_t _servo_type;
      _servo_type servo;
      typedef float _value_type;
      _value_type value;

    Servo():
      servo(0),
      value(0)
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
        float real;
        uint32_t base;
      } u_value;
      u_value.real = this->value;
      *(outbuffer + offset + 0) = (u_value.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_value.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_value.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_value.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->value);
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
        float real;
        uint32_t base;
      } u_value;
      u_value.base = 0;
      u_value.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_value.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_value.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_value.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->value = u_value.real;
      offset += sizeof(this->value);
     return offset;
    }

    const char * getType(){ return "i2cpwm_board/Servo"; };
    const char * getMD5(){ return "5e89689ba58524f73da7d375fa49d8a1"; };

  };

}
#endif
