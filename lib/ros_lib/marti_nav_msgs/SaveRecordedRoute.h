#ifndef _ROS_SERVICE_SaveRecordedRoute_h
#define _ROS_SERVICE_SaveRecordedRoute_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace marti_nav_msgs
{

static const char SAVERECORDEDROUTE[] = "marti_nav_msgs/SaveRecordedRoute";

  class SaveRecordedRouteRequest : public ros::Msg
  {
    public:
      typedef const char* _name_type;
      _name_type name;
      typedef const char* _thumbnail_type;
      _thumbnail_type thumbnail;

    SaveRecordedRouteRequest():
      name(""),
      thumbnail("")
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      uint32_t length_name = strlen(this->name);
      varToArr(outbuffer + offset, length_name);
      offset += 4;
      memcpy(outbuffer + offset, this->name, length_name);
      offset += length_name;
      uint32_t length_thumbnail = strlen(this->thumbnail);
      varToArr(outbuffer + offset, length_thumbnail);
      offset += 4;
      memcpy(outbuffer + offset, this->thumbnail, length_thumbnail);
      offset += length_thumbnail;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      uint32_t length_name;
      arrToVar(length_name, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_name; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_name-1]=0;
      this->name = (char *)(inbuffer + offset-1);
      offset += length_name;
      uint32_t length_thumbnail;
      arrToVar(length_thumbnail, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_thumbnail; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_thumbnail-1]=0;
      this->thumbnail = (char *)(inbuffer + offset-1);
      offset += length_thumbnail;
     return offset;
    }

    const char * getType(){ return SAVERECORDEDROUTE; };
    const char * getMD5(){ return "0fbcf26e6340aaedd12defe956e94dc7"; };

  };

  class SaveRecordedRouteResponse : public ros::Msg
  {
    public:
      typedef bool _success_type;
      _success_type success;
      typedef const char* _message_type;
      _message_type message;

    SaveRecordedRouteResponse():
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

    const char * getType(){ return SAVERECORDEDROUTE; };
    const char * getMD5(){ return "937c9679a518e3a18d831e57125ea522"; };

  };

  class SaveRecordedRoute {
    public:
    typedef SaveRecordedRouteRequest Request;
    typedef SaveRecordedRouteResponse Response;
  };

}
#endif
