#ifndef _ROS_ros_arduino_msgs_ArduinoConstants_h
#define _ROS_ros_arduino_msgs_ArduinoConstants_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace ros_arduino_msgs
{

  class ArduinoConstants : public ros::Msg
  {
    public:
      enum { LOW = 0 };
      enum { HIGH = 1 };
      enum { INPUT = 0 };
      enum { OUTPUT = 1 };

    ArduinoConstants()
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

    const char * getType(){ return "ros_arduino_msgs/ArduinoConstants"; };
    const char * getMD5(){ return "6ca092be59914d9e8dd11612f0a5c895"; };

  };

}
#endif
