/*============================================================================*/
/* CommonTypes.cpp -- Common Types                                            */
/*============================================================================*/

#include "BASE.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static const i8 C_DAYS_IN_MONTH[12]      = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
static const i8 C_DAYS_IN_MONTH_LEAP[12] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };


/*----------------------------------------------------------------------------*/
/*-- Date --------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Tests if the given value is a valid date and if it is within range.        */
/* Writes to sErrorBuf if not                                                 */
/*----------------------------------------------------------------------------*/
b1 Dte::bIsValid(i8 iYear, i8 iMonth, i8 iDay, s256& sErrorBuf) {
  if (iYear > 9999 || iYear < -9999) {
    sErrorBuf.vCopy("Year value is out of range"_Stz);
    return false;
  }

  if (iMonth > 12 || iMonth < 1) {
    sErrorBuf.vCopy("Month value is out of range"_Stz);
    return false;
  }

  if (iDay > 31 || iDay < 1) {
    sErrorBuf.vCopy("Day value is out of range"_Stz);
    return false;
  }

  b1 bIsLeapYear = ( (iYear % 4 == 0) && (iYear % 100 != 0) ) || (iYear % 400 == 0);

  // Check days in month
  if ( (!bIsLeapYear && iDay > C_DAYS_IN_MONTH[iMonth - 1]) ||
       (bIsLeapYear &&  iDay > C_DAYS_IN_MONTH_LEAP[iMonth - 1]) ) {
    sErrorBuf.vCopy("Day value is out of range"_Stz);
    return false;
  }

  // Check that the date is within range of Dte class
  if (iYear < (i8) C_YEAR_MIN || iYear > (i8) C_YEAR_MAX) {
    sErrorBuf.vCopy("Year value is out of range"_Stz);
    return false;
  }

  return true;
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/*----------------------------------------------------------------------------*/
/*-- Time---------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Tests if the given value is a valid time and if it is within range.        */
/* Writes to sErrorBuf if not                                                 */
/*----------------------------------------------------------------------------*/
b1 Tme::bIsValid(i8 iHour, i8 iMin, i8 iSec, s256& sErrorBuf) {
  if (iHour > 23 || iHour < 0) {
    sErrorBuf.vCopy("Hour value is out of range"_Stz);
    return false;
  }

  if (iMin > 59 || iMin < 0) {
    sErrorBuf.vCopy("Minute value is out of range"_Stz);
    return false;
  }

  if (iSec > 59 || iSec < 0) {
    sErrorBuf.vCopy("Second value is out of range"_Stz);
    return false;
  }

  return true;
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/*----------------------------------------------------------------------------*/
/*-- Timestamp ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Tests if the given value is a valid timestamp and if it is within range.   */
/* Writes to sErrorBuf if not                                                 */
/*----------------------------------------------------------------------------*/
b1 Tsp::bIsValid(i8 iYear, i8 iMonth, i8 iDay, i8 iHour, i8 iMin, i8 iSec, i8 iMicro, s256& sErrorBuf)
{
  if (!Dte::bIsValid(iYear, iMonth, iDay, sErrorBuf)) {
    return false;
  }

  if (!Tme::bIsValid(iHour, iMin, iSec, sErrorBuf)) {
    return false;
  }

  if (iMicro > (i8)C_TSP_USEC_MAX || iMicro < (i8)C_TSP_USEC_MIN) {
    sErrorBuf.vCopy("Microseconds value is out of range"_Stz);
    return false;
  }

  return true;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
