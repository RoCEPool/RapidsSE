/*============================================================================*/
/* Bcd.cpp -- Bcd datum                                                       */
/*============================================================================*/

#include "Datums.h"

#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Bcd::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  vFatal("fatal: Bcd class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Bcd::Bcd(void)
{
  vInitZero();
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Bcd::~Bcd(void)
{
}

/*----------------------------------------------------------------------------*/
/*-- Operators ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is this decimal less than given decimal?                                   */
/*----------------------------------------------------------------------------*/
bool Bcd::operator < (Bcd& xBcd)
{
  if (m_iBcd < xBcd.m_iBcd)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this decimal less than or equal to given decimal?                       */
/*----------------------------------------------------------------------------*/
bool Bcd::operator <= (Bcd& xBcd)
{
  if (m_iBcd <= xBcd.m_iBcd)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this decimal equal to given decimal?                                    */
/*----------------------------------------------------------------------------*/
bool Bcd::operator == (Bcd& xBcd)
{
  if (m_iBcd == xBcd.m_iBcd)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this decimal not equal to given decimal?                                */
/*----------------------------------------------------------------------------*/
bool Bcd::operator != (Bcd& xBcd)
{
  if (m_iBcd != xBcd.m_iBcd)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this decimal greater than or equal to given decimal?                    */
/*----------------------------------------------------------------------------*/
bool Bcd::operator >= (Bcd& xBcd)
{
  if (m_iBcd >= xBcd.m_iBcd)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this decimal greater than given decimal?                                */
/*----------------------------------------------------------------------------*/
bool Bcd::operator > (Bcd& xBcd)
{
  if (m_iBcd > xBcd.m_iBcd)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get as i8                                                                  */
/*----------------------------------------------------------------------------*/
i8 Bcd::iAsI8(void)
{
  return m_iBcd;
}

/*----------------------------------------------------------------------------*/
/* initialse to all zero                                                      */
/*----------------------------------------------------------------------------*/
void Bcd::vInitZero(void)
{
                                                                 //! to be coded
}

/*----------------------------------------------------------------------------*/
/*-- Null --------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* test if null                                                               */
/*----------------------------------------------------------------------------*/
bool Bcd::bIsNull(void)
{
  if (C_NULL_INT == m_iBcd)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* set to null                                                                */
/*----------------------------------------------------------------------------*/
void Bcd::vSetNull(void)
{
  m_iBcd = C_NULL_INT;
}

/*----------------------------------------------------------------------------*/
/*-- VLQ ---------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Extract and Sprint ------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* copy bcd datum                                                             */
/*----------------------------------------------------------------------------*/
i8 Bcd::nCopy(void* aDst, void* aSrc)
{
  *(i8*)aDst = *(i8*)aSrc;
  
  return sizeof(i8);
}

/*----------------------------------------------------------------------------*/
/* copy bcd datum                                                             */
/*----------------------------------------------------------------------------*/
void Bcd::vCopy(void* aDst, void* aSrc)
{
  *(i8*)aDst = *(i8*)aSrc;
}

/*----------------------------------------------------------------------------*/
/* extract a decimal from substr                                              */
/*----------------------------------------------------------------------------*/
bool Bcd::bExtract(char** aaSrc, Bcd* aDecimal, DBobjects::Column* pColumn)
{
  i8 iResult = 0;
  bool bNegative = false;
  
  // skip leading whitespace
  while (' ' == **aaSrc)
    (*aaSrc)++;
  
  // test for null
  if ( ! **aaSrc || '|' == **aaSrc ) {
    aDecimal->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for negative
  
  if (**aaSrc && '-' == **aaSrc ) {
    bNegative = true;
    (*aaSrc)++;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // collect leading portion (till '.')
  
  while (**aaSrc && '|' != **aaSrc && ',' != **aaSrc
         && StrLib::bIsDigit(**aaSrc)) {
    iResult = iResult * 10 + StrLib::iDigitToI8(**aaSrc);
    (*aaSrc)++;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // collect trailing portion (after '.')
  
//  if ('.' == **aaSrc) {
//    (*aaSrc)++; // skip '.'
//    i8 iPrecision = pColumn ? pColumn->iColumnPrecision() : 15;
//    while (**aaSrc && '|' != **aaSrc && ',' != **aaSrc
//           && StrLib::bIsDigit(**aaSrc) && iPrecision--) {
//      iResult = iResult * 10 + StrLib::iDigitToI8(**aaSrc);
//      (*aaSrc)++;
//    }
//  }

  i8 iPrecision = pColumn->iColumnPrecision();
  if ('.' == **aaSrc) {
    (*aaSrc)++; // skip '.'
    while (**aaSrc && '|' != **aaSrc && ',' != **aaSrc
           && StrLib::bIsDigit(**aaSrc) && iPrecision--) {
      iResult = iResult * 10 + StrLib::iDigitToI8(**aaSrc);
      (*aaSrc)++;
    }
  }
  
  while (iPrecision--)
    iResult = iResult * 10 + 0;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // compute final result
  
  aDecimal->m_iBcd = bNegative ? - iResult : iResult;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* sprint bcd datum                                                           */
/*----------------------------------------------------------------------------*/
void Bcd::vSprint(Stz* aDst, Stz* aSrc, DBobjects::Column* pColumn)
{
  i8 iBcd = *(i8*)aSrc;
  
  i8 iDigits = 1;
  for (i8 i_Digit = 0; i_Digit < pColumn->iColumnPrecision(); i_Digit++)
    iDigits *= 10;
  
  i8 iBcd_Whole = iBcd / iDigits;
  i8 iBcd_Fraction = iBcd % iDigits;
  
  if (iBcd_Fraction < 0)
    iBcd_Fraction = iBcd_Fraction * -1;
  
  StrLib::vSprint(aDst,"%lld.%lld%c"_Stz,iBcd_Whole,iBcd_Fraction,'\0');
}

/*----------------------------------------------------------------------------*/
/*-- Embedded Comparators ----------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is bol argA < aArgB?                                                       */
/*----------------------------------------------------------------------------*/
bool Bcd::bLessThan(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
/* is bol argA <= aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Bcd::bLessEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
/* is bol argA == aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Bcd::bEqualTo(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
/* is bol argA != aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Bcd::bNotEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
/* is bol argA >= aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Bcd::bGreaterEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
/* is bol argA > aArgB?                                                       */
/*----------------------------------------------------------------------------*/
bool Bcd::bGreaterThan(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
i8 Bcd::iCmp(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
