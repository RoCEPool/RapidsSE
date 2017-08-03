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
void* Bol::operator new(size_t iSize)
{
  RASSERT(iSize);
  
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
Bol::~Bol(void)
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
  if (m_bBol < xBol.m_bBol)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this boolean less than or equal to given boolean?                       */
/*----------------------------------------------------------------------------*/
bool Bol::operator <= (Bol& xBol)
{
  if (m_bBol <= xBol.m_bBol)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this boolean equal to given boolean?                                    */
/*----------------------------------------------------------------------------*/
bool Bol::operator == (Bol& xBol)
{
  if (m_bBol == xBol.m_bBol)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this boolean not equal to given boolean?                                */
/*----------------------------------------------------------------------------*/
bool Bol::operator != (Bol& xBol)
{
  if (m_bBol != xBol.m_bBol)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this boolean greater than or equal to given boolean?                    */
/*----------------------------------------------------------------------------*/
bool Bol::operator >= (Bol& xBol)
{
  if (m_bBol >= xBol.m_bBol)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this boolean greater than given boolean?                                */
/*----------------------------------------------------------------------------*/
bool Bol::operator > (Bol& xBol)
{
  if (m_bBol > xBol.m_bBol)
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
  if (C_NULL_INT == m_bBol)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* set to null                                                                */
/*----------------------------------------------------------------------------*/
void Bol::vSetNull(void)
{
  m_bBol = C_NULL_INT;
}

/*----------------------------------------------------------------------------*/
/*-- VLQ ---------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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

/*----------------------------------------------------------------------------*/
/* extract a boolean from substr                                              */
/*----------------------------------------------------------------------------*/
#ifdef _ROWFORMAT
bool Bol::bExtract(char*aSrc, Bol* aBoolean, i8 iLength, Column* pColumn)
{
  bool bResult = false;
  
  for (i8 i_CurrentChar = 0; i_CurrentChar < iLength; i_CurrentChar++) {
  
    char c = aSrc[i_CurrentChar];
    
    // skip leading whitespace
    while (' ' == c) {
      if (i_CurrentChar == iLength) {
        aBoolean->vSetNull();
        return true;
      }
      continue;
    }
    
    // test for true
    if ('t' == c ||
        'T' == c ||
        'y' == c ||
        'Y' == c ||
        '1' == c)
      bResult = true;
  }
  
  aBoolean->m_bBol = bResult;
  
  return true;
}

#else
bool Bol::bExtract(char** aaSrc, Bol* aBoolean, Column* pColumn)
{
  bool bResult = false;
  
  // skip leading whitespace
  while (' ' == **aaSrc)
    (*aaSrc)++;
  
  // test for null
  if ( ! **aaSrc || '|' == **aaSrc ) {
    aBoolean->vSetNull();
    return true;
  }
  
  // test for true
  if ('t' == *(*aaSrc) ||
      'T' == *(*aaSrc) ||
      'y' == *(*aaSrc) ||
      'Y' == *(*aaSrc) ||
      '1' == *(*aaSrc))
    bResult = true;
  
  while (**aaSrc && '|' != **aaSrc && ',' != **aaSrc
         && '\n' != **aaSrc && '\r' != **aaSrc) {
    (*aaSrc)++;
  }
  
  aBoolean->m_bBol = bResult;
  
  return true;
}
#endif

/*----------------------------------------------------------------------------*/
/* sprint bol datum                                                           */
/*----------------------------------------------------------------------------*/
void Bol::vSprint(Stz* aDst, Stz* aSrc)
{
  StrLib::vSprint(aDst,"%lld%c"_Stz,*(i8*)aSrc,'\0');
}

/*----------------------------------------------------------------------------*/
/* valid boolean in substr?                                                   */
/*----------------------------------------------------------------------------*/
#ifdef _ROWFORMAT
bool Bol::bValid(char*aSrc, i8 iLength, Column* pColumn)
{
  Bol xBol;
  
  bool bValid = bExtract(aSrc,&xBol,iLength,pColumn);
  
  return bValid;
}
#else
bool Bol::bValid(char** aaSrc, Column* pColumn)
{
  Bol xBol;
  
  bool bValid = bExtract(aaSrc,&xBol,pColumn);
  
  return bValid;
}
#endif
/*----------------------------------------------------------------------------*/
/*-- Embedded Comparators ----------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is bol argA < aArgB?                                                       */
/*----------------------------------------------------------------------------*/
bool Bol::bLessThan(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
bool Bol::bLessEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
bool Bol::bEqualTo(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
bool Bol::bNotEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
bool Bol::bGreaterEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
bool Bol::bGreaterThan(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
i8 Bol::iCmp(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
