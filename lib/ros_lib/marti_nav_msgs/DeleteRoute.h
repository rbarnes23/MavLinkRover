#ifndef _ROS_SERVICE_DeleteRoute_h
#define _ROS_SERVICE_DeleteRoute_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace marti_nav_msgs
{

static const char DELETEROUTE[] = "marti_nav_msgs/DeleteRoute";

  class DeleteRouteRequest : public ros::Msg
  {
    public:
      typedef const char* _guid_type;
      _guid_type guid;

    DeleteRouteRequest():
      guid("")
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      uint32_t length_guid = strlen(this->guid);
      varToArr(outbuffer + offset, length_guid);
      offset += 4;
      memcpy(outbuffer + offset, this->guid, length_guid);
      offset += length_guid;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      uint32_t length_guid;
      arrToVar(length_guid, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_guid; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_guid-1]=0;
      this->guid = (char *)(inbuffer + offset-1);
      offset += length_guid;
     return offset;
    }

    const char * getType(){ return DELETEROUTE; };
    const char * getMD5(){ return "1cfe9d879d6e044ada83c3105996467b"; };

  };

  class DeleteRouteResponse : public ros::Msg
  {
    public:
      typedef bool _success_type;
      _success_type success;
      typedef const char* _message_type;
      _message_type message;

    DeleteRouteResponse():
      success(0),
      message("")
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_success;
      u_success.real = this->success;
      *(outbuffer + offset + 0) = (u_success.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->success);
      uint32_t length_message = strlen(this->message);
      varToArr(outbuffer + offset, length_message);
      offset += 4;
      memcpy(outbuffer + offset, this->message, length_message);
      offset += length_message;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_success;
      u_success.base = 0;
      u_success.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->success = u_success.real;
      offset += sizeof(this->success);
      uint32_t length_message;
      arrToVar(length_message, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_message; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_message-1]=0;
      this->message = (char *)(inbuffer + offset-1);
      offset += length_message;
     return offset;
    }

    const char * getType(){ return DELETEROUTE; };
    const char * getMD5(){ return "937c9679a518e3a18d831e57125ea522"; };

  };

  class DeleteRoute {
    public:
    typedef DeleteRouteRequest Request;
    typedef DeleteRouteResponse Response;
  };

}
#endif
