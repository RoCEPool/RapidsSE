/*============================================================================*/
/* Dte.cpp -- date datum                                                      */
/*============================================================================*/

#include "Datums.h"

#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Dte::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  vFatal("fatal: Dte class cannot be new'd\n"_Str);
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Dte::Dte(void)
{
  vInitZero();
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Dte::~Dte(void)
{
}

/*----------------------------------------------------------------------------*/
/*-- Operators ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is this date less than given date?                                         */
/*----------------------------------------------------------------------------*/
bool Dte::operator < (Dte& xDte)
{
  if (*(i8*)&x < *(i8*)&xDte.x)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this date less than or equal to given date?                             */
/*----------------------------------------------------------------------------*/
bool Dte::operator <= (Dte& xDte)
{
  if (*(i8*)&x <= *(i8*)&xDte.x)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this date equal to given date?                                          */
/*----------------------------------------------------------------------------*/
bool Dte::operator == (Dte& xDte)
{
  if (*(i8*)&x == *(i8*)&xDte.x)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this date not equal to given date?                                      */
/*----------------------------------------------------------------------------*/
bool Dte::operator != (Dte& xDte)
{
  if (*(i8*)&x != *(i8*)&xDte.x)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this date greater than or equal to given date?                          */
/*----------------------------------------------------------------------------*/
bool Dte::operator >= (Dte& xDte)
{
  if (*(i8*)&x >= *(i8*)&xDte.x)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this date greater than given date?                                      */
/*----------------------------------------------------------------------------*/
bool Dte::operator > (Dte& xDte)
{
  if (*(i8*)&x > *(i8*)&xDte.x)
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
void Dte::vInitZero(void)
{
  x.m_uYear   = 0;
  x.m_uMonth  = 0;
  x.m_uDay    = 0;
}

/*----------------------------------------------------------------------------*/
/* get as i8                                                                  */
/*----------------------------------------------------------------------------*/
i8 Dte::iAsI8(void)
{
  i8 i = *(i8*)&x;
  
  return i;
}

/*----------------------------------------------------------------------------*/
/* get year                                                                   */
/*----------------------------------------------------------------------------*/
i8 Dte::iYear(void)
{
  return x.m_uYear;
}

/*----------------------------------------------------------------------------*/
/* get month                                                                  */
/*----------------------------------------------------------------------------*/
i8 Dte::iMonth(void)
{
  return x.m_uMonth;
}

/*----------------------------------------------------------------------------*/
/* get day                                                                    */
/*----------------------------------------------------------------------------*/
i8 Dte::iDay(void)
{
  return x.m_uDay;
}

/*----------------------------------------------------------------------------*/
/*-- Null --------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* test if null                                                               */
/*----------------------------------------------------------------------------*/
bool Dte::bIsNull(void)
{
  if (C_NULL_INT == *(i8*)&x)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* set to null                                                                */
/*----------------------------------------------------------------------------*/
void Dte::vSetNull(void)
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
/* extract a date from substr                                     yyyy-mo-ddZ */
/*----------------------------------------------------------------------------*/
bool Dte::bExtract(char** aaSrc, Dte* aDate, Column* pColumn)
{
  u8 uField;
  
  // skip leading whitespace
  while (' ' == **aaSrc)
    (*aaSrc)++;
  
  // test for null
  if ( ! **aaSrc || '|' == **aaSrc ) {
    aDate->vSetNull();
    return true;
  }
  
  aDate->vInitZero();
  
  if ( ! *aaSrc )
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - -
  // date portion
  
  // extract year
  if ( ! StrLib::bExtractU8(aaSrc,&uField) )
    return false;
  aDate->x.m_uYear = uField;
  
  // skip - separator
  if ('-' != *(*aaSrc)++)
    return false;
  
  // extract month
  if ( ! StrLib::bExtractU8(aaSrc,&uField) )
    return false;
  aDate->x.m_uMonth = uField;
  
  // skip - separator
  if ('-' != *(*aaSrc)++) return false;
  
  // extract day
  if ( ! StrLib::bExtractU8(aaSrc,&uField) )
    return false;
  aDate->x.m_uDay = uField;
  
  // skip Z separator
  if ('Z' == **aaSrc)
    (*aaSrc)++;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* valid date in substr?                                                      */
/*----------------------------------------------------------------------------*/
bool Dte::bValid(char** aaSrc, Column* pColumn)
{
  Dte xDte;
  
  bool bValid = bExtract(aaSrc,&xDte,pColumn);
  
  return bValid;
}

/*----------------------------------------------------------------------------*/
/*-- Embedded Comparators ----------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is dte argA < aArgB?                                                       */
/*----------------------------------------------------------------------------*/
bool Dte::bLessThan(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
/* is dte argA <= aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Dte::bLessEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
/* is dte argA == aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Dte::bEqualTo(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
/* is dte argA != aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Dte::bNotEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
/* is dte argA >= aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Dte::bGreaterEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
/* is dte argA > aArgB?                                                       */
/*----------------------------------------------------------------------------*/
bool Dte::bGreaterThan(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
i8 Dte::iCmp(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
