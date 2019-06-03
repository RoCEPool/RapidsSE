/*============================================================================*/
/* Bol.cpp -- boolean datum                                                   */
/*============================================================================*/

#include "Datums.h"

#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Bol::operator new(size_t)
{
  vFatal("fatal: Bol class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Bol::Bol(void)
{
  vInitZero();
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Bol::~Bol(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Operators ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is this boolean less than given boolean?                                   */
/*----------------------------------------------------------------------------*/
bool Bol::operator < (Bol& xBol)
{
  // NEED TO CHECK FOR NULLS
  
  bool bLessThan;
  
  if (m_bBol < xBol.m_bBol)
    bLessThan = true;
  else
    bLessThan = false;
  
  ASSERT(0);
  
  return bLessThan;
}

/*----------------------------------------------------------------------------*/
/* is this boolean less than or equal to given boolean?                       */
/*----------------------------------------------------------------------------*/
bool Bol::operator <= (Bol& xBol)
{
  // NEED TO CHECK FOR NULLS
  
  bool bLessEqual;
  
  if (m_bBol <= xBol.m_bBol)
    bLessEqual = true;
  else
    bLessEqual = false;
  
  ASSERT(0);
  
  return bLessEqual;
}

/*----------------------------------------------------------------------------*/
/* is this boolean equal to given boolean?                                    */
/*----------------------------------------------------------------------------*/
bool Bol::operator == (Bol& xBol)
{
  // NEED TO CHECK FOR NULLS
  
  bool bEqualTo;
  
  if (m_bBol == xBol.m_bBol)
    bEqualTo = true;
  else
    bEqualTo = false;
  
  ASSERT(0);
  
  return bEqualTo;
}

/*----------------------------------------------------------------------------*/
/* is this boolean not equal to given boolean?                                */
/*----------------------------------------------------------------------------*/
bool Bol::operator != (Bol& xBol)
{
  // NEED TO CHECK FOR NULLS
  
  bool bNotEqual;
  
  if (m_bBol != xBol.m_bBol)
    bNotEqual = true;
  else
    bNotEqual = false;
  
  ASSERT(0);
  
  return bNotEqual;
}

/*----------------------------------------------------------------------------*/
/* is this boolean greater than or equal to given boolean?                    */
/*----------------------------------------------------------------------------*/
bool Bol::operator >= (Bol& xBol)
{
  // NEED TO CHECK FOR NULLS
  
  bool bGreaterEqual;
  
  if (m_bBol >= xBol.m_bBol)
    bGreaterEqual = true;
  else
    bGreaterEqual = false;
  
  ASSERT(0);
  
  return bGreaterEqual;
}

/*----------------------------------------------------------------------------*/
/* is this boolean greater than given boolean?                                */
/*----------------------------------------------------------------------------*/
bool Bol::operator > (Bol& xBol)
{
  // NEED TO CHECK FOR NULLS
  
  bool bGreaterThan;
  
  if (m_bBol > xBol.m_bBol)
    bGreaterThan = true;
  else
    bGreaterThan = false;
  
  ASSERT(0);
  
  return bGreaterThan;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* initialse to all zero                                                      */
/*----------------------------------------------------------------------------*/
void Bol::vInitZero(void)
{
  m_bBol = 0;
}

/*----------------------------------------------------------------------------*/
/* get as i8                                                                  */
/*----------------------------------------------------------------------------*/
i8 Bol::iAsI8(void)
{
  return m_bBol;
}

/*----------------------------------------------------------------------------*/
/*-- Null --------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* test if null                                                               */
/*----------------------------------------------------------------------------*/
bool Bol::bIsNull(void)
{
  if (C_DATUM_NULL == m_bBol)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* set to null                                                                */
/*----------------------------------------------------------------------------*/
void Bol::vSetNull(void)
{
  m_bBol = C_DATUM_NULL;
}

/*----------------------------------------------------------------------------*/
/* sprint bol datum                                                           */
/*----------------------------------------------------------------------------*/
void Bol::vSprint(Stz* aDst)
{
  if (bIsNull())
    StrLib::vSprint(aDst,"null"_Stz);
  else
    StrLib::vSprint(aDst,"%s"_Stz,m_bBol ? "true" : "false");
}


/*----------------------------------------------------------------------------*/
/*-- Extract and Sprint ------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* copy bol datum                                                             */
/*----------------------------------------------------------------------------*/
i8 Bol::nCopy(void* aDst, void* aSrc)
{
  *(i8*)aDst = *(i8*)aSrc;
  
  return sizeof(i8);
}

/*----------------------------------------------------------------------------*/
/* copy bol datum                                                             */
/*----------------------------------------------------------------------------*/
void Bol::vCopy(void* aDst, void* aSrc)
{
  *(i8*)aDst = *(i8*)aSrc;
}
/*
 TRUE,one,101,1,1.001,1001.001,1901-01-01 00:00:00,
 ,zero,100,0,0.000,1000.000,1900-01-01 00:00:00,true
 x,zero,100,0,0.000,1000.000,1900-01-01 00:00:00,true
 txyz,zero,100,0,0.000,1000.000,1900-01-01 00:00:00,true
 12ab,zero,100,0,0.000,1000.000,1900-01-01 00:00:00,true
 */

/*----------------------------------------------------------------------------*/
/* sprint bol datum                                                           */
/*----------------------------------------------------------------------------*/
void Bol::vSprint(Stz* aDst, Stz* aSrc)
{
  Bol* aBol = (Bol*)aSrc;
  
  if (aBol->bIsNull())
    StrLib::vSprint(aDst,"null%c"_Stz,
                    '\0');
  else
    StrLib::vSprint(aDst,"%s%c"_Stz,*(i8*)aSrc ? "true" : "false",'\0');
}

/*----------------------------------------------------------------------------*/
/*-- Embedded Comparators ----------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is bol argA < aArgB?                                                       */
/*----------------------------------------------------------------------------*/
bool Bol::bLessThan(char* aArgA, char* aArgB)
{
  bool bLessThan;
  
  if (((Bol*)aArgA)->bIsNull())
    bLessThan = true;
  
  else if (((Bol*)aArgB)->bIsNull())
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
/* is bol argA <= aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Bol::bLessEqual(char* aArgA, char* aArgB)
{
  bool bLessEqual;
  
  if (((Bol*)aArgA)->bIsNull() && ((Bol*)aArgB)->bIsNull())
    bLessEqual = true;
  
  else if (((Bol*)aArgA)->bIsNull())
    bLessEqual = true;
  
  else if (((Bol*)aArgB)->bIsNull())
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
/* is bol argA == aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Bol::bEqualTo(char* aArgA, char* aArgB)
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
/* is bol argA != aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Bol::bNotEqual(char* aArgA, char* aArgB)
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
/* is bol argA >= aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Bol::bGreaterEqual(char* aArgA, char* aArgB)
{
  bool bGreaterEqual;
  
  if (((Bol*)aArgA)->bIsNull() && ((Bol*)aArgB)->bIsNull())
    bGreaterEqual = true;
  
  else if (((Bol*)aArgA)->bIsNull())
    bGreaterEqual = false;
  
  else if (((Bol*)aArgB)->bIsNull())
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
/* is bol argA > aArgB?                                                       */
/*----------------------------------------------------------------------------*/
bool Bol::bGreaterThan(char* aArgA, char* aArgB)
{
  bool bGreaterThan;
  
  if (((Bol*)aArgA)->bIsNull())
    bGreaterThan = false;
  
  else if (((Bol*)aArgB)->bIsNull())
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
i8 Bol::iCmp(char* aArgA, char* aArgB)
{
  //----------------------------------------------------------------------------
  // check for nulls first
  if (((Bol*)aArgA)->bIsNull() && ((Bol*)aArgB)->bIsNull())
    return 0;
  
  if (((Bol*)aArgA)->bIsNull())
    return -1;
  
  if (((Bol*)aArgB)->bIsNull())
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
