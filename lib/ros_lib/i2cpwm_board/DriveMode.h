#ifndef _ROS_SERVICE_DriveMode_h
#define _ROS_SERVICE_DriveMode_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "i2cpwm_board/Position.h"

namespace i2cpwm_board
{

static const char DRIVEMODE[] = "i2cpwm_board/DriveMode";

  class DriveModeRequest : public ros::Msg
  {
    public:
      typedef const char* _mode_type;
      _mode_type mode;
      typedef float _rpm_type;
      _rpm_type rpm;
      typedef float _radius_type;
      _radius_type radius;
      typedef float _track_type;
      _track_type track;
      typedef float _scale_type;
      _scale_type scale;
      uint32_t servos_length;
      typedef i2cpwm_board::Position _servos_type;
      _servos_type st_servos;
      _servos_type * servos;

    DriveModeRequest():
      mode(""),
      rpm(0),
      radius(0),
      track(0),
      scale(0),
      servos_length(0), servos(NULL)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      uint32_t length_mode = strlen(this->mode);
      varToArr(outbuffer + offset, length_mode);
      offset += 4;
      memcpy(outbuffer + offset, this->mode, length_mode);
      offset += length_mode;
      union {
        float real;
        uint32_t base;
      } u_rpm;
      u_rpm.real = this->rpm;
      *(outbuffer + offset + 0) = (u_rpm.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_rpm.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_rpm.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_rpm.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->rpm);
      union {
        float real;
        uint32_t base;
      } u_radius;
      u_radius.real = this->radius;
      *(outbuffer + offset + 0) = (u_radius.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_radius.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_radius.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_radius.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->radius);
      union {
        float real;
        uint32_t base;
      } u_track;
      u_track.real = this->track;
      *(outbuffer + offset + 0) = (u_track.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_track.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_track.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_track.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->track);
      union {
        float real;
        uint32_t base;
      } u_scale;
      u_scale.real = this->scale;
      *(outbuffer + offset + 0) = (u_scale.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_scale.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_scale.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_scale.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->scale);
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
      uint32_t length_mode;
      arrToVar(length_mode, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_mode; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_mode-1]=0;
      this->mode = (char *)(inbuffer + offset-1);
      offset += length_mode;
      union {
        float real;
        uint32_t base;
      } u_rpm;
      u_rpm.base = 0;
      u_rpm.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_rpm.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_rpm.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_rpm.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->rpm = u_rpm.real;
      offset += sizeof(this->rpm);
      union {
        float real;
        uint32_t base;
      } u_radius;
      u_radius.base = 0;
      u_radius.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_radius.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_radius.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_radius.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->radius = u_radius.real;
      offset += sizeof(this->radius);
      union {
        float real;
        uint32_t base;
      } u_track;
      u_track.base = 0;
      u_track.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_track.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_track.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_track.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->track = u_track.real;
      offset += sizeof(this->track);
      union {
        float real;
        uint32_t base;
      } u_scale;
      u_scale.base = 0;
      u_scale.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_scale.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_scale.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_scale.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->scale = u_scale.real;
      offset += sizeof(this->scale);
      uint32_t servos_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      servos_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      servos_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      servos_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->servos_length);
      if(servos_lengthT > servos_length)
        this->servos = (i2cpwm_board::Position*)realloc(this->servos, servos_lengthT * sizeof(i2cpwm_board::Position));
      servos_length = servos_lengthT;
      for( uint32_t i = 0; i < servos_length; i++){
      offset += this->st_servos.deserialize(inbuffer + offset);
        memcpy( &(this->servos[i]), &(this->st_servos), sizeof(i2cpwm_board::Position));
      }
     return offset;
    }

    const char * getType(){ return DRIVEMODE; };
    const char * getMD5(){ return "8696ff83760d5f4079edcd8ccb545ea0"; };

  };

  class DriveModeResponse : public ros::Msg
  {
    public:
      typedef int16_t _error_type;
      _error_type error;

    DriveModeResponse():
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

    const char * getType(){ return DRIVEMODE; };
    const char * getMD5(){ return "62df06fbed46eb687891e363579eb0f0"; };

  };

  class DriveMode {
    public:
    typedef DriveModeRequest Request;
    typedef DriveModeResponse Response;
  };

}
#endif
