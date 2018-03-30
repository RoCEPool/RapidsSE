/*============================================================================*/
/* Flt.cpp -- float datum                                                     */
/*============================================================================*/

#include "Datums.h"

#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Flt::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  vFatal("fatal: Flt class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Flt::Flt(void)
{
  vInitZero();
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Flt::~Flt(void)
{
}

/*----------------------------------------------------------------------------*/
/*-- Operators ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is this float less than given float?                                       */
/*----------------------------------------------------------------------------*/
bool Flt::operator < (Flt& xFlt)
{
  if (m_dFlt < xFlt.m_dFlt)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this float less than or equal to given float?                           */
/*----------------------------------------------------------------------------*/
bool Flt::operator <= (Flt& xFlt)
{
  if (m_dFlt <= xFlt.m_dFlt)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this float equal to given float?                                        */
/*----------------------------------------------------------------------------*/
bool Flt::operator == (Flt& xFlt)
{
  if (m_dFlt == xFlt.m_dFlt)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this float not equal to given float?                                    */
/*----------------------------------------------------------------------------*/
bool Flt::operator != (Flt& xFlt)
{
  if (m_dFlt != xFlt.m_dFlt)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this float greater than or equal to given float?                        */
/*----------------------------------------------------------------------------*/
bool Flt::operator >= (Flt& xFlt)
{
  if (m_dFlt >= xFlt.m_dFlt)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this float greater than given float?                                    */
/*----------------------------------------------------------------------------*/
bool Flt::operator > (Flt& xFlt)
{
  if (m_dFlt > xFlt.m_dFlt)
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
void Flt::vInitZero(void)
{
  m_dFlt = 0.0;
}

/*----------------------------------------------------------------------------*/
/* get as d8                                                                  */
/*----------------------------------------------------------------------------*/
d8 Flt::dAsD8(void)
{
  return m_dFlt;
}

/*----------------------------------------------------------------------------*/
/*-- Null --------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* test if null                                                               */
/*----------------------------------------------------------------------------*/
bool Flt::bIsNull(void)
{
  if (C_NULL_FLT == m_dFlt)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* set to null                                                                */
/*----------------------------------------------------------------------------*/
void Flt::vSetNull(void)
{
  m_dFlt = C_NULL_FLT;
}

/*----------------------------------------------------------------------------*/
/*-- VLQ ---------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Extract and Sprint ------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* copy flt datum                                                             */
/*----------------------------------------------------------------------------*/
i8 Flt::nCopy(void* aDst, void* aSrc)
{
  *(d8*)aDst = *(d8*)aSrc;
  
  return sizeof(d8);
}

/*----------------------------------------------------------------------------*/
/* copy dte datum                                                             */
/*----------------------------------------------------------------------------*/
void Flt::vCopy(void* aDst, void* aSrc)
{
  *(d8*)aDst = *(d8*)aSrc;
}

/*----------------------------------------------------------------------------*/
/* extract a float field from csv substr                                      */
/*----------------------------------------------------------------------------*/
bool Flt::bExtract(char** aaSrc, Flt* aFlt, Column* pColumn, Formatters* pFormatters)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  bool bSawEnclosedLeading =
    StrLib::bSkipLeading(aaSrc,pFormatters->m_cEnclosedBy_Leading);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (DtmLib::bTestForNull(aaSrc,pColumn,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    aFlt->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // float field processing
  
  if ( ! StrLib::bExtractFloat(aaSrc,aFlt->m_dFlt) )
    return false;

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
/* sprint flt datum                                                           */
/*----------------------------------------------------------------------------*/
void Flt::vSprint(Stz* aDst, Stz* aSrc)
{
  Flt* aFlt = (Flt*)aSrc;
  
  if (aFlt->bIsNull())
    StrLib::vSprint(aDst,"null%c"_Stz,
                    '\0');
  else
//    StrLib::vSprint(aDst,"%.15f%c"_Stz,
//                    aFlt->m_dFlt,
//                    '\0');
    StrLib::vSprint(aDst,"%g%c"_Stz,
                    aFlt->m_dFlt,
                    '\0');
  //vTrace("%f %s\n"_Stz, aFlt->m_dFlt, aDst);
}

/*----------------------------------------------------------------------------*/
/*-- Embedded Comparators ----------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* is the size valid ?                                                        */
/*----------------------------------------------------------------------------*/
bool Flt::bIsValidSize(i8 iPrecision)
{
  if (iPrecision > C_MAX_PRECISION_FLT)
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* is flt argA < aArgB?                                                       */
/*----------------------------------------------------------------------------*/
bool Flt::bLessThan(char* aArgA, char* aArgB)
{
  bool bLessThan;
  
  if (((Flt*)aArgA)->bIsNull())
    bLessThan = true;
  
  else if (((Flt*)aArgB)->bIsNull())
    bLessThan = false;
  
  else {
    d8 dA = *(d8*)aArgA;
    d8 dB = *(d8*)aArgB;
    
    if (dA < dB)
      bLessThan = true;
    else
      bLessThan = false;
  }
  
  return bLessThan;
}

/*----------------------------------------------------------------------------*/
/* is flt argA <= aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Flt::bLessEqual(char* aArgA, char* aArgB)
{
  bool bLessEqual;
  
  if (((Flt*)aArgA)->bIsNull())
    bLessEqual = true;
  
  else if (((Flt*)aArgB)->bIsNull())
    bLessEqual = false;
  
  else {
    d8 dA = *(d8*)aArgA;
    d8 dB = *(d8*)aArgB;
    
    if (dA <= dB)
      bLessEqual = true;
    else
      bLessEqual = false;
  }
  
  return bLessEqual;
}

/*----------------------------------------------------------------------------*/
/* is flt argA == aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Flt::bEqualTo(char* aArgA, char* aArgB)
{
  bool bEqualTo;
  
  if (((Flt*)aArgA)->bIsNull())
    bEqualTo = true;
  
  else if (((Flt*)aArgB)->bIsNull())
    bEqualTo = false;
  
  else {
    d8 dA = *(d8*)aArgA;
    d8 dB = *(d8*)aArgB;
    
    if (dA == dB)
      bEqualTo = true;
    else
      bEqualTo = false;
  }
  
  return bEqualTo;
}

/*----------------------------------------------------------------------------*/
/* is flt argA != aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Flt::bNotEqual(char* aArgA, char* aArgB)
{
  bool bNotEqual;
  
  if (((Flt*)aArgA)->bIsNull())
    bNotEqual = true;
  
  else if (((Flt*)aArgB)->bIsNull())
    bNotEqual = false;
  
  else {
    d8 dA = *(d8*)aArgA;
    d8 dB = *(d8*)aArgB;
    
    if (dA != dB)
      bNotEqual = true;
    else
      bNotEqual = false;
  }
  
  return bNotEqual;
}

/*----------------------------------------------------------------------------*/
/* is flt argA >= aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Flt::bGreaterEqual(char* aArgA, char* aArgB)
{
  bool bGreaterEqual;
  
  if (((Flt*)aArgA)->bIsNull())
    bGreaterEqual = true;
  
  else if (((Flt*)aArgB)->bIsNull())
    bGreaterEqual = false;
  
  else {
    d8 dA = *(d8*)aArgA;
    d8 dB = *(d8*)aArgB;
    
    if (dA >= dB)
      bGreaterEqual = true;
    else
      bGreaterEqual = false;
  }
  
  return bGreaterEqual;
}

/*----------------------------------------------------------------------------*/
/* is flt argA > aArgB?                                                       */
/*----------------------------------------------------------------------------*/
bool Flt::bGreaterThan(char* aArgA, char* aArgB)
{
  bool bGreaterThan;
  
  if (((Flt*)aArgA)->bIsNull())
    bGreaterThan = true;
  
  else if (((Flt*)aArgB)->bIsNull())
    bGreaterThan = false;
  
  else {
    d8 dA = *(d8*)aArgA;
    d8 dB = *(d8*)aArgB;
    
    if (dA > dB)
      bGreaterThan = true;
    else
      bGreaterThan = false;
  }
  
  return bGreaterThan;
}

/*----------------------------------------------------------------------------*/
/* cmp argA with argB (-ve: argA < argB, 0: argA == argB, +ve: argA > argB)   */
/*----------------------------------------------------------------------------*/
i8 Flt::iCmp(char* aArgA, char* aArgB)
{
  
  if (((Flt*)aArgA)->bIsNull())
    return -1;
  
  if (((Flt*)aArgB)->bIsNull())
    return 1;
  
  d8 dA = *(d8*)aArgA;
  d8 dB = *(d8*)aArgB;
  
  if (dA > dB)
    return 1;
  
  if (dA < dB)
    return -1;
  
  return 0;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
