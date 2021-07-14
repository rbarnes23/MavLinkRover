#ifndef _ROS_image_transport_tutorial_ResizedImage_h
#define _ROS_image_transport_tutorial_ResizedImage_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "sensor_msgs/Image.h"

namespace image_transport_tutorial
{

  class ResizedImage : public ros::Msg
  {
    public:
      typedef uint32_t _original_height_type;
      _original_height_type original_height;
      typedef uint32_t _original_width_type;
      _original_width_type original_width;
      typedef sensor_msgs::Image _image_type;
      _image_type image;

    ResizedImage():
      original_height(0),
      original_width(0),
      image()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->original_height >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->original_height >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->original_height >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->original_height >> (8 * 3)) & 0xFF;
      offset += sizeof(this->original_height);
      *(outbuffer + offset + 0) = (this->original_width >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->original_width >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->original_width >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->original_width >> (8 * 3)) & 0xFF;
      offset += sizeof(this->original_width);
      offset += this->image.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      this->original_height =  ((uint32_t) (*(inbuffer + offset)));
      this->original_height |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->original_height |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->original_height |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->original_height);
      this->original_width =  ((uint32_t) (*(inbuffer + offset)));
      this->original_width |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->original_width |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->original_width |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->original_width);
      offset += this->image.deserialize(inbuffer + offset);
     return offset;
    }

    const char * getType(){ return "image_transport_tutorial/ResizedImage"; };
    const char * getMD5(){ return "37d63d52feac66587bdbae1a040ffc70"; };

  };

}
#endif
