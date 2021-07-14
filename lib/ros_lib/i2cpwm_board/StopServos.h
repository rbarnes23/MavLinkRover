#ifndef _ROS_SERVICE_StopServos_h
#define _ROS_SERVICE_StopServos_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace i2cpwm_board
{

static const char STOPSERVOS[] = "i2cpwm_board/StopServos";

  class StopServosRequest : public ros::Msg
  {
    public:

    StopServosRequest()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
     return offset;
    }

    const char * getType(){ return STOPSERVOS; };
    const char * getMD5(){ return "d41d8cd98f00b204e9800998ecf8427e"; };

  };

  class StopServosResponse : public ros::Msg
  {
    public:

    StopServosResponse()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
     return offset;
    }

    const char * getType(){ return STOPSERVOS; };
    const char * getMD5(){ return "d41d8cd98f00b204e9800998ecf8427e"; };

  };

  class StopServos {
    public:
    typedef StopServosRequest Request;
    typedef StopServosResponse Response;
  };

}
#endif
