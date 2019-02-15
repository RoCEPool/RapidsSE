/*============================================================================*/
/* Tsp.cpp -- timestamp datum                                                 */
/*============================================================================*/

#include "Datums.h"

#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Tsp::operator new(size_t)
{
  vFatal("fatal: Tsp class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Tsp::Tsp(void)
{
  vInitZero();
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Tsp::~Tsp(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Operators ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is this timestamp less than given timestamp?                               */
/*----------------------------------------------------------------------------*/
bool Tsp::operator < (Tsp& xTsp)
{
  if (m_iValue < xTsp.m_iValue)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this timestamp less than or equal to given timestamp?                   */
/*----------------------------------------------------------------------------*/
bool Tsp::operator <= (Tsp& xTsp)
{
  if (m_iValue <= xTsp.m_iValue)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this timestamp equal to given timestamp?                                */
/*----------------------------------------------------------------------------*/
bool Tsp::operator == (Tsp& xTsp)
{
  if (m_iValue == xTsp.m_iValue)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this timestamp not equal to given timestamp?                            */
/*----------------------------------------------------------------------------*/
bool Tsp::operator != (Tsp& xTsp)
{
  if (m_iValue != xTsp.m_iValue)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this timestamp greater than or equal to given timestamp?                */
/*----------------------------------------------------------------------------*/
bool Tsp::operator >= (Tsp& xTsp)
{
  if (m_iValue >= xTsp.m_iValue)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this timestamp greater than given timestamp?                            */
/*----------------------------------------------------------------------------*/
bool Tsp::operator > (Tsp& xTsp)
{
  if (m_iValue > xTsp.m_iValue)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* initialse to all zero                                                      */
/*----------------------------------------------------------------------------*/
void Tsp::vInitZero(void)
{
  x.m_uYear   = 0;
  x.m_uMonth  = 0;
  x.m_uDay    = 0;
  x.m_uHour   = 0;
  x.m_uMinute = 0;
  x.m_uSecond = 0;
  x.m_uMicro  = 0;
  x.m_uSpare  = 0;
}

/*----------------------------------------------------------------------------*/
/* get as i8                                                                  */
/*----------------------------------------------------------------------------*/
i8 Tsp::iAsI8(void)
{
  return m_iValue;
}

/*----------------------------------------------------------------------------*/
/* get year                                                                   */
/*----------------------------------------------------------------------------*/
i8 Tsp::iYear(void)
{
  return x.m_uYear;
}

/*----------------------------------------------------------------------------*/
/* get month                                                                  */
/*----------------------------------------------------------------------------*/
i8 Tsp::iMonth(void)
{
  return x.m_uMonth;
}

/*----------------------------------------------------------------------------*/
/* get day                                                                    */
/*----------------------------------------------------------------------------*/
i8 Tsp::iDay(void)
{
  return x.m_uDay;
}

/*----------------------------------------------------------------------------*/
/* get hour                                                                   */
/*----------------------------------------------------------------------------*/
i8 Tsp::iHour(void)
{
  return x.m_uHour;
}

/*----------------------------------------------------------------------------*/
/* get minute                                                                 */
/*----------------------------------------------------------------------------*/
i8 Tsp::iMinute(void)
{
  return x.m_uMinute;
}

/*----------------------------------------------------------------------------*/
/* get second                                                                 */
/*----------------------------------------------------------------------------*/
i8 Tsp::iSecond(void)
{
  return x.m_uSecond;
}

/*----------------------------------------------------------------------------*/
/* get microseconds                                                           */
/*----------------------------------------------------------------------------*/
i8 Tsp::iMicroseconds(void)
{
  return x.m_uMicro;
}

/*----------------------------------------------------------------------------*/
/* set from arg                                                               */
/*----------------------------------------------------------------------------*/
void Tsp::vSet(Tsp* aTspArg)
{
  m_iValue = aTspArg->m_iValue;
}

/*----------------------------------------------------------------------------*/
/* set as now                                                                 */
/*----------------------------------------------------------------------------*/
void Tsp::vSetAsNow(void)
{
  i8 iSeconds;
  i8 iMicroseconds;
  SysCall xSysCall;
  
  Since::vGetTimeOfDay(iSeconds,iMicroseconds,xSysCall);
  
  // convert current epoch time into LOCAL:date&time
  time_t xSeconds_t;
  xSeconds_t = iSeconds;
  struct tm* aLocalDateTime = localtime(&xSeconds_t);
  
  if (! aLocalDateTime)
    vWarn("localtime() error; errno=%d"_Stz,errno);
  
  x.m_uYear   = aLocalDateTime->tm_year + 1900;
  x.m_uMonth  = aLocalDateTime->tm_mon  + 1;
  x.m_uDay    = aLocalDateTime->tm_mday;
  x.m_uHour   = aLocalDateTime->tm_hour;
  x.m_uMinute = aLocalDateTime->tm_min;
  x.m_uSecond = aLocalDateTime->tm_sec;
  x.m_uMicro  = iMicroseconds;
}

/*----------------------------------------------------------------------------*/
/*-- Null --------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* test if null                                                               */
/*----------------------------------------------------------------------------*/
bool Tsp::bIsNull(void)
{
  if (C_NULL_INT == m_iValue)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* set to null                                                                */
/*----------------------------------------------------------------------------*/
void Tsp::vSetNull(void)
{
  m_iValue = C_NULL_INT;
}

/*----------------------------------------------------------------------------*/
/*-- VLQ ---------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Extract and Sprint ------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* copy tsp datum                                                             */
/*----------------------------------------------------------------------------*/
i8 Tsp::nCopy(void* aDst, void* aSrc)
{
  *(i8*)aDst = *(i8*)aSrc;
  
  return sizeof(i8);
}

/*----------------------------------------------------------------------------*/
/* copy tsp datum                                                             */
/*----------------------------------------------------------------------------*/
void Tsp::vCopy(void* aDst, void* aSrc)
{
  *(i8*)aDst = *(i8*)aSrc;
}

/*----------------------------------------------------------------------------*/
/* extract a timestamp field from csv substr         yyyy-mo-ddThh:mi:ss.fffZ */
/*----------------------------------------------------------------------------*/
bool Tsp::bExtract(char** aaSrc, Stz* aDst, Column* pColumn, Formatters* pFormatters)
{
  // refer: http://en.wikipedia.org/wiki/ISO_8601
  
  Tsp* aTsp = (Tsp*)aDst;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  bool bSawEnclosedLeading =
    StrLib::bSkipLeading(aaSrc,pFormatters->m_cEnclosedBy_Leading);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (DtmLib::bTestForNull(aaSrc,pColumn,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    aTsp->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // timestamp field processing
  
  i8 iYear, iMonth, iDay;
  i8 iHour, iMinute, iSecond, iMicro;
  
  if ( ! StrLib::bExtractTimestamp2(aaSrc, iYear, iMonth, iDay,
                                           iHour, iMinute, iSecond, iMicro,
                                    pFormatters->m_eDateComponent,
                                    pFormatters->m_cDateSeparator) )
    return false;
  
  aTsp->x.m_uYear   = iYear;
  aTsp->x.m_uMonth  = iMonth;
  aTsp->x.m_uDay    = iDay;
  aTsp->x.m_uHour   = iHour;
  aTsp->x.m_uMinute = iMinute;
  aTsp->x.m_uSecond = iSecond;
  aTsp->x.m_uMicro  = iMicro;
  aTsp->x.m_uSpare  = 0;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  bool bSawEnclosedTrailing =
    StrLib::bSkipTrailing(aaSrc,bSawEnclosedLeading,pFormatters->m_cEnclosedBy_Trailing);
  if (! bSawEnclosedTrailing)
    return false;
  
  // should see a valid field end
  if ( ! DtmLib::bValidFieldEnd(*aaSrc,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a timestamp field from csv substr         yyyy-mo-ddThh:mi:ss.fffZ */
/*----------------------------------------------------------------------------*/
bool Tsp::bExtractDefault(char** aaSrc, Stz* aDst, Column* pColumn, Formatters* pFormatters)
{
  // refer: http://en.wikipedia.org/wiki/ISO_8601
  
  Tsp* aTsp = (Tsp*)aDst;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  StrLib::vSkipSpaces(aaSrc);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (DtmLib::bTestForNullDefault(aaSrc,pColumn,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    aTsp->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // timestamp field processing
  
  i8 iYear, iMonth, iDay;
  i8 iHour, iMinute, iSecond, iMicro;
  
  if ( ! StrLib::bExtractTimestamp2(aaSrc, iYear, iMonth, iDay,
                                    iHour, iMinute, iSecond, iMicro,
                                    pFormatters->m_eDateComponent,
                                    pFormatters->m_cDateSeparator) )
    return false;
  
  aTsp->x.m_uYear   = iYear;
  aTsp->x.m_uMonth  = iMonth;
  aTsp->x.m_uDay    = iDay;
  aTsp->x.m_uHour   = iHour;
  aTsp->x.m_uMinute = iMinute;
  aTsp->x.m_uSecond = iSecond;
  aTsp->x.m_uMicro  = iMicro;
  aTsp->x.m_uSpare  = 0;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  StrLib::vSkipSpaces(aaSrc);
  
  // should see a valid field end
  if ( ! DtmLib::bValidFieldEndDefault(*aaSrc,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* sprint tsp datum                                                           */
/*----------------------------------------------------------------------------*/
void Tsp::vSprint(Stz* aDst)
{
  if (bIsNull()) {
    StrLib::vSprint(aDst,"null%c"_Stz,'\0');
    return;
  }
  
  char caFraction[16];
  StrLib::vFormatFraction(caFraction,x.m_uMicro,6);
  
  StrLib::vSprint(aDst,"%04lld-%02lld-%02lld %02lld:%02lld:%02lld.%s%c"_Stz,
                  (i8)x.m_uYear,
                  (i8)x.m_uMonth,
                  (i8)x.m_uDay,
                  (i8)x.m_uHour,
                  (i8)x.m_uMinute,
                  (i8)x.m_uSecond,
                  caFraction,
                  '\0');
}

/*----------------------------------------------------------------------------*/
/* sprint tsp datum                                                           */
/*----------------------------------------------------------------------------*/
void Tsp::vSprint(Stz* aDst, Stz* aSrc)
{
  Tsp* aTsp = (Tsp*)aSrc;
  u8 uMicro = (i8)aTsp->x.m_uMicro;
  
  if (aTsp->bIsNull()) {
    StrLib::vSprint(aDst,"null%c"_Stz,
                    '\0');
    return;
  }
  
  if (uMicro) {
    char caFraction[16];
    StrLib::vFormatFraction(caFraction,uMicro,6);
    
    StrLib::vSprint(aDst,"TIMESTAMP '%02lld-%02lld-%02lld %02lld:%02lld:%02lld.%s'%c"_Stz,
                         (i8)aTsp->x.m_uYear,
                         (i8)aTsp->x.m_uMonth,
                         (i8)aTsp->x.m_uDay,
                         (i8)aTsp->x.m_uHour,
                         (i8)aTsp->x.m_uMinute,
                         (i8)aTsp->x.m_uSecond,
                         caFraction,
                         '\0');
    return;
  }

  StrLib::vSprint(aDst,"TIMESTAMP '%02lld-%02lld-%02lld %02lld:%02lld:%02lld'%c"_Stz,
                       (i8)aTsp->x.m_uYear,
                       (i8)aTsp->x.m_uMonth,
                       (i8)aTsp->x.m_uDay,
                       (i8)aTsp->x.m_uHour,
                       (i8)aTsp->x.m_uMinute,
                       (i8)aTsp->x.m_uSecond,
                       '\0');
}

/*----------------------------------------------------------------------------*/
/* sprint tsp datum in escaped form                                           */
/*----------------------------------------------------------------------------*/
void Tsp::vSprintEscaped(Stz* aDst, Stz* aSrc)
{
  Tsp* aTsp = (Tsp*)aSrc;
  u8 uMicro = (i8)aTsp->x.m_uMicro;
  
  if (aTsp->bIsNull()) {
    StrLib::vSprint(aDst,"null%c"_Stz,
                    '\0');
    return;
  }
  
  if (uMicro) {
    char caFraction[16];
    StrLib::vFormatFraction(caFraction,uMicro,6);

    StrLib::vSprint(aDst,"%02lld-%02lld-%02lld %02lld:%02lld:%02lld.%s%c"_Stz,
                         (i8)aTsp->x.m_uYear,
                         (i8)aTsp->x.m_uMonth,
                         (i8)aTsp->x.m_uDay,
                         (i8)aTsp->x.m_uHour,
                         (i8)aTsp->x.m_uMinute,
                         (i8)aTsp->x.m_uSecond,
                         caFraction,
                         '\0');
    return;
  }

  StrLib::vSprint(aDst,"%02lld-%02lld-%02lld %02lld:%02lld:%02lld%c"_Stz,
                       (i8)aTsp->x.m_uYear,
                       (i8)aTsp->x.m_uMonth,
                       (i8)aTsp->x.m_uDay,
                       (i8)aTsp->x.m_uHour,
                       (i8)aTsp->x.m_uMinute,
                       (i8)aTsp->x.m_uSecond,
                       '\0');
}

/*----------------------------------------------------------------------------*/
/* sprint tsp datum                                                           */
/*----------------------------------------------------------------------------*/
void Tsp::vSprintNonSQL(Stz* aDst, Stz* aSrc)
{
  Tsp* aTsp = (Tsp*)aSrc;
  u8 uMicro = (i8)aTsp->x.m_uMicro;
  
  if (aTsp->bIsNull()) {
    StrLib::vSprint(aDst,"null%c"_Stz,
                    '\0');
    return;
  }
  
  if (uMicro) {
    char sFraction[16];
    StrLib::vFormatFraction(sFraction,uMicro,6);

    StrLib::vSprint(aDst,"%02lld-%02lld-%02lld %02lld:%02lld:%02lld.%s%c"_Stz,
                         (i8)aTsp->x.m_uYear,
                         (i8)aTsp->x.m_uMonth,
                         (i8)aTsp->x.m_uDay,
                         (i8)aTsp->x.m_uHour,
                         (i8)aTsp->x.m_uMinute,
                         (i8)aTsp->x.m_uSecond,
                         sFraction,
                         '\0');
    return;
  }
  
  StrLib::vSprint(aDst,"%02lld-%02lld-%02lld %02lld:%02lld:%02lld%c"_Stz,
                       (i8)aTsp->x.m_uYear,
                       (i8)aTsp->x.m_uMonth,
                       (i8)aTsp->x.m_uDay,
                       (i8)aTsp->x.m_uHour,
                       (i8)aTsp->x.m_uMinute,
                       (i8)aTsp->x.m_uSecond,
                       '\0');
}

/*----------------------------------------------------------------------------*/
/*-- Embedded Comparators ----------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is tsp argA < aArgB?                                                       */
/*----------------------------------------------------------------------------*/
bool Tsp::bLessThan(char* aArgA, char* aArgB)
{
  bool bLessThan;
  
  if (((Tsp*)aArgA)->bIsNull())
    bLessThan = true;
  
  else if (((Tsp*)aArgB)->bIsNull())
    bLessThan = false;
  
  else {
    i8 iA = *(i8*)aArgA;
    i8 iB = *(i8*)aArgB;
    
    if (iA < iB)
      bLessThan = true;
    else
      bLessThan = false;
  }
  
  return bLessThan;
}

/*----------------------------------------------------------------------------*/
/* is tsp argA <= aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Tsp::bLessEqual(char* aArgA, char* aArgB)
{
  bool bLessEqual;
  
  if (((Tsp*)aArgA)->bIsNull() && ((Tsp*)aArgB)->bIsNull())
    bLessEqual = true;
  
  else if (((Tsp*)aArgA)->bIsNull())
    bLessEqual = true;
  
  else if (((Tsp*)aArgB)->bIsNull())
    bLessEqual = false;
  
  else {
    i8 iA = *(i8*)aArgA;
    i8 iB = *(i8*)aArgB;
    
    if (iA <= iB)
      bLessEqual = true;
    else
      bLessEqual = false;
  }
  
  return bLessEqual;
}

/*----------------------------------------------------------------------------*/
/* is tsp argA == aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Tsp::bEqualTo(char* aArgA, char* aArgB)
{
  bool bEqualTo;
  
  i8 iA = *(i8*)aArgA;
  i8 iB = *(i8*)aArgB;
  
  if (iA == iB)
    bEqualTo = true;
  else
    bEqualTo = false;
  
  return bEqualTo;
}

/*----------------------------------------------------------------------------*/
/* is tsp argA != aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Tsp::bNotEqual(char* aArgA, char* aArgB)
{
  bool bNotEqual;
  
  i8 iA = *(i8*)aArgA;
  i8 iB = *(i8*)aArgB;
  
  if (iA != iB)
    bNotEqual = true;
  else
    bNotEqual = false;
  
  return bNotEqual;
}

/*----------------------------------------------------------------------------*/
/* is tsp argA >= aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Tsp::bGreaterEqual(char* aArgA, char* aArgB)
{
  bool bGreaterEqual;
  
  if (((Tsp*)aArgA)->bIsNull() && ((Tsp*)aArgB)->bIsNull())
    bGreaterEqual = true;
  
  else if (((Tsp*)aArgA)->bIsNull())
    bGreaterEqual = false;
  
  else if (((Tsp*)aArgB)->bIsNull())
    bGreaterEqual = true;
  
  else {
    i8 iA = *(i8*)aArgA;
    i8 iB = *(i8*)aArgB;
    
    if (iA >= iB)
      bGreaterEqual = true;
    else
      bGreaterEqual = false;
  }
  
  return bGreaterEqual;
}

/*----------------------------------------------------------------------------*/
/* is tsp argA > aArgB?                                                       */
/*----------------------------------------------------------------------------*/
bool Tsp::bGreaterThan(char* aArgA, char* aArgB)
{
  bool bGreaterThan;
  
  if (((Tsp*)aArgA)->bIsNull())
    bGreaterThan = false;
  
  else if (((Tsp*)aArgB)->bIsNull())
    bGreaterThan = true;
  
  else {
    i8 iA = *(i8*)aArgA;
    i8 iB = *(i8*)aArgB;
    
    if (iA > iB)
      bGreaterThan = true;
    else
      bGreaterThan = false;
  }
  
  return bGreaterThan;
}

/*----------------------------------------------------------------------------*/
/* cmp argA with argB (-ve: argA < argB, 0: argA == argB, +ve: argA > argB)   */
/*----------------------------------------------------------------------------*/
i8 Tsp::iCmp(char* aArgA, char* aArgB)
{
  //----------------------------------------------------------------------------
  // check for nulls first
  if (((Tsp*)aArgA)->bIsNull() && ((Tsp*)aArgB)->bIsNull())
    return 0;
  
  if (((Tsp*)aArgA)->bIsNull())
    return -1;
  
  if (((Tsp*)aArgB)->bIsNull())
    return 1;
  
  i8 iA = *(i8*)aArgA;
  i8 iB = *(i8*)aArgB;
  
  Tsp* pA = (Tsp*)aArgA;
  Tsp* pB = (Tsp*)aArgB;
 
  if (iA > iB)
    return 1;
  
  if (iA < iB)
    return -1;
    
  return 0;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
