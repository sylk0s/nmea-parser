// define error codes
#define INVALID_START -2
#define NOT_ONE_CHAR  -3
#define NO_MATCH      -4

// todo make a better name
// some GPS position data
typedef struct DM {
  int degrees;
  double minutes;
} dm_t;

// i just implemented my own time struct since i didn't want to bother finding a ctime
typedef struct ms_time {
  int hour;
  int min;
  int sec;
  int ms;
} ms_time_t;

// Cardinal Direction
typedef enum Direction {
  NORTH,
  EAST,
  SOUTH,
  WEST,
} dir_e;

// Unit for altitude
typedef enum AltUint {
  FEET,
  METERS,
} aunit_e;

// data structure modeling a NMEA string
typedef struct nema {
  char* msg_type;
  ms_time_t timestamp;
  dm_t latitude;
  dir_e ns;
  dm_t longitude;
  dir_e ew;
  int sats;
  double altitude;
  aunit_e alt_units;
  char* checksum;
} nmea_t;

dm_t parse_pos(char* pos_str);
int parse_nmea(char nmea_str[], nmea_t* result);
void print_nmea(nmea_t* nmea);
