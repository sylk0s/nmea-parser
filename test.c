#include <check.h>
#include <stdlib.h>
#include "nmea.h"

// basic test with a known working test string
START_TEST (basic_test) {
  char string[] = "$GPGGA,181908.00,3404.7041778,N,07044.3966270,W,13,495.144,M*31";
  nmea_t nmea;

  int result = parse_nmea(string, &nmea);
  // print_nmea(&nmea);

  ck_assert_int_eq(result, 0);

  ck_assert_str_eq(nmea.msg_type, "GPGGA");

  ck_assert_int_eq(nmea.timestamp.hour, 18);
  ck_assert_int_eq(nmea.timestamp.min, 19);
  ck_assert_int_eq(nmea.timestamp.sec, 8);
  ck_assert_int_eq(nmea.timestamp.ms, 0);

  ck_assert_int_eq(nmea.longitude.degrees, 70);
  ck_assert(nmea.latitude.minutes - 4.7041778 < 0.001);
  ck_assert_int_eq(nmea.ns, NORTH);

  ck_assert_int_eq(nmea.latitude.degrees, 34);
  ck_assert(nmea.longitude.minutes - 44.396627 < 0.001);
  ck_assert_int_eq(nmea.ew, WEST);

  ck_assert_int_eq(nmea.sats, 13);
  ck_assert(nmea.altitude - 495.144 < 0.001);
  ck_assert_int_eq(nmea.alt_units, METERS);
  ck_assert_str_eq(nmea.checksum, "*31");
}

// another working test with a modified string to try and hit all of the other cases
START_TEST (basic_test_2) {
  char string[] = "$GPGGA,082359.10,304.746578,S,1724.9762171,E,42,123.456,F*16";
  nmea_t nmea;

  int result = parse_nmea(string, &nmea);
  // print_nmea(&nmea);

  ck_assert_int_eq(result, 0);

  ck_assert_str_eq(nmea.msg_type, "GPGGA");

  ck_assert_int_eq(nmea.timestamp.hour, 8);
  ck_assert_int_eq(nmea.timestamp.min, 23);
  ck_assert_int_eq(nmea.timestamp.sec, 59);
  ck_assert_int_eq(nmea.timestamp.ms, 10);

  ck_assert_int_eq(nmea.latitude.degrees, 3);
  ck_assert(nmea.latitude.minutes - 4.746578 < 0.001);
  ck_assert_int_eq(nmea.ns, SOUTH);

  ck_assert_int_eq(nmea.longitude.degrees, 17);
  ck_assert(nmea.longitude.minutes - 24.9762171 < 0.001);
  ck_assert_int_eq(nmea.ew, EAST);

  ck_assert_int_eq(nmea.sats, 42);
  ck_assert(nmea.altitude - 123.456 < 0.001);
  ck_assert_int_eq(nmea.alt_units, FEET);
  ck_assert_str_eq(nmea.checksum, "*16");
}

// tests for erroring on an invalid start
START_TEST (invalid_start) {
  char string[] = "#GPGGA,181908.00,3404.7041778,N,07044.3966270,W,13,495.144,M*31";
  nmea_t nmea;

  int result = parse_nmea(string, &nmea);

  ck_assert_int_eq(result, INVALID_START);
}

// tests for erroring when the dir field is too long
START_TEST (invalid_dir) {
  char string[] = "$GPGGA,181908.00,3404.7041778,NW,07044.3966270,W,13,495.144,M*31";
  nmea_t nmea;

  int result = parse_nmea(string, &nmea);

  ck_assert_int_eq(result, NOT_ONE_CHAR);
}

// tests to confirm that a field not matching it's pattern throws an error
START_TEST (invalid_match) {
  char string[] = "$GPGGA,181908.00,3404.7041778,Z,07044.3966270,W,13,495.144,M*31";
  nmea_t nmea;

  int result = parse_nmea(string, &nmea);

  ck_assert_int_eq(result, NO_MATCH);
}

Suite * nmea_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("NMEA Parser");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, basic_test);
    tcase_add_test(tc_core, basic_test_2);
    tcase_add_test(tc_core, invalid_start);
    tcase_add_test(tc_core, invalid_dir);
    tcase_add_test(tc_core, invalid_match);
    suite_add_tcase(s, tc_core);

    return s;
}

 int main(void)
 {
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = nmea_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
