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
void* Ivl::operator new(size_t)
{
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
Ivl::~Ivl(void) noexcept(false)
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
  if (m_iValue < xIvl.m_iValue)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this interval less than or equal to given interval?                     */
/*----------------------------------------------------------------------------*/
bool Ivl::operator <= (Ivl& xIvl)
{
  if (m_iValue <= xIvl.m_iValue)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this interval equal to given interval?                                  */
/*----------------------------------------------------------------------------*/
bool Ivl::operator == (Ivl& xIvl)
{
  if (m_iValue == xIvl.m_iValue)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this interval not equal to given interval?                              */
/*----------------------------------------------------------------------------*/
bool Ivl::operator != (Ivl& xIvl)
{
  if (m_iValue != xIvl.m_iValue)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this interval greater than or equal to given interval?                  */
/*----------------------------------------------------------------------------*/
bool Ivl::operator >= (Ivl& xIvl)
{
  if (m_iValue >= xIvl.m_iValue)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this interval greater than given interval?                              */
/*----------------------------------------------------------------------------*/
bool Ivl::operator > (Ivl& xIvl)
{
  if (m_iValue > xIvl.m_iValue)
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
  return m_iValue;
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
  if (C_DATUM_NULL == m_iValue)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* set to null                                                                */
/*----------------------------------------------------------------------------*/
void Ivl::vSetNull(void)
{
  m_iValue = C_DATUM_NULL;
}

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
/* extract an interval field from csv substr                                  */
/*----------------------------------------------------------------------------*/
bool Ivl::bExtractIvlSupplied(char** aaSrc, Stz* aDst, Column* pColumn, Formatters* pFormatters)
{
  Ivl* aIvl = (Ivl*)aDst;
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  bool bSawEnclosedLeading =
    StrLib::bSkipLeading(aaSrc,pFormatters->m_cEnclosedBy_Leading);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (DtmLib::bTestForNull(pColumn,aaSrc,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    aIvl->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // interval field processing
  
  i8 iSecond, iMilli;
  
  if ( ! StrLib::bExtractInterval(aaSrc, iSecond, iMilli) )
    return false;
  
  aIvl->x.m_uSecond = iSecond;
  aIvl->x.m_uMilli  = iMilli;
  
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
/* extract an interval field from csv substr                                  */
/*----------------------------------------------------------------------------*/
bool Ivl::bExtractIvlDefault(char** aaSrc, Stz* aDst, Column* pColumn, Formatters* pFormatters)
{
  Ivl* aIvl = (Ivl*)aDst;
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  StrLib::vSkipSpaces(aaSrc);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (DtmLib::bTestForNullDefault(pColumn,aaSrc,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    aIvl->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // interval field processing
  
  i8 iSecond, iMilli;
  
  if ( ! StrLib::bExtractInterval(aaSrc, iSecond, iMilli) )
    return false;
  
  aIvl->x.m_uSecond = iSecond;
  aIvl->x.m_uMilli  = iMilli;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  StrLib::vSkipSpaces(aaSrc);
  
  // should see a valid field end
  if ( ! DtmLib::bValidFieldEndDefault(*aaSrc,pFormatters) )
    return false;
  
  return true;
}

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
/*-- Embedded Comparators ----------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is ivl argA < aArgB?                                                       */
/*----------------------------------------------------------------------------*/
bool Ivl::bLessThan(char* aArgA, char* aArgB)
{
  bool bLessThan;
  
  if (((Ivl*)aArgA)->bIsNull())
    bLessThan = true;
  
  else if (((Ivl*)aArgB)->bIsNull())
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
/* is ivl argA <= aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Ivl::bLessEqual(char* aArgA, char* aArgB)
{
  bool bLessEqual;
  
  if (((Ivl*)aArgA)->bIsNull() && ((Ivl*)aArgB)->bIsNull())
    bLessEqual = true;
  
  else if (((Ivl*)aArgA)->bIsNull())
    bLessEqual = true;
  
  else if (((Int*)aArgB)->bIsNull())
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
/* is ivl argA == aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Ivl::bEqualTo(char* aArgA, char* aArgB)
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
/* is ivl argA != aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Ivl::bNotEqual(char* aArgA, char* aArgB)
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
/* is ivl argA >= aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Ivl::bGreaterEqual(char* aArgA, char* aArgB)
{
  bool bGreaterEqual;
  
  if (((Ivl*)aArgA)->bIsNull() && ((Ivl*)aArgB)->bIsNull())
    bGreaterEqual = true;
  
  else if (((Ivl*)aArgA)->bIsNull())
    bGreaterEqual = false;
  
  else if (((Ivl*)aArgB)->bIsNull())
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
/* is ivl argA > aArgB?                                                       */
/*----------------------------------------------------------------------------*/
bool Ivl::bGreaterThan(char* aArgA, char* aArgB)
{
  bool bGreaterThan;
  
  if (((Ivl*)aArgA)->bIsNull())
    bGreaterThan = false;
  
  else if (((Ivl*)aArgB)->bIsNull())
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
i8 Ivl::iCmp(char* aArgA, char* aArgB)
{
  //----------------------------------------------------------------------------
  // check for nulls first
  if (((Ivl*)aArgA)->bIsNull() && ((Ivl*)aArgB)->bIsNull())
    return 0;
  
  if (((Ivl*)aArgA)->bIsNull())
    return -1;
  
  if (((Ivl*)aArgB)->bIsNull())
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
