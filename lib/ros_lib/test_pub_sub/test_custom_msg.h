#ifndef _ROS_test_pub_sub_test_custom_msg_h
#define _ROS_test_pub_sub_test_custom_msg_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace test_pub_sub
{

  class test_custom_msg : public ros::Msg
  {
    public:
      typedef const char* _data_type;
      _data_type data;
      typedef int32_t _counter_type;
      _counter_type counter;

    test_custom_msg():
      data(""),
      counter(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      uint32_t length_data = strlen(this->data);
      varToArr(outbuffer + offset, length_data);
      offset += 4;
      memcpy(outbuffer + offset, this->data, length_data);
      offset += length_data;
      union {
        int32_t real;
        uint32_t base;
      } u_counter;
      u_counter.real = this->counter;
      *(outbuffer + offset + 0) = (u_counter.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_counter.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_counter.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_counter.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->counter);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      uint32_t length_data;
      arrToVar(length_data, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_data; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_data-1]=0;
      this->data = (char *)(inbuffer + offset-1);
      offset += length_data;
      union {
        int32_t real;
        uint32_t base;
      } u_counter;
      u_counter.base = 0;
      u_counter.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_counter.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_counter.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_counter.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->counter = u_counter.real;
      offset += sizeof(this->counter);
     return offset;
    }

    const char * getType(){ return "test_pub_sub/test_custom_msg"; };
    const char * getMD5(){ return "0fa0f9ced31bb7ac9407d6fdd3a2b92b"; };

  };

}
#endif
