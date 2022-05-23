/*============================================================================*/
/* CommonTypes.cpp -- Common Types                                            */
/*============================================================================*/

#include "BASE.h"
#include "eIV.h"

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

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/*----------------------------------------------------------------------------*/
/*-- Interval ----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Interval constructor                                                       */
/*----------------------------------------------------------------------------*/
Ivl::Ivl(eIV eIvlType)
{
  T.m_xIvl.m_eType = eIvlType;
  vSetZero();
}

/*----------------------------------------------------------------------------*/
/* Get the minimum value to the interval                                      */
/*----------------------------------------------------------------------------*/
i8 Ivl::iMinValue(eIV eIvlType)
{
  Ivl ivl{eIvlType};
  ivl.vSetMin();
  return ivl.T.m_iAsI8;
}

/*----------------------------------------------------------------------------*/
/* Get the maximum value to the interval                                      */
/*----------------------------------------------------------------------------*/
i8 Ivl::iMaxValue(eIV eIvlType)
{
  Ivl ivl{eIvlType};
  ivl.vSetMax();
  return ivl.T.m_iAsI8;
}


/*----------------------------------------------------------------------------*/
/* Sets the maximum value to the interval                                     */
/*----------------------------------------------------------------------------*/
void Ivl::vSetMax()
{
  T.m_xIvl.m_bIsNull = false;
  T.m_xIvl.m_bIsNeg = false;
  T.m_xIvl.m_iSpare = 0;

  switch(T.m_xIvl.m_eType)    {
    case IV_YEAR:             { T.m_xIvl.u.m_xIvlYr.m_uMonths      = 72057594037927935LL; break; } // 2^56-1

    case IV_MONTH:            { T.m_xIvl.u.m_xIvlMon.m_uMonths     = 72057594037927935LL; break; } // 2^56-1

    case IV_YEAR_TO_MONTH:    { T.m_xIvl.u.m_xIvlYrToMon.m_uMonths = 72057594037927935LL; break; } // 2^56-1

    case IV_DAY:              { T.m_xIvl.u.m_xIvlDay.m_uDays       = 72057594037927935LL; break; } // 2^56-1

    case IV_HOUR:             { T.m_xIvl.u.m_xIvlHr.m_uHours       = 72057594037927935LL; break; } // 2^56-1

    case IV_MINUTE:           { T.m_xIvl.u.m_xIvlMin.m_uMinutes    = 72057594037927935LL; break; } // 2^56-1

    case IV_SECOND:           { T.m_xIvl.u.m_xIvlSec.m_uSeconds    = 68719476735LL; // 2^36-1
                                T.m_xIvl.u.m_xIvlSec.m_uMicros = 999999;
                                break; }

    case IV_DAY_TO_HOUR:      { T.m_xIvl.u.m_xIvlDayToHr.m_uDays = 268435455LL; // 2^28-1
                                T.m_xIvl.u.m_xIvlDayToHr.m_uMinutes = 268435455LL; // 2^28-1
                                break; }

    case IV_DAY_TO_MINUTE:    { T.m_xIvl.u.m_xIvlDayToMin.m_uDays = 268435455LL; // 2^28-1
                                T.m_xIvl.u.m_xIvlDayToMin.m_uMinutes = 268435455LL; // 2^28-1
                                break; }

    case IV_DAY_TO_SECOND:    { T.m_xIvl.u.m_xIvlDayToSec.m_uDays = 4095; // 2^12-1
                                T.m_xIvl.u.m_xIvlDayToSec.m_uMinutes = 4095; // 2^12-1
                                T.m_xIvl.u.m_xIvlDayToSec.m_uSeconds = 4095; // 2^12-1
                                T.m_xIvl.u.m_xIvlDayToSec.m_uFrac = 999999;
                                break; }

    case IV_HOUR_TO_MINUTE:   { T.m_xIvl.u.m_xIvlHrToMin.m_uMinutes = 72057594037927935LL; break; } // 2^56-1

    case IV_HOUR_TO_SECOND:   { T.m_xIvl.u.m_xIvlHrToSec.m_uMinutes = 262143; // 2^18-1
                                T.m_xIvl.u.m_xIvlHrToSec.m_uSeconds = 262143; // 2^18-1
                                T.m_xIvl.u.m_xIvlHrToSec.m_uFrac = 999999;
                                break; }

    case IV_MINUTE_TO_SECOND: { T.m_xIvl.u.m_xIvlMinToSec.m_uMinutes = 262143; // 2^18-1
                                T.m_xIvl.u.m_xIvlMinToSec.m_uSeconds = 262143; // 2^18-1
                                T.m_xIvl.u.m_xIvlMinToSec.m_uFrac = 999999;
                                break; }

    default:
     NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Sets the interval value fields to zero                                     */
/*----------------------------------------------------------------------------*/
void Ivl::vSetZero()
{
  // Don't change the interval type
  T.m_xIvl.m_bIsNull = false;
  T.m_xIvl.m_bIsNeg = false;
  T.m_xIvl.m_iSpare = 0;

  switch(T.m_xIvl.m_eType)    {
    case IV_YEAR:             { T.m_xIvl.u.m_xIvlYr.m_uMonths      = 0; break; }

    case IV_MONTH:            { T.m_xIvl.u.m_xIvlMon.m_uMonths     = 0; break; }

    case IV_YEAR_TO_MONTH:    { T.m_xIvl.u.m_xIvlYrToMon.m_uMonths = 0; break; }

    case IV_DAY:              { T.m_xIvl.u.m_xIvlDay.m_uDays       = 0; break; }

    case IV_HOUR:             { T.m_xIvl.u.m_xIvlHr.m_uHours       = 0; break; }

    case IV_MINUTE:           { T.m_xIvl.u.m_xIvlMin.m_uMinutes    = 0; break; }

    case IV_SECOND:           { T.m_xIvl.u.m_xIvlSec.m_uSeconds    = 0;
                                T.m_xIvl.u.m_xIvlSec.m_uMicros = 0;
                                break; }

    case IV_DAY_TO_HOUR:      { T.m_xIvl.u.m_xIvlDayToHr.m_uDays = 0;
                                T.m_xIvl.u.m_xIvlDayToHr.m_uMinutes = 0;
                                break; }

    case IV_DAY_TO_MINUTE:    { T.m_xIvl.u.m_xIvlDayToMin.m_uDays = 0;
                                T.m_xIvl.u.m_xIvlDayToMin.m_uMinutes = 0;
                                break; }

    case IV_DAY_TO_SECOND:    { T.m_xIvl.u.m_xIvlDayToSec.m_uDays = 0;
                                T.m_xIvl.u.m_xIvlDayToSec.m_uMinutes = 0;
                                T.m_xIvl.u.m_xIvlDayToSec.m_uSeconds = 0;
                                T.m_xIvl.u.m_xIvlDayToSec.m_uFrac = 0;
                                break; }

    case IV_HOUR_TO_MINUTE:   { T.m_xIvl.u.m_xIvlHrToMin.m_uMinutes = 0; break; }

    case IV_HOUR_TO_SECOND:   { T.m_xIvl.u.m_xIvlHrToSec.m_uMinutes = 0;
                                T.m_xIvl.u.m_xIvlHrToSec.m_uSeconds = 0;
                                T.m_xIvl.u.m_xIvlHrToSec.m_uFrac = 0;
                                break; }

    case IV_MINUTE_TO_SECOND: { T.m_xIvl.u.m_xIvlMinToSec.m_uMinutes = 0;
                                T.m_xIvl.u.m_xIvlMinToSec.m_uSeconds = 0;
                                T.m_xIvl.u.m_xIvlMinToSec.m_uFrac = 0;
                                break; }

    default:
      NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Sets the maximum value to the interval                                     */
/*----------------------------------------------------------------------------*/
void Ivl::vSetMin()
{
  vSetMax();
  T.m_xIvl.m_bIsNeg = true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
