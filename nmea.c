#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "nmea.h"

// parses positions in DDDMM.MMMMM format
dm_t parse_pos(char* pos_str) {
  // old data to mess with in strtok
  char old_pos_str[32];
  strcpy(old_pos_str, pos_str);

  // calculates the place for the memcpy
  char* tok = strtok(old_pos_str, ".");
  int len = strlen(tok);

  // gets the degree amount 
  char day_str[len - 2];
  memcpy(day_str, pos_str, len - 2);
  int deg = atoi(day_str);

  // gets the minutes amount
  double min = atof(pos_str + (len - 2));
  
  dm_t pos = {deg, min};
  return pos;
}

// parses a NMEA string into the nmea_t data structure
int parse_nmea(char nmea_str[], nmea_t* result) {
  // assert first bytes is $  
  if (nmea_str[0] != '$') {
    return INVALID_START;
  }

  // parse out msg type
  char* type_str = strtok_r(nmea_str, ",", &nmea_str);
  result->msg_type = type_str + 1;
  

  // parses timestamp to time_t struct
  char* timestamp_str = strtok_r(NULL, ",", &nmea_str);
  // hour 2, min 2, sec 2,. ms 2
  char time_str[2];
  memcpy(time_str, timestamp_str, 2); 
  int hour = atoi(time_str); 
  memcpy(time_str, timestamp_str + 2, 2); 
  int min  = atoi(time_str); 
  memcpy(time_str, timestamp_str + 4, 2); 
  int sec  = atoi(time_str); 
  memcpy(time_str, timestamp_str + 7, 2); 
  int ms   = atoi(time_str); 

  ms_time_t time = {hour, min, sec, ms};
  result->timestamp = time;


  // gps latitude
  char* lat_str = strtok_r(NULL, ",", &nmea_str);
  result->latitude = parse_pos(lat_str);
  

  // lat dir string
  char* lat_dir_str = strtok_r(NULL, ",", &nmea_str);

  // confirms that this is the right length
  if (strlen(lat_dir_str) != 1) {
    return NOT_ONE_CHAR;
  }

  // matches to result
  switch (*lat_dir_str) {
    case 'N':
      result->ns = NORTH;
    break;
    case 'S':
      result->ns = SOUTH;
    break;
    default:
      return NO_MATCH;
  }

  // gps longitude
  char* lon_str = strtok_r(NULL, ",", &nmea_str);
  result->longitude = parse_pos(lon_str);


  // lon dir string
  char* lon_dir_str = strtok_r(NULL, ",", &nmea_str);

  // confirms that this is the right length
  if (strlen(lon_dir_str) != 1) {
    return NOT_ONE_CHAR;
  }

  // matches to result
  switch (*lon_dir_str) {
    case 'E':
      result->ew = EAST;
    break;
    case 'W':
      result->ew = WEST;
    break;
    default:
      return NO_MATCH;
  }


  // number of satellites used :O
  char* num_sats_str = strtok_r(NULL, ",", &nmea_str);
  result->sats = atoi(num_sats_str);


  // altitude of gps
  char* gps_alt_str = strtok_r(NULL, ",", &nmea_str);
  result->altitude = atof(gps_alt_str);


  // units of altitude
  char* alt_units_str = strtok_r(NULL, ",", &nmea_str);
  
  // matches to the unit val
  switch (*alt_units_str) {
    case 'M':
      result->alt_units = METERS;
    break;
    case 'F':
      result->alt_units = FEET;
    break;
    default:
      return NO_MATCH;
  }


  // checksum
  // currently parsed as "everything else"
  // char* checksum_str = strtok_r(NULL, ",", &nmea_str);
  result->checksum = alt_units_str + 1; 
  

  // return 0 if everything is OK
  return 0;
}

void print_nmea(nmea_t* nmea) {
  printf("Type: %s\nTime: %d:%d:%d:%d\n Lat: %dd, %fmin\nDir: %d\nLon: %dd, %fmin\nDir: %d\nSats: %d\nAlt: %f u%d\nCksm:%s\n", 
      nmea->msg_type, 
      nmea->timestamp.hour, 
      nmea->timestamp.min, 
      nmea->timestamp.sec, 
      nmea->timestamp.ms, 
      nmea->latitude.degrees, 
      nmea->latitude.minutes, 
      nmea->ns, 
      nmea->longitude.degrees, 
      nmea->longitude.minutes, 
      nmea->ew, 
      nmea->sats, 
      nmea->altitude, 
      nmea->alt_units, 
      nmea->checksum
      );
}

// int main() {
  // test data for now
//  char string[] = "$GPGGA,181908.00,3404.7041778,N,07044.3966270,W,13,495.144,M*31";
//  nmea_t nmea;

//  parse_nmea(string, &nmea);

//  return 0;
//}
