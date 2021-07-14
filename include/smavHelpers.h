#include <VectorXf.h>

void goToNextWP(uint8_t wp_no) ;

///*****************************************************************************
/// Name: printXY
/// Type: Helper function
/// Parameters:  X and Y coordinates to print
/// Description: Print X and Y coordinates to the serial monitor. Used for
///              debugging (especially with the manual control messages).
///*****************************************************************************

void printXY(float X, float Y)
{
  cout.print("(");
  cout.print(X, DEC);
  cout.print(",");
  cout.print(Y, DEC);
  cout.print(")");
  cout.print("\n");
}

void printXYZV(float X, float Y, float Z, float V)
{
  cout.print("(");
  cout.print(X, DEC);
  cout.print(",");
  cout.print(Y, DEC);
  cout.print(",");
  cout.print(Z, DEC);
  cout.print(",");
  cout.print(V, DEC);
  cout.print(")");
  cout.print("\n");
}


///*****************************************************************************
/// Name:
/// Type:
/// Parameters:  X
/// Description:
///
///*****************************************************************************

void print_heartbeat() {
  cout.print("  Sys ID: ");   cout.print(receivedMsg.sysid, DEC);
  cout.print("  Comp ID: ");  cout.print(receivedMsg.compid, DEC);
  cout.print("  Len ID: ");   cout.print(receivedMsg.len, DEC);
  cout.print("  Msg ID: ");   cout.print(receivedMsg.msgid, DEC);
  cout.print("\n");
}

void print_setMode() {
  cout.print("Target System: ");   cout.print(mode.target_system);
  cout.print("\n");
  cout.print("New Base Mode: ");   cout.print(mode.base_mode);  cout.print("\n");
  cout.print("New Custom Mode: "); cout.print(mode.custom_mode);
  cout.print("\n");
}

void print_newHearbeat() {
  //           mavlink_msg_heartbeat_decode(&receivedMsg, &heartbeat);
  //           Serial.println("heartbeat Received");
  //           Serial.print("New Custom Mode: ");    Serial.print(heartbeat.custom_mode);
  //           Serial.print("\n");
  //           Serial.print("New Type: ");           Serial.print(heartbeat.type);
  //           Serial.print("\n");
  //           Serial.print("New Autopilot: ");      Serial.print(heartbeat.autopilot);
  //           Serial.print("\n");
  //           Serial.print("New Base Mode: ");      Serial.print(heartbeat.base_mode);
  //           Serial.print("\n");
  //           Serial.print("New System Status: ");  Serial.print(heartbeat.system_status);
  //           Serial.print("\n");
  //           Serial.print("New Mavlink Version: ");Serial.print(heartbeat.mavlink_version);
  //           Serial.print("\n");
}

void print_mission_item() {
  cout.println("Mission item:");
  cout.print("  TSys ID: ");   Serial.print(misItem.target_system, DEC);
  cout.print("  TComp ID: ");  Serial.print(misItem.target_component, DEC);
  cout.print("  Seq: ");   Serial.print(misItem.seq, DEC);
  cout.print("  Frame: ");   Serial.println(misItem.frame, DEC);

  cout.print("  Command: ");   cout.print(misItem.command, DEC);
  cout.print("  Current: ");  cout.print(misItem.current, DEC);
  cout.print("  Autocontinue: ");   cout.println(misItem.autocontinue, DEC);
  cout.print("  Param1: ");   cout.print(misItem.param1, 8);
  cout.print("  Param2: ");   cout.print(misItem.param2, 8);
  cout.print("  Param3: ");   cout.print(misItem.param3, 8);
  cout.print("  Param4: ");   cout.println(misItem.param4, 8);

  cout.print("  X: ");   cout.print(misItem.x, 8);
  cout.print("  Y: ");   cout.print(misItem.y, 8);
  cout.print("  Z: ");   cout.println(misItem.z, 8);
}

void save_waypoints() {
  wpm[misItem.seq].lat = misItem.x;
  wpm[misItem.seq].lon = misItem.y;
  wpm[misItem.seq].alt = misItem.z;
  wpm[misItem.seq].cmd = misItem.command;
  wpm[misItem.seq].auto_cont = misItem.autocontinue;
  wpm[misItem.seq].wpcount = wp_count;
  if (misItem.seq == (wp_count - 1)) wpm_rcvd = 1;
}

void print_waypoints() {
  cout.println("WAYPOINTS RECEIVED:");
  for (uint8_t i = 0; i < wpm[0].wpcount; i++) {
    cout.print(i); cout.print('\t');
    cout.print(wpm[i].wpcount); cout.print('\t');
    cout.print(wpm[i].lat, 6); cout.print('\t');
    cout.print(wpm[i].lon, 6); cout.print('\t');
    cout.print(wpm[i].alt, 2); cout.print('\t');
    cout.print(wpm[i].cmd, 2); cout.print('\t');
    cout.print(wpm[i].auto_cont); cout.println();
    if (wpm[i].cmd == 16) {
      goToNextWP(i);
    }
  }
}



double distanceBetween(Vec2f v1, Vec2f v2)
{
  // returns distance in meters between two positions, both specified
  // as signed decimal-degrees latitude and longitude. Uses great-circle
  // distance computation for hypothetical sphere of radius 6372795 meters.
  // Because Earth is no exact sphere, rounding errors may be up to 0.5%.
  // Courtesy of Maarten Lamers
  double delta = radians(v1.y - v2.y);
  double sdlong = sin(delta);
  double cdlong = cos(delta);
  v1.x = radians(v1.x);
  v2.x = radians(v2.x);
  double slat1 = sin(v1.x);
  double clat1 = cos(v1.x);
  double slat2 = sin(v2.x);
  double clat2 = cos(v2.x);
  delta = (clat1 * slat2) - (slat1 * clat2 * cdlong);
  delta = sq(delta);
  delta += sq(clat2 * sdlong);
  delta = sqrt(delta);
  double denom = (slat1 * slat2) + (clat1 * clat2 * cdlong);
  delta = atan2(delta, denom);
  return delta * 6372795;
}

double courseTo(Vec2f v1, Vec2f v2)
{
  // returns course in degrees (North=0, West=270) from position 1 to position 2,
  // both specified as signed decimal-degrees latitude and longitude.
  // Because Earth is no exact sphere, calculated course may be off by a tiny fraction.
  // Courtesy of Maarten Lamers
  double dlon = radians(v2.y - v1.y);
  v1.x = radians(v1.x);
  v2.x = radians(v2.x);
  double a1 = sin(dlon) * cos(v2.x);
  double a2 = sin(v1.x) * cos(v2.x) * cos(dlon);
  a2 = cos(v1.x) * sin(v2.x) - a2;
  a2 = atan2(a1, a2);
  if (a2 < 0.0)
  {
    a2 += TWO_PI;
  }
  return degrees(a2);
}

Vec3f getNormalPoint(Vec3f p, Vec3f a, Vec3f b) {

  // Vector from a to p
  Vec3f ap = p - a;
  // Vector from a to b
  Vec3f ab = b - a;

  ab.normalize(); // Normalize the line


  // Project vector "diff" onto line by using the dot product
  ab = (ab * ap.dot(ab));
  printf("AB: %.6f %.6f %.6f %.6f %.6f %.6f %.6f %.6f\n", a.x, a.y, b.x, b.y, ap.x, ap.y, ab.x, ab.y);


  //ab.scale(ap.dot(ab));
  printf("SCALE %lf %lf\n", ab.x, ab.y);
  Vec3f normalPoint = a + ab;
  return normalPoint;
}

uint8_t cnt = 0;
double dir;
double angleBetween;
double dst;


void goToNextWP(uint8_t wp_no) {

  if (wpm_rcvd == 1 && wp_no <= wpm[wp_no].wpcount) {
    if (wpm[wp_no].cmd == 16) {
      //Get current GPS position and find direction to waypoint
      dst = gps.distanceBetween(rtk.lat1 * 1e-7, rtk.lon1 * 1e-7, wpm[wp_no].lat, wpm[wp_no].lon);
      dir = gps.courseTo(rtk.lat1 * 1e-7, rtk.lon1 * 1e-7, wpm[wp_no].lat, wpm[wp_no].lon);
      //Limits the direction between -180 and 180
      angleBetween = degrees(atan2(sin(dir), cos(dir)));
      cout.println("DST: " + String(dst) + " COR: " + String(dir) +  " AngleBetween: " + String(angleBetween));
    }
  }
}

void followPath(uint8_t startingWp) {
  //Get the vector to the 1st point on path
  //seek that vector from gpsLocal
  //When get close to point on path get the next point on path
  //Repeat

  // Loop through all points of the path
  for (int wp_no = startingWp; wp_no <= wpm[wp_no].wpcount; wp_no++) {
    goToNextWP(wp_no);
    //      calculateMotorSpeeds(e, max_speed);
  }
}



Vec3f closestPointOnPath(Vec3f currentPos, Vec3f predictPos, int lookAhead) {
  Vec3f pop(0, 0, 0);
  Vec2f normal(0, 0);
  Vec2f target(0, 0);
  float worldRecord = 1e6;  // Start with a very high record distance that can easily be beaten
  if (wpm[0].wpcount < 1) {
    return pop;
  }


  // Loop through all points of the path
  for (int i = 0; i < wpm[0].wpcount - 1; i++) {

    // Look at a line segment
    Vec2f a (wpm[i].lat, wpm[i].lon);
    Vec2f b (wpm[i + 1].lat, wpm[i + 1].lon);


    // Get the normal point to that line
    Vec3f normalPoint = getNormalPoint(predictPos, a, b);
    printf("NP: %lf %lf %lf %lf\n", a.x, a.y, normalPoint.x, normalPoint.y);

    // This only works because we know our path goes from left to right
    // We could have a more sophisticated test to tell if the point is in the line segment or not
    if (normalPoint.x < a.x || normalPoint.x > b.x) {
      // This is something of a hacky solution, but if it's not within the line segment
      // consider the normal to just be the end of the line segment (point b)
      normalPoint = b;
    }


    // How far away are we from the path?
    //double distance = distanceBetween(predictPos, normalPoint ); // distance is x
    double distance = distanceBetween(a, b ); // distance is x
    printf("Distance: %.8lf\n", distance);
    //We need to estimate time to
    //float estimated = calculateTimeFromVelocityAndAcceleration(_maxSpeed, _currSpeed, _maxSpeed);
    //If we know the distance and velocity we can calculate acceleration
    //float acceleration = calculateAcceleration(distance, estimated);
    //printf("Estimated Time and Acceleration: %.8lf %.8lf\n", estimated, acceleration);
    // Did we beat the record and find the closest line segment?
    if (distance < worldRecord) {
      worldRecord = distance;
      pop.set(normalPoint.x, normalPoint.y, i);
      printf("Point On Path: %20.14lf %20.14lf\n", normalPoint.x, normalPoint.y);
    }
  }
  printf("Closest Point On Path: %.6lf %.6lf", pop.x, pop.y);
  return pop;
}