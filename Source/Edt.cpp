/*============================================================================*/
/* Edt.cpp -- era based date                                                  */
/*============================================================================*/

#include "Datums.h"

#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Edt::operator new(size_t)
{
  vFatal("fatal: Edt class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Edt::Edt(void)
{
  vInitZero();
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Edt::~Edt(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Operators ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is this date less than given date?                                         */
/*----------------------------------------------------------------------------*/
bool Edt::operator < (Edt& xEdt)
{
  if (m_iValue < xEdt.m_iValue)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this date less than or equal to given date?                             */
/*----------------------------------------------------------------------------*/
bool Edt::operator <= (Edt& xEdt)
{
  if (m_iValue <= xEdt.m_iValue)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this date equal to given date?                                          */
/*----------------------------------------------------------------------------*/
bool Edt::operator == (Edt& xEdt)
{
  if (m_iValue == xEdt.m_iValue)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this date not equal to given date?                                      */
/*----------------------------------------------------------------------------*/
bool Edt::operator != (Edt& xEdt)
{
  if (m_iValue != xEdt.m_iValue)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this date greater than or equal to given date?                          */
/*----------------------------------------------------------------------------*/
bool Edt::operator >= (Edt& xEdt)
{
  if (m_iValue >= xEdt.m_iValue)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this date greater than given date?                                      */
/*----------------------------------------------------------------------------*/
bool Edt::operator > (Edt& xEdt)
{
  if (m_iValue > xEdt.m_iValue)
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
void Edt::vInitZero(void)
{
  x.m_uYearInEra   = 0;
  x.m_uMonth  = 0;
  x.m_uDay    = 0;
  x.m_uSpare  = 0;
}

/*----------------------------------------------------------------------------*/
/* get as i8                                                                  */
/*----------------------------------------------------------------------------*/
i8 Edt::iAsI8(void)
{
  return m_iValue;
}

/*----------------------------------------------------------------------------*/
/* set as i8                                                                  */
/*----------------------------------------------------------------------------*/
void Edt::vAsI8(i8 iAsI8)
{
  m_iValue = iAsI8;
}

/*----------------------------------------------------------------------------*/
/* get year                                                                   */
/*----------------------------------------------------------------------------*/
i8 Edt::iYear(void)
{
  return x.m_uYearInEra + C_BASE_YEAR;
}

/*----------------------------------------------------------------------------*/
/* get month                                                                  */
/*----------------------------------------------------------------------------*/
i8 Edt::iMonth(void)
{
  return x.m_uMonth;
}

/*----------------------------------------------------------------------------*/
/* get day                                                                    */
/*----------------------------------------------------------------------------*/
i8 Edt::iDay(void)
{
  return x.m_uDay;
}

/*----------------------------------------------------------------------------*/
/*-- Null --------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* test if null                                                               */
/*----------------------------------------------------------------------------*/
bool Edt::bIsNull(void)
{
  if (C_DATUM_NULL == m_iValue)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* set to null                                                                */
/*----------------------------------------------------------------------------*/
void Edt::vSetNull(void)
{
  m_iValue = C_DATUM_NULL;
}

/*----------------------------------------------------------------------------*/
/* sprint Edt datum                                                           */
/*----------------------------------------------------------------------------*/
void Edt::vSprint(Stz* aDst)
{
  if (bIsNull()) {
    StrLib::vSprint(aDst,"null%c"_Stz,'\0');
    return;
  }
  
  StrLib::vSprint(aDst,"%02lld-%02lld-%02lld%c"_Stz,
                  (i8)x.m_uYearInEra + C_BASE_YEAR,
                  (i8)x.m_uMonth,
                  (i8)x.m_uDay,
                  '\0');
}

/*----------------------------------------------------------------------------*/
/*-- Extract and Sprint ------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* copy Edt datum                                                             */
/*----------------------------------------------------------------------------*/
i8 Edt::nCopy(void* aDst, void* aSrc)
{
  *(i8*)aDst = *(i8*)aSrc;
  
  return sizeof(i8);
}

/*----------------------------------------------------------------------------*/
/* copy Edt datum                                                             */
/*----------------------------------------------------------------------------*/
void Edt::vCopy(void* aDst, void* aSrc)
{
  *(i8*)aDst = *(i8*)aSrc;
}

/*----------------------------------------------------------------------------*/
/* sprint Edt datum                                                           */
/*----------------------------------------------------------------------------*/
void Edt::vSprint(Stz* aDst, Stz* aSrc)
{
  Edt* aEdt = (Edt*)aSrc;
  
  if (aEdt->bIsNull())
    StrLib::vSprint(aDst,"null%c"_Stz,
                    '\0');
  else
    StrLib::vSprint(aDst,"%02lld-%02lld-%02lld%c"_Stz,
                    aEdt->x.m_uYearInEra + C_BASE_YEAR,
                    aEdt->x.m_uMonth,
                    aEdt->x.m_uDay,
                    '\0');
}

/*----------------------------------------------------------------------------*/
/* sprint Edt datum                                                           */
/*----------------------------------------------------------------------------*/
void Edt::vSprintEscaped(Stz* aDst, Stz* aSrc)
{
  Edt* aEdt = (Edt*)aSrc;
  
  if (aEdt->bIsNull())
    StrLib::vSprint(aDst,"null%c"_Stz,
                    '\0');
  else
    StrLib::vSprint(aDst,"%02lld-%02lld-%02lld%c"_Stz,
                    aEdt->x.m_uYearInEra + C_BASE_YEAR,
                    aEdt->x.m_uMonth,
                    aEdt->x.m_uDay,
                    '\0');
}

/*----------------------------------------------------------------------------*/
/*-- Embedded Comparators ----------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is Edt argA < aArgB?                                                       */
/*----------------------------------------------------------------------------*/
bool Edt::bLessThan(char* aArgA, char* aArgB)
{
  bool bLessThan;
  
  if (((Edt*)aArgA)->bIsNull())
    bLessThan = true;
  
  else if (((Edt*)aArgB)->bIsNull())
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
/* is Edt argA <= aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Edt::bLessEqual(char* aArgA, char* aArgB)
{
  bool bLessEqual;
  
  if (((Edt*)aArgA)->bIsNull() && ((Edt*)aArgB)->bIsNull())
    bLessEqual = true;
  
  else if (((Edt*)aArgA)->bIsNull())
    bLessEqual = true;
  
  else if (((Edt*)aArgB)->bIsNull())
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
/* is Edt argA == aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Edt::bEqualTo(char* aArgA, char* aArgB)
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
/* is Edt argA != aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Edt::bNotEqual(char* aArgA, char* aArgB)
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
/* is Edt argA >= aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Edt::bGreaterEqual(char* aArgA, char* aArgB)
{
  bool bGreaterEqual;
  
  if (((Edt*)aArgA)->bIsNull() && ((Edt*)aArgB)->bIsNull())
    bGreaterEqual = true;
  
  else if (((Edt*)aArgA)->bIsNull())
    bGreaterEqual = false;
  
  else if (((Edt*)aArgB)->bIsNull())
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
/* is Edt argA > aArgB?                                                       */
/*----------------------------------------------------------------------------*/
bool Edt::bGreaterThan(char* aArgA, char* aArgB)
{
  bool bGreaterThan;
  
  if (((Edt*)aArgA)->bIsNull())
    bGreaterThan = false;
  
  else if (((Edt*)aArgB)->bIsNull())
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
i8 Edt::iCmp(char* aArgA, char* aArgB)
{
  //----------------------------------------------------------------------------
  // check for nulls first
  if (((Edt*)aArgA)->bIsNull() && ((Edt*)aArgB)->bIsNull())
    return 0;
  
  if (((Edt*)aArgA)->bIsNull())
    return -1;
  
  if (((Edt*)aArgB)->bIsNull())
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
