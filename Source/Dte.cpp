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
void* Dte::operator new(size_t)
{
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
Dte::~Dte(void) noexcept(false)
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
  if (m_iValue < xDte.m_iValue)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this date less than or equal to given date?                             */
/*----------------------------------------------------------------------------*/
bool Dte::operator <= (Dte& xDte)
{
  if (m_iValue <= xDte.m_iValue)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this date equal to given date?                                          */
/*----------------------------------------------------------------------------*/
bool Dte::operator == (Dte& xDte)
{
  if (m_iValue == xDte.m_iValue)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this date not equal to given date?                                      */
/*----------------------------------------------------------------------------*/
bool Dte::operator != (Dte& xDte)
{
  if (m_iValue != xDte.m_iValue)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this date greater than or equal to given date?                          */
/*----------------------------------------------------------------------------*/
bool Dte::operator >= (Dte& xDte)
{
  if (m_iValue >= xDte.m_iValue)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this date greater than given date?                                      */
/*----------------------------------------------------------------------------*/
bool Dte::operator > (Dte& xDte)
{
  if (m_iValue > xDte.m_iValue)
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
  x.m_uYearDelta   = 0;
  x.m_uMonth  = 0;
  x.m_uDay    = 0;
  x.m_uSpare  = 0;
}

/*----------------------------------------------------------------------------*/
/* get as i8                                                                  */
/*----------------------------------------------------------------------------*/
i8 Dte::iAsI8(void)
{
  return m_iValue;
}

/*----------------------------------------------------------------------------*/
/* set as i8                                                                  */
/*----------------------------------------------------------------------------*/
void Dte::vAsI8(i8 iAsI8)
{
  m_iValue = iAsI8;
}

/*----------------------------------------------------------------------------*/
/* get year                                                                   */
/*----------------------------------------------------------------------------*/
i8 Dte::iYear(void)
{
  return x.m_uYearDelta + C_BASE_YEAR;
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
  if (C_DATUM_NULL == m_iValue)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* set to null                                                                */
/*----------------------------------------------------------------------------*/
void Dte::vSetNull(void)
{
  m_iValue = C_DATUM_NULL;
}

/*----------------------------------------------------------------------------*/
/* sprint dte datum                                                           */
/*----------------------------------------------------------------------------*/
void Dte::vSprint(Stz* aDst)
{
  if (bIsNull()) {
    StrLib::vSprint(aDst,"null%c"_Stz,'\0');
    return;
  }
  
  StrLib::vSprint(aDst,"%02lld-%02lld-%02lld%c"_Stz,
                  (i8)x.m_uYearDelta + C_BASE_YEAR,
                  (i8)x.m_uMonth,
                  (i8)x.m_uDay,
                  '\0');
}

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
                    aDte->x.m_uYearDelta + C_BASE_YEAR,
                    aDte->x.m_uMonth,
                    aDte->x.m_uDay,
                    '\0');
}

/*----------------------------------------------------------------------------*/
/* sprint dte datum                                                           */
/*----------------------------------------------------------------------------*/
void Dte::vSprintEscaped(Stz* aDst, Stz* aSrc)
{
  Dte* aDte = (Dte*)aSrc;
  
  if (aDte->bIsNull())
    StrLib::vSprint(aDst,"null%c"_Stz,
                    '\0');
  else
    StrLib::vSprint(aDst,"%02lld-%02lld-%02lld%c"_Stz,
                    aDte->x.m_uYearDelta + C_BASE_YEAR,
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
bool Dte::bLessThan(char* aArgA, char* aArgB)
{
  bool bLessThan;
  
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
bool Dte::bLessEqual(char* aArgA, char* aArgB)
{
  bool bLessEqual;
  
  if (((Dte*)aArgA)->bIsNull() && ((Dte*)aArgB)->bIsNull())
    bLessEqual = true;
  
  else if (((Dte*)aArgA)->bIsNull())
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
bool Dte::bEqualTo(char* aArgA, char* aArgB)
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
/* is dte argA != aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Dte::bNotEqual(char* aArgA, char* aArgB)
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
/* is dte argA >= aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Dte::bGreaterEqual(char* aArgA, char* aArgB)
{
  bool bGreaterEqual;
  
  if (((Dte*)aArgA)->bIsNull() && ((Dte*)aArgB)->bIsNull())
    bGreaterEqual = true;
  
  else if (((Dte*)aArgA)->bIsNull())
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
bool Dte::bGreaterThan(char* aArgA, char* aArgB)
{
  bool bGreaterThan;
  
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
i8 Dte::iCmp(char* aArgA, char* aArgB)
{
  //----------------------------------------------------------------------------
  // check for nulls first
  if (((Dte*)aArgA)->bIsNull() && ((Dte*)aArgB)->bIsNull())
    return 0;
  
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
