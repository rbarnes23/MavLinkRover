#ifndef _ROS_i2cpwm_board_ServoArray_h
#define _ROS_i2cpwm_board_ServoArray_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "i2cpwm_board/Servo.h"

namespace i2cpwm_board
{

  class ServoArray : public ros::Msg
  {
    public:
      uint32_t servos_length;
      typedef i2cpwm_board::Servo _servos_type;
      _servos_type st_servos;
      _servos_type * servos;

    ServoArray():
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
        this->servos = (i2cpwm_board::Servo*)realloc(this->servos, servos_lengthT * sizeof(i2cpwm_board::Servo));
      servos_length = servos_lengthT;
      for( uint32_t i = 0; i < servos_length; i++){
      offset += this->st_servos.deserialize(inbuffer + offset);
        memcpy( &(this->servos[i]), &(this->st_servos), sizeof(i2cpwm_board::Servo));
      }
     return offset;
    }

    const char * getType(){ return "i2cpwm_board/ServoArray"; };
    const char * getMD5(){ return "7dcfd407a2095ba27997610e5008ca84"; };

  };

}
#endif
