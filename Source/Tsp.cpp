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
void* Tsp::operator new(size_t iSize)
{
  RASSERT(iSize);
  
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
Tsp::~Tsp(void)
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
  if (*(i8*)&x < *(i8*)&xTsp.x)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this timestamp less than or equal to given timestamp?                   */
/*----------------------------------------------------------------------------*/
bool Tsp::operator <= (Tsp& xTsp)
{
  if (*(i8*)&x <= *(i8*)&xTsp.x)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this timestamp equal to given timestamp?                                */
/*----------------------------------------------------------------------------*/
bool Tsp::operator == (Tsp& xTsp)
{
  if (*(i8*)&x == *(i8*)&xTsp.x)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this timestamp not equal to given timestamp?                            */
/*----------------------------------------------------------------------------*/
bool Tsp::operator != (Tsp& xTsp)
{
  if (*(i8*)&x != *(i8*)&xTsp.x)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this timestamp greater than or equal to given timestamp?                */
/*----------------------------------------------------------------------------*/
bool Tsp::operator >= (Tsp& xTsp)
{
  if (*(i8*)&x >= *(i8*)&xTsp.x)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this timestamp greater than given timestamp?                            */
/*----------------------------------------------------------------------------*/
bool Tsp::operator > (Tsp& xTsp)
{
  if (*(i8*)&x > *(i8*)&xTsp.x)
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
  x.m_uMilli  = 0;
  x.m_uSpare  = 0;
}

/*----------------------------------------------------------------------------*/
/* get as i8                                                                  */
/*----------------------------------------------------------------------------*/
i8 Tsp::iAsI8(void)
{
  i8 i = *(i8*)&x;
  
  return i;
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
/* get milli                                                                  */
/*----------------------------------------------------------------------------*/
i8 Tsp::iMilli(void)
{
  return x.m_uMilli;
}

/*----------------------------------------------------------------------------*/
/*-- Null --------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* test if null                                                               */
/*----------------------------------------------------------------------------*/
bool Tsp::bIsNull(void)
{
  if (C_NULL_INT == *(i8*)&x)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* set to null                                                                */
/*----------------------------------------------------------------------------*/
void Tsp::vSetNull(void)
{
  *(i8*)&x = C_NULL_INT;
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
bool Tsp::bExtract(char** aaSrc, Tsp* aTsp, Column* pColumn, Formatters* pFormatters)
{
  // refer: http://en.wikipedia.org/wiki/ISO_8601
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  // skip leading spaces
  StrLib::vSkipSpaces(aaSrc);
    
  // is there any data ?
  //if ( ! DtmLib::bIsThereFieldData(**aaSrc,pFormatters) )
  //  return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull;
  if (DtmLib::bTestForNull(aaSrc,pColumn,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    aTsp->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // timestamp field processing
  
  i8 iYear, iMonth, iDay;
  i8 iHour, iMinute, iSecond, iMilli;
  
  if ( ! StrLib::bExtractTimestamp(aaSrc, iYear, iMonth, iDay,
                                          iHour, iMinute, iSecond, iMilli) )
    return false;

  aTsp->x.m_uYear   = iYear;
  aTsp->x.m_uMonth  = iMonth;
  aTsp->x.m_uDay    = iDay;
  aTsp->x.m_uHour   = iHour;
  aTsp->x.m_uMinute = iMinute;
  aTsp->x.m_uSecond = iSecond;
  aTsp->x.m_uMilli  = iMilli;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  // skip trailing spaces
  StrLib::vSkipSpaces(aaSrc);

  // should see a FS, NL or CR
  if ( ! DtmLib::bValidFieldEnd(aaSrc,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* sprint tsp datum                                                           */
/*----------------------------------------------------------------------------*/
void Tsp::vSprint(Stz* aDst, Stz* aSrc)
{
  Tsp* aTsp = (Tsp*)aSrc;
  
  if (aTsp->bIsNull())
    StrLib::vSprint(aDst,"null%c"_Stz,
                    '\0');
  else
    StrLib::vSprint(aDst,"TIMESTAMP '%02lld-%02lld-%02lld %02lld:%02lld:%02lld.%lld'%c"_Stz,
                    (i8)aTsp->x.m_uYear,
                    (i8)aTsp->x.m_uMonth,
                    (i8)aTsp->x.m_uDay,
                    (i8)aTsp->x.m_uHour,
                    (i8)aTsp->x.m_uMinute,
                    (i8)aTsp->x.m_uSecond,
                    (i8)aTsp->x.m_uMilli,
                    '\0');
}

/*----------------------------------------------------------------------------*/
/* sprint tsp datum in escaped form                                           */
/*----------------------------------------------------------------------------*/
void Tsp::vSprintEscaped(Stz* aDst, Stz* aSrc)
{
  Tsp* aTsp = (Tsp*)aSrc;
  
  if (aTsp->bIsNull())
    StrLib::vSprint(aDst,"null%c"_Stz,
                    '\0');
  else
    StrLib::vSprint(aDst,"%02lld-%02lld-%02lld %02lld:%02lld:%02lld.%03lld%c"_Stz,
                    (i8)aTsp->x.m_uYear,
                    (i8)aTsp->x.m_uMonth,
                    (i8)aTsp->x.m_uDay,
                    (i8)aTsp->x.m_uHour,
                    (i8)aTsp->x.m_uMinute,
                    (i8)aTsp->x.m_uSecond,
                    (i8)aTsp->x.m_uMilli,
                    '\0');
}

/*----------------------------------------------------------------------------*/
/* sprint tsp datum                                                           */
/*----------------------------------------------------------------------------*/
void Tsp::vSprintNonSQL(Stz* aDst, Stz* aSrc)
{
  Tsp* aTsp = (Tsp*)aSrc;
  
  if (aTsp->bIsNull())
    StrLib::vSprint(aDst,"null%c"_Stz,
                    '\0');
  else
    StrLib::vSprint(aDst,"%02lld-%02lld-%02lld %02lld:%02lld:%02lld.%03lld%c"_Stz,
                    (i8)aTsp->x.m_uYear,
                    (i8)aTsp->x.m_uMonth,
                    (i8)aTsp->x.m_uDay,
                    (i8)aTsp->x.m_uHour,
                    (i8)aTsp->x.m_uMinute,
                    (i8)aTsp->x.m_uSecond,
                    (i8)aTsp->x.m_uMilli,
                    '\0');
}

/*----------------------------------------------------------------------------*/
/*-- Embedded Comparators ----------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is tsp argA < aArgB?                                                       */
/*----------------------------------------------------------------------------*/
bool Tsp::bLessThan(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bLessThan;
  
  ASSERT(sizeof(i8) == nBytesA);
  ASSERT(sizeof(i8) == nBytesB);
  
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
bool Tsp::bLessEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bLessEqual;
  
  ASSERT(sizeof(i8) == nBytesA);
  ASSERT(sizeof(i8) == nBytesB);
  
  if (((Tsp*)aArgA)->bIsNull())
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
bool Tsp::bEqualTo(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bEqualTo;
  
  ASSERT(sizeof(i8) == nBytesA);
  ASSERT(sizeof(i8) == nBytesB);
  
  if (((Tsp*)aArgA)->bIsNull())
    bEqualTo = true;
  
  else if (((Tsp*)aArgB)->bIsNull())
    bEqualTo = false;
  
  else {
    i8 iA = *(i8*)aArgA;
    i8 iB = *(i8*)aArgB;
    
    if (iA == iB)
      bEqualTo = true;
    else
      bEqualTo = false;
  }
  
  return bEqualTo;
}

/*----------------------------------------------------------------------------*/
/* is tsp argA != aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Tsp::bNotEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bNotEqual;
  
  ASSERT(sizeof(i8) == nBytesA);
  ASSERT(sizeof(i8) == nBytesB);
  
  if (((Tsp*)aArgA)->bIsNull())
    bNotEqual = true;
  
  else if (((Tsp*)aArgB)->bIsNull())
    bNotEqual = false;
  
  else {
    i8 iA = *(i8*)aArgA;
    i8 iB = *(i8*)aArgB;
    
    if (iA != iB)
      bNotEqual = true;
    else
      bNotEqual = false;
  }
  
  return bNotEqual;
}

/*----------------------------------------------------------------------------*/
/* is tsp argA >= aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Tsp::bGreaterEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bGreaterEqual;
  
  ASSERT(sizeof(i8) == nBytesA);
  ASSERT(sizeof(i8) == nBytesB);
  
  if (((Tsp*)aArgA)->bIsNull())
    bGreaterEqual = true;
  
  else if (((Tsp*)aArgB)->bIsNull())
    bGreaterEqual = false;
  
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
bool Tsp::bGreaterThan(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bGreaterThan;
  
  ASSERT(sizeof(i8) == nBytesA);
  ASSERT(sizeof(i8) == nBytesB);
  
  if (((Tsp*)aArgA)->bIsNull())
    bGreaterThan = true;
  
  else if (((Tsp*)aArgB)->bIsNull())
    bGreaterThan = false;
  
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
i8 Tsp::iCmp(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  ASSERT(sizeof(i8) == nBytesA);
  ASSERT(sizeof(i8) == nBytesB);
  
  if (((Tsp*)aArgA)->bIsNull())
    return -1;
  
  if (((Tsp*)aArgB)->bIsNull())
    return 1;
  
  i8 iA = *(i8*)aArgA;
  i8 iB = *(i8*)aArgB;
 
  if (iA > iB)
    return 1;
  
  if (iA < iB)
    return -1;
    
  return 0;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
