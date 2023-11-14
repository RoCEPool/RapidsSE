/*============================================================================*/
/* eIV.h -- Interval Type                                                     */
/*============================================================================*/
#pragma once

enum eIV : i1 // Interval Type
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

extern Stz* aIV[IV_ENUMS];
extern Stz* aIV_field[IV_ENUMS];
extern Stz* aIV_field2[IV_ENUMS];

// Returns the number of interval fields covered by a given interval type
extern i8 iIV_numFields[IV_ENUMS];

/*----------------------------------------------------------------------------*/
/* Returns true if an interval is of a year-month category                    */
/*----------------------------------------------------------------------------*/
static b1 bIsYearMonthInterval(eIV eType)
{
  return eType > IV_SUBCATEGORY_YEARMONTH_START && eType < IV_SUBCATEGORY_YEARMONTH_END;
}

/*----------------------------------------------------------------------------*/
/* Returns true if an interval is of a day-time category                      */
/*----------------------------------------------------------------------------*/
static b1 bIsDayTimeInterval(eIV eType)
{
  return eType > IV_SUBCATEGORY_DAYTIME_START && eType < IV_SUBCATEGORY_DAYTIME_END;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
