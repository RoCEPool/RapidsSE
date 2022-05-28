/*============================================================================*/
/* eIV.h -- Interval Type                                                     */
/*============================================================================*/
#pragma once

enum eIV // Interval Type
{
  /* 0  */ IV_NONE=0,
  /* 1  */ IV_SUBCATEGORY_YEARMONTH_START,   // Subcategory start (not a concrete type)
  /* 2  */ IV_YEAR,
  /* 3  */ IV_MONTH,
  /* 4  */ IV_YEAR_TO_MONTH,
  /* 5  */ IV_SUBCATEGORY_YEARMONTH_END,     // Subcategory end (not a concrete type)
  /* 6  */ IV_SUBCATEGORY_DAYTIME_START,     // Subcategory start (not a concrete type)
  /* 7  */ IV_DAY,
  /* 8  */ IV_HOUR,
  /* 9  */ IV_MINUTE,
  /* 10 */ IV_SECOND,
  /* 11 */ IV_DAY_TO_HOUR,
  /* 12 */ IV_DAY_TO_MINUTE,
  /* 13 */ IV_DAY_TO_SECOND,
  /* 14 */ IV_HOUR_TO_MINUTE,
  /* 15 */ IV_HOUR_TO_SECOND,
  /* 16 */ IV_MINUTE_TO_SECOND,
  /* 17 */ IV_SUBCATEGORY_DAYTIME_END,       // Subcategory end (not a concrete type)
  /* 18 */ IV_ENUMS
};

static Stz* aIV[]
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
    /* 18 */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
