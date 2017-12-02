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
bool Bcd::bExtract(char** aaSrc, Bcd* aBcd, DBobjects::Column* pColumn, Formatters* pFormatters)
{
  u8 uResult = 0;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  // skip leading spaces
  StrLib::vSkipSpaces(aaSrc);

  // is there any data ?
  if ( ! DtmLib::bIsThereData(**aaSrc,pFormatters) )
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
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
    
    aBcd->vSetNull();
    return true;
  }
//  bool bValidNull;
//  if (DtmLib::bTestForNull(aaSrc,pColumn,pFormatters,bValidNull)) {
//    if ( ! bValidNull )
//      return false;
//    aBcd->vSetNull();
//    return true;
//  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for negative or positive sign
  
  bool bNegative = false;
  if (**aaSrc && '-' == **aaSrc ) {
    bNegative = true;
    (*aaSrc)++;
  }
  else
  if (**aaSrc && '+' == **aaSrc ) {
    (*aaSrc)++;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // collect leading portion (till '.')
  i8 iPrecision = pColumn->iColumnLength() - pColumn->iColumnScale();
  while (**aaSrc
         && C_FS != **aaSrc
         && C_NL != **aaSrc
         && C_CR != **aaSrc
         && StrLib::bIsDigit(**aaSrc)
         && iPrecision--) {
    
    uResult = uResult * 10 + StrLib::iDigitToI8(**aaSrc);
    (*aaSrc)++;
  }
  
  if (iPrecision < 0)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // collect trailing portion (after '.')
  
//  if ('.' == **aaSrc) {
//    (*aaSrc)++; // skip '.'
//    i8 iPrecision = pColumn ? pColumn->iColumnPrecision() : 15;
//    while (**aaSrc && C_FS != **aaSrc
//           && StrLib::bIsDigit(**aaSrc) && iPrecision--) {
//      iResult = iResult * 10 + StrLib::iDigitToI8(**aaSrc);
//      (*aaSrc)++;
//    }
//  }

  i8 iScale = pColumn->iColumnScale();
  if ('.' == **aaSrc) {
    (*aaSrc)++; // skip '.'
    
    while (**aaSrc
           && C_FS != **aaSrc
           && C_NL != **aaSrc
           && C_CR != **aaSrc
           && StrLib::bIsDigit(**aaSrc) && iScale) {
      uResult = uResult * 10 + StrLib::iDigitToI8(**aaSrc);
      iScale--;
      (*aaSrc)++;
    }
  }
  
  while (iScale--) {
    uResult = uResult * 10 + 0;
  }
  
  // have we exceeded the max ?
  if (uResult > (u8)C_I8_MAX)
    return false;
  
  // round up if there any more digits after decimal
  if (**aaSrc
      && C_FS != **aaSrc
      && C_NL != **aaSrc
      && C_CR != **aaSrc
      && StrLib::bIsDigit(**aaSrc))
    if (StrLib::iDigitToI8(**aaSrc) >= 5)
      uResult += 1;
  
  // skip trailing digits
  while (**aaSrc
         && C_FS != **aaSrc
         && C_NL != **aaSrc
         && C_CR != **aaSrc
         && StrLib::bIsDigit(**aaSrc))
    (*aaSrc)++;
  
  // if any non-digit return false
  while (**aaSrc
         && C_FS != **aaSrc
         && C_NL != **aaSrc
         && C_CR != **aaSrc) {
    
    if (' ' == **aaSrc) {
      (*aaSrc)++;
      break;
    }
    
    if (!StrLib::bIsDigit(**aaSrc))
    return false;
    (*aaSrc)++;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  // skip trailing spaces
  StrLib::vSkipSpaces(aaSrc);
  
  // should see a FS, NL or CR
  if (**aaSrc
      && C_FS != **aaSrc
      && C_NL != **aaSrc
      && C_CR != **aaSrc)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // compute final result
  
  uResult = bNegative ? - uResult : uResult;
  
  // have we gone beyond the min ?
  if (uResult != 0 && bNegative && (uResult < (u8)C_I8_MIN))
    return false;

  aBcd->m_iBcd = uResult;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* sprint bcd datum                                                           */
/*----------------------------------------------------------------------------*/
void Bcd::vSprint(Stz* aDst, Stz* aSrc, DBobjects::Column* pColumn)
{
  bool bNegative = false;
  Bcd* aBcd = (Bcd*)aSrc;
  
  if (aBcd->bIsNull())
    StrLib::vSprint(aDst,"null%c"_Stz,
                    '\0');
  else {
    i8 iBcd = *(i8*)aSrc;
    i8 iDigits = 1;
    for (i8 i_Digit = 0; i_Digit < pColumn->iColumnScale(); i_Digit++)
      iDigits *= 10;
    
    i8 iBcd_Whole = iBcd / iDigits;
    i8 iBcd_Fraction = iBcd % iDigits;
    
    if (iBcd_Fraction < 0 || iBcd_Whole < 0) {
      bNegative = true;
      iBcd_Fraction = iBcd_Fraction * -1;
      iBcd_Whole = iBcd_Whole * -1;
    }
    
    if (pColumn->iColumnScale())
      StrLib::vSprint(aDst,"%s%lld.%.*lld%c"_Stz,
                      bNegative ? "-" : "",
                      iBcd_Whole,
                      pColumn->iColumnScale(),
                      iBcd_Fraction,'\0');
    else
      StrLib::vSprint(aDst,"%s%lld%c"_Stz, bNegative ? "-" : "",iBcd_Whole, '\0');
  }
}

/*----------------------------------------------------------------------------*/
/* is the size valid ?                                                        */
/*----------------------------------------------------------------------------*/
bool Bcd::bIsValidSize(i8 iPrecision, i8 iScale)
{
  if (iPrecision > C_MAX_PRECISION_BCD)
    return false;
  
  if (iScale > iPrecision)
    return false;
  
  if (iScale > C_MAX_PRECISION_BCD)
    return false;
  
  return true;
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
  
  if (((Bcd*)aArgA)->bIsNull())
    bLessThan = true;
  
  else if (((Bcd*)aArgB)->bIsNull())
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
bool Bcd::bLessEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bLessEqual;
  
  ASSERT(sizeof(i8) == nBytesA);
  ASSERT(sizeof(i8) == nBytesB);
  
  if (((Bcd*)aArgA)->bIsNull())
    bLessEqual = true;
  
  else if (((Bcd*)aArgB)->bIsNull())
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
bool Bcd::bEqualTo(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bEqualTo;
  
  ASSERT(sizeof(i8) == nBytesA);
  ASSERT(sizeof(i8) == nBytesB);
  
  if (((Bcd*)aArgA)->bIsNull())
    bEqualTo = true;
  
  else if (((Bcd*)aArgB)->bIsNull())
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
/* is bol argA != aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Bcd::bNotEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bNotEqual;
  
  ASSERT(sizeof(i8) == nBytesA);
  ASSERT(sizeof(i8) == nBytesB);
  
  if (((Bcd*)aArgA)->bIsNull())
    bNotEqual = true;
  
  else if (((Bcd*)aArgB)->bIsNull())
    bNotEqual = false;
  
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
/* is bol argA >= aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Bcd::bGreaterEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bGreaterEqual;
  
  ASSERT(sizeof(i8) == nBytesA);
  ASSERT(sizeof(i8) == nBytesB);
  
  if (((Bcd*)aArgA)->bIsNull())
    bGreaterEqual = true;
  
  else if (((Bcd*)aArgB)->bIsNull())
    bGreaterEqual = false;
  
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
bool Bcd::bGreaterThan(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bGreaterThan;
  
  ASSERT(sizeof(i8) == nBytesA);
  ASSERT(sizeof(i8) == nBytesB);
  
  if (((Bcd*)aArgA)->bIsNull())
    bGreaterThan = true;
  
  else if (((Bcd*)aArgB)->bIsNull())
    bGreaterThan = false;
  
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
i8 Bcd::iCmp(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  ASSERT(sizeof(i8) == nBytesA);
  ASSERT(sizeof(i8) == nBytesB);
  
  if (((Bcd*)aArgA)->bIsNull())
    return -1;
  
  if (((Bcd*)aArgB)->bIsNull())
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
