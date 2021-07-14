#ifndef _ROS_rosserial_msgs_FlySky_h
#define _ROS_rosserial_msgs_FlySky_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace rosserial_msgs
{

  class FlySky : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef uint16_t _x0_type;
      _x0_type x0;
      typedef uint16_t _y1_type;
      _y1_type y1;
      typedef uint16_t _x2_type;
      _x2_type x2;
      typedef uint16_t _y3_type;
      _y3_type y3;
      typedef uint16_t _swa4_type;
      _swa4_type swa4;
      typedef uint16_t _swb5_type;
      _swb5_type swb5;
      typedef uint32_t _swc6_type;
      _swc6_type swc6;

    FlySky():
      header(),
      x0(0),
      y1(0),
      x2(0),
      y3(0),
      swa4(0),
      swb5(0),
      swc6(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->x0 >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->x0 >> (8 * 1)) & 0xFF;
      offset += sizeof(this->x0);
      *(outbuffer + offset + 0) = (this->y1 >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->y1 >> (8 * 1)) & 0xFF;
      offset += sizeof(this->y1);
      *(outbuffer + offset + 0) = (this->x2 >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->x2 >> (8 * 1)) & 0xFF;
      offset += sizeof(this->x2);
      *(outbuffer + offset + 0) = (this->y3 >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->y3 >> (8 * 1)) & 0xFF;
      offset += sizeof(this->y3);
      *(outbuffer + offset + 0) = (this->swa4 >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->swa4 >> (8 * 1)) & 0xFF;
      offset += sizeof(this->swa4);
      *(outbuffer + offset + 0) = (this->swb5 >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->swb5 >> (8 * 1)) & 0xFF;
      offset += sizeof(this->swb5);
      *(outbuffer + offset + 0) = (this->swc6 >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->swc6 >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->swc6 >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->swc6 >> (8 * 3)) & 0xFF;
      offset += sizeof(this->swc6);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      this->x0 =  ((uint16_t) (*(inbuffer + offset)));
      this->x0 |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->x0);
      this->y1 =  ((uint16_t) (*(inbuffer + offset)));
      this->y1 |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->y1);
      this->x2 =  ((uint16_t) (*(inbuffer + offset)));
      this->x2 |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->x2);
      this->y3 =  ((uint16_t) (*(inbuffer + offset)));
      this->y3 |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->y3);
      this->swa4 =  ((uint16_t) (*(inbuffer + offset)));
      this->swa4 |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->swa4);
      this->swb5 =  ((uint16_t) (*(inbuffer + offset)));
      this->swb5 |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->swb5);
      this->swc6 =  ((uint32_t) (*(inbuffer + offset)));
      this->swc6 |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->swc6 |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->swc6 |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->swc6);
     return offset;
    }

    const char * getType(){ return "rosserial_msgs/FlySky"; };
    const char * getMD5(){ return "a39e164366c297a948c90ae01358d7d8"; };

  };

}
#endif
