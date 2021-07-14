#ifndef _ROS_SERVICE_ServosConfig_h
#define _ROS_SERVICE_ServosConfig_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "i2cpwm_board/ServoConfig.h"

namespace i2cpwm_board
{

static const char SERVOSCONFIG[] = "i2cpwm_board/ServosConfig";

  class ServosConfigRequest : public ros::Msg
  {
    public:
      uint32_t servos_length;
      typedef i2cpwm_board::ServoConfig _servos_type;
      _servos_type st_servos;
      _servos_type * servos;

    ServosConfigRequest():
      servos_length(0), servos(NULL)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->servos_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->servos_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->servos_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->servos_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->servos_length);
      for( uint32_t i = 0; i < servos_length; i++){
      offset += this->servos[i].serialize(outbuffer + offset);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      uint32_t servos_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      servos_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      servos_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      servos_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->servos_length);
      if(servos_lengthT > servos_length)
        this->servos = (i2cpwm_board::ServoConfig*)realloc(this->servos, servos_lengthT * sizeof(i2cpwm_board::ServoConfig));
      servos_length = servos_lengthT;
      for( uint32_t i = 0; i < servos_length; i++){
      offset += this->st_servos.deserialize(inbuffer + offset);
        memcpy( &(this->servos[i]), &(this->st_servos), sizeof(i2cpwm_board::ServoConfig));
      }
     return offset;
    }

    const char * getType(){ return SERVOSCONFIG; };
    const char * getMD5(){ return "8f9c43713b493f265aec1e926dd292b8"; };

  };

  class ServosConfigResponse : public ros::Msg
  {
    public:
      typedef int16_t _error_type;
      _error_type error;

    ServosConfigResponse():
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

    const char * getType(){ return SERVOSCONFIG; };
    const char * getMD5(){ return "62df06fbed46eb687891e363579eb0f0"; };

  };

  class ServosConfig {
    public:
    typedef ServosConfigRequest Request;
    typedef ServosConfigResponse Response;
  };

}
#endif
