/*============================================================================*/
/* eIV.cpp -- Interval Type                                                   */
/*============================================================================*/
#include "BASE.h"
#include "eIV.h"

Stz* aIV[IV_ENUMS]
{
    /* 0  */ "NONE"_Stz,
    /* 1  */ "SUBCATEGORY_YEARMONTH_START"_Stz,   // Subcategory start (not a concrete type)
    /* 2  */ "INTERVAL YEAR"_Stz,
    /* 3  */ "INTERVAL MONTH"_Stz,
    /* 4  */ "INTERVAL YEAR TO MONTH"_Stz,
    /* 5  */ "SUBCATEGORY_YEARMONTH_END"_Stz,      // Subcategory end (not a concrete type)
    /* 6  */ "SUBCATEGORY_DAYTIME_START"_Stz,      // Subcategory start (not a concrete type)
    /* 7  */ "INTERVAL DAY"_Stz,
    /* 8  */ "INTERVAL HOUR"_Stz,
    /* 9  */ "INTERVAL MINUTE"_Stz,
    /* 10 */ "INTERVAL SECOND"_Stz,
    /* 11 */ "INTERVAL DAY TO HOUR"_Stz,
    /* 12 */ "INTERVAL DAY TO MINUTE"_Stz,
    /* 13 */ "INTERVAL DAY TO SECOND"_Stz,
    /* 14 */ "INTERVAL HOUR TO MINUTE"_Stz,
    /* 15 */ "INTERVAL HOUR TO SECOND"_Stz,
    /* 16 */ "INTERVAL MINUTE TO SECOND"_Stz,
    /* 17 */ "SUBCATEGORY_DAYTIME_END"_Stz,        // Subcategory end (not a concrete type)
//    /* 18 */ "ENUMS"_Stz
};

Stz* aIV_field[IV_ENUMS]
{
    /* IV_NONE=0                      */ ""_Stz,
    /* IV_SUBCATEGORY_YEARMONTH_START */ ""_Stz,
    /* IV_YEAR                        */ "YEAR"_Stz,
    /* IV_MONTH                       */ "MONTH"_Stz,
    /* IV_YEAR_TO_MONTH               */ "YEAR_TO_MONTH"_Stz,
    /* IV_SUBCATEGORY_YEARMONTH_END   */ ""_Stz,
    /* IV_SUBCATEGORY_DAYTIME_START   */ ""_Stz,
    /* IV_DAY                         */ "DAY"_Stz,
    /* IV_HOUR                        */ "HOUR"_Stz,
    /* IV_MINUTE                      */ "MINUTE"_Stz,
    /* IV_SECOND                      */ "SECOND"_Stz,
    /* IV_DAY_TO_HOUR                 */ "DAY TO HOUR"_Stz,
    /* IV_DAY_TO_MINUTE               */ "DAY TO MINUTE"_Stz,
    /* IV_DAY_TO_SECOND               */ "DAY TO SECOND"_Stz,
    /* IV_HOUR_TO_MINUTE              */ "HOUR TO MINUTE"_Stz,
    /* IV_HOUR_TO_SECOND              */ "HOUR TO SECOND"_Stz,
    /* IV_MINUTE_TO_SECOND            */ "MINUTE TO SECOND"_Stz,
    /* IV_SUBCATEGORY_DAYTIME_END     */ ""_Stz,
//    /* IV_ENUMS                       */ ""_Stz
};

Stz* aIV_field2[IV_ENUMS]
{
		/* IV_NONE=0                      */ ""_Stz,
		/* IV_SUBCATEGORY_YEARMONTH_START */ ""_Stz,
		/* IV_YEAR                        */ "YEAR"_Stz,
		/* IV_MONTH                       */ "MONTH"_Stz,
		/* IV_YEAR_TO_MONTH               */ "YEAR_TO_MONTH"_Stz,
		/* IV_SUBCATEGORY_YEARMONTH_END   */ ""_Stz,
		/* IV_SUBCATEGORY_DAYTIME_START   */ ""_Stz,
		/* IV_DAY                         */ "DAY"_Stz,
		/* IV_HOUR                        */ "HOUR"_Stz,
		/* IV_MINUTE                      */ "MINUTE"_Stz,
		/* IV_SECOND                      */ "SECOND"_Stz,
		/* IV_DAY_TO_HOUR                 */ "DAY_TO_HOUR"_Stz,
		/* IV_DAY_TO_MINUTE               */ "DAY_TO_MINUTE"_Stz,
		/* IV_DAY_TO_SECOND               */ "DAY_TO_SECOND"_Stz,
		/* IV_HOUR_TO_MINUTE              */ "HOUR_TO_MINUTE"_Stz,
		/* IV_HOUR_TO_SECOND              */ "HOUR_TO_SECOND"_Stz,
		/* IV_MINUTE_TO_SECOND            */ "MINUTE_TO_SECOND"_Stz,
		/* IV_SUBCATEGORY_DAYTIME_END     */ ""_Stz,
//    /* IV_ENUMS                       */ ""_Stz
				};

/*----------------------------------------------------------------------------*/
/* Returns the number of interval fields covered by a given interval type     */
/*----------------------------------------------------------------------------*/
i8 iIV_numFields[IV_ENUMS]
{
  /* IV_NONE=0                      */ 0,
  /* IV_SUBCATEGORY_YEARMONTH_START */ 0,
  /* IV_YEAR                        */ 1,
  /* IV_MONTH                       */ 1,
  /* IV_YEAR_TO_MONTH               */ 2,
  /* IV_SUBCATEGORY_YEARMONTH_END   */ 0,
  /* IV_SUBCATEGORY_DAYTIME_START   */ 0,
  /* IV_DAY                         */ 1,
  /* IV_HOUR                        */ 1,
  /* IV_MINUTE                      */ 1,
  /* IV_SECOND                      */ 1,
  /* IV_DAY_TO_HOUR                 */ 2,
  /* IV_DAY_TO_MINUTE               */ 3,
  /* IV_DAY_TO_SECOND               */ 4,
  /* IV_HOUR_TO_MINUTE              */ 2,
  /* IV_HOUR_TO_SECOND              */ 3,
  /* IV_MINUTE_TO_SECOND            */ 2,
  /* IV_SUBCATEGORY_DAYTIME_END     */ 0,
//  /* IV_ENUMS                       */ 0,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
