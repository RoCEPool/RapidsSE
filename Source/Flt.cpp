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
/* extract a float from substr                                                */
/*----------------------------------------------------------------------------*/
bool Flt::bExtract(char** aaSrc, Flt* aFloat, Column* pColumn)
{
  d8 dResult = 0.0;
  bool bNegative = false;
  char xBuffer[256];
  char* aChar = xBuffer;
  
  // skip leading whitespace
  while (' ' == **aaSrc)
    (*aaSrc)++;
 
  /*
  // test for null
  if ( ! **aaSrc
      || C_FS == **aaSrc
      || C_NL == **aaSrc
      || C_CR == **aaSrc) {
    
    if ( !pColumn->bIsNullable() )
      return false;
    
    aFloat->vSetNull();
    return true;
  }
  */
  
  // is there data ?
  if ( ! **aaSrc
      || C_FS == **aaSrc
      || C_NL == **aaSrc
      || C_CR == **aaSrc) {
    
    return false;
  }
  
  // test for null
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
    
    aFloat->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for negative
  
  if (**aaSrc && '-' == **aaSrc ) {
    bNegative = true;
    (*aaSrc)++;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for positive
  
  if (**aaSrc && '+' == **aaSrc ) {
    (*aaSrc)++;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // collect leading portion (till '.')
  while (**aaSrc
         && C_FS != **aaSrc
         && C_NL != **aaSrc
         && C_CR != **aaSrc
         && StrLib::bIsDigit(**aaSrc)) {
    *aChar++ = *(*aaSrc)++;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // collect trailing portion (after '.')
  
  if ('.' == **aaSrc) {
    *aChar++ = *(*aaSrc)++;
    while (**aaSrc
           && C_FS != **aaSrc
           && C_NL != **aaSrc
           && C_CR != **aaSrc
           && StrLib::bIsDigit(**aaSrc)) {
      *aChar++ = *(*aaSrc)++;
    }
  }
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for exponent
  
  if (**aaSrc && ('e' == **aaSrc || 'E' == **aaSrc) ) {
    *aChar++ = *(*aaSrc)++;
    
    // test for -
    if (**aaSrc && '-' == **aaSrc ) {
      *aChar++ = *(*aaSrc)++;
    }
    
    // test for +
    if (**aaSrc && '+' == **aaSrc ) {
      *aChar++ = *(*aaSrc)++;
    }
    
    // collect exponent digits
    while (**aaSrc
           && C_FS != **aaSrc
           && C_NL != **aaSrc
           && C_CR != **aaSrc
           && StrLib::bIsDigit(**aaSrc)) {
      *aChar++ = *(*aaSrc)++;
    }
  }
  
  // terminate
  *aChar++ = '\0';
  
  // skip trailing whitespace
  while (' ' == **aaSrc)
    (*aaSrc)++;
  
  // if any non-digit return false
  while (**aaSrc
         && C_FS != **aaSrc
         && C_NL != **aaSrc
         && C_CR != **aaSrc) {
    
    if (!StrLib::bIsDigit(**aaSrc))
      return false;
    (*aaSrc)++;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // compute final result
  
  dResult = atof(xBuffer);
  
  aFloat->m_dFlt = bNegative ? - dResult : dResult;
  
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
    StrLib::vSprint(aDst,"%.15f%c"_Stz,
                    aFlt->m_dFlt,
                    '\0');
  //vTrace("%f %s\n"_Stz, aFlt->m_dFlt, aDst);
}

/*----------------------------------------------------------------------------*/
/*-- Embedded Comparators ----------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is flt argA < aArgB?                                                       */
/*----------------------------------------------------------------------------*/
bool Flt::bLessThan(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bLessThan;
  
  ASSERT(sizeof(d8) == nBytesA);
  ASSERT(sizeof(d8) == nBytesB);
  
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
bool Flt::bLessEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bLessEqual;
  
  ASSERT(sizeof(d8) == nBytesA);
  ASSERT(sizeof(d8) == nBytesB);
  
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
bool Flt::bEqualTo(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bEqualTo;
  
  ASSERT(sizeof(d8) == nBytesA);
  ASSERT(sizeof(d8) == nBytesB);
  
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
bool Flt::bNotEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bNotEqual;
  
  ASSERT(sizeof(d8) == nBytesA);
  ASSERT(sizeof(d8) == nBytesB);
  
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
bool Flt::bGreaterEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bGreaterEqual;
  
  ASSERT(sizeof(d8) == nBytesA);
  ASSERT(sizeof(d8) == nBytesB);
  
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
bool Flt::bGreaterThan(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bGreaterThan;
  
  ASSERT(sizeof(d8) == nBytesA);
  ASSERT(sizeof(d8) == nBytesB);
  
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
i8 Flt::iCmp(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  ASSERT(sizeof(d8) == nBytesA);
  ASSERT(sizeof(d8) == nBytesB);
  
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
