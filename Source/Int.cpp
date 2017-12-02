/*============================================================================*/
/* Int.cpp -- int datum                                                       */
/*============================================================================*/

#include "Datums.h"

#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Int::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  vFatal("fatal: Int class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Int::Int(void)
{
  vInitZero();
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Int::~Int(void)
{
}

/*----------------------------------------------------------------------------*/
/*-- Operators ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is this integer less than given integer?                                   */
/*----------------------------------------------------------------------------*/
bool Int::operator < (Int& xInt)
{
  // NEED TO CHECK FOR NULLS
  
  bool bLessThan;
  
  if (m_iInt < xInt.m_iInt)
    bLessThan = true;
  else
    bLessThan = false;
  
  ASSERT(0);
  
  return bLessThan;
}

/*----------------------------------------------------------------------------*/
/* is this integer less than or equal to given integer?                       */
/*----------------------------------------------------------------------------*/
bool Int::operator <= (Int& xInt)
{
  // NEED TO CHECK FOR NULLS
  
  bool bLessEqual;
  
  if (m_iInt <= xInt.m_iInt)
    bLessEqual = true;
  else
    bLessEqual = false;
  
  ASSERT(0);
  
  return bLessEqual;
}

/*----------------------------------------------------------------------------*/
/* is this integer equal to given integer?                                    */
/*----------------------------------------------------------------------------*/
bool Int::operator == (Int& xInt)
{
  // NEED TO CHECK FOR NULLS
  
  bool bEqualTo;
  
  if (m_iInt == xInt.m_iInt)
    bEqualTo = true;
  else
    bEqualTo = false;
  
  ASSERT(0);
  
  return bEqualTo;
}

/*----------------------------------------------------------------------------*/
/* is this integer not equal to given integer?                                */
/*----------------------------------------------------------------------------*/
bool Int::operator != (Int& xInt)
{
  // NEED TO CHECK FOR NULLS
  
  bool bNotEqual;
  
  if (m_iInt != xInt.m_iInt)
    bNotEqual = true;
  else
    bNotEqual = false;
  
  ASSERT(0);
  
  return bNotEqual;
}

/*----------------------------------------------------------------------------*/
/* is this integer greater than or equal to given integer?                    */
/*----------------------------------------------------------------------------*/
bool Int::operator >= (Int& xInt)
{
  // NEED TO CHECK FOR NULLS
  
  bool bGreaterEqual;
  
  if (m_iInt >= xInt.m_iInt)
    bGreaterEqual = true;
  else
    bGreaterEqual = false;
  
  ASSERT(0);
  
  return bGreaterEqual;
}

/*----------------------------------------------------------------------------*/
/* is this integer greater than given integer?                                */
/*----------------------------------------------------------------------------*/
bool Int::operator > (Int& xInt)
{
  // NEED TO CHECK FOR NULLS
  
  bool bGreaterThan;
  
  if (m_iInt > xInt.m_iInt)
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
void Int::vInitZero(void)
{
  m_iInt = 0;
}

/*----------------------------------------------------------------------------*/
/* get as i8                                                                  */
/*----------------------------------------------------------------------------*/
i8 Int::iAsI8(void)
{
  return m_iInt;
}

/*----------------------------------------------------------------------------*/
/*-- Null --------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* test if null                                                               */
/*----------------------------------------------------------------------------*/
bool Int::bIsNull(void)
{
  if (C_NULL_INT == m_iInt)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* set to null                                                                */
/*----------------------------------------------------------------------------*/
void Int::vSetNull(void)
{
  m_iInt = C_NULL_INT;
}

/*----------------------------------------------------------------------------*/
/*-- VLQ ---------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert to vlq                                                             */
/*----------------------------------------------------------------------------*/
void Int::vIntToVlq(char* aVq)
{
}

/*----------------------------------------------------------------------------*/
/* convert to vlq                                                             */
/*----------------------------------------------------------------------------*/
void Int::vVlqToInt(char* aVq)
{
}

/*----------------------------------------------------------------------------*/
/*-- Extract and Sprint ------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* copy int datum                                                             */
/*----------------------------------------------------------------------------*/
i8 Int::nCopy(void* aDst, void* aSrc)
{
  *(i8*)aDst = *(i8*)aSrc;
  
  return sizeof(i8);
}

/*----------------------------------------------------------------------------*/
/* copy int datum                                                             */
/*----------------------------------------------------------------------------*/
void Int::vCopy(void* aDst, void* aSrc)
{
  *(i8*)aDst = *(i8*)aSrc;
}

/*----------------------------------------------------------------------------*/
/* extract an integer from substr                                             */
/*----------------------------------------------------------------------------*/
bool Int::bExtract(char** aaSrc, Int* aInt, Column* pColumn, Formatters* pFormatters)
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
    
    aInt->vSetNull();
    return true;
  }
//  bool bValidNull;
//  if (DtmLib::bTestForNull(aaSrc,pColumn,pFormatters,bValidNull)) {
//    if ( ! bValidNull )
//      return false;
//    aInteger->vSetNull();
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
  // collect the csv
  
  while (**aaSrc
         && C_FS != **aaSrc
         && C_NL != **aaSrc
         && C_CR != **aaSrc
         && StrLib::bIsDigit(**aaSrc) ) {
      uResult = uResult * 10 + StrLib::iDigitToI8(**aaSrc);
    
    // have we exceeded the max ?
    if (uResult > (u8)C_I8_MAX)
      return false;
    
    (*aaSrc)++;
  }

 // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 // ignore trailing portion (after '.')

  if ('.' == **aaSrc) {
    
    // skip the .
    (*aaSrc)++;
    
    while (**aaSrc
           && C_FS != **aaSrc
           && C_NL != **aaSrc
           && C_CR != **aaSrc) {
      
      if (!StrLib::bIsDigit(**aaSrc))
        return false;
      
      (*aaSrc)++;
    }
  }
  
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
  if (bNegative && (uResult < (u8)C_I8_MIN))
    return false;
  
  aInt->m_iInt = uResult;;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* sprint int datum                                                           */
/*----------------------------------------------------------------------------*/
void Int::vSprint(Stz* aDst, Stz* aSrc)
{
  Int* aInt = (Int*)aSrc;
  
  if (aInt->bIsNull())
    StrLib::vSprint(aDst,"null%c"_Stz,
                    '\0');
  else
    StrLib::vSprint(aDst,"%lld%c"_Stz,*(i8*)aSrc,'\0');
}

/*----------------------------------------------------------------------------*/
/*-- Embedded Comparators ----------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is embedded int argA < embedded aArgB?                                     */
/*----------------------------------------------------------------------------*/
bool Int::bLessThan(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bLessThan;

  ASSERT(sizeof(i8) == nBytesA);
  ASSERT(sizeof(i8) == nBytesB);
  
  if (((Int*)aArgA)->bIsNull())
    bLessThan = true;
  
  else if (((Int*)aArgB)->bIsNull())
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
/* is embedded int argA <= embedded aArgB?                                    */
/*----------------------------------------------------------------------------*/
bool Int::bLessEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bLessEqual;
  
  ASSERT(sizeof(i8) == nBytesA);
  ASSERT(sizeof(i8) == nBytesB);
  
  if (((Int*)aArgA)->bIsNull())
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
/* is embedded int argA == embedded aArgB?                                    */
/*----------------------------------------------------------------------------*/
bool Int::bEqualTo(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bEqualTo;
  
  ASSERT(sizeof(i8) == nBytesA);
  ASSERT(sizeof(i8) == nBytesB);
  
  if (((Int*)aArgA)->bIsNull())
    bEqualTo = false;
  
  else if (((Int*)aArgB)->bIsNull())
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
/* is embedded int argA != embedded aArgB?                                    */
/*----------------------------------------------------------------------------*/
bool Int::bNotEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bNotEqual;
  
  ASSERT(sizeof(i8) == nBytesA);
  ASSERT(sizeof(i8) == nBytesB);
  
  if (((Int*)aArgA)->bIsNull())
    bNotEqual = true;
  
  else if (((Int*)aArgB)->bIsNull())
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
/* is embeddedint argA >= embeddedaArgB?                                      */
/*----------------------------------------------------------------------------*/
bool Int::bGreaterEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bGreaterEqual;
  
  ASSERT(sizeof(i8) == nBytesA);
  ASSERT(sizeof(i8) == nBytesB);
  
  if (((Int*)aArgA)->bIsNull())
    bGreaterEqual = false;
  
  else if (((Int*)aArgB)->bIsNull())
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
/* is embedded int argA > embedded aArgB?                                     */
/*----------------------------------------------------------------------------*/
bool Int::bGreaterThan(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bGreaterThan;
  
  ASSERT(sizeof(i8) == nBytesA);
  ASSERT(sizeof(i8) == nBytesB);
  
  if (((Int*)aArgA)->bIsNull())
    bGreaterThan = false;
  
  else if (((Int*)aArgB)->bIsNull())
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
i8 Int::iCmp(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  
  ASSERT(sizeof(i8) == nBytesA);
  ASSERT(sizeof(i8) == nBytesB);
  
  //----------------------------------------------------------------------------
  // check for nulls first
  if (((Int*)aArgA)->bIsNull())
    return -1;
  
  if (((Int*)aArgB)->bIsNull())
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
