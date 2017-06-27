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
  
  vFatal("fatal: Int class cannot be new'd\n"_Str);
  
  RASSERT(0);
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
  if (m_iInt < xInt.m_iInt)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this integer less than or equal to given integer?                       */
/*----------------------------------------------------------------------------*/
bool Int::operator <= (Int& xInt)
{
  if (m_iInt <= xInt.m_iInt)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this integer equal to given integer?                                    */
/*----------------------------------------------------------------------------*/
bool Int::operator == (Int& xInt)
{
  if (m_iInt == xInt.m_iInt)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this integer not equal to given integer?                                */
/*----------------------------------------------------------------------------*/
bool Int::operator != (Int& xInt)
{
  if (m_iInt != xInt.m_iInt)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this integer greater than or equal to given integer?                    */
/*----------------------------------------------------------------------------*/
bool Int::operator >= (Int& xInt)
{
  if (m_iInt >= xInt.m_iInt)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this integer greater than given integer?                                */
/*----------------------------------------------------------------------------*/
bool Int::operator > (Int& xInt)
{
  if (m_iInt > xInt.m_iInt)
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
/* extract an integer from substr                                             */
/*----------------------------------------------------------------------------*/
bool Int::bExtract(char** aaSrc, Int* aInteger, Column* pColumn)
{
  if (pColumn->iColumnNo() == -1)
    int a = 1;
  
  i8 iResult = 0;
  char xBuffer[256];
  char* aChar = xBuffer;
  
  // skip leading whitespace
  while (' ' == **aaSrc)
    (*aaSrc)++;
  
  // test for null
  if ( ! **aaSrc || '|' == **aaSrc ) {
    aInteger->vSetNull();
    return true;
  }
  
  // collect the csv
  while (**aaSrc && '\n' != **aaSrc && '\r' != **aaSrc && '|' != **aaSrc && ',' != **aaSrc) {
    if ('|' == **aaSrc)
      return true;
    if ( ! StrLib::bIsDigit(**aaSrc) )
      return false;
    *aChar++ = *(*aaSrc)++;
  }
  *aChar++ = '\0';
  
  // find first char
  for (aChar = xBuffer; *aChar && ' ' == *aChar; aChar++)
    ;
  
  // convert
  iResult = atol(aChar);
  
  aInteger->m_iInt = iResult;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* sprint int datum                                                           */
/*----------------------------------------------------------------------------*/
void Int::vSprint(Str* aDst, Str* aSrc, i8& nBytesTaken)
{
  StrLib::vSprint(aDst,"%lld%c"_Str,*(i8*)aSrc,'\0');
  
  nBytesTaken = sizeof(i8);
}

/*----------------------------------------------------------------------------*/
/* valid integer in substr?                                                   */
/*----------------------------------------------------------------------------*/
bool Int::bValid(char** aaSrc, Column* pColumn)
{
  Int xInt;
  
  bool bValid = Int::bExtract(aaSrc,&xInt,pColumn);
  
  return bValid;
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
  
  i8 iA = *(i8*)aArgA;
  i8 iB = *(i8*)aArgB;
  
  if (iA < iB)
    bLessThan = true;
  else
    bLessThan = false;
  
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
  
  i8 iA = *(i8*)aArgA;
  i8 iB = *(i8*)aArgB;
  
  if (iA <= iB)
    bLessEqual = true;
  else
    bLessEqual = false;
  
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
  
  i8 iA = *(i8*)aArgA;
  i8 iB = *(i8*)aArgB;
  
  if (iA == iB)
    bEqualTo = true;
  else
    bEqualTo = false;
  
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
  
  i8 iA = *(i8*)aArgA;
  i8 iB = *(i8*)aArgB;
  
  if (iA != iB)
    bNotEqual = true;
  else
    bNotEqual = false;
  
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
  
  i8 iA = *(i8*)aArgA;
  i8 iB = *(i8*)aArgB;
  
  if (iA >= iB)
    bGreaterEqual = true;
  else
    bGreaterEqual = false;
  
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
i8 Int::iCmp(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
