#ifndef _ROS_SERVICE_IntValue_h
#define _ROS_SERVICE_IntValue_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace i2cpwm_board
{

static const char INTVALUE[] = "i2cpwm_board/IntValue";

  class IntValueRequest : public ros::Msg
  {
    public:
      typedef int16_t _value_type;
      _value_type value;

    IntValueRequest():
      value(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        int16_t real;
        uint16_t base;
      } u_value;
      u_value.real = this->value;
      *(outbuffer + offset + 0) = (u_value.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_value.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->value);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      union {
        int16_t real;
        uint16_t base;
      } u_value;
      u_value.base = 0;
      u_value.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_value.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->value = u_value.real;
      offset += sizeof(this->value);
     return offset;
    }

    const char * getType(){ return INTVALUE; };
    const char * getMD5(){ return "55daaea9ec64e37c8a6144d42a7265e2"; };

  };

  class IntValueResponse : public ros::Msg
  {
    public:
      typedef int16_t _error_type;
      _error_type error;

    IntValueResponse():
      error(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        int16_t real;
        uint16_t base;
      } u_error;
      u_error.real = this->error;
      *(outbuffer + offset + 0) = (u_error.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_error.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->error);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      union {
        int16_t real;
        uint16_t base;
      } u_error;
      u_error.base = 0;
      u_error.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_error.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->error = u_error.real;
      offset += sizeof(this->error);
     return offset;
    }

    const char * getType(){ return INTVALUE; };
    const char * getMD5(){ return "62df06fbed46eb687891e363579eb0f0"; };

  };

  class IntValue {
    public:
    typedef IntValueRequest Request;
    typedef IntValueResponse Response;
  };

}
#endif
