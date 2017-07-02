/*============================================================================*/
/* Tme.cpp -- time datum                                                      */
/*============================================================================*/

#include "Datums.h"

#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Tme::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  vFatal("fatal: Tme class cannot be new'd\n"_Str);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Tme::Tme(void)
{
  vInitZero();
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Tme::~Tme(void)
{
}

/*----------------------------------------------------------------------------*/
/*-- Operators ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is this time less than given time?                                         */
/*----------------------------------------------------------------------------*/
bool Tme::operator < (Tme& xTme)
{
  if (*(i8*)&x < *(i8*)&xTme.x)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this time less than or equal to given time?                             */
/*----------------------------------------------------------------------------*/
bool Tme::operator <= (Tme& xTme)
{
  if (*(i8*)&x <= *(i8*)&xTme.x)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this time equal to given time?                                          */
/*----------------------------------------------------------------------------*/
bool Tme::operator == (Tme& xTme)
{
  if (*(i8*)&x == *(i8*)&xTme.x)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this time not equal to given time?                                      */
/*----------------------------------------------------------------------------*/
bool Tme::operator != (Tme& xTme)
{
  if (*(i8*)&x != *(i8*)&xTme.x)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this time greater than or equal to given time?                          */
/*----------------------------------------------------------------------------*/
bool Tme::operator >= (Tme& xTme)
{
  if (*(i8*)&x >= *(i8*)&xTme.x)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this time greater than given time?                                      */
/*----------------------------------------------------------------------------*/
bool Tme::operator > (Tme& xTme)
{
  if (*(i8*)&x > *(i8*)&xTme.x)
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
void Tme::vInitZero(void)
{
  x.m_uHour   = 0;
  x.m_uMinute = 0;
  x.m_uSecond = 0;
  x.m_uMilli  = 0;
}

/*----------------------------------------------------------------------------*/
/* get as i8                                                                  */
/*----------------------------------------------------------------------------*/
i8 Tme::iAsI8(void)
{
  i8 i = *(i8*)&x;
  
  return i;
}

/*----------------------------------------------------------------------------*/
/* get hour                                                                   */
/*----------------------------------------------------------------------------*/
i8 Tme::iHour(void)
{
  return x.m_uHour;
}

/*----------------------------------------------------------------------------*/
/* get minute                                                                 */
/*----------------------------------------------------------------------------*/
i8 Tme::iMinute(void)
{
  return x.m_uMinute;
}

/*----------------------------------------------------------------------------*/
/* get second                                                                 */
/*----------------------------------------------------------------------------*/
i8 Tme::iSecond(void)
{
  return x.m_uSecond;
}

/*----------------------------------------------------------------------------*/
/* get milli                                                                  */
/*----------------------------------------------------------------------------*/
i8 Tme::iMilli(void)
{
  return x.m_uMilli;
}

/*----------------------------------------------------------------------------*/
/*-- Null --------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* test if null                                                               */
/*----------------------------------------------------------------------------*/
bool Tme::bIsNull(void)
{
  if (C_NULL_INT == *(i8*)&x)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* set to null                                                                */
/*----------------------------------------------------------------------------*/
void Tme::vSetNull(void)
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
/* copy ivl datum                                                             */
/*----------------------------------------------------------------------------*/
i8 Tme::nCopy(void* aDst, void* aSrc)
{
  *(i8*)aDst = *(i8*)aSrc;
  
  return sizeof(i8);
}

/*----------------------------------------------------------------------------*/
/* copy tme datum                                                             */
/*----------------------------------------------------------------------------*/
void Tme::vCopy(void* aDst, void* aSrc)
{
  *(i8*)aDst = *(i8*)aSrc;
}

/*----------------------------------------------------------------------------*/
/* extract a time from substr                                   hh:mi:ss.fffZ */
/*----------------------------------------------------------------------------*/
bool Tme::bExtract(char** aaSrc, Tme* aTime, Column* pColumn)
{
  u8 uField;
  
  // skip leading whitespace
  while (' ' == **aaSrc)
    (*aaSrc)++;
  
  // test for null
  if ( ! **aaSrc || '|' == **aaSrc ) {
    aTime->vSetNull();
    return true;
  }
  
  aTime->vInitZero();
  
  if ( ! *aaSrc )
    return false;
  
  // extract hour
  if ( ! StrLib::bExtractU8(aaSrc,&uField) )
    return true;
  aTime->x.m_uHour = uField;
  
  // skip : separator
  if (':' != *(*aaSrc)++)
    return true;
  
  // extract minute
  if ( ! StrLib::bExtractU8(aaSrc,&uField) )
    return true;
  aTime->x.m_uMinute = uField;
  
  // skip : separator
  if (':' != *(*aaSrc)++)
    return true;
  
  // extract second
  if ( ! StrLib::bExtractU8(aaSrc,&uField) )
    return true;
  aTime->x.m_uSecond = uField;
  
  // extract fraction (assuming milliseconds)
  if ('.' == **aaSrc) {
    (*aaSrc)++;
    if ( StrLib::bExtractU8(aaSrc,&uField) )
      aTime->x.m_uMilli = uField;
  }
  
  // skip Z separator
  if ('Z' == **aaSrc)
    (*aaSrc)++;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* sprint tme datum                                                           */
/*----------------------------------------------------------------------------*/
void Tme::vSprint(Str* aDst, Str* aSrc)
{
  Tme* aTme = (Tme*)aSrc;
  
  StrLib::vSprint(aDst,"%02lld:%02lld:%02lld.%03lld%c"_Str,
                  aTme->x.m_uHour,
                  aTme->x.m_uMinute,
                  aTme->x.m_uSecond,
                  aTme->x.m_uMilli,
                  '\0');
}

/*----------------------------------------------------------------------------*/
/* valid time in substr?                                                      */
/*----------------------------------------------------------------------------*/
bool Tme::bValid(char** aaSrc, Column* pColumn)
{
  Tme xTme;
  
  bool bValid = bExtract(aaSrc,&xTme,pColumn);
  
  return bValid;
}

/*----------------------------------------------------------------------------*/
/*-- Embedded Comparators ----------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is tme argA < aArgB?                                                       */
/*----------------------------------------------------------------------------*/
bool Tme::bLessThan(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
/* is tme argA <= aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Tme::bLessEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
/* is tme argA == aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Tme::bEqualTo(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
/* is tme argA != aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Tme::bNotEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
/* is tme argA >= aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Tme::bGreaterEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
  
  Tme xTmeA;
  Tme xTmeB;
  
  *(i8*)&xTmeA.x = iA;
  *(i8*)&xTmeB.x = iB;
  
  vTrace("%d:%d:%d.%d >= %d:%d:%d.%d %s\n"_Str,
         xTmeA.iHour(),
         xTmeA.iMinute(),
         xTmeA.iSecond(),
         xTmeA.iMilli(),

         xTmeB.iHour(),
         xTmeB.iMinute(),
         xTmeB.iSecond(),
         xTmeB.iMilli(),
         
         bGreaterEqual ? "true" : "false"
         );
  
  return bGreaterEqual;
}

/*----------------------------------------------------------------------------*/
/* is tme argA > aArgB?                                                       */
/*----------------------------------------------------------------------------*/
bool Tme::bGreaterThan(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
i8 Tme::iCmp(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
