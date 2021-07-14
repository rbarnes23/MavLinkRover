#ifndef _ROS_i2cpwm_board_Position_h
#define _ROS_i2cpwm_board_Position_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace i2cpwm_board
{

  class Position : public ros::Msg
  {
    public:
      typedef int16_t _servo_type;
      _servo_type servo;
      typedef int16_t _position_type;
      _position_type position;

    Position():
      servo(0),
      position(0)
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
      } u_position;
      u_position.real = this->position;
      *(outbuffer + offset + 0) = (u_position.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_position.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->position);
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
      } u_position;
      u_position.base = 0;
      u_position.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_position.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->position = u_position.real;
      offset += sizeof(this->position);
     return offset;
    }

    const char * getType(){ return "i2cpwm_board/Position"; };
    const char * getMD5(){ return "46d1769fd9d3e30e5c4274bd2f84d885"; };

  };

}
#endif
