#include <check.h>
#include <stdlib.h>
#include "nmea.h"

START_TEST (basic_test) {
  char string[] = "$GPGGA,181908.00,3404.7041778,N,07044.3966270,W,13,495.144,M*31";
  nmea_t nmea;

  int result = parse_nmea(string, &nmea);

  ck_assert_str_eq(nmea.msg_type, "GPGGA");

  ck_assert_int_eq(nmea.timestamp.hour, 18);
  ck_assert_int_eq(nmea.timestamp.min, 19);
  ck_assert_int_eq(nmea.timestamp.sec, 8);
  ck_assert_int_eq(nmea.timestamp.ms, 0);

  ck_assert_int_eq(nmea.latitude.degrees, 70);
  //ck_assert_str_eq(nmea.latitude.minutes, )
  ck_assert_int_eq(nmea.ns, NORTH);

  ck_assert_int_eq(nmea.latitude.degrees, 34);
  //ck_assert_str_eq(nmea.latitude.minutes, )
  ck_assert_int_eq(nmea.ns, WEST);

  ck_assert_int_eq(nmea.sats, 13);
  //ck_assert_str_eq(nmea.altitude,);
  ck_assert_int_eq(nmea.alt_units, METERS);
  ck_assert_str_eq(nmea.checksum, "*31");
}

Suite * nmea_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("NMEA Parser");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, basic_test);
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
