/*============================================================================*/
/* Ivl.cpp -- interval datum                                                  */
/*============================================================================*/

#include "Datums.h"

#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Ivl::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  vFatal("fatal: Ivl class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Ivl::Ivl(void)
{
  vInitZero();
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Ivl::~Ivl(void)
{
}

/*----------------------------------------------------------------------------*/
/*-- Operators ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is this interval less than given interval?                                 */
/*----------------------------------------------------------------------------*/
bool Ivl::operator < (Ivl& xIvl)
{
  if (*(i8*)&x < *(i8*)&xIvl.x)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this interval less than or equal to given interval?                     */
/*----------------------------------------------------------------------------*/
bool Ivl::operator <= (Ivl& xIvl)
{
  if (*(i8*)&x <= *(i8*)&xIvl.x)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this interval equal to given interval?                                  */
/*----------------------------------------------------------------------------*/
bool Ivl::operator == (Ivl& xIvl)
{
  if (*(i8*)&x == *(i8*)&xIvl.x)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this interval not equal to given interval?                              */
/*----------------------------------------------------------------------------*/
bool Ivl::operator != (Ivl& xIvl)
{
  if (*(i8*)&x != *(i8*)&xIvl.x)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this interval greater than or equal to given interval?                  */
/*----------------------------------------------------------------------------*/
bool Ivl::operator >= (Ivl& xIvl)
{
  if (*(i8*)&x >= *(i8*)&xIvl.x)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this interval greater than given interval?                              */
/*----------------------------------------------------------------------------*/
bool Ivl::operator > (Ivl& xIvl)
{
  if (*(i8*)&x > *(i8*)&xIvl.x)
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
void Ivl::vInitZero(void)
{
  x.m_uSecond   = 0;
  x.m_uMilli = 0;
}

/*----------------------------------------------------------------------------*/
/* get as i8                                                                  */
/*----------------------------------------------------------------------------*/
i8 Ivl::iAsI8(void)
{
  i8 i = *(i8*)&x;
  
  return i;
}

/*----------------------------------------------------------------------------*/
/* get second                                                                 */
/*----------------------------------------------------------------------------*/
i8 Ivl::iSecond(void)
{
  return x.m_uSecond;
}

/*----------------------------------------------------------------------------*/
/* get milli                                                                  */
/*----------------------------------------------------------------------------*/
i8 Ivl::iMilli(void)
{
  return x.m_uMilli;
}

/*----------------------------------------------------------------------------*/
/* set members                                                                */
/*----------------------------------------------------------------------------*/
void Ivl::vSet(u8 uSecond, u8 uMilli)
{
  x.m_uSecond = uSecond;
  x.m_uMilli  = uMilli;
}

/*----------------------------------------------------------------------------*/
/*-- Null --------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* test if null                                                               */
/*----------------------------------------------------------------------------*/
bool Ivl::bIsNull(void)
{
  if (C_NULL_INT == *(i8*)&x)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* set to null                                                                */
/*----------------------------------------------------------------------------*/
void Ivl::vSetNull(void)
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
i8 Ivl::nCopy(void* aDst, void* aSrc)
{
  *(i8*)aDst = *(i8*)aSrc;
  
  return sizeof(i8);
}

/*----------------------------------------------------------------------------*/
/* copy ivl datum                                                             */
/*----------------------------------------------------------------------------*/
void Ivl::vCopy(void* aDst, void* aSrc)
{
  *(i8*)aDst = *(i8*)aSrc;
}

/*----------------------------------------------------------------------------*/
/* extract an interval from substr                                            */
/*----------------------------------------------------------------------------*/
#ifdef _ROWFORMAT
bool Ivl::bExtract(char* aSrc, Ivl* aInterval, i8 iLength, Column* pColumn)
{
  u8 uField;
  i8 i_Length = 1;
  
  // skip leading whitespace
  while (' ' == *aSrc) {
    aSrc++;
    i_Length++;
  }
  
  // test for null
  if ( ! *aSrc && i_Length == iLength ) {
    aInterval->vSetNull();
    return true;
  }
  
  aInterval->vInitZero();
  
  if ( ! *aSrc )
    return false;
  
  // extract second
  if ( ! StrLib::bExtractU8(&aSrc,&uField) )
    return true;
  aInterval->x.m_uSecond = uField;
  
  //  // skip . separator
  //  if ('.' != *(*aaSrc)++)
  //    return true;
  
  // extract fraction (assuming milliseconds)
  if ('.' == *aSrc) {
    aSrc++;
    if ( StrLib::bExtractU8(&aSrc,&uField) )
      aInterval->x.m_uMilli = uField;
  }
  
  return true;
}

#else
bool Ivl::bExtract(char** aaSrc, Ivl* aInterval, Column* pColumn)
{
  u8 uField;
  
  // skip leading whitespace
  while (' ' == **aaSrc)
    (*aaSrc)++;
  
  // test for null
  if ( ! **aaSrc || '|' == **aaSrc ) {
    aInterval->vSetNull();
    return true;
  }
  
  aInterval->vInitZero();
  
  if ( ! *aaSrc )
    return false;
  
  // extract second
  if ( ! StrLib::bExtractU8(aaSrc,&uField) )
    return true;
  aInterval->x.m_uSecond = uField;
  
  //  // skip . separator
  //  if ('.' != *(*aaSrc)++)
  //    return true;
  
  // extract fraction (assuming milliseconds)
  if ('.' == **aaSrc) {
    (*aaSrc)++;
    if ( StrLib::bExtractU8(aaSrc,&uField) )
      aInterval->x.m_uMilli = uField;
  }
  
  return true;
}
#endif

/*----------------------------------------------------------------------------*/
/* sprint ivl datum                                                           */
/*----------------------------------------------------------------------------*/
void Ivl::vSprint(Stz* aDst, Stz* aSrc)
{
  Ivl* aIvl = (Ivl*)aSrc;
  
  StrLib::vSprint(aDst,"%lld.%03lld%c"_Stz,
                  aIvl->x.m_uSecond,
                  aIvl->x.m_uMilli,
                  '\0');
}

/*----------------------------------------------------------------------------*/
/* valid interval in substr?                                                  */
/*----------------------------------------------------------------------------*/
#ifdef _ROWFORMAT
bool Ivl::bValid(char* aSrc, i8 iLength, Column* pColumn)
{
  Ivl xIvl;
  
  bool bValid = bExtract(aSrc,&xIvl,iLength,pColumn);
  
  return bValid;
}

#else
bool Ivl::bValid(char** aaSrc, Column* pColumn)
{
  Ivl xIvl;
  
  bool bValid = bExtract(aaSrc,&xIvl,pColumn);
  
  return bValid;
}
#endif

/*----------------------------------------------------------------------------*/
/*-- Embedded Comparators ----------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is ivl argA < aArgB?                                                       */
/*----------------------------------------------------------------------------*/
bool Ivl::bLessThan(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
/* is ivl argA <= aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Ivl::bLessEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
/* is ivl argA == aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Ivl::bEqualTo(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
/* is ivl argA != aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Ivl::bNotEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
/* is ivl argA >= aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Ivl::bGreaterEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
/* is ivl argA > aArgB?                                                       */
/*----------------------------------------------------------------------------*/
bool Ivl::bGreaterThan(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
i8 Ivl::iCmp(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
