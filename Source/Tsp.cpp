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
  
  vFatal("fatal: Tsp class cannot be new'd\n"_Str);
  
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
/* extract a timestamp from substr                   yyyy-mo-ddThh:mi:ss.fffZ */
/*----------------------------------------------------------------------------*/
bool Tsp::bExtract(char** aaSrc, Tsp* aTimestamp, Column* pColumn)
{
  // refer: http://en.wikipedia.org/wiki/ISO_8601
  
  u8 uField;
  
  // skip leading whitespace
  while (' ' == **aaSrc)
    (*aaSrc)++;
  
  // test for null
  if ( ! **aaSrc || '|' == **aaSrc ) {
    aTimestamp->vSetNull();
    return true;
  }
  
  aTimestamp->vInitZero();
  
  if ( ! *aaSrc )
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - -
  // date portion
  
  // extract year
  if ( ! StrLib::bExtractU8(aaSrc,&uField) )
    return false;
  aTimestamp->x.m_uYear = uField;
  
  // skip - separator
  if ('-' != *(*aaSrc)++)
    return false;
  
  // extract month
  if ( ! StrLib::bExtractU8(aaSrc,&uField) )
    return false;
  aTimestamp->x.m_uMonth = uField;
  
  // skip - separator
  if ('-' != *(*aaSrc)++) return false;
  
  // extract day
  if ( ! StrLib::bExtractU8(aaSrc,&uField) )
    return false;
  aTimestamp->x.m_uDay = uField;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - -
  // time portion
  
  // skip T or space separator
  if ( ! ('T' == **aaSrc || ' ' == **aaSrc) )
    return true;
  (*aaSrc)++;
  
  // extract hour
  if ( ! StrLib::bExtractU8(aaSrc,&uField) )
    return true;
  aTimestamp->x.m_uHour = uField;
  
  // skip : separator
  if (':' != *(*aaSrc)++)
    return true;
  
  // extract minute
  if ( ! StrLib::bExtractU8(aaSrc,&uField) )
    return true;
  aTimestamp->x.m_uMinute = uField;
  
  // skip : separator
  if (':' != *(*aaSrc)++)
    return true;
  
  // extract second
  if ( ! StrLib::bExtractU8(aaSrc,&uField) )
    return true;
  aTimestamp->x.m_uSecond = uField;
  
  // extract fraction (assuming milliseconds)
  if ('.' == **aaSrc) {
    (*aaSrc)++;
    if ( StrLib::bExtractU8(aaSrc,&uField) )
      aTimestamp->x.m_uMilli = uField;
  }
  
  // skip Z separator
  if ('Z' == **aaSrc)
    (*aaSrc)++;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* sprint tsp datum                                                           */
/*----------------------------------------------------------------------------*/
void Tsp::vSprint(Str* aDst, Str* aSrc)
{
  Tsp* aTsp = (Tsp*)aSrc;
  
  StrLib::vSprint(aDst,"%02lld-%02lld-%02lld %02lld:%02lld:%02lld.%03lld%c"_Str,
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
/* valid timestamp in substr?                                                 */
/*----------------------------------------------------------------------------*/
bool Tsp::bValid(char** aaSrc, Column* pColumn)
{
  Tsp xTsp;
  
  bool bValid = bExtract(aaSrc,&xTsp,pColumn);
  
  return bValid;
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
  
  i8 iA = *(i8*)aArgA;
  i8 iB = *(i8*)aArgB;
  
  if (iA < iB)
    bLessThan = true;
  else
    bLessThan = false;
  
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
  
  i8 iA = *(i8*)aArgA;
  i8 iB = *(i8*)aArgB;
  
  if (iA <= iB)
    bLessEqual = true;
  else
    bLessEqual = false;
  
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
bool Tsp::bNotEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bNotEqual;
  
  ASSERT(sizeof(i8) == nBytesA);
  ASSERT(sizeof(i8) == nBytesB);
  
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
bool Tsp::bGreaterEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bGreaterEqual;
  
  ASSERT(sizeof(i8) == nBytesA);
  ASSERT(sizeof(i8) == nBytesB);
  
  i8 iA = *(i8*)aArgA;
  i8 iB = *(i8*)aArgB;
  
  if (iA >= iB)
    bGreaterEqual = true;
  else
    bGreaterEqual = false;
  
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
  
  i8 iA = *(i8*)aArgA;
  i8 iB = *(i8*)aArgB;
  
  if (iA > iB)
    bGreaterThan = true;
  else
    bGreaterThan = false;
  
  return bGreaterThan;
}

/*----------------------------------------------------------------------------*/
/* cmp argA with argB (-ve: argA < argB, 0: argA == argB, +ve: argA > argB)   */
/*----------------------------------------------------------------------------*/
i8 Tsp::iCmp(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  ASSERT(sizeof(i8) == nBytesA);
  ASSERT(sizeof(i8) == nBytesB);
  
  i8 iA = *(i8*)aArgA;
  i8 iB = *(i8*)aArgB;
  
  i8 iCmp = iA - iB;
  
  return iCmp;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
