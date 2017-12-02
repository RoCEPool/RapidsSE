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
  
  vFatal("fatal: Dte class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
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
  x.m_uSpare  = 0;
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
/* copy dte datum                                                             */
/*----------------------------------------------------------------------------*/
i8 Dte::nCopy(void* aDst, void* aSrc)
{
  *(i8*)aDst = *(i8*)aSrc;
  
  return sizeof(i8);
}

/*----------------------------------------------------------------------------*/
/* copy dte datum                                                             */
/*----------------------------------------------------------------------------*/
void Dte::vCopy(void* aDst, void* aSrc)
{
  *(i8*)aDst = *(i8*)aSrc;
}

/*----------------------------------------------------------------------------*/
/* extract a date from substr                                     yyyy-mo-ddZ */
/*----------------------------------------------------------------------------*/
bool Dte::bExtract(char** aaSrc, Dte* aDte, Column* pColumn, Formatters* pFormatters)
{
  // refer: http://en.wikipedia.org/wiki/ISO_8601
  
  u8 uField;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  // skip leading spaces
  StrLib::vSkipSpaces(aaSrc);
   
  // is there any data ?
  if ( ! DtmLib::bIsThereData(**aaSrc,pFormatters) )
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  if (StrLib::iCompare(*aaSrc, "NULL", 4, C_NO_CASE) == 0) {
    
    *aaSrc = (*aaSrc) + 4;
    
    // skip trailing whitespace
    while (' ' == **aaSrc)
      (*aaSrc)++;
    
    if (**aaSrc
        && C_FS != **aaSrc
        && C_NL != **aaSrc
        && C_CR != **aaSrc)
      return false;
    
    if ( !pColumn->bIsNullable() )
      return false;
    
    aDte->vSetNull();
    return true;
  }
//  bool bValidNull;
//  if (DtmLib::bTestForNull(aaSrc,pColumn,pFormatters,bValidNull)) {
//    if ( ! bValidNull )
//      return false;
//    aDte->vSetNull();
//    return true;
//  }
  
  aDte->vInitZero();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // date field processing
  
  // extract year
  if ( ! StrLib::bExtractU8(aaSrc,&uField) )
    return false;
  aDte->x.m_uYear = uField;
  
  // skip - separator
  if ('-' != *(*aaSrc)++)
    return false;
  
  // extract month
  if ( ! StrLib::bExtractU8(aaSrc,&uField) )
    return false;
  aDte->x.m_uMonth = uField;
  
  // skip - separator
  if ('-' != *(*aaSrc)++) return false;
  
  // extract day
  if ( ! StrLib::bExtractU8(aaSrc,&uField) )
    return false;
  aDte->x.m_uDay = uField;
  
  // skip optional Z (UTC zone designator)
  if ('Z' == **aaSrc)
    (*aaSrc)++;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  // skip trailing spaces
  StrLib::vSkipSpaces(aaSrc);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* sprint dte datum                                                           */
/*----------------------------------------------------------------------------*/
void Dte::vSprint(Stz* aDst, Stz* aSrc)
{
  Dte* aDte = (Dte*)aSrc;
  
  if (aDte->bIsNull())
    StrLib::vSprint(aDst,"null%c"_Stz,
                    '\0');
  else
    StrLib::vSprint(aDst,"%02lld-%02lld-%02lld%c"_Stz,
                    aDte->x.m_uYear,
                    aDte->x.m_uMonth,
                    aDte->x.m_uDay,
                    '\0');
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
  
  if (((Dte*)aArgA)->bIsNull())
    bLessThan = true;
  
  else if (((Dte*)aArgB)->bIsNull())
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
/* is dte argA <= aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Dte::bLessEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bLessEqual;
  
  ASSERT(sizeof(i8) == nBytesA);
  ASSERT(sizeof(i8) == nBytesB);
  
  if (((Dte*)aArgA)->bIsNull())
    bLessEqual = true;
  
  else if (((Dte*)aArgB)->bIsNull())
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
/* is dte argA == aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Dte::bEqualTo(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bEqualTo;
  
  ASSERT(sizeof(i8) == nBytesA);
  ASSERT(sizeof(i8) == nBytesB);
  
  if (((Dte*)aArgA)->bIsNull())
    bEqualTo = false;
  
  else if (((Dte*)aArgB)->bIsNull())
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
/* is dte argA != aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Dte::bNotEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bNotEqual;
  
  ASSERT(sizeof(i8) == nBytesA);
  ASSERT(sizeof(i8) == nBytesB);
  
  if (((Dte*)aArgA)->bIsNull())
    bNotEqual = true;
  
  else if (((Dte*)aArgB)->bIsNull())
    bNotEqual = true;
  
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
/* is dte argA >= aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Dte::bGreaterEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bGreaterEqual;
  
  ASSERT(sizeof(i8) == nBytesA);
  ASSERT(sizeof(i8) == nBytesB);
  
  if (((Dte*)aArgA)->bIsNull())
    bGreaterEqual = false;
  
  else if (((Dte*)aArgB)->bIsNull())
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
/* is dte argA > aArgB?                                                       */
/*----------------------------------------------------------------------------*/
bool Dte::bGreaterThan(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bGreaterThan;
  
  ASSERT(sizeof(i8) == nBytesA);
  ASSERT(sizeof(i8) == nBytesB);
  
  if (((Dte*)aArgA)->bIsNull())
    bGreaterThan = false;
  
  else if (((Dte*)aArgB)->bIsNull())
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
i8 Dte::iCmp(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  ASSERT(sizeof(i8) == nBytesA);
  ASSERT(sizeof(i8) == nBytesB);
  
  if (((Dte*)aArgA)->bIsNull())
    return -1;
  
  if (((Dte*)aArgB)->bIsNull())
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
